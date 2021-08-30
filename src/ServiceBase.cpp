/************************************************************************
*	DICOMLIB
*	Copyright 2003 Sunnybrook and Women's College Health Science Center
*	Implemented by Trevor Morgan  (morgan@sten.sunnybrook.utoronto.ca)
*
*	See LICENSE.txt for copyright and licensing info.
*************************************************************************/

#include <vector>
#include <iostream>
#include "ServiceBase.hpp"
#include "pdata.hpp"
#include "Encoder.hpp"
#include "Decoder.hpp"
#include "aarj.hpp"
#include "aarq.hpp"

using std::cout;using std::endl;
using std::vector;

namespace dicom
{
	using namespace primitive;

	ServiceBase::ServiceBase()
	{}

	//ServiceBase::ServiceBase(Network::Socket* socket):socket_(socket)
	//{
	//	//might want to do some validation here, e.g. check socket state?
	//}

	ServiceBase::~ServiceBase()
	{

	}

	void ServiceBase::Write(MessageControlHeader::Code msgHead, const DataSet& ds,
		const UID& AbstractSyntaxUID, TS ts)
	{

		//UID absUID(as);
		UID tsUID(ts.getUID());

		BYTE PresentationContextID = GetPresentationContextID(AbstractSyntaxUID);

		//check that it's valid for the current transfer syntax.
		/*
			I've removed this check because this is used for sending commands, that are
			always Implicit/LittleEndian, even if that transfer syntax hasn't been negotiated.
		*/
		//if (!GetPresentationContextID(AbstractSyntaxUID, tsUID))
		//	throw exception("No presentation context ID");//bad transfer syntax, or something?

		int ByteOrder=ts.isBigEndian()?__BIG_ENDIAN:__LITTLE_ENDIAN;
		Buffer buffer(ByteOrder);

		dicom::WriteToBuffer(ds,buffer,ts);

		UINT32 MaxPDULength=AAssociateRQ_.UserInfo_.MaxSubLength_.MaximumLength_;
		Write(buffer,msgHead,PresentationContextID,MaxPDULength);
	}

	void ServiceBase::WriteCommand(const DataSet& ds,const UID& uid )
	{
		Write(MessageControlHeader::COMMAND, ds, uid, TS(IMPL_VR_LE_TRANSFER_SYNTAX)/*::IMPL_VR_LE*/);//Commands MUST have VR/LE Transfer Syntax.
	}

	void ServiceBase::WriteDataSet(const dicom::DataSet& ds, const UID& uid, TS ts)
	{
		Write(MessageControlHeader::DATASET, ds, uid, ts);
	}

	/*!
		This functionality used to be in PDATATF
	*/
	void ServiceBase::Write(Buffer& buffer,MessageControlHeader::Code msgHead,BYTE PresentationContextID,UINT32 MaxPDULength)
	{
		//slice buffer and send as a series of P_DATA_TF thingies

		//each P_DATA_TF shall have only one PDV, to make life easier.

		//buffer.reset();

		if(buffer.position()!=buffer.begin())
		{
			throw exception("buffer.position()!=buffer.begin(), in ServiceBase::Write()");
		}

		if(MaxPDULength==0)//Does this ever happen?  Is this valid to do?  Must have
							//rationale here please.   TODO.
			MaxPDULength=static_cast<UINT32>(buffer.size());


		Network::Socket* socket=GetSocket();

		while(buffer.position()!=buffer.end())
		{

		/*
			See Part 8, table 9-22 and 9-23 to understand what we're sending here,
			and realise that I've opted to only ever send ONE pdv with each P-DATA-TF.
		*/
			*socket << BYTE(0x04);
			*socket << BYTE(0x00);
			
			UINT32 BytesLeftToSend=static_cast<UINT32>((buffer.end()-buffer.position()));
			
			const UINT32 BytesInThisChunk=std::min(BytesLeftToSend,MaxPDULength-6);


			*socket << UINT32(BytesInThisChunk+6);
			//now the PDV item
			//We should use the PDV class to mediate this!

			*socket << UINT32(BytesInThisChunk+2);
			*socket << PresentationContextID;


			if(buffer.position()+(BytesInThisChunk)==buffer.end())
				msgHead |=MessageControlHeader::LAST_FRAGMENT;

			*socket	<< msgHead;

			//then send data...
			BYTE* Begin=&(*(buffer.position()));
			socket->Sendn(Begin,(BytesInThisChunk));

			//buffer.position()+=(BytesInThisChunk);//this is now a bug...
			buffer.Increment(BytesInThisChunk);
		}
	}

