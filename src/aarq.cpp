/************************************************************************
*	DICOMLIB
*	Copyright 2003 Sunnybrook and Women's College Health Science Center
*	Implemented by Trevor Morgan  (morgan@sten.sunnybrook.utoronto.ca)
*
*	See LICENSE.txt for copyright and licensing info.
*************************************************************************/
#include <algorithm>

#include "aarq.hpp"
#include "UIDs.hpp"
#include "Exceptions.hpp"
using std::for_each;

/*IMPL_VERSION_NAME
There is much repeated functionality in this file,
implying a need for a shared base class.

Some of the size members can be consolidated?
*/

/*
	We need a clearer approach to managing message sizes.  Generally speaking,
	the Size() member function does two things.  It sets the Length_ member
	and returns a value.  Generally Size() is the entire byte-size of the message,
	and Length_ is the size of the message minus the initial couple of fields.
	(The initial fields generally being 'item type', 'reserved' and 'length'

	So Length_ corresponds to the Length_ field, and Size() to the entire
	message Size(). 

	There are two problems here from a coding perspective.  One is that the 
	Size() function has non-obvious side effects, and the other is that we 
	seem to be duplicating information, which is asking for bugs, as has
	already occured a few times.  My proposal is to lose Length_ completely
	as a member, and when it is needed use Size()-(size of initial fields).

	I think this would increase data integrity.


	IMPORTANT!  Are all Size() fields UINT16 ????
	NO THEY ARE NOT!  EXTRA CAUTION REQUIRED!
*/


/*************************************************************************
*
* Application Context Class
*
************************************************************************/



namespace dicom
{

	

	void EnforceItemType(BYTE Given, BYTE Expected)
	{
		if(Expected!=Given)
			throw BadItemType(Given,Expected);
	}

	namespace primitive
	{
#ifndef _MSC_VER
		/*
		difference of opinion here between gcc and msvc as to whether constant static members
		need to be instantiated seperately.
		*/
		const BYTE PresentationContext::ItemType_;
		const BYTE PresentationContext::Reserved1_;
		const BYTE PresentationContext::Reserved2_;
		const BYTE PresentationContext::Reserved3_;
		const BYTE PresentationContext::Reserved4_;

		const BYTE MaximumSubLength::ItemType_;
		const BYTE MaximumSubLength::Reserved1_;
		const UINT16 MaximumSubLength::Length_;

		const BYTE UserInformation::ItemType_;
		const BYTE UserInformation::Reserved_;

		const BYTE AAssociateRQ::ItemType_;
		const BYTE AAssociateRQ::Reserved1_;
		const UINT16 AAssociateRQ::ProtocolVersion_;
		const UINT16 AAssociateRQ::Reserved2_;

		const BYTE ApplicationContext::ItemType_;
		const BYTE ApplicationContext::Reserved_;

		const BYTE AbstractSyntax::ItemType_;
		const BYTE AbstractSyntax::Reserved_;

		const BYTE TransferSyntax::ItemType_;
		const BYTE TransferSyntax::Reserved_;

		const BYTE ImplementationClass::ItemType_;
		const BYTE ImplementationClass::Reserved_;

		const BYTE ImplementationVersion::ItemType_;
		const BYTE ImplementationVersion::Reserved_;

		const BYTE SCPSCURoleSelect::ItemType_;
		const BYTE SCPSCURoleSelect::Reserved_;
#endif

		/*
		CRIMINALLY DANGEROUS CODE AHEAD! PLEASE FIX!
		*/

		namespace
		{	//	these  aren't thread safe, which could lead to horrendous complications if
			//	you ever trust the data in them.  Only use them for dummy data
			//	that never gets read.
			
			BYTE tmpBYTE;
			UINT32 tmpUINT32;
			UINT16 tmpUINT16;
		}

		ApplicationContext::ApplicationContext(const UID	&uid)
			:UID_(uid)
		{
		}


		UINT32 ApplicationContext::Size()const
		{
			return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + UID_.str().size() );
		}

		void ApplicationContext::ReadDynamic(Network::Socket& socket)
		{
			socket >> tmpBYTE;
			UINT16 UIDLength;
			socket >> UIDLength;

			std::string s(UIDLength, ' ');
			socket.Read(s);
			UID_=UID(s);
		}
		void ApplicationContext::Write(Network::Socket& socket)const
		{
			socket << ItemType_;
			socket << Reserved_;
			UINT16 Length=UINT16(UID_.str().size());	//--|
			socket << Length;							//--|these two lines could be amalgamated

			socket.Send(UID_.str());
		}
		/*************************************************************************
		*
		* Abstract Syntax Class
		*
		************************************************************************/


