#ifndef ONFI_H
#define ONFI_H

#define ONFI_SIGNATURE	0x49464E4F
#define ONFI_SIZE		256

#define ONFI_REVISION_RESERVED0		0x0001
#define ONFI_REVISION_1_0			0x0002
#define ONFI_REVISION_2_0			0x0004
#define ONFI_REVISION_2_1			0x0008
#define ONFI_REVISION_2_2			0x0010
#define ONFI_REVISION_2_3			0x0020
#define ONFI_REVISION_3_0			0x0040
#define ONFI_REVISION_3_1			0x0080
#define ONFI_REVISION_3_2			0x0100
#define ONFI_REVISION_4_0			0x0200
#define ONFI_REVISION_4_1			0x0400
#define ONFI_REVISION_4_2			0x0800
#define ONFI_REVISION_5_0			0x1000
#define ONFI_REVISION_RESERVED13	0x2000
#define ONFI_REVISION_RESERVED14	0x4000
#define ONFI_REVISION_RESERVED15	0x8000

#define ONFI_FEATURES_16B_BUS				0x0001
#define ONFI_FEATURES_MULTIPLE_LUN			0x0002
#define ONFI_FEATURES_NONSEQ_PAGE_PROG		0x0004
#define ONFI_FEATURES_MULTIPLANE_PROG_ERASE	0x0008
#define ONFI_FEATURES_ODD_TO_EV_PG_COPYBACK	0x0010
#define ONFI_FEATURES_NV_DDR				0x0020
#define ONFI_FEATURES_MULTIPLANE_READ		0x0040
#define ONFI_FEATURES_EXTENDED_PAR_PAGE		0x0080
#define ONFI_FEATURES_PROG_PG_REG_CLEAN_ENH	0x0100
#define ONFI_FEATURES_NV_LPDDR4				0x0200
#define ONFI_FEATURES_NV_DDR2				0x0400
#define ONFI_FEATURES_VOLUME_ADDR			0x0800
#define ONFI_FEATURES_EXTERNAL_VPP			0x1000
#define ONFI_FEATURES_NV_DDR3				0x2000
#define ONFI_FEATURES_ZQ_CALIBRATION		0x2000
#define ONFI_FEATURES_PACK_EL_SPEC			0x8000

#define ONFI_COMMANDS_PAGE_CACHE_PROG		0x0001
#define ONFI_COMMANDS_READ_CACHE			0x0002
#define ONFI_COMMANDS_GET_FEAT_SET_FEAT		0x0004
#define ONFI_COMMANDS_READ_STATUS_ENH		0x0008
#define ONFI_COMMANDS_COPYBACK				0x0010
#define ONFI_COMMANDS_READ_UNIQUE_ID		0x0020
#define ONFI_COMMANDS_CHANGE_RD_COL_ENH		0x0040
#define ONFI_COMMANDS_CHANGE_ROW_ADDRESS	0x0080
#define ONFI_COMMANDS_SMALL_DATA_MOVE		0x0100
#define ONFI_COMMANDS_RESET_LUN				0x0200
#define ONFI_COMMANDS_VOLUME_SELECT			0x0400
#define ONFI_COMMANDS_ODT_CONFIGURE			0x0800
#define ONFI_COMMANDS_LUN_GET_LUN_SET_FEAT	0x1000
#define ONFI_COMMANDS_ZQ_CALIBRATION_SH_LON	0x2000
#define ONFI_COMMANDS_RESERVED14			0x4000
#define ONFI_COMMANDS_RESERVED15			0x8000

#define ONFI_JEDEC_RANDOM_DATA_OUT			0x01
#define ONFI_JEDEC_MULTIPLANE_PAGE_PROGRAM	0x02
#define ONFI_JEDEC_MULTIPLANE_COPYBACK_PROG	0x04
#define ONFI_JEDEC_MULTIPLANE_BLOCK_ERASE	0x08
#define ONFI_JEDEC_RESERVED4				0x10
#define ONFI_JEDEC_RESERVED5				0x20
#define ONFI_JEDEC_RESERVED6				0x40
#define ONFI_JEDEC_RESERVED7				0x80

#define ONFI_TRAININGCMDS_EXPLICIT_DCC		0x01
#define ONFI_TRAININGCMDS_IMPLICIT_DCC		0x02
#define ONFI_TRAININGCMDS_READ_DQ			0x04
#define ONFI_TRAININGCMDS_WRITE_TX_DQ		0x08
#define ONFI_TRAININGCMDS_WRITE_RX_DQ		0x10
#define ONFI_TRAININGCMDS_RESERVED5			0x20
#define ONFI_TRAININGCMDS_RESERVED6			0x40
#define ONFI_TRAININGCMDS_RESERVED7			0x80

