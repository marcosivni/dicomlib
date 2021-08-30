#include "PresentationContexts.hpp"

namespace dicom
{
	void PresentationContexts::Add(const UID& uid)
	{
		primitive::AbstractSyntax as(uid);
		primitive::PresentationContext p(as,IDGenerator_());
		push_back(p);
	}

	void PresentationContexts::Add(const UID& uid, const TS ts)
	{
		std::vector<primitive::TransferSyntax> transfer_syntaxes;
		transfer_syntaxes.push_back(primitive::TransferSyntax(ts.getUID()));

		primitive::AbstractSyntax as(uid);
		primitive::PresentationContext p(as,transfer_syntaxes,IDGenerator_());
		push_back(p);
	}
}//namespace dicom
