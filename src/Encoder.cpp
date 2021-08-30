/************************************************************************
*	DICOMLIB
*	Copyright 2003 Sunnybrook and Women's College Health Science Center
*	Implemented by Trevor Morgan  (morgan@sten.sunnybrook.utoronto.ca)
*
*	See LICENSE.txt for copyright and licensing info.
*************************************************************************/
#include <iostream>
#include "Encoder.hpp"
#include "Exceptions.hpp"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/date_parsing.hpp>
#include "iso646.h"
using namespace std;

namespace dicom
{

	class Encoder
	{
	public:
		Encoder(Buffer& buffer,const DataSet& ds,TS ts):buffer_(buffer),dataset_(ds),ts_(ts){}
		void Encode();
	private:
		Buffer& buffer_;
		const DataSet& dataset_;
		TS ts_;

		void EncodeElement(const DataSet::value_type& element)const;
		void WriteLengthAndVR(UINT32 length,VR vr);
		void SendRange(DataSet::const_iterator Begin,DataSet::const_iterator End);
		
		void SendSequence(const Sequence& sequence);

		template<VR vr>
		void SendFundamentalType(DataSet::const_iterator Begin,DataSet::const_iterator End)
		{
			typedef typename TypeFromVR<vr>::Type Type;
			BOOST_STATIC_ASSERT(boost::is_fundamental<Type>::value);

			UINT32 Length = (UINT32)dataset_.count(Begin->first);//Should be identical to End-Begin, but we don't have subtraction operator available.
			WriteLengthAndVR(sizeof(Type)*Length,vr);

			for(;Begin!=End;Begin++)
			{
				Type data;
				Begin->second >> data;
				buffer_ << data;
			}
		}

		void SendAttributeTag(DataSet::const_iterator Begin,DataSet::const_iterator End)
		{
			UINT32 Length = (UINT32)dataset_.count(Begin->first);//Should be identical to End-Begin, but we don't have subtraction operator available.

			WriteLengthAndVR(sizeof(Tag)*Length,VR_AT);
			for(;Begin!=End;Begin++)
			{
				Tag data;
				Begin->second >> data;
				buffer_ << data;
			}
		}


		template <VR vr>
		void SendString(DataSet::const_iterator Begin,DataSet::const_iterator End)
		{
			StaticVRCheck<std::string,vr>();
			Tag tag=Begin->first;
			//delimiter character is '\'
			std::string StringToSend;
			for(;Begin!=End;Begin++)
			{
				std::string s;
				const Value& value=Begin->second;
				if((vr!=value.vr()) or(tag!=Begin->first))// this block is basically an ASSERT, ie I expect it to never be entered.
					//we have a major problem.
					throw dicom::exception("Some inconsistency in dataset.");
				value >> s;
				StringToSend.append(s);
				StringToSend.append(1,'\\');//delimiter
			}
			StringToSend.erase(StringToSend.end()-1);//remove last delimiter.
			if(StringToSend.size() bitand 0x01)//length is odd
				StringToSend.append(1,' ');//string length must be even.

			WriteLengthAndVR((UINT32)StringToSend.size(),vr);
			buffer_<<StringToSend;
		}
		void SendUID(DataSet::const_iterator Begin, DataSet::const_iterator End)
		{
			Tag tag=Begin->first;
			std::string StringToSend;
			for(;Begin!=End;Begin++)
			{
				UID uid;
				const Value& value=Begin->second;
				if((VR_UI!=value.vr()) or(tag!=Begin->first))
					throw dicom::exception("Some inconsistency in dataset.");
				value >> uid;
				StringToSend.append(uid.str());
				StringToSend.append(1,'\\');//delimiter
			}
			StringToSend.erase(StringToSend.end()-1);//remove last delimiter.
			if(StringToSend.size() bitand 0x01)	//length is odd
				StringToSend.append(1,'\0');	//length must be even, NULL character is used for padding UIDs

			WriteLengthAndVR((UINT32)StringToSend.size(),VR_UI);
			buffer_<<StringToSend;
		}

