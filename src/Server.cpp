/************************************************************************
*	DICOMLIB
*	Copyright 2003 Sunnybrook and Women's College Health Science Center
*	Implemented by Trevor Morgan  (morgan@sten.sunnybrook.utoronto.ca)
*
*	See LICENSE.txt for copyright and licensing info.
*************************************************************************/

/*
	Design note.

	We make frequent use of callbacks here, implemented using the boost::function library.

	This is necessary in a multithreaded, event-driven environment.  Basically we're saying
	'when event X occurs, call function Y'.  Y could be a callback or an overriden virtual function.

	In most cases we could use callbacks or virtual functions, and it's a question of choosing
	which is simpler.   Callbacks make sense for the dicom command handlers, as it means
	we can dynamically change the command handlers available by modifying a map of
	callbacks; the basic structure is this:
			std::map<UID,HandlerFunction> Handlers_;

*/



#include <exception>
#include <iostream>

#include <boost/thread/thread.hpp>

#include "Server.hpp"
#include "UIDs.hpp"
#include "ImplementationUID.hpp"
#include "pdata.hpp"
#include "TransferSyntax.hpp"
#include "Decoder.hpp"
#include "Cdimse.hpp"
#include "UID.hpp"
#include "ServiceBase.hpp"

#include "ThreadSpecificServer.hpp"

using std::string;using std::cout; using std::endl;

namespace dicom
{

	using namespace primitive;
	boost::mutex Server::cerr_mutex;
	boost::mutex Server::cout_mutex;



	void Server::Logger::LogError(std::string Error)
	{
 		boost::mutex::scoped_lock lock(cerr_mutex);
 		std::cerr << Error << std::endl;
	}

	void Server::Logger::LogMessage	(std::string Message)
	{
 		boost::mutex::scoped_lock lock(cout_mutex);
 		std::cout << Message << std::endl;
	}

	Server::Server()
		:ServerThread_(NULL)
		,KillFlag(false)
		,CurrentLogger_(&DefaultLogger_)
	{
		//is the following line a standard requirment???
		//AcceptableAbstractSyntaxes_.insert(UID_VERIFICATION_SOP_CLASS);
	}

	Server::~Server()
	{
		this->Stop();

	}
	//!Start listening for connections on 'port'
	/*!
		Note that this runs in the current thread, and will not exit until the kill
		flag is raised. If you want the server to run in the background, use
		ServeInNewThread()
	*/
	void Server::Serve(short port)
	{
		//PRECONDITIONS:Server is not running.

		//here's where we ask the framework to start listening
		//on the relevant port, and start up server threads
		//as needed.

		boost::thread_group threads;

		//Open a socket to listen for new connections.
		Network::ServerSocket TheServerSocket(port);

		while(ClientConnectionPending(&TheServerSocket))
		{
			Network::AcceptedSocket* pAccepter=new Network::AcceptedSocket(TheServerSocket);//blocks, waiting for a client.

			//Note that the socket object must be deleted in the newly created thread.
			//see comments in ThreadSpecificServer::operator()

			//having created an AcceptedSocket object, we pass it to a new thread
			threads.create_thread(Implementation::ThreadSpecificServer(pAccepter,*this));
		}
		//if we get here, the kill flag has been raised, so wait for
		//all threads to terminate nicely...
		threads.join_all();
	}


	//!Functor that gets passed to new thread.
	/*!
		Gets called by newly created server thread, and runs
		server_ in new thread.
	*/
	struct ServerThreadStarter
	{
		Server& server_;
		short port_;
		ServerThreadStarter(Server& server,short port):server_(server),port_(port){}
		void operator()()
		{
			server_.Serve(port_);
		}
	};


