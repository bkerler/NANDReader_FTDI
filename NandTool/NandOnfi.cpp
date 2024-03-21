#include <stdio.h>
#include <stdlib.h>
#include "NandOnfi.hpp"
#include "NandCmds.h"
#include "onfi.h"

NandOnfi::NandOnfi(FtdiNand *fn)
{
	unsigned char onfi_check_buf[4] = { 0 };
	char onfi_buf[256];
	m_hasOnfi = false;
	fn->sendCmd(NAND_CMD_READID);
	fn->sendAddr(0x20, 1);
	fn->readData((char *)onfi_check_buf, 4);
	if ((onfi_check_buf[0] == 'O' && onfi_check_buf[1] == 'N' && onfi_check_buf[2] == 'F' && onfi_check_buf[3] == 'I'))
		m_hasOnfi = true;
	if (!m_hasOnfi)
		return;
	fn->sendCmd(NAND_CMD_ONFI);
	fn->sendAddr(0, 1);
	fn->waitReady();
	fn->readData((char*)&m_onfiData, sizeof(m_onfiData));
	if (ONFI_SIGNATURE != m_onfiData.rev.magic)
		m_hasOnfi = false; //onfi info included in ReadID, but Onfi read failed
}

bool NandOnfi::hasOnfi(void)
{
	return m_hasOnfi;
}

void NandOnfi::printPrintableChar(char c)
{
	if (c < 0x20)
		c = '_';
	printf("%c", c);
}