		AbstractSyntax::AbstractSyntax(const UID &uid)
			:UID_(uid)
		{}

		UINT32 AbstractSyntax::Size()
		{
			return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + UID_.str().size());
		}

		void AbstractSyntax::Read(Network::Socket& socket)
		{
			BYTE b;
			socket >> b;
			EnforceItemType(b,ItemType_);
			ReadDynamic(socket);
		}
		void AbstractSyntax::ReadDynamic(Network::Socket& socket)
		{
			socket >> tmpBYTE;
			UINT16 Length;
			socket >> Length;

			//UID_.assign(Length,' ');
			std::string s(Length,' ');
			socket.Read(s);
			UID_=UID(s);
		}
		void AbstractSyntax::Write(Network::Socket& socket)
		{
			socket << ItemType_;
			socket << Reserved_;
			socket << UINT16(UID_.str().size());
			socket.Send(UID_.str());
		}





		TransferSyntax::TransferSyntax(const UID& uid)
			//:ProtocolDataUnit(uid,0x40)
			:UID_(uid)
		{}


		UINT32 TransferSyntax::Size()
		{
			return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + UID_.str().size());
		}

		void TransferSyntax::Write(Network::Socket& socket)
		{
			socket << ItemType_;
			socket << Reserved_;
			socket << UINT16(UID_.str().size());
			socket.Send(UID_.str());
		}

		void TransferSyntax::Read(Network::Socket& socket)
		{
			socket >> tmpBYTE;
			socket >> tmpBYTE;
			UINT16 Length;
			socket >> Length;
			std::string s(Length,' ');

			socket.Read(s);
			UID_=UID(s);
		}
		/************************************************************************
		*
		*  Implementation Class
		*
		************************************************************************/
		ImplementationClass::ImplementationClass(const UID	&uid)
			//:ProtocolDataUnit(uid,0x52)
			:UID_(uid)
		{}

		void ImplementationClass::ReadDynamic(Network::Socket& socket)
		{
			socket >> tmpBYTE;
			UINT16 Length;
			socket >> Length;
			std::string s(Length,' ');

			socket.Read(s);
			UID_=UID(s);
		}

		UINT32 ImplementationClass::Size()
		{
			return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + UID_.str().size() );
		}

		void ImplementationClass::Write(Network::Socket& socket)
		{
			socket << ItemType_;
			socket << Reserved_;
			socket << UINT16(UID_.str().size());
			socket.Send(UID_.str());
		}
		/************************************************************************
		*
		*  Implementation Version
		*
		************************************************************************/

		UINT32 ImplementationVersion::Size()
		{
			if(Name.empty())
				return 0;//message doesn't get sent if it's empty.

			return ( sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) + Name.size() );
		}
		void ImplementationVersion::ReadDynamic(Network::Socket& socket)
		{
			socket >> tmpBYTE;
			UINT16 Length;
			socket >> Length;

			if(Length>16 )
				throw dicom::exception("Implementation Version Length too long.");
			Name.assign(Length,' ');
			socket.Read(Name);
		}
		void ImplementationVersion::Write(Network::Socket& socket)
		{
			if(Name.empty())//message doesn't get sent if it's empty.
				return;
			socket << ItemType_;
			socket << Reserved_;
			socket << UINT16(Name.size());
			socket.Send(Name);
		}
		/************************************************************************
		*
		* Presentation Context
		*
		************************************************************************/

		PresentationContext::PresentationContext(
			const AbstractSyntax& Abs
			,const std::vector<TransferSyntax>& AcceptableTransferSyntaxes
			,BYTE id)
			:
		ID_(id)
			,AbsSyntax_(Abs)
			,TransferSyntaxes_(AcceptableTransferSyntaxes)

		{

		}

		/*!
		This form defaults to ImplicitVR/Little Endian
		*/
		PresentationContext::PresentationContext(
			const AbstractSyntax& Abs
			,BYTE id)
			:ID_(id)
			,AbsSyntax_(Abs)

		{
			TransferSyntaxes_.push_back(TransferSyntax(IMPL_VR_LE_TRANSFER_SYNTAX));
		}

		PresentationContext::PresentationContext()
			:ID_(0x00)
			,AbsSyntax_(UID(""))

		{
		}

		void PresentationContext::AddTransferSyntax(TransferSyntax& Tran)
		{
			TransferSyntaxes_.push_back (Tran);
		}

		void PresentationContext::Write(Network::Socket& socket)
		{

			socket << ItemType_;
			socket << Reserved1_;

			socket << Length_;
			socket << ID_;
			socket << Reserved2_;
			socket << Reserved3_;
			socket << Reserved4_;
			AbsSyntax_.Write(socket);

			for_each(TransferSyntaxes_.begin(),TransferSyntaxes_.end(),WriteToSocket(socket));
		}


		void PresentationContext::ReadDynamic(Network::Socket& socket)
		{
			TransferSyntax	Tran(UID(""));

			socket >> tmpBYTE;

			socket >> Length_;

			socket >> ID_;
			socket >> tmpBYTE;
			socket >> tmpBYTE;
			socket >> tmpBYTE;

			int Count = Length_ - sizeof(BYTE) - sizeof(BYTE) - sizeof(BYTE) - sizeof(BYTE);

			AbsSyntax_.Read(socket);
			Count = Count - AbsSyntax_.Size();

			//Read in the transfer syntaxes...
			while ( Count > 0)
			{
				//Read<TransferSyntax>(Tran,socket);
				Tran.Read(socket);
				//Tran.Read ( socket );
				Count = Count - Tran.Size();
				TransferSyntaxes_.push_back ( Tran );
			}

			Enforce(0==Count);
		}

		UINT32 PresentationContext::Size()
		{
			Length_ = sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE);
			Length_ += AbsSyntax_.Size();
			size_t Index = 0;

			while(Index < TransferSyntaxes_.size() )
			{
				Length_ += TransferSyntaxes_.at(Index).Size();
				++Index;
			}
			return ( Length_ + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16));
		}

		/************************************************************************
		*
		* Maximum Sub Length
		*
		************************************************************************/

		MaximumSubLength::MaximumSubLength()
		{
		}

		MaximumSubLength::MaximumSubLength(UINT32 Max)
		{
			MaximumLength_ = Max;
		}

		void MaximumSubLength::Set(UINT32 Max)
		{
			MaximumLength_ = Max;
		}

		UINT32 MaximumSubLength::Get()
		{
			return ( MaximumLength_ );
		}

		void MaximumSubLength::Write(Network::Socket& socket)
		{
			socket << ItemType_;
			socket << Reserved1_;
			socket << Length_;
			socket << MaximumLength_;
			//socket.Flush
		}

		void MaximumSubLength::ReadDynamic(Network::Socket& socket)
		{
			socket >> tmpBYTE;
			UINT16 Length;
			socket >> Length;
			if(Length!=Length_)
				throw dicom::exception("itemlength of MaximumSubLength must be 0x04");
			socket >> MaximumLength_;

		}

		UINT32 MaximumSubLength::Size()
		{
			return ( Length_ + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16));
		}



		/******** SCP / SCU Role Select ***********/


		SCPSCURoleSelect::SCPSCURoleSelect()
			:UID_("")
		{}



		void SCPSCURoleSelect::Write(Network::Socket& socket)
		{
			socket << ItemType_;
			socket << Reserved_;
			socket << ItemLength;
			socket << static_cast<UINT16>(UID_.str().size());
			socket.Send(UID_.str());

			socket << SCURole_;
			socket << SCPRole_;
		}

		void SCPSCURoleSelect::ReadDynamic(Network::Socket& socket)
		{
			socket >> tmpBYTE;
			socket >> ItemLength;

			UINT16 UIDLength;
			socket >> UIDLength;
			std::string s(UIDLength,' ');
			socket.Read(s);
			UID_=UID(s);
			socket >> SCURole_;
			socket >> SCPRole_;
		}


		//this should be fixed too  FIXME
		UINT32 SCPSCURoleSelect::Size()
		{
			ItemLength = sizeof(UINT16) + UID_.str().size() + sizeof(BYTE) + sizeof(BYTE);
			return ( ItemLength + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16) );
		}

		/************************************************************************
		*
		* User Information
		*
		************************************************************************/

		UserInformation::UserInformation()
			:ImpClass_(UID(""))
			,UserInfoBaggage_ (0)
		{
			
		}



		//do we ever use this?
		void UserInformation::SetMax(MaximumSubLength	&Max)
		{
			MaxSubLength_ = Max;
		}

		//UINT32 UserInformation::GetMax()
		//{
		//	return(MaxSubLength.Get());
		//}

		void UserInformation::Write(Network::Socket& socket)
		{
			socket << ItemType_;
			socket << Reserved_;



			socket << UINT16 (Size()-4);
			//socket << Length_;//I think this should be a call to Size()

			MaxSubLength_.Write(socket);
			ImpClass_.Write(socket);

			//should only send this if it really exists...
			ImpVersion_.Write(socket);
		
			/*
				Note that we don't currently support writing the 
				SCPSCURoleSelect Sub-item (which is optional.)
			*/
		
		
		}
		void UserInformation::ReadDynamic(Network::Socket& socket)
		{
			UserInfoBaggage_ = 0;

			UINT16 length;
			socket >> tmpBYTE;
			socket >> length;
			int BytesLeftToRead = length;

			while(BytesLeftToRead > 0)
			{
				socket >> tmpBYTE;

				switch ( tmpBYTE )
				{
				case	0x51:
					MaxSubLength_.ReadDynamic(socket);
					BytesLeftToRead = BytesLeftToRead - MaxSubLength_.Size();
					break;
				case	0x52:
					ImpClass_.ReadDynamic(socket);
					BytesLeftToRead = BytesLeftToRead - ImpClass_.Size();
					break;
				case	0x54:	// Role selection
					/*
						This is very ugly, the use of UserInfoBaggage_ is not a nice idea.
						It would be better to do some clever trick in SCPSCURole to manage
						optionality, maybe.
					*/
					SCPSCURole_.ReadDynamic(socket);
					BytesLeftToRead = BytesLeftToRead - SCPSCURole_.Size();
					UserInfoBaggage_ += SCPSCURole_.Size();
					break;
				case	0x55:
					ImpVersion_.ReadDynamic(socket);//optional!
					BytesLeftToRead = BytesLeftToRead - ImpVersion_.Size();
					break;
				default:
					throw BadItemType(tmpBYTE,0);

				}
			}
			Enforce(0==BytesLeftToRead,"0!=BytesLeftToRead");
			Enforce((Size()-4)==length,"bad size in UserInformation");

		}

