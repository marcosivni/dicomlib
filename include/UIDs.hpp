#ifndef HPP_INCLUDE_GUARD_57343948
#define HPP_INCLUDE_GUARD_57343948

#include <string>
#include "UID.hpp"
namespace dicom
{
	/*
		Should the following constants be strings or UIDs?
	*/

	/*
		Should these be in a sub-namespace?
	*/



	/*
		transfer syntaxes
	*/


	//!Implicit VR, Little Endian
	const UID IMPL_VR_LE_TRANSFER_SYNTAX			= UID("1.2.840.10008.1.2");

	//!Explicit VR, Little Endian
	const UID EXPL_VR_LE_TRANSFER_SYNTAX			= UID("1.2.840.10008.1.2.1");

	//!Not sure what this is!  Probably shouldn't advertise as supporting it!
	const UID DEFLATED_EXPL_VR_LE_TRANSFER_SYNTAX	= UID("1.2.840.10008.1.2.1.99");

	//!Explicit VR, Big Endian
	const UID EXPL_VR_BE_TRANSFER_SYNTAX			= UID("1.2.840.10008.1.2.2");


	//JPEG encoding:

	//!Baseline JPEG Compression (coding Process 1)
	/*!
		This must be supported by an implementation that supports any 8-bit lossy compression.
		See Part 5 Section 8.2.1
	*/
	const UID JPEG_BASELINE_TRANSFER_SYNTAX 		= UID("1.2.840.10008.1.2.4.50");

	//!Lossless JPEG compression (coding process 14, first-order horizontal prediction)
	/*!
		This must be supported by an implementation that supports any lossless compression.
		See Part 5 Section 8.2.1
	*/
		//has to be supported if we do any lossless jpeg, see note 
	const UID JPEG_LOSSLESS_NON_HIERARCHICAL		= UID("1.2.840.10008.1.2.4.70");


	/*
		There are more to still go in here, mostly to do with JPEG encoding.
		These are in the range 1.2.840.10008.1.2.4.50 to 1.2.840.10008.1.2.4.70

		Part 5, Annex A.4 says that the entire dataset in these cases is encoded
		Little Endian, Explicit VR, and then provides descriptions of how pixel
		data is to be encoded.  This is outside the scope of this library,
		but we should at least recognize the transfer syntaxes

	*/



	/*
		End of transfer syntaxes
	*/

	const UID MEDIA_DIR_STORAGE_SOP_CLASS			= UID("1.2.840.10008.1.3.10");

	//!Part 7, Annex A.21 says that this is the ONLY application context name
	const UID APPLICATION_CONTEXT					= UID("1.2.840.10008.3.1.1.1");

	const UID MODALITY_PPS_SOP_CLASS				= UID("1.2.840.10008.3.1.2.3.3");


	//!Where is this defined?
	
	const UID VERIFICATION_SOP_CLASS	        	= UID("1.2.840.10008.1.1");


	/*
	*	Following are defined in Part 4, Table B.5-1
	*/