#define ONFI_PARTIAL_ATTR_PARTIAL_HAS_CONTR	0x01
#define ONFI_PARTIAL_ATTR_RESERVED1			0x02
#define ONFI_PARTIAL_ATTR_RESERVED2			0x04
#define ONFI_PARTIAL_ATTR_RESERVED3			0x08
#define ONFI_PARTIAL_ATTR_PG_LAYOUT_SPARE	0x10
#define ONFI_PARTIAL_ATTR_RESERVED5			0x20
#define ONFI_PARTIAL_ATTR_RESERVED6			0x40
#define ONFI_PARTIAL_ATTR_RESERVED7			0x80


#define ONFI_INTER_ATTR_OVERLAPPED			0x01
#define ONFI_INTER_ATTR_NO_BL_ADDR_RESTR	0x02
#define ONFI_INTER_ATTR_PROG_CACHE_SUP		0x04
#define ONFI_INTER_ATTR_ADDR_RESTR_CACHE	0x08
#define ONFI_INTER_ATTR_READ_CACHE			0x10
#define ONFI_INTER_ATTR_LOW_BIT_XNOR_RESTR	0x20
#define ONFI_INTER_ATTR_RESERVED6			0x40
#define ONFI_INTER_ATTR_RESERVED7			0x80


#pragma pack(push,1)
struct ONFI_Param {
	struct rev_info
	{
		unsigned int magic;				//0-3
		unsigned short revision;		//4-5
		unsigned short features;		//6-7
		unsigned short optionalCmds;	//8-9
		unsigned char jedecCmds;		//10
		unsigned char trainingCmds;		//11
		unsigned short extParamPageLen;	//12-13
		unsigned char numOfParamPages;	//14
		unsigned char reserved[17];		//15-31
	};
	rev_info rev;
	struct man_info
	{
		unsigned char name[12];			//32-43
		unsigned char model[20];		//44-63
		unsigned char jedecManID;		//64
		unsigned short dateCode;		//65-66
		unsigned char reserved[13];		//67-79
	};
	man_info man;
	struct mem_org
	{
		unsigned int numOfDataBytesPerPage;				//80-83
		unsigned short numOfSpareBytesPerPage;			//84-85
		unsigned int numOfDataBytesPerPartialPage;		//86-89 ONFI 5.0 reserved
		unsigned short numOfSpareBytesPerPartialPage;	//90-91 ONFI 5.0 reserved
		unsigned int numOfPagesPerBlock;				//92-95
		unsigned int numOfBlocksPerLUN;					//96-99
		unsigned char numOfLUNs;						//100
		struct addr_cyc
		{
			unsigned char percol : 4;
			unsigned char perrow : 4;
		};
		addr_cyc numOfAddrCycles;						//101
		unsigned char numOfBitsPerCell;					//102
		unsigned short badBlocksMaxPerLUN;				//103-104
		unsigned short blocksEndurance;					//105-106
		unsigned char guaranteedValidBlocksAtBeginning; //107
		unsigned short blocksEnduranceForGuaranteed;	//108-109
		unsigned char numOfProgramsPerPage;				//110
		unsigned char partitialProgrammingAttr;			//111
		unsigned char numOfBitsECCCorrectability;		//112
		unsigned char numOfInterleavedAddrBits;			//113 ONFI 5.0 number of plan address bits
		unsigned char interleavedOperationAttr;			//114 ONFI 5.0 multiplan operation atributes
		unsigned char reserved1;						//115
		unsigned short nvDDR3TimingMode;				//116-117
		unsigned int nvLPDDR4TimingMode;				//118-121
		unsigned char reserved2[6];						//122-127
	};
	mem_org mem;
	struct elec_param
	{
		unsigned char ioPinCapacitance;						//128
		unsigned short asyncTimingModeSupport;				//129-130
		unsigned short asyncProgramCacheTimingModeSupport;	//131-132
		unsigned short maxPageProgramTime;					//133-134
		unsigned short maxBlockEraseTime;					//135-136
		unsigned short maxPageReadTime;						//137-138
		unsigned short minChangeColSetupTime;				//139-140
		unsigned char nvDDRTiming;							//141
		unsigned char nvDDR2Timing;							//142
		unsigned char nvDDR12Features;						//143
		unsigned char reserved1[7];							//144-150
		unsigned char drvStrenght;							//151
		unsigned char reserved2[6];							//152-157
		unsigned char nvDDR23Features;						//158
		unsigned char nvDDR23WarmupCycles;					//159
		unsigned short nvDDR3TimingMode;					//160-161
		unsigned char nvDDR2TimingMode;						//162
		unsigned char reserved3;							//163
	};
	elec_param elec;
	struct vendor
	{
		unsigned short revision;						//164-165
		unsigned char specific[88];						//166-253
		unsigned short integrityCrc;					//254-255
	};
	vendor vendor;
};
#pragma pack(pop)

#endif
