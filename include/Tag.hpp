
#ifndef TAG_HPP_INCLUDE_GUARD_5738938745
#define TAG_HPP_INCLUDE_GUARD_5738938745


#include <utility>//for std::pair
#include "Types.hpp"


namespace dicom
{

	//!Hexadecimal values of the 'Tag' part of Tag/Value pairs
	/*!
		These are defined in Part 6, section 6.

		Not every elemented listed in 6/6 is represented here yet, feel
		free to add in missing entries as needed.

		A Tag in DICOM is represented as an ordered pair of 16-bit numbers.
		For convenience, in this library we represent them as a single
		32-bit enum.  This just means that we need to be careful when
		reading to/from a stream (eg file or socket) that we treat it as
		two 16 bit numbers, not one 32 bit number.  Use the functions
		GroupTag(), ElementTag() and makeTag() for this purpose.
	*/
	enum Tag
	{
		// DICOM command fields
		TAG_NULL                      = 0x00000000,
		TAG_AFF_SOP_CLASS_UID         = 0x00000002,
		TAG_REQ_SOP_CLASS_UID         = 0x00000003,
		TAG_CMD_FIELD                 = 0x00000100,
		TAG_MSG_ID                    = 0x00000110,
		TAG_MSG_ID_RSP                = 0x00000120,
		TAG_MOVE_DEST                 = 0x00000600,
		TAG_PRIORITY                  = 0x00000700,
		TAG_DATA_SET_TYPE             = 0x00000800,
		TAG_STATUS                    = 0x00000900,
		TAG_OFFEND_ELEM               = 0x00000901,
		TAG_ERR_COMMENT               = 0x00000902,
		TAG_ERR_ID                    = 0x00000903,
		TAG_AFF_SOP_INST_UID          = 0x00001000,
		TAG_REQ_SOP_INST_UID          = 0x00001001,
		TAG_EVENT_TYPE_ID             = 0x00001002,
		TAG_ATTR_ID_LIST              = 0x00001005,
		TAG_ACTION_TYPE_ID            = 0x00001008,
		TAG_NUM_REMAIN_SUBOP          = 0x00001020,
		TAG_NUM_COMPL_SUBOP           = 0x00001021,
		TAG_NUM_FAIL_SUBOP            = 0x00001022,
		TAG_NUM_WARN_SUBOP            = 0x00001023,
		TAG_MOVE_ORIG_AET             = 0x00001030,
		TAG_MOVE_ORIG_MSG_ID          = 0x00001031,

		// DICOM file meta elements
		TAG_FILE_INFO_GR_LEN          = 0x00020000,
		TAG_FILE_INFO_VERS            = 0x00020001,
		TAG_MEDIA_SOP_CLASS_UID       = 0x00020002,
		TAG_MEDIA_SOP_INST_UID        = 0x00020003,
		TAG_TRANSFER_SYNTAX_UID       = 0x00020010,
		TAG_IMPL_CLASS_UID            = 0x00020012,
		TAG_IMPL_VERS_NAME            = 0x00020013,
		TAG_SRC_AET                   = 0x00020016,
		TAG_PRIV_INFO_CREATOR_UID     = 0x00020100,
		TAG_PRIV_INFO                 = 0x00020102,

		// DICOM directory structuring elements
		TAG_FILE_SET_ID               = 0x00041130,
		TAG_DESC_FILE_ID              = 0x00041141,
		TAG_CHAR_SET_DESC_FILE        = 0x00041142,
		TAG_OFFSET_FIRST_REC          = 0x00041200,
		TAG_OFFSET_LAST_REC           = 0x00041202,
		TAG_FILE_SET_CONS             = 0x00041212,
		TAG_REC_SEQ                   = 0x00041220,
		TAG_OFFSET_NEXT_REC           = 0x00041400,
		TAG_REC_IN_USE                = 0x00041410,
		TAG_OFFSET_REF_LOWER          = 0x00041420,
		TAG_REC_TYPE                  = 0x00041430,
		TAG_PRIV_REC_UID              = 0x00041432,
		TAG_REF_FILE_ID               = 0x00041500,
		TAG_MRDR_OFFSET               = 0x00041504,
		TAG_REF_FILE_SOP_CLASS_UID    = 0x00041510,
		TAG_REF_FILE_SOP_INST_UID     = 0x00041511,
		TAG_REF_FILE_TS_UID           = 0x00041512,
		TAG_NUM_OF_REF                = 0x00041600,