	//!Starts Server running in new thread.
	/*!
		This is useful if your application is more than just a DICOM
		server (for example, a GUI app that can receive DICOM C-STORE
		requests) as it enables you to run a server on a different thread
		than the main event loop.
	*/
	void Server::ServeInNewThread(short port)
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);
		if(ServerThread_)
			return;//we're already doing it....


		ServerThreadStarter s(*this,port);
		ServerThread_=new boost::thread(s);
	}


	//!This is kind of pointless as there is only one application context acceptable in the standard.
	bool Server::IsAcceptableApplicationContext(const UID& uid)
	{
		return (uid==APPLICATION_CONTEXT);
	}

	/*
	Callback setting functions.
	*/

	void Server::SetCheckLocalAETCallback(StringCheckFunction f)
	{
		boost::mutex::scoped_lock scoped_lock(AETMutex_);
		CheckLocalAET=f;
	}
	void Server::SetCheckRemoteAETCallback(StringCheckFunction f)
	{
		boost::mutex::scoped_lock scoped_lock(AETMutex_);
		CheckRemoteAET=f;
	}



	void Server::AssociationNegotiated(const primitive::AAssociateRQ& request)
	{
		return CurrentLogger_->AssociationNegotiated(request);

	}
	void Server::AssociationTerminated()
	{
		CurrentLogger_->AssociationTerminated();
	}

	bool Server::IsAcceptableRemoteApplicationTitle(const std::string& title)
	{
		boost::mutex::scoped_lock scoped_lock(AETMutex_);
		return CheckRemoteAET.empty()?
			false
			:
			CheckRemoteAET(title);
	}
	bool Server::IsAcceptableLocalApplicationTitle(const std::string& title)
	{
		boost::mutex::scoped_lock scoped_lock(AETMutex_);
		return CheckLocalAET.empty()?
			false
			:
			CheckLocalAET(title);
	}
	void Server::AddHandler(const UID& uid,HandlerFunction Handler)
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);
		Handlers_[uid]=Handler;
	}
	void Server::AddFindHandler(const UID& uid,CFindFunction Handler)
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);
		FindHandlers_[uid]=Handler;
	}

	CFindFunction Server::GetFindHandler(const UID& uid)
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);
		std::map<UID,CFindFunction>::iterator I = FindHandlers_.find(uid);
		if(I==FindHandlers_.end())
		{
			LogError("No available handler.");
			throw NoAvailableHandler();//or something
		}
		else
			return I->second;
	}
	HandlerFunction Server::GetHandler(const UID& uid)
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);
		std::map<UID,HandlerFunction>::iterator I = Handlers_.find(uid);
		if(I==Handlers_.end())
		{
			LogError("No available handler.");
			throw NoAvailableHandler();
		}
		else
			return I->second;
	}


	//!i.e, "Do we have a function capable of handling a command of type uid"
	bool Server::IsAcceptableAbstractSyntax(const UID& uid)
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);
		if((Handlers_.find(uid)!=Handlers_.end()) || (FindHandlers_.find(uid)!=FindHandlers_.end()))
			return true;
		if(VERIFICATION_SOP_CLASS==uid)
			return true;//we accept this by default.
		return false;
		//return (Handlers_.find(uid)!=Handlers_.end());
	}

	/*
		currently only support Implicit VR/LittleEndian, but there's
		no real technical reason why we couldn't support the others if
		needed.
	*/

	bool Server::CanHandleTransferSyntax(TransferSyntax &TrnSyntax)
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);
		if(TrnSyntax.UID_ == IMPL_VR_LE_TRANSFER_SYNTAX)
			return true;

		//I think we should also publish that we can handle Explicit Transfer syntax as well, yes no?
		//if(TrnSyntax.UID_ ==  UID_EXPL_VR_LE_TRANSFER_SYNTAX))
		//	return ( true );
		return (false);
	}


	void Server::GetImplementationClass(ImplementationClass &ImpClass)
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);//do we really need this?
		ImpClass.UID_=ImplementationClassUID;
	}

	void Server::GetImplementationVersion(ImplementationVersion &ImpVersion)
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);//not sure we really need this.
		ImpVersion.Name=ImplementationVersionName;
	}



	/*!
		Waits for a connection to be made, checking every few seconds
		for KillFlagRaised.
	*/
	bool Server::ClientConnectionPending(Network::Socket* pSocket)
	{
		for(;;)
		{
			fd_set rfds;//need to reset all this stuff after each call to select.
			timeval tv;

			FD_ZERO(&rfds);
			FD_SET(pSocket->GetSocketDescriptor(), &rfds);
			/* Wait up to five seconds.  We may want to tune this value.*/
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			if(KillFlagRaised())
			{
				LogMessage("Kill flag raised...");
				break;
			}
			//select() examines the state of the socket.
			int retval = select((int)pSocket->GetSocketDescriptor()+1, &rfds, NULL, NULL, &tv);

			if(KillFlagRaised())
				break;
			if(retval==-1)
				throw SystemError("Select");
			if (retval)
				return true;
		}
		LogMessage ("Kill flag raised, accepting no more connections.");
		return false;
	}



	/*!
		presumably should call RaiseKillFlag and wait until all
		threads are stopped?

		Should it raise an error if it's not running?
	*/
	void Server::Stop()
	{
		boost::mutex::scoped_lock scoped_lock(mutex_);

		RaiseKillFlag();
		if(ServerThread_)
		{
			ServerThread_->join();
			delete ServerThread_;
			ServerThread_=NULL;
		}
	}



	bool Server::KillFlagRaised()
	{

		boost::mutex::scoped_lock lock(killflag_mutex);
		return KillFlag;
	}
	void Server::RaiseKillFlag()
	{
		boost::mutex::scoped_lock lock(killflag_mutex);
		KillFlag=true;
	}


	void Server::LogError(std::string Error)
	{
		CurrentLogger_->LogError(Error);
	}

	void Server::LogMessage(std::string Message)
	{
		CurrentLogger_->LogMessage(Message);
	}

	void Server::SetLogger(Logger* logger)
	{
		CurrentLogger_=logger;
	}

}//namespace dicom




