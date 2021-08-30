#ifndef VALUE_TO_STREAM_HPP_INCLUDE_GUARD_4397641354
#define VALUE_TO_STREAM_HPP_INCLUDE_GUARD_4397641354
#include "Value.hpp"
#include "VR.hpp"
#include <ostream>
#include <istream>
namespace dicom
{
	//!pipes a string representation of value onto the stream.
	std::ostream& operator <<  (std::ostream& out, const dicom::Value& value);

	//!Create a value from a valid stream.
	Value ValueFromStream(VR vr, std::istream& In);

	//!Get a human readable name for a given VR
	std::string GetVRName(VR vr);//


	//!Converts e.g. VR_US to "US"
	std::string GetVRShortName(VR vr);

	//!pipes short name onto stream
	std::ostream& operator << (std::ostream& out, VR vr);

}
#endif //VALUE_TO_STREAM_HPP_INCLUDE_GUARD_4397641354