		TAG_CHAR_SET                  = 0x00080005,
		TAG_IMAGE_TYPE                = 0x00080008,
		TAG_INST_CREATE_DATE          = 0x00080012,
		TAG_INST_CREATE_TIME          = 0x00080013,
		TAG_INST_CREATOR_UID          = 0x00080014,
		TAG_SOP_CLASS_UID             = 0x00080016,
		TAG_SOP_INST_UID              = 0x00080018,
		TAG_STUDY_DATE                = 0x00080020,
		TAG_SERIES_DATE				= 0x00080021,
		TAG_ACQUISITION_DATE			= 0x00080022,
		TAG_IMAGE_DATE                = 0x00080023,
		TAG_STUDY_TIME                = 0x00080030,
		TAG_SERIES_TIME               = 0x00080031,
		TAG_ACQUITION_TIME            = 0x00080032,
		TAG_IMAGE_TIME                = 0x00080033,

		TAG_ACCESS_NO                 = 0x00080050,
		TAG_QR_LEVEL                  = 0x00080052,
		TAG_RETR_AET                  = 0x00080054,
		TAG_MODALITY                  = 0x00080060,
		TAG_CONVERSION_TYPE           = 0x00080064,
		TAG_PRESENTATION_TYPE         = 0x00080068,
		TAG_MANUFACTOR                = 0x00080070,
		TAG_INSTITUT_NAME             = 0x00080080,
		TAG_INSTITUT_ADDRESS          = 0x00080081,
		TAG_REF_PHYS_NAME             = 0x00080090,
		TAG_REF_PHYS_ADDR             = 0x00080092,
		TAG_REF_PHYS_TEL              = 0x00080094,

		TAG_CODE_VALUE				= 0x00080100,
		TAG_CODING_SCHEME_DESIGNER	= 0x00080102,
		TAG_CODE_MEANING				= 0x00080104,

		TAG_STATION_NAME              = 0x00081010,
		TAG_STUDY_DESC                = 0x00081030,
		TAG_PROC_CODE_SEQ             = 0x00081032,
		TAG_SERIES_DESC               = 0x0008103e,
		TAG_INSTITUT_DEPT_NAME        = 0x00081040,
		TAG_PERF_PHYS_NAME            = 0x00081050,
		TAG_READ_PHYS_NAME            = 0x00081060,
		TAG_OPERATOR_NAME             = 0x00081070,
		TAG_ADMIT_DIAG_DESC           = 0x00081080,
		TAG_MANFAC_MODEL_NAME         = 0x00081090,
		TAG_REF_STUDY_SEQ             = 0x00081110,
		TAG_REF_STUDY_COMPONENT_SEQ	  = 0x00081111,
		TAG_REF_PAT_SEQ               = 0x00081120,
		TAG_REF_IMAGE_SEQ             = 0x00081140,
		TAG_REF_SOP_CLASS_UID         = 0x00081150,

		TAG_REF_SOP_INST_UID          = 0x00081155,
		TAG_REF_SOP_SEQ				= 0x00081199,