		void SendDate(DataSet::const_iterator Begin,DataSet::const_iterator End)
		{
			Tag tag=Begin->first;
			std::string StringToSend;
			for(;Begin!=End;Begin++)
			{
				const Value& value=Begin->second;
				if(value.vr()!=VR_DA or tag!=Begin->first)
					throw dicom::exception("Inconsistency in dataset.");

				boost::gregorian::date d=value.Get<boost::gregorian::date>();
				StringToSend.append(boost::gregorian::to_iso_string(d));
				StringToSend.append(1,'\\');
			}
			StringToSend.erase(StringToSend.end()-1);//remove last delimiter.
			if(StringToSend.size() bitand 0x01)//length is odd
				StringToSend.append(1,' ');//string length must be even.

			WriteLengthAndVR((UINT32)StringToSend.size(),VR_DA);
			buffer_<<StringToSend;
		}
		//previous two functions are virtually identical apart from small detail in
		//inner loop.  Please rationalise.

		void SendOB(DataSet::const_iterator Begin, DataSet::const_iterator End)
		{	
			typedef TypeFromVR<VR_OB>::Type Type;
			
			Tag tag = Begin->first;
			int fragments=dataset_.count(tag);

			Enforce(ts_.isEncoded() || (1==fragments),"Only encoded data can have multiple image fragments.");

			if(1==fragments)//just send the data
			{
				const Type& ByteVector = Begin->second.Get<Type>();
				WriteLengthAndVR((UINT32)ByteVector.size(),VR_OB);
				buffer_.AddVector(ByteVector);
				return;		
			}
			else	//send the data as a series of fragments as defined in Part5 Annex 4
			{
				WriteLengthAndVR(UNDEFINED_LENGTH,VR_OB);

				//send empty offset table - not supported.

				buffer_ << TAG_ITEM;
				buffer_ << UINT32(0x00);//no offset table.

				for(;Begin!=End;Begin++)
				{
					buffer_ << TAG_ITEM;
					const Type& ByteVector = Begin->second.Get<Type>();
					buffer_ << UINT32(ByteVector.size());
					buffer_.AddVector(ByteVector);
				}

				buffer_ << TAG_SEQ_DELIM_ITEM;
				buffer_ << UINT32(0x00);			
			}

		}
	};



