#ifndef PRESENTATION_CONTEXTS_HPP_INCLUDE_GUARD_di82d531bk97k8
#define PRESENTATION_CONTEXTS_HPP_INCLUDE_GUARD_di82d531bk97k8
#include <vector>
#include "aarq.hpp"
#include "TransferSyntax.hpp"
#include "Utility.hpp"


namespace dicom
{
	//!Takes some of the grunt work out of maintaining a list of presentation contexts.
	/*!
		Need standard reference here... TODO
	*/
	class PresentationContexts : public  std::vector<primitive::PresentationContext>
	{
		OddSeries<BYTE> IDGenerator_; 
	public:
		void Add(const dicom::UID& uid);
		void Add(const UID& uid, const TS ts);
	};
}//namespace dicom
#endif//PRESENTATION_CONTEXTS_HPP_INCLUDE_GUARD_di82d531bk97k8