		TAG_PAT_NAME                  = 0x00100010,
		TAG_PAT_ID                    = 0x00100020,
		TAG_PAT_BIRTH_DATE            = 0x00100030,
		TAG_PAT_SEX                   = 0x00100040,
		TAG_OTHER_PAT_ID              = 0x00101000,
		TAG_PAT_AGE                   = 0x00101010,
		TAG_PAT_SIZE                  = 0x00101020,
		TAG_PAT_WEIGHT                = 0x00101030,
		TAG_PAT_ADDR                  = 0x00101040,
		TAG_PAT_TEL                   = 0x00102154,
		TAG_OCCUPATION                = 0x00102180,
		TAG_ADDITIONAL_PT_HISTORY		= 0x001021b0,
		TAG_PAT_COMMENT               = 0x00104000,

		TAG_BODY_PART_EXAMINED        = 0x00180015,
		TAG_SLANT_ANGLE				= 0x00180059,
		TAG_KVP						= 0x00180060,

		TAG_DEVICE_SERIAL_NUMBER		= 0x00181000,
		TAG_SOFTWARE_VERSION			= 0x00181020,

		//added by morgan@sten.sunnybrook.utoronto.ca

		TAG_SECONDARY_CAPTURE_DATE		= 0x00181012,

		//// added by ming
		//TAG_FILE_NAME                 = 0x00181017,
		//TAG_MOVE_HOST                 = 0x00181018,
		//TAG_MOVE_PORT                 = 0x00181019,
		//TAG_MOVE_APP                  = 0x00181021,
		//TAG_PASSWORD                  = 0x00181022,
		//// End 

		/*
			The above tag names make no sense, and were obviously added
			to facilitate some undocumented hack.  I have replaced them
			with more appropriate names.  morgan@sten.sunnybrook.utoronto.ca
		*/
		TAG_HARDCOPY_DEVICE_MANUFACTURER				= 0x00181017,
		TAG_SECONDARY_CAPTURE_DEVICE_MODEL_NAME			= 0x00181018,
		TAG_SECONDARY_CAPTURE_DEVICE_SOFTWARE_VERSION	= 0x00181019,
		TAG_VIDEO_IMAGE_FORMAT_ACQUIRED					= 0x00181022,

		TAG_PROT_NAME                 = 0x00181030,
		TAG_EXPOSURE_MAS		        = 0x00181152,
		TAG_IMGR_PIXEL_SPACING        = 0x00181164,
		TAG_ANODE_MATERIAL	        = 0x00181191,
		TAG_SENSITIVITY		        = 0x00181191,


		//following block added by Trevor Morgan 
		//-------------------------------------------
		TAG_BODY_PART_THICKNESS		= 0x001811a0,
		TAG_COMPRESSION_FORCE			= 0x001811a2,
		TAG_ESTIMATED_RADIOGRAPHIC_MAGNIFICATION_FACTOR=0x00181114,
		TAG_FOCAL_SPOT				= 0x00181190,
		TAG_COLLIMATOR_LEFT_EDGE	= 0x00181702,
		TAG_COLLIMATOR_RIGHT_EDGE	= 0x00181704,
		TAG_COLLIMATOR_UPPER_EDGE= 0x00181706,
		TAG_COLLIMATOR_LOWER_EDGE	= 0x00181708,
		//-------------------------------------------


		TAG_VIEW_POSITION				= 0x00185101,
		TAG_FILTER_MATERIAL	        = 0x00187050,

		TAG_EXPOSURE_CONTROL_MODE     = 0x00187060,
		TAG_EXPOSURE_CONTROL_MODE_DESC= 0x00187062,

		TAG_STUDY_INST_UID            = 0x0020000d,
		TAG_SERIES_INST_UID           = 0x0020000e,
		TAG_STUDY_ID                  = 0x00200010,
		TAG_SERIES_NO                 = 0x00200011,
		TAG_ACQUISITION_NO            = 0x00200012,
		TAG_IMAGE_NO                  = 0x00200013,

		TAG_PATIENT_ORIENTATION       = 0x00200020,
		TAG_OVERLAY_NO                = 0x00200022,
		TAG_CURVE_NO                  = 0x00200024,
		TAG_LUT_NO                    = 0x00200026,
		TAG_IMAGE_ORIENTATION         = 0x00200037,
		TAG_LATERALITY	            = 0x00200060,
		TAG_IMAGE_LATERALITY	        = 0x00200062,