	void Encoder::SendRange(DataSet::const_iterator Begin,DataSet::const_iterator End)
	{
	//we might want an ASSERT here to check that the range truly is consistent,
	//i.e. only consists of elements sharing the same Tag and VR...

		Tag tag = Begin->first;
		VR vr = Begin->second.vr();

		buffer_ << tag;

		switch(vr)
		{
		case VR_AE:
			return SendString<VR_AE>(Begin,End);
			//returSendFundamentalTypepe<VR_AE>(Begin,End);

		case VR_AS:
			return SendString<VR_AS>(Begin,End);
		case VR_AT:
			return SendAttributeTag(Begin,End);
		case VR_CS:
			return SendString<VR_CS>(Begin,End);
		case VR_DA:
			return SendDate(Begin,End);
		case VR_DS:
			return SendString<VR_DS>(Begin,End);
		case VR_DT:
			return SendString<VR_DT>(Begin,End);
		case VR_FD:
			return SendFundamentalType <VR_FD>(Begin,End);
		case VR_FL:
			return SendFundamentalType<VR_FL>(Begin,End);
		case VR_IS:
			return SendString<VR_IS>(Begin,End);
		case VR_LO:
			return SendString<VR_LO>(Begin,End);
		case VR_LT:
			return SendString<VR_LT>(Begin,End);
		case VR_OB:
			return SendOB(Begin,End);
		case VR_OW:
			{
				typedef TypeFromVR<VR_OW>::Type Type;
				const Type& WordVector = Begin->second.Get<Type>();

				UINT32 ByteLength=WordVector.size()*2;
				WriteLengthAndVR(ByteLength,VR_OW);

				buffer_.AddVector(WordVector);
			}
			return;
		case VR_PN:
			return SendString<VR_PN>(Begin,End);
		case VR_SH:
			return SendString<VR_SH>(Begin,End);
		case VR_SL:
			return SendFundamentalType<VR_SL>(Begin,End);
		case VR_SQ:
		{
			const Sequence& sequence=Begin->second.Get<Sequence>();
			return SendSequence(sequence);
		}
		case VR_SS:
			return SendFundamentalType<VR_SS>(Begin,End);
		case VR_ST:
			return SendString<VR_ST>(Begin,End);
		case VR_TM:
			return SendString<VR_TM>(Begin,End);
		case VR_UI:
			return SendUID(Begin,End);
		case VR_UL:
			return SendFundamentalType<VR_UL>(Begin,End);
		case VR_UN:
			{
				typedef TypeFromVR<VR_UN>::Type Type;
				const Type& ByteVector = Begin->second.Get<Type>();
				WriteLengthAndVR((UINT32)ByteVector.size(),VR_UN);
				buffer_.AddVector(ByteVector);
			}
			return;
		case VR_US:
			return SendFundamentalType<VR_US>(Begin,End);
		case VR_UT:
			return SendString<VR_UT>(Begin,End);
		default:
			cout << "Unknown VR: " << vr  << " in EncodeElement()" << endl;
			throw BadVR(vr);
		}
	}


	void Encoder::WriteLengthAndVR(UINT32 length,VR vr)
	{
		if(ts_.isExplicitVR())
		{

			buffer_ << UINT16(vr);
			if( VR_UN == vr || VR_SQ == vr || VR_OW == vr || VR_OB == vr || VR_UT == vr)
			{

				buffer_ << UINT16(0);	//reserved
				buffer_ << length;		//4 bytes
			}
			else
			{
				buffer_<<UINT16(length);//2 bytes
			}
		}
		else
		{
			//no VR info sent
			buffer_ << length;			//4 bytes
		}

	}

	void Encoder::Encode()
	{
		DataSet::const_iterator I = dataset_.begin();

		while(I!=dataset_.end())
		{
			Tag tag=I->first;
			pair<DataSet::const_iterator,DataSet::const_iterator> p=dataset_.equal_range(tag);
   			SendRange(p.first,p.second);
			I=p.second;
		}
	}

	/*!

		This should be simpler than Decoder::DecodeSequence(), because we only need
		to implement ONE way of sending multiple data sets.
		Which shall we use?  Exlicit length, or Sequence Delimitation Items?  I
		think the second is the simplest...
	*/

	void Encoder::SendSequence(const Sequence& sequence)
	{
		//write tag
		//write undefined length
		//for each item:
			//write item tag
			//write undefined length
			//write dataset
			//write item delim. tag
			//write length 0
		//write sequence delim tag
		//write length 0

		WriteLengthAndVR(UNDEFINED_LENGTH,VR_SQ);

		for(Sequence::const_iterator I=sequence.begin();I!=sequence.end();I++)
		{
			buffer_ << TAG_ITEM;

			buffer_<<UNDEFINED_LENGTH;
			Encoder E(buffer_,*I,ts_);
			E.Encode();
//write dataset
			buffer_ << TAG_ITEM_DELIM_ITEM;

			buffer_<<UINT32(0x00);
		}
		buffer_ << TAG_SEQ_DELIM_ITEM;
		buffer_<<UINT32(0x00);
	}






	void WriteToBuffer(const DataSet& data, Buffer& buffer, TS transfer_syntax)
	{
		Encoder E(buffer,data,transfer_syntax);
		E.Encode();
	}


}//namespace dicom