void NandOnfi::showOnfi(void)
{
	if (!m_hasOnfi)
	{
		printf("No ONFI data\n");
		return;
	}
	printf("ONFI content:\n");
	printf("Text {in braces} is alternative naming in later version of the ONFI standard.\n");
	printf(" [0-3] Signature: %02Xh %02Xh %02Xh %02Xh\n", (m_onfiData.rev.magic)&0xFF, (m_onfiData.rev.magic >> 8) & 0xFF, (m_onfiData.rev.magic >> 16) & 0xFF, (m_onfiData.rev.magic >> 24) & 0xFF);
	printf(" [4-5] Revision: %04Xh =\n", m_onfiData.rev.revision);
	if (m_onfiData.rev.revision & ONFI_REVISION_RESERVED0)
		printf("       * reserved bit 0\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_1_0)
		printf("       - ONFI version 1.0\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_2_0)
		printf("       - ONFI version 2.0\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_2_1)
		printf("       - ONFI version 2.1\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_2_2)
		printf("       - ONFI version 2.2\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_2_3)
		printf("       - ONFI version 2.3\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_3_0)
		printf("       - ONFI version 3.0\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_3_1)
		printf("       - ONFI version 3.1\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_3_2)
		printf("       - ONFI version 3.2\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_4_0)
		printf("       - ONFI version 4.0\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_4_1)
		printf("       - ONFI version 4.1\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_4_2)
		printf("       - ONFI version 4.2\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_5_0)
		printf("       - ONFI version 5.0\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_RESERVED13)
		printf("       * reserved bit 13\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_RESERVED14)
		printf("       * reserved bit 14\n");
	if (m_onfiData.rev.revision & ONFI_REVISION_RESERVED15)
		printf("       * reserved bit 15\n");

	printf(" [6-7] Features: %04Xh =\n", m_onfiData.rev.features);
	if (m_onfiData.rev.features & ONFI_FEATURES_16B_BUS)
		printf("       - 16-bit data bus width\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_MULTIPLE_LUN)
		printf("       - multiple LUN operations\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_NONSEQ_PAGE_PROG)
		printf("       - non-sequential page programming\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_MULTIPLANE_PROG_ERASE)
		printf("       - multi-plane program and erase operations\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_ODD_TO_EV_PG_COPYBACK)
		printf("       - odd to even page Copyback\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_NV_DDR)
		printf("       - NV-DDR\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_MULTIPLANE_READ)
		printf("       - multi-plane read operations\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_EXTENDED_PAR_PAGE)
		printf("       - extended parameter page\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_PROG_PG_REG_CLEAN_ENH)
		printf("       - program page register clean enhancement\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_NV_LPDDR4)
		printf("       - NV-LPDDR4\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_NV_DDR2)
		printf("       - NV-DDR2\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_VOLUME_ADDR)
		printf("       - Volume addressing\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_EXTERNAL_VPP)
		printf("       - external Vpp\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_NV_DDR3)
		printf("       - NV-DDR3\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_ZQ_CALIBRATION)
		printf("       - ZQ calibration\n");
	if (m_onfiData.rev.features & ONFI_FEATURES_PACK_EL_SPEC)
		printf("       - Package Electrical Specification\n");

	printf(" [8-9] Optional commands: %04Xh =\n", m_onfiData.rev.optionalCmds);
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_PAGE_CACHE_PROG)
		printf("       - Page Cache Program command\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_READ_CACHE)
		printf("       - Read Cache commands\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_GET_FEAT_SET_FEAT)
		printf("       - Get Features and Set Features\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_READ_STATUS_ENH)
		printf("       - Read Status Enhanced\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_COPYBACK)
		printf("       - Copyback\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_READ_UNIQUE_ID)
		printf("       - Read Unique ID\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_CHANGE_RD_COL_ENH)
		printf("       - Change Read Column Enhanced\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_CHANGE_ROW_ADDRESS)
		printf("       - Change Row Address\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_SMALL_DATA_MOVE)
		printf("       - Small Data Move\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_RESET_LUN)
		printf("       - Reset LUN\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_VOLUME_SELECT)
		printf("       - Volume Select\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_ODT_CONFIGURE)
		printf("       - ODT Configure\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_LUN_GET_LUN_SET_FEAT)
		printf("       - LUN Get and LUN Set Features\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_ZQ_CALIBRATION_SH_LON)
		printf("       - ZQ Calibration (Long and Short)\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_RESERVED14)
		printf("       * reserved bit 14\n");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_RESERVED15)
		printf("       * reserved bit 15\n");

	printf(" [10] Reserved {or ONFI-JEDEC JTG primary advanced command support}: %02Xh =\n", m_onfiData.rev.jedecCmds);
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RANDOM_DATA_OUT)
		printf("       - Random Data Out\n");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_MULTIPLANE_PAGE_PROGRAM)
		printf("       - Multi-plane Page Program\n");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_MULTIPLANE_COPYBACK_PROG)
		printf("       - Multi-plane Copyback Program\n");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_MULTIPLANE_BLOCK_ERASE)
		printf("       - Multi-plane Block Erase\n");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RESERVED4)
		printf("       * reserved bit 4\n");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RESERVED5)
		printf("       * reserved bit 5\n");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RESERVED6)
		printf("       * reserved bit 6\n");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RESERVED7)
		printf("       * reserved bit 7\n");

	printf(" [11] Reserved {or Training commands supported}: %02Xh =\n", m_onfiData.rev.trainingCmds);
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_EXPLICIT_DCC)
		printf("       - Explicit DCC Training\n");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_IMPLICIT_DCC)
		printf("       - Implicit (command based) DCC Training\n");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_READ_DQ)
		printf("       - Read DQ Training\n");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_WRITE_TX_DQ)
		printf("       - Write TX DQ Training\n");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_WRITE_RX_DQ)
		printf("       - Write RX DQ Training\n");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_RESERVED5)
		printf("       * reserved bit 5\n");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_RESERVED6)
		printf("       * reserved bit 6\n");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_RESERVED7)
		printf("       * reserved bit 7\n");

	printf(" [12-13] Reserved {or Extended parameter page length}: %04Xh\n", m_onfiData.rev.extParamPageLen);

	printf(" [14] Reserved {or Number of parameter pages}: %02Xh\n", m_onfiData.rev.numOfParamPages);

	printf(" [15-31] Reserved: ");
	for (int i = 0; i < 8; i++)
		printf("%02Xh ", m_onfiData.rev.reserved[i]);
	printf("\n                   ");
	for (int i = 8; i < 16; i++)
		printf("%02Xh ", m_onfiData.rev.reserved[i]);
	printf("\n");
	printf("                   %02Xh\n", m_onfiData.rev.reserved[16]);

	printf(" [32-43] Manufactured = \"");
	for (int i = 0; i < 12; i++)
		printPrintableChar(m_onfiData.man.name[i]);
	printf("\"\n");

	printf(" [44-63] Model = \"");
	for (int i = 0; i < 20; i++)
		printPrintableChar(m_onfiData.man.model[i]);
	printf("\"\n");

	printf(" [64] JEDEC manufacturer ID: %02Xh\n", m_onfiData.man.jedecManID);

	printf(" [65-66] Date code: %04Xh = Year %04d, Week %02d\n", m_onfiData.man.dateCode, (2000 + (m_onfiData.man.dateCode & 0xFF)), ((m_onfiData.man.dateCode >> 8) + 1));
	if (0x00 == m_onfiData.man.dateCode)
		printf("                            or not implemented (value 0000h)\n");
	printf(" [67-79] Reserved: ");
	for (int i = 0; i < 8; i++)
		printf("%02Xh ", m_onfiData.man.reserved[i]);
	printf("\n                   ");
	for (int i = 8; i < 13; i++)
		printf("%02Xh ", m_onfiData.man.reserved[i]);
	printf("\n");

	printf(" [80-83] Number of data bytes per page: %08Xh = %d\n", m_onfiData.mem.numOfDataBytesPerPage, m_onfiData.mem.numOfDataBytesPerPage);

	printf(" [84-85] Number of spare bytes per page: %04Xh = %d\n", m_onfiData.mem.numOfSpareBytesPerPage, m_onfiData.mem.numOfSpareBytesPerPage);

	printf(" [86-89] Number of data bytes per partial page {or Reserved}: %08Xh = %d\n", m_onfiData.mem.numOfDataBytesPerPartialPage, m_onfiData.mem.numOfDataBytesPerPartialPage);

	printf(" [90-91] Number of spare bytes per partial page {or Reserved}: %04Xh = %d\n", m_onfiData.mem.numOfSpareBytesPerPartialPage, m_onfiData.mem.numOfSpareBytesPerPartialPage);

	printf(" [92-95] Number of pages per block: %08Xh = %d\n", m_onfiData.mem.numOfPagesPerBlock, m_onfiData.mem.numOfPagesPerBlock);

	printf(" [96-99] Number of blocks per LUN: %08Xh = %d\n", m_onfiData.mem.numOfBlocksPerLUN, m_onfiData.mem.numOfBlocksPerLUN);

	printf(" [100] Number of LUNs: %02Xh = %d\n", m_onfiData.mem.numOfLUNs, m_onfiData.mem.numOfLUNs);

	printf(" [101] Number of address cycles: %02Xh = Columns: %d, Rows %d\n", m_onfiData.mem.numOfAddrCycles, m_onfiData.mem.numOfAddrCycles.percol, m_onfiData.mem.numOfAddrCycles.perrow);

	printf(" [102] Number of bits per cell: %02Xh = %d\n", m_onfiData.mem.numOfBitsPerCell, m_onfiData.mem.numOfBitsPerCell);

	printf(" [103-104] Bad blocks max per LUN: %04Xh = %d\n", m_onfiData.mem.badBlocksMaxPerLUN, m_onfiData.mem.badBlocksMaxPerLUN);

	printf(" [105-106] Block endurance: %04Xh = %d\n", m_onfiData.mem.blocksEndurance, m_onfiData.mem.blocksEndurance);

	printf(" [107] Guaranteed valid blocks at beginning of target: %02Xh = %d\n", m_onfiData.mem.guaranteedValidBlocksAtBeginning, m_onfiData.mem.guaranteedValidBlocksAtBeginning);

	printf(" [108-109] Block endurance for guaranteed valid blocks: %04Xh = %d\n", m_onfiData.mem.blocksEnduranceForGuaranteed, m_onfiData.mem.blocksEnduranceForGuaranteed);

	printf(" [110] Number of programs per page: %02Xh = %d\n", m_onfiData.mem.numOfProgramsPerPage, m_onfiData.mem.numOfProgramsPerPage);

	printf(" [111] Partitial programming attributes {or Reserved}: %02Xh =\n", m_onfiData.mem.partitialProgrammingAttr);
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_PARTIAL_HAS_CONTR)
		printf("       - partial page programming has constraints\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED1)
		printf("       * reserved bit 1\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED2)
		printf("       * reserved bit 2\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED3)
		printf("       * reserved bit 3\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_PG_LAYOUT_SPARE)
		printf("       - partial page layout is partial page data followed by partial page spare\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED5)
		printf("       * reserved bit 5\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED6)
		printf("       * reserved bit 6\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED7)
		printf("       * reserved bit 7\n");

	printf(" [112] Number of bits ECC correctability: %02Xh = %d\n", m_onfiData.mem.numOfBitsECCCorrectability, m_onfiData.mem.numOfBitsECCCorrectability);

	printf(" [113] Number of interleaved {or planes} address bits: %02Xh = %d\n", m_onfiData.mem.numOfInterleavedAddrBits, m_onfiData.mem.numOfInterleavedAddrBits & 0x0F);

	printf(" [114] Interleaved {or Multi-plane} operation attributes: %02Xh =\n", m_onfiData.mem.interleavedOperationAttr);
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_OVERLAPPED)
		printf("       - overlapped support\n");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_NO_BL_ADDR_RESTR)
		printf("       - no block address restrictions\n");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_PROG_CACHE_SUP)
		printf("       - program cache supported\n");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_ADDR_RESTR_CACHE)
		printf("       - address restrictions for cache operations\n");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_READ_CACHE)
		printf("       - read cache supported\n");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_LOW_BIT_XNOR_RESTR)
		printf("       - lower bit XNOR block address restriction\n");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_RESERVED6)
		printf("       * reserved bit 6\n");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_RESERVED7)
		printf("       * reserved bit 7\n");

	printf(" [115] Reserved: %02Xh\n", m_onfiData.mem.reserved1);

	printf(" [116-117] Reserved {or NV-DDR3 timing mode support}: %04Xh\n", m_onfiData.mem.nvDDR3TimingMode);

	printf(" [118-121] Reserved {or NV-LPDDR4 timing mode support}: %08Xh\n", m_onfiData.mem.nvLPDDR4TimingMode);

	printf(" [122-127] Reserved: ");
	for (int i = 0; i < 6; i++)
		printf("%02Xh ", m_onfiData.mem.reserved2[i]);
	printf("\n");

	printf(" [128] I/O pin capacitance, maximum {or Reserved}: %02Xh = %d\n", m_onfiData.elec.ioPinCapacitance, m_onfiData.elec.ioPinCapacitance);

	printf(" [129-130] Async. {SDR} timing mode support: %04Xh\n", m_onfiData.elec.asyncTimingModeSupport);

	printf(" [131-132] Async. program cache timing mode support {or Reserved}: %04Xh\n", m_onfiData.elec.asyncProgramCacheTimingModeSupport);

	printf(" [133-134] tPROG Maximum page program time (us) {or Reserved}: %04Xh = %d\n", m_onfiData.elec.maxPageProgramTime, m_onfiData.elec.maxPageProgramTime);

	printf(" [135-136] tBERS Maximum block erase time (us) {or Reserved}: %04Xh = %d\n", m_onfiData.elec.maxBlockEraseTime, m_onfiData.elec.maxBlockEraseTime);

	printf(" [137-138] tR Maximum page read time (us) {or Reserved}: %04Xh = %d\n", m_onfiData.elec.maxPageReadTime, m_onfiData.elec.maxPageReadTime);

	printf(" [139-140] tCCS Min. change column setup time (ns) {or Reserved}: %04Xh = %d\n", m_onfiData.elec.minChangeColSetupTime, m_onfiData.elec.minChangeColSetupTime);

	printf(" [141] Reserved {or NV-DDR timing mode support}: %02Xh\n", m_onfiData.elec.nvDDRTiming);

	printf(" [142] Reserved {or NV-DDR2 timing mode support}: %02Xh\n", m_onfiData.elec.nvDDR2Timing);

	printf(" [143] Reserved {or NV-DDR/NV-DDR2 features}: %02Xh\n", m_onfiData.elec.nvDDR12Features);

	printf(" [144-150] Reserved: ");
	for (int i = 0; i < 7; i++)
		printf("%02Xh ", m_onfiData.elec.reserved1[i]);
	printf("\n");

	printf(" [151] Reserved {or Driver strength support}: %02Xh\n", m_onfiData.elec.nvDDR12Features);

	printf(" [152-157] Reserved: ");
	for (int i = 0; i < 6; i++)
		printf("%02Xh ", m_onfiData.elec.reserved1[i]);
	printf("\n");

	printf(" [158] Reserved {or NV-DDR2/3 features}: %02Xh\n", m_onfiData.elec.nvDDR23Features);

	printf(" [159] Reserved {or NV-DDR2/3 warmup cycles}: %02Xh\n", m_onfiData.elec.nvDDR23WarmupCycles);

	printf(" [160-161] Reserved {or NV-DDR3 timing mode support}: %04Xh\n", m_onfiData.elec.nvDDR3TimingMode);

	printf(" [162] Reserved {or NV-DDR2 timing mode support}: %02Xh\n", m_onfiData.elec.nvDDR2TimingMode);

	printf(" [163] Reserved: %02Xh\n", m_onfiData.elec.reserved3);

	printf(" [164-165] Vendor revision: %04Xh = %d\n", m_onfiData.vendor.revision, m_onfiData.vendor.revision);

	printf(" [166-253] Vendor specific: ");
	for (int j = 0; j < 11; j++)
	{
		if (0 != j)
			printf("                            ");
		for (int i = 0; i < 8; i++)
			printf("%02Xh ", m_onfiData.vendor.specific[i+j*8]);
		printf("\n");
	}

	printf(" [254-255] CRC: %04Xh\n", m_onfiData.vendor.integrityCrc, m_onfiData.vendor.integrityCrc);
}

int NandOnfi::readRaw(char* buffer, int max)
{
	if (ONFI_SIZE != max)
		return 0;
	memcpy(buffer, (char*) &m_onfiData, ONFI_SIZE);
	return ONFI_SIZE;
}