		TAG_NO_PAT_REL_STUDIES        = 0x00201200,
		TAG_NO_PAT_REL_SERIES         = 0x00201202,
		TAG_NO_PAT_REL_IMAGES         = 0x00201204,
		TAG_NO_STUDY_REL_SERIES       = 0x00201206,
		TAG_NO_STUDY_REL_IMAGES       = 0x00201208,
		TAG_NO_SERIES_REL_IMAGES      = 0x00201209,

		TAG_SAMPLES_PER_PX            = 0x00280002,
		TAG_PHOTOMETRIC               = 0x00280004,
		TAG_ROWS                      = 0x00280010,
		TAG_COLUMNS                   = 0x00280011,
		TAG_PLANES                    = 0x00280012,
		TAG_PIXEL_SPACING             = 0x00280030,

		TAG_BITS_ALLOC                = 0x00280100,
		TAG_BITS_STORED               = 0x00280101,
		TAG_HIGH_BIT                  = 0x00280102,
		TAG_PX_REPRESENT              = 0x00280103,

		TAG_PXL_INTENSITY_RELATIONSHIP= 0x00281040,
		TAG_PXL_INTENSITY_SIGN        = 0x00281041,
		TAG_WINDOW_CENTER             = 0x00281050,
		TAG_WINDOW_WIDTH              = 0x00281051,
		TAG_RESCALE_INTERCEPT         = 0x00281052,
		TAG_RESCALE_SLOPE             = 0x00281053,
		TAG_RESCALE_TYPE              = 0x00281054,

		TAG_RED_PAL_LUT               = 0x00281201,
		TAG_GREEN_PAL_LUT             = 0x00281202,
		TAG_BLUE_PAL_LUT              = 0x00281203,
		TAG_SEG_RED_PAL_LUT           = 0x00281221,
		TAG_SEG_GREEN_PAL_LUT         = 0x00281222,
		TAG_SEG_BLUE_PAL_LUT          = 0x00281223,

		TAG_ICON_IMAGE_SEQ            = 0x00880200,

		TAG_TOPIC_TITLE               = 0x00880904,
		TAG_TOPIC_SUBJECT             = 0x00880906,
		TAG_TOPIC_AUTHOR              = 0x00880910,
		TAG_TOPIC_KEYWORDS            = 0x00880912,

        TAG_STUDY_STATUS_ID	          = 0x0032000a,
		TAG_REQ_PHYS                  = 0x00321032,
		TAG_REQ_SERVICE               = 0x00321033,
		TAG_REQ_PROC_DESC             = 0x00321060,
		TAG_REQ_CONTRAST_AGENT        = 0x00321070,


		TAG_ADMISSION_ID              = 0x00380010,
		TAG_ROUTE_OF_ADMISS           = 0x00380016,
		TAG_ADMIT_DATE                = 0x00380020,