	/*!
		returns false on association termination, else true.


		(We really shouldn't _handle_ the termination here, functions should do
		one and only one thing.

		This needs to be re-thought - it's really ugly...

	*/
	bool ServiceBase::Read(DataSet& ds,TS ts)
	{
		int ByteOrder=ts.isBigEndian()?__BIG_ENDIAN:__LITTLE_ENDIAN;

		PDataTF p_data_tf(ByteOrder);
		p_data_tf.MsgStatus=0;


		//Specified at Part 8/figure 9-2
		/*
			What we basically do here is pull data off of the TCP/IP stream onto
			a PDataTF object(which has a BUFFER), and then at the end read
			from the buffer onto the DataSet.  I'm not entirely happy about this
			design, but there doesn't seem to be any other feasible approach.
		*/

		Network::Socket* socket=GetSocket();
		while(true)//loop, apparently implying that we can expect more than one PDATATF object.
		{
			BYTE		ItemType;
			*socket >> ItemType;
			switch ( ItemType )//shouldn't be 1, 2 or 3.
			{
			case	0x04:	// P-DATA-TF
				{

					p_data_tf.ReadDynamic(*socket);

					if (p_data_tf.MsgStatus > 0)//what is the corresponding 'else' ?
					{
						ParseRawVRIntoDataSet(p_data_tf,ds,ts);
						return true;
					}
					else
						break;	//keep going through loop
					
				}
				break;
			case	0x05:	// A-RELEASE-RQ
				{
					AReleaseRQ release_request;
					release_request.ReadDynamic(*socket);

					// also drop
					AReleaseRP release_response;
					release_response.Write(*socket);
					return false;
				}
				// 			case	0x06:	// A-RELEASE-RP//shouldn't happen for a server

			case	0x07:	// A-ABORT-RQ
				{
					//shouldn't we try to read the abort request here
					AAbortRQ abort_request(*socket);
					throw AssociationAborted(abort_request);
				}
			default:
				{
					AAbortRQ abort_request(AAbortRQ::DICOM_SERVICE_PROVIDER,AAbortRQ::UNRECOGNIZED_PDU);
					abort_request.Write (*socket);
					throw BadItemType(ItemType,0);
				}
			}
		}

	}


	void ServiceBase::ParseRawVRIntoDataSet(PDataTF& p_data_tf,DataSet& ds,TS ts)
	{
		//UID uid("");
		//UID uid=GetTransferSyntaxUID(p_data_tf.PresentationContextID);

		///*if(!GetTransferSyntaxUID(p_data_tf.PresentationContextID,uid))
		//	throw std::runtime_error("Couldn't get Transfer Syntax UID");*/

		//TS ts(uid);

		/*
			the buffer's internal pointer is somehow getting corrupted, so we reset it.
			(maybe the internal memory is getting re-allocated, so begin() is somewhere
			else than when it was first constructed?
		*/

		//p_data_tf.buffer_.reset();

		if(p_data_tf.buffer_.position()!=p_data_tf.buffer_.begin())
		{
			throw exception("buffer.position()!=buffer.begin(), in ServiceBase::ParseRawVRIntoDataSet()");
		}

		ReadFromBuffer(p_data_tf.buffer_,ds,ts);
	}

	/*
		I'm still not sure about the following two functions.
	*/



	/*
		return the ID member of the first PresentationContext in
		AAssociateRQ_.ProposedPresentationContexts_ whose AbsSyntax.UID_ matches uid.
	*/