	const UID CR_IMAGE_STORAGE_SOP_CLASS	    		= UID("1.2.840.10008.5.1.4.1.1.1");
	const UID CT_IMAGE_STORAGE_SOP_CLASS	    		= UID("1.2.840.10008.5.1.4.1.1.2");
	const UID USOLD_MF_IMAGE_STORAGE_SOP_CLASS  		= UID("1.2.840.10008.5.1.4.1.1.3");
	const UID US_MF_IMAGE_STORAGE_SOP_CLASS  			= UID("1.2.840.10008.5.1.4.1.1.3.1");
	const UID MR_IMAGE_STORAGE_SOP_CLASS	    		= UID("1.2.840.10008.5.1.4.1.1.4");
	const UID USOLD_IMAGE_STORAGE_SOP_CLASS				= UID("1.2.840.10008.5.1.4.1.1.6");
	const UID US_IMAGE_STORAGE_SOP_CLASS			    = UID("1.2.840.10008.5.1.4.1.1.6.1");
	const UID SC_IMAGE_STORAGE_SOP_CLASS			    = UID("1.2.840.10008.5.1.4.1.1.7");
	const UID NM_IMAGE_STORAGE_SOP_CLASS		    	= UID("1.2.840.10008.5.1.4.1.1.20");
	const UID XA_IMAGE_STORAGE_SOP_CLASS			    = UID("1.2.840.10008.5.1.4.1.1.12.1");
	const UID XRF_IMAGE_STORAGE_SOP_CLASS				= UID("1.2.840.10008.5.1.4.1.1.12.2");
	const UID XA2_IMAGE_STORAGE_SOP_CLASS				= UID("1.2.840.10008.5.1.4.1.1.12.3");
	const UID PET_IMAGE_STORAGE_SOP_CLASS				= UID("1.2.840.10008.5.1.4.1.1.128");
	const UID DX_PRES_IMAGE_STORAGE_SOP_CLASS			= UID("1.2.840.10008.5.1.4.1.1.1.1");
	const UID DX_PROC_IMAGE_STORAGE_SOP_CLASS			= UID("1.2.840.10008.5.1.4.1.1.1.1.1");
	const UID MAMMO_PRES_IMAGE_STORAGE_SOP_CLASS		= UID("1.2.840.10008.5.1.4.1.1.1.2");
	const UID MAMMO_PROC_IMAGE_STORAGE_SOP_CLASS		= UID("1.2.840.10008.5.1.4.1.1.1.2.1");
	const UID BASIC_TEXT_SR_STORAGE_SOP_CLASS			= UID("1.2.840.10008.5.1.4.1.1.88.11");
	const UID ENHANCED_SR_STORAGE_SOP_CLASS				= UID("1.2.840.10008.5.1.4.1.1.88.22");
	const UID COMPREHENSIVE_SR_STORAGE_SOP_CLASS		= UID("1.2.840.10008.5.1.4.1.1.88.33");
	const UID INTRAORAL_PRES_IMAGE_STORAGE_SOP_CLASS	= UID("1.2.840.10008.5.1.4.1.1.1.3");
	const UID INTRAORAL_PROC_IMAGE_STORAGE_SOP_CLASS	= UID("1.2.840.10008.5.1.4.1.1.1.3.1");


	//!Feed in one of the above entries and get a human-readable string in return.
	std::string GetUIDName(UID StorageSOP);
	/*
		See Part 4, Section C.3.1

		Defined in Part 4, Sections C.6.1.3, C.6.3.3, and ???
	*/

	const UID PATIENT_ROOT_QR_FIND_SOP_CLASS		= UID("1.2.840.10008.5.1.4.1.2.1.1");
	const UID PATIENT_ROOT_QR_MOVE_SOP_CLASS		= UID("1.2.840.10008.5.1.4.1.2.1.2");
	const UID PATIENT_ROOT_QR_GET_SOP_CLASS		= UID("1.2.840.10008.5.1.4.1.2.1.3");

	const UID STUDY_ROOT_QR_FIND_SOP_CLASS	= UID("1.2.840.10008.5.1.4.1.2.2.1");
	const UID STUDY_ROOT_QR_MOVE_SOP_CLASS	= UID("1.2.840.10008.5.1.4.1.2.2.2");
	const UID STUDY_ROOT_QR_GET_SOP_CLASS	= UID("1.2.840.10008.5.1.4.1.2.2.3");

	const UID PATIENT_STUDY_ONLY_QR_FIND_SOP_CLASS	= UID("1.2.840.10008.5.1.4.1.2.3.1");
	const UID PATIENT_STUDY_ONLY_QR_MOVE_SOP_CLASS	= UID("1.2.840.10008.5.1.4.1.2.3.2");
	const UID PATIENT_STUDY_ONLY_QR_GET_SOP_CLASS	= UID("1.2.840.10008.5.1.4.1.2.3.3");

	const UID MODALITY_WORKLIST_SOP_CLASS	= UID("1.2.840.10008.5.1.4.31");

}//namespace dicom
#endif //HPP_INCLUDE_GUARD_57343948