		TAG_SPS_STATION_AET           = 0x00400001,
		TAG_SPS_START_DATE            = 0x00400002,
		TAG_SPS_START_TIME            = 0x00400003,
		TAG_SPS_END_DATE              = 0x00400004,
		TAG_SPS_END_TIME              = 0x00400005,
		TAG_SPS_PERF_PHYS_NAME        = 0x00400006,
		TAG_SPS_DESC                  = 0x00400007,
		TAG_SPS_ACTION_SEQ            = 0x00400008,
		TAG_SPS_ID                    = 0x00400009,
		TAG_SPS_STATION_NAME          = 0x00400010,
		TAG_SPS_LOCATION              = 0x00400011,
		TAG_PRE_MEDICATION            = 0x00400012,
		TAG_SPS_STATUS                = 0x00400020,
		TAG_SPS_SEQ                   = 0x00400100,
		TAG_REF_STANDALONE_SOP_INST_SEQ = 0x00400220,
		TAG_PERF_STATION_AET          = 0x00400241,
		TAG_PERF_STATION_NAME         = 0x00400242,
		TAG_PERF_LOCATION             = 0x00400243,
		TAG_PPS_START_DATE            = 0x00400244,
		TAG_PPS_START_TIME            = 0x00400245,
		TAG_PPS_END_DATE              = 0x00400250,
		TAG_PPS_END_TIME              = 0x00400251,
		TAG_PPS_STATUS                = 0x00400252,
		TAG_PPS_ID                    = 0x00400253,
		TAG_PPS_DESC                  = 0x00400254,
		TAG_PERF_TYPE_DESC            = 0x00400255,
		TAG_PERF_ACTION_SEQ           = 0x00400260,
		TAG_SPS_ATTRIB_SEQ            = 0x00400270,
		TAG_REQ_ATTRIB_SEQ            = 0x00400275,
		TAG_COMMENT_PPS               = 0x00400280,
		TAG_QUANTITY_SEQ              = 0x00400293,
		TAG_QUANTITY                  = 0x00400294,
		TAG_MEASURING_UNITS_SEQ       = 0x00400295,
		TAG_BILLING_ITEM_SEQ          = 0x00400296,
		TAG_TOT_TIME_FLUOROS          = 0x00400300,
		TAG_TOT_NUM_EXPOS             = 0x00400301,
		TAG_ENTRANCE_DOSE             = 0x00400302,
		TAG_EXPOSED_AREA              = 0x00400303,
		TAG_DISTANCE_SOURCE_ENTRANCE  = 0x00400306,
		TAG_COMMENT_RADIATION_DOSE    = 0x00400310,
		TAG_BILLING_PPS_SEQ           = 0x00400320,
		TAG_FILM_CONSUM_SEQ           = 0x00400321,
		TAG_BILLING_SUPPL_DEVICES_SEQ = 0x00400324,
		TAG_REF_PPS_SEQ               = 0x00400330,
		TAG_PERF_SERIES_SEQ           = 0x00400340,
		TAG_COMMENTS_ON_SPS           = 0x00400400,
		TAG_REQ_PROC_ID               = 0x00401001,
		TAG_REASON_REQ_PROC           = 0x00401002,
		TAG_REQ_PROC_PRIORITY         = 0x00401003,
		TAG_PAT_TRANS_ARRANGE         = 0x00401004,
		TAG_REQ_PROCEDURE_LOC         = 0x00401005,
		TAG_PLACER_ORDER_NUM_PROC     = 0x00401006,
		TAG_FILLER_ORDER_NUM_PROC     = 0x00401007,
		TAG_CONFID_CODE               = 0x00401008,
		TAG_REPORT_PRIORITY           = 0x00401009,
		TAG_RECIPIENTS_OF_RESULT      = 0x00401010,
		TAG_REQ_PROC_COMMENT          = 0x00401400,
		TAG_REASON_ISRQ               = 0x00402001,
		TAG_ISSUE_DATE_ISRQ           = 0x00402004,
		TAG_ISSUE_TIME_ISRQ           = 0x00402005,
		TAG_PLACER_ORDER_NO_ISRQ      = 0x00402006,
		TAG_FILLER_ORDER_NO_ISRQ      = 0x00402007,
		TAG_ORDER_ENTEREDBY           = 0x00402008,
		TAG_ORDER_ENTERER             = 0x00402009,
		TAG_ORDER_CALLBACK_TEL        = 0x00402010,
		TAG_ISRQ_COMMENTS             = 0x00402400,
		TAG_CONFID_CONSTRAIN_PAT_DESC = 0x00403001,
		TAG_RELATIONSHIP_TYPE			= 0x0040a010,
		TAG_VERIFYING_ORGANIZATION	= 0x0040a027,
		TAG_VERIFICATION_DATE_TIME	= 0x0040a030,
		TAG_OBSERVATION_DATE_TIME		= 0x0040a032,
		TAG_VALUE_TYPE				= 0x0040a040,
		TAG_CONCEPT_NAME_CODE_SEQ		= 0x0040a043,
		TAG_CONTINUITY_OF_CONTENT		= 0x0040a050,
		TAG_VERIFYING_OBSERVER_SEQ	= 0x0040a073,
		TAG_VERIFYING_OBSERVER_NAME	= 0x0040a075,
		TAG_VERF_OBSERVER_ID_CODE_SEQ	= 0x0040a088,
		TAG_DATE_TIME					= 0x0040a120,
		TAG_MEASURED_VALUE_SEQ		= 0x0040a300,
		TAG_PREDECESSOR_DOCUMENTS_SEQ	= 0x0040a360,
		TAG_REFERENCED_REQUEST_SEQ	= 0x0040a370,
		TAG_PERFORMED_PROCEDURE_CODE_SEQ= 0x0040a372,
		TAG_REQEUSTED_PROCEDURE_EVIDENCE_SEQ= 0x0040a375,
		TAG_PERTINENT_OTHER_EVIDENCE_SEQ= 0x0040a385,
		TAG_COMPLETION_FLAG			= 0x0040a491,
		TAG_COMPLETION_FLAG_DESC		= 0x0040a492,
		TAG_VERIFICATION_FLAG			= 0x0040a493,
		TAG_CONTENT_TEMPLATE_SEQ		= 0x0040a504,
		TAG_IDENTICAL_DOCUMENTS_SEQ	= 0x0040a525,
		TAG_CONTENT_SEQ				= 0x0040a730,
		TAG_TEMPLATE_ID				= 0x0040db00,
		TAG_TEMPLATE_VERSION			= 0x0040db06,
		TAG_TEMPLATE_LOCAL_VERSION	= 0x0040db07,
		TAG_TEMPLATE_EXTENSION_FLAG	= 0x0040db0b,
		TAG_TEMPLATE_EXTENSION_ORGANIZATION_UID	= 0x0040db0c,
		TAG_TEMPLATE_EXTENSION_CREATOR_UID	= 0x0040db0d,
		TAG_REFERENCED_CONTENT_ITEM_ID= 0x0040db73,