	BYTE ServiceBase::GetPresentationContextID(const UID& uid)
	{
		//shouldn't we just be interested in accepted presentation contexts?
		const vector<PresentationContext>&	PCArray = AAssociateRQ_.ProposedPresentationContexts_;
		//const vector<PresentationContext>& PCArray=AcceptedPresentationContexts_;
		
		
		size_t Index = 0;
		while ( Index < PCArray.size())
		{
			const PresentationContext& PresContext = PCArray.at ( Index );
			if(PresContext.AbsSyntax_.UID_ == uid)
				return (PresContext.ID_);
			++Index;
		}
		//	we could replace above with a call to find_if ???


		throw dicom::exception("Couldn't get Presentation Context ID");	// You're probably trying to use a SOP class that
																		// wasn't negotiated during association!

	}

	/*!
		Get the PCID for a given AbsUID and TrnUID
	*/

	BYTE ServiceBase::GetPresentationContextID(const UID& AbsUID, const UID& TrnUID)
	{
		size_t Index = 0;

		//shouldn't we just be interested in accepted presentation contexts?
		
		while ( Index < AAssociateRQ_.ProposedPresentationContexts_.size())
		{
			PresentationContext	PresContext = AAssociateRQ_.ProposedPresentationContexts_.at ( Index );
			
			if(PresContext.AbsSyntax_.UID_ == AbsUID)
			{
				PresentationContextAccept	PCA;
				size_t Index = 0;
				while (Index < AcceptedPresentationContexts_.size() )
				{

					PCA = AcceptedPresentationContexts_.at ( Index );
					if(PCA.TrnSyntax_.UID_ == TrnUID &&
						PCA.PresentationContextID_ == PresContext.ID_)
					{
						return ( PCA.PresentationContextID_);
					}
					++Index;
				}
			}
			++Index;
		}
		throw dicom::exception("given presentation context does not exist with specified transfer syntax.");
	}

	//BYTE ServiceBase::GetPresentationContextID(const UID& AbsUID, const UID& TrnUID)
	//{
	//	struct match
	//	{
	//		match(const UID& a, const UID& t):a_(a),t_(t){}
	//		bool operator()(AcceptedPresentationContext a)
	//		{
	//			return (a.first.UID_==a_ && a.second.UID_==t_);
	//		}
	//	};
	//	std::vector<AcceptedPresentationContext>::const_iterator I =
	//		std::find_if(AcceptedPresentationContexts_.begin(),AcceptedPresentationContexts_.end()
	//		match(AbsUID,TrnUID));
	//	if(I!=AcceptedPresentationContexts_.end())
	//		return 
	//	/*
	//		search 	AcceptedPresentationContexts_
	//		for matching pair.
	//	*/
	//	throw dicom::exception("given presentation context does not exist with specified transfer syntax.");
	//}

	


	UID ServiceBase::GetTransferSyntaxUID(BYTE PresentationContextID)
	{
		typedef std::vector<primitive::PresentationContextAccept>::const_iterator Iter;
		
		for(Iter I=AcceptedPresentationContexts_.begin();I!=AcceptedPresentationContexts_.end();I++)
			if(I->PresentationContextID_==PresentationContextID)
				return I->TrnSyntax_.UID_;
		throw std::runtime_error("Couldn't identify Presentation Context");
	}

	//bool ServiceBase::GetTransferSyntaxUID(BYTE PCID, UID& uid)
	//{
	//	PresentationContextAccept	PCA;

	//	size_t	Index = 0;

	//	while (Index < AcceptedPresentationContexts_.size() )
	//	{
	//		PCA = AcceptedPresentationContexts_.at ( Index );
	//		if(PCA.PresentationContextID_ == PCID)
	//		{
	//			uid = PCA.TrnSyntax_.UID_;
	//			return ( true );
	//		}
	//		++Index;
	//	}
	//	return ( false );//should be a throw...
	//}
}//namespace dicom
