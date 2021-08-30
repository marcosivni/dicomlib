#include "UIDs.hpp"
#include<map>
using namespace std;
namespace dicom
{
	namespace
	{

		//!Construct a map of human-readable names for each SOP class.

		void PopulateMap(map<string,string>& THEMAP)
		{
			THEMAP[UID_CR_IMAGE_STORAGE_SOP_CLASS]="Computed Radiography Image Storage";
			THEMAP[UID_CT_IMAGE_STORAGE_SOP_CLASS]="CT Image Storage";
			THEMAP[UID_USOLD_MF_IMAGE_STORAGE_SOP_CLASS]="Ultrasound Multi-frame Image Storage (Retired)";
			THEMAP[UID_US_MF_IMAGE_STORAGE_SOP_CLASS]="Ultrasound Multi-frame Image Storage";
			THEMAP[UID_MR_IMAGE_STORAGE_SOP_CLASS]="MR Image Storage";
			THEMAP[UID_USOLD_IMAGE_STORAGE_SOP_CLASS]="Ultrasound Image Storage (Retired)";
			THEMAP[UID_US_IMAGE_STORAGE_SOP_CLASS]="Ultrasound Image Storage";
			THEMAP[UID_SC_IMAGE_STORAGE_SOP_CLASS]="Secondary Capture Image Storage";
			THEMAP[UID_NM_IMAGE_STORAGE_SOP_CLASS]="Nuclear Medicine Image Storage";
			THEMAP[UID_XA_IMAGE_STORAGE_SOP_CLASS]="X-Ray Angiographic Image Storage";
			THEMAP[UID_XRF_IMAGE_STORAGE_SOP_CLASS]="X-Ray Radiofluoroscopic Image Storage";
			THEMAP[UID_XA2_IMAGE_STORAGE_SOP_CLASS]="X-Ray Angiographic Bi-Plane Image Storage (Retired)";
			THEMAP[UID_PET_IMAGE_STORAGE_SOP_CLASS]="Positron Emission Tomography Image Storage";
			THEMAP[UID_DX_PRES_IMAGE_STORAGE_SOP_CLASS]="Digital X-Ray Image Storage - For Presentation";
			THEMAP[UID_DX_PROC_IMAGE_STORAGE_SOP_CLASS]="Digital X-Ray Image Storage - For Processing";
			THEMAP[UID_MAMMO_PRES_IMAGE_STORAGE_SOP_CLASS]="Digital Mammography X-Ray Image Storage - For Presentation";
			THEMAP[UID_MAMMO_PROC_IMAGE_STORAGE_SOP_CLASS]="Digital Mammography X-Ray Image Storage - For Processing";
			THEMAP[UID_INTRAORAL_PRES_IMAGE_STORAGE_SOP_CLASS]="Digital Intra-oral X-Ray Image Storage - For Presentation";
			THEMAP[UID_INTRAORAL_PROC_IMAGE_STORAGE_SOP_CLASS]="Digital Intra-oral X-Ray Image Storage - For Processing";
			THEMAP[UID_BASIC_TEXT_SR_STORAGE_SOP_CLASS]="Basic Text Structured Report Storage";
			THEMAP[UID_ENHANCED_SR_STORAGE_SOP_CLASS]="Enhanced Structured Report Storage";
			THEMAP[UID_COMPREHENSIVE_SR_STORAGE_SOP_CLASS]="Comprehensive Structured Report Storage";
		}

		map<string,string>THEMAP;
		bool MapIsPopulated=false;
	}//namespace

	string GetUIDName(string uid)
	{
		if(!MapIsPopulated)
			PopulateMap(THEMAP);//threading issues?
		if(THEMAP.find(uid)!=THEMAP.end()
			return THEMAP[uid];
		else
			return "Unknown";
	}
}