		TAG_GRAPHIC_DATA				= 0x00700022,
		TAG_GRAPHIC_TYPE				= 0x00700023,


		TAG_FILM_SESSION_LABEL        = 0x20000050,

		TAG_IMAGE_POS                 = 0x20200010,

		TAG_EXECUTION_STATUS          = 0x21000020,

		TAG_PRINTER_NAME              = 0x21100030,
		TAG_PRINT_QUEUE_ID            = 0x21100099,

		TAG_RESULT_ID                 = 0x40080040,
		TAG_INTERPRET_TRANS_DATE      = 0x40080108,
		TAG_INTERPRET_AUTHOR          = 0x4008010C,
		TAG_INTERPRET_DIAG_DESC       = 0x40080115,
		TAG_INTERPRET_DIAG_CODE_SEQ   = 0x40080117,
		TAG_INTERPRET_ID              = 0x40080200,
		TAG_INTERPRET_TYPE_ID         = 0x40080210,
		TAG_INTERPRET_STATUS_ID       = 0x40080212,

        TAG_PIXEL_DATA                = 0x7fe00010,

		TAG_DATA_SET_PADDING          = 0xfffcfffc,

		//sequence control


		TAG_ITEM                      = 0xfffee000,
		TAG_ITEM_DELIM_ITEM           = 0xfffee00d,
		TAG_SEQ_DELIM_ITEM            = 0xfffee0dd
	};


	//!This value is always used to indicate undefined length
	/*!
	See e.g. Part5, Section 7.5.1
	*/
	const UINT32 UNDEFINED_LENGTH=0xffffffff;

