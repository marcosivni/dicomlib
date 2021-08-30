#ifndef SERVICE_BASE_HPP_23847239487238
#define SERVICE_BASE_HPP_23847239487238
#include <string>
//#include "socket/Socket.hpp"
#include "Buffer.hpp"
#include "DataSet.hpp"
#include "TransferSyntax.hpp"
#include "pdata.hpp"
#include "aaac.hpp"
#include "aarj.hpp"
#include "UIDs.hpp"

namespace dicom
{


	//!Thrown if connection is aborted.
	struct AssociationAborted : public dicom::exception
	{
		primitive::AAbortRQ abort_request_;
		AssociationAborted (const primitive::AAbortRQ& abort_request) 
			: dicom::exception("Association Aborted"),
			abort_request_(abort_request)
		{}
	};

	//!Holds shared functionality for dicom client and server classes.
	/*!
		Manages reading and writing control messages and datasets to/from
		a socket.

		Keeps track of conditions under which association was set up
	*/
	struct ServiceBase
	{

		//ServiceBase(Network::Socket* socket);
		ServiceBase();

		virtual ~ServiceBase();


		void Write(MessageControlHeader::Code msgHead, const DataSet& ds,const UID& AbstractSyntaxUID, TS ts);

		void Write(Buffer& buffer,MessageControlHeader::Code msgHead,BYTE PresentationContextID,UINT32 MaxPDULength);

		void WriteCommand(const DataSet& ds,const UID& uid);

		void WriteDataSet(const dicom::DataSet& ds, const UID& uid, TS ts = TS(IMPL_VR_LE_TRANSFER_SYNTAX)/*IMPL_VR_LE*/);

		void ParseRawVRIntoDataSet(PDataTF& p_data_tf,DataSet& data,TS ts);

		bool Read(DataSet& ds,TS ts=TS(IMPL_VR_LE_TRANSFER_SYNTAX)/*::IMPL_VR_LE*/);

		BYTE GetPresentationContextID(const UID& uid);
		BYTE GetPresentationContextID(const UID& AbsUID,	const UID& TrnUID);

		//bool GetTransferSyntaxUID(BYTE, UID& TrnUID);

		UID GetTransferSyntaxUID(BYTE PresentationContextID);
	

		//Following two parameters keep a record of the conditions under which
		//this services association was set up.

		//!The association we accepted.
		primitive::AAssociateRQ AAssociateRQ_;

		//!The presentation contexts we accepted.
		std::vector<primitive::PresentationContextAccept>	AcceptedPresentationContexts_;

		//!The socket on which we're communicating
		/*!
			Currently this pointer is managed externally to this class, which
			is unfortunate.  I'd like to tighten this up.
			Should this be a pure virtual function rather than a member?

			TODO  make a pure virtual function
		*/

        //virtual Network::Socket* GetSocket()=0;

		//Network::Socket* socket_;


	};
}//namespace dicom
#endif//SERVICE_BASE_CLASS_23847239487238