/*
	This has caused us some serious headaches.  The Size() member function
	of our various message classes MUST be re-thought, it's unmaintainable
	in it's current form and has been the root of a number of problems.

	We need a more intelligent, unified way of creating and serializing these
	message classes.  
	 ____        ____________        ____        ____________        ____
	|data| <--> |MessageClass| <--> |Wire| <--> |MessageClass| <--> |data|
     ----        ------------        ----        ------------        ----



	 One approach would be to insist that each message has two constructors.

	 One taking the input data, one taking a socket.

	 class AAssociateRQ
	 {
		AAssociateRQ(socket& s);
		AAssociateRQ(some data);
		Write(socket);
	  }
	 Alternatively, the messages would be data only, and the read/write
	 functions would be seperate.  This might actually make more sense!

	struct AAssociateRQ;

	Read(socket,AAssociateRQ);
	Write(socket,AAssociateRQ);
	
	Most writes require size information, which needs to be either available
	or calculatable.  I'm not sure that we should ever hold onto this.

	Sometimes the information is redundant, or always a constant.

	Currently the Size() member function both modifies state and returns
	information.  This seriously violates the no-side-effects rule.


	In determining size we could either call recursive Size() functions
	on the objects, or write all the data to some kind of a buffer
	and get the size of that???

1)

AAssociateRQ request(data);
request.Write(socket);
and Size() never modifies data?

We should maybe branch to try this out

*/


		UINT16 UserInformation::Size()
		{
			UINT16 length = MaxSubLength_.Size();
			length += ImpClass_.Size();
			
			/*
				now ImpVersion_ and SCPSCURoleSelect are both optional.  This fact needs to be
				taken into account when sending and receiving.

			*/
			
			length += ImpVersion_.Size();


	//		Length_=length;//should this be before previous line?

			length+=UserInfoBaggage_;//need to do this better.
			//problem is, SCP/SCU role is an optional sub-item

			
			return length+4;

			//return ( Length_ + UserInfoBaggage + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT16));
		}

		/************************************************************************
		*
		* AAssociateRQ Packet
		*
		************************************************************************/

		AAssociateRQ::AAssociateRQ()
			:AppContext_(APPLICATION_CONTEXT)
		{
			std::fill(Reserved3_,Reserved3_+32,0);
		}

		AAssociateRQ::AAssociateRQ(const std::string& CallingAp, const std::string& CalledAp)
			:CalledAppTitle_(CalledAp)
			,CallingAppTitle_(CallingAp)
			,AppContext_(APPLICATION_CONTEXT)
		{
			std::fill(Reserved3_,Reserved3_+32,0);
		}



		//!This should be in the constructor I think
		void AAssociateRQ::SetUserInformation(UserInformation& User)
		{
			UserInfo_ = User;
		}

		void AAssociateRQ::Write(Network::Socket& socket)
		{
			Size();
			socket << ItemType_;
			socket << Reserved1_;

			socket << UINT32(Size()-6);//Length_;


			socket << ProtocolVersion_;
			socket << Reserved2_;
			char StringBuffer[16];

			std::fill(StringBuffer,StringBuffer+16,' ');
			std::copy(CalledAppTitle_.begin(),CalledAppTitle_.end(),StringBuffer);
			socket.Send( StringBuffer, StringBuffer+16);

			std::fill(StringBuffer,StringBuffer+16,' ');
			std::copy(CallingAppTitle_.begin(),CallingAppTitle_.end(),StringBuffer);
			socket.Send( StringBuffer,StringBuffer+ 16);

			socket.Send( Reserved3_,Reserved3_+ 32);

			AppContext_.Write(socket);

			for_each(ProposedPresentationContexts_.begin(),ProposedPresentationContexts_.end(),WriteToSocket(socket));

			UserInfo_.Write(socket);
		}

		void AAssociateRQ::Read(Network::Socket& socket)
		{
			BYTE b;

			socket >> b;
			EnforceItemType(b,ItemType_);

			ReadDynamic(socket);
		}

		void AAssociateRQ::ReadDynamic(Network::Socket& socket)
		{


			BYTE				TempByte;

			socket >> tmpBYTE;

			UINT32 length;
			socket >> length;

			socket >> tmpUINT16;
			//check protocol version...
			socket >>	tmpUINT16;

			char StringBuffer[17];
			StringBuffer[16] = '\0';
			socket.Read(StringBuffer,StringBuffer+16);
			//socket.Readn(StringBuffer,16);
			CalledAppTitle_=std::string(StringBuffer);
			StripTrailingWhitespace(CalledAppTitle_);

			//socket.Readn((BYTE *) CallingApTitle, 16);
			//socket.Readn(StringBuffer,16);
			socket.Read(StringBuffer,StringBuffer+16);
			CallingAppTitle_=std::string(StringBuffer);
			StripTrailingWhitespace(CallingAppTitle_);


			//socket.Readn(/*(BYTE *)*/ Reserved3_, 32);
			socket.Read(Reserved3_,Reserved3_+32);

			//CalledApTitle[16] = '\0';
			//CallingApTitle[16] = '\0';

			int BytesLeftToRead = length - sizeof(UINT16) - sizeof(UINT16) - 16 - 16 - 32;
			while(BytesLeftToRead > 0)
			{
				socket >> TempByte;

				switch(TempByte)
				{
				case	0x50:	// user information
					UserInfo_.ReadDynamic(socket);
					BytesLeftToRead = BytesLeftToRead - UserInfo_.Size();// - UserInfo_.UserInfoBaggage;
					break;
				case	0x20:
					{

						PresentationContext PresContext;//I'd rather these two lines were compacted to one 
						PresContext.ReadDynamic(socket);//using a constructor.

						BytesLeftToRead = BytesLeftToRead - PresContext.Size();
						ProposedPresentationContexts_.push_back(PresContext);


					}
					break;
				case	0x10:
					{
						ApplicationContext app_context(APPLICATION_CONTEXT);
						app_context.ReadDynamic(socket);
						BytesLeftToRead = BytesLeftToRead - app_context.Size();
						if(app_context.UID_!=APPLICATION_CONTEXT)
							throw exception("Unsupported Application Context");//this should be throw AssociationException(...)
					}
					break;
				default:
					throw BadItemType(tmpBYTE,0);
				}
			}

			if(BytesLeftToRead)
				throw dicom::exception("BytesLeftToRead!=0 at end of aarq read." );

		}

		UINT32 AAssociateRQ::Size()
		{
			UINT32 length;
			length = sizeof(UINT16) + sizeof(UINT16) + 16 + 16 + 32;
			length += AppContext_.Size();
			size_t Index = 0;


			while(Index < ProposedPresentationContexts_.size())
			{
				length += ProposedPresentationContexts_[Index].Size();
				++Index;
			}
			length += UserInfo_.Size();
			return ( length + sizeof(BYTE) + sizeof(BYTE) + sizeof(UINT32) );
		}

	}//namespace primitive
}//namespace dicom