	//typedef std::vector<Tag> TagVector;
	//Tags are represented as two words.
	//think there is a better name for these?
	inline UINT16 GroupTag(Tag tag) { return UINT16(tag >> 16); }
	inline UINT16 ElementTag(Tag tag) { return UINT16(tag); }
	inline Tag makeTag(UINT16 gggg, UINT16 eeee) {
		return Tag((UINT32(gggg) << 16) | eeee); }

	//should this stuff really be in this file?
	//why does it appear that we're flipping the Tag data?
	/*
	inline Buffer& operator<<(Buffer& buf, Tag tag) {
	return buf << getGrTag(tag) << getElTag(tag); }

	inline Buffer& operator>>(Buffer& buf, Tag& tag) {
	UINT16 gggg, eeee;
	buf >> gggg >> eeee;
	tag = makeTag(gggg,eeee);
	return buf;
	}
	*/
	struct TagMask : public std::pair<UINT32,UINT32>
	{
		TagMask(UINT32 tag, UINT32 mask) : std::pair<UINT32,UINT32>(tag, mask) {}
	};

	const TagMask TAG_CURVE_DATA	(0x50003000,0xff00ffff);
	const TagMask TAG_OVERLAY_DATA	(0x60003000,0xff00ffff);

	inline bool operator==(const Tag& tag, const TagMask& mask)
	{
		return (tag & mask.second) == mask.first;
	}

	inline bool operator==(const TagMask& mask, const Tag& tag)
	{
		return operator==(tag, mask);
	}

	inline bool operator!=(const Tag& tag, const TagMask& tagMask)
	{
		return !operator==(tag, tagMask);
	}

	inline bool operator!=(const TagMask& tagMask, const Tag& tag)
	{
		return !operator==(tag, tagMask);
	}

	//should probably split this into N and C commands.
	namespace Command
	{
		typedef UINT16 Code;
		const Code
			//C
			C_STORE_RQ			= 0x0001,	C_STORE_RSP			= 0x8001,
			C_GET_RQ			= 0x0010,	C_GET_RSP			= 0x8010,
			C_FIND_RQ			= 0x0020,	C_FIND_RSP			= 0x8020,
			C_MOVE_RQ			= 0x0021,	C_MOVE_RSP			= 0x8021,
			C_ECHO_RQ			= 0x0030,	C_ECHO_RSP			= 0x8030,
			//N
			N_EVENT_REPORT_RQ	= 0x0100,	N_EVENT_REPORT_RSP  = 0x8100,
			N_GET_RQ			= 0x0110,	N_GET_RSP			= 0x8110,
			N_SET_RQ			= 0x0120,	N_SET_RSP			= 0x8120,
			N_ACTION_RQ			= 0x0130,	N_ACTION_RSP	    = 0x8130,
			N_CREATE_RQ			= 0x0140,	N_CREATE_RSP	    = 0x8140,
			N_DELETE_RQ			= 0x0150,	N_DELETE_RSP	    = 0x8150,
			//cancel
			C_CANCEL_RQ			= 0x0fff;
	}
	namespace Priority
	{
		typedef UINT16 Code;
		const Code
			LOW    = 0x0002,
			MEDIUM = 0x0000,
			HIGH   = 0x0001;
	}

	namespace DataSetStatus
	{
		typedef UINT16 Code;
		const Code
			NO_DATA_SET		= 0x0101,
			YES_DATA_SET  	= 0x0102;
	}

	//!See Part 7, Annex C for detailed definitions.
	namespace Status
	{
		typedef UINT16 Code;
		const Code
			SUCCESS       	 	= 0x0000,
			UNKNOWN_SOP_CLASS 	= 0x0122,
			PENDING			 	= 0xff00,
			PENDING1      	 	= 0xff01,
			WARNING				= 0xb000;
	}


}//namespace dicom
#endif //TAG_HPP_INCLUDE_GUARD_5738938745
