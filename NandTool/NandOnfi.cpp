#include <stdio.h>
#include <stdlib.h>
#include "NandOnfi.hpp"
#include "NandCmds.h"
#include "onfi.h"

NandOnfi::NandOnfi(FtdiNand *fn)
{
	unsigned char onfi_check_buf[4] = { 0 };
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

NandOnfi::NandOnfi(char* raw_data)
{
	memcpy((char*)&m_onfiData, raw_data, sizeof(m_onfiData));
	if (ONFI_SIGNATURE != m_onfiData.rev.magic)
		m_hasOnfi = false; //no signature
}

bool NandOnfi::hasOnfi(void)
{
	return m_hasOnfi;
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
	print_chars("[0-3] Signature", (char*)&m_onfiData.rev.magic, 3 - 0 + 1);
	print16_hex("[4-5] Revision", m_onfiData.rev.revision);
	if (m_onfiData.rev.revision & ONFI_REVISION_RESERVED0)
		print_indented("* reserved bit 0");
	if (m_onfiData.rev.revision & ONFI_REVISION_1_0)
		print_indented("- ONFI version 1.0");
	if (m_onfiData.rev.revision & ONFI_REVISION_2_0)
		print_indented("- ONFI version 2.0");
	if (m_onfiData.rev.revision & ONFI_REVISION_2_1)
		print_indented("- ONFI version 2.1");
	if (m_onfiData.rev.revision & ONFI_REVISION_2_2)
		print_indented("- ONFI version 2.2");
	if (m_onfiData.rev.revision & ONFI_REVISION_2_3)
		print_indented("- ONFI version 2.3");
	if (m_onfiData.rev.revision & ONFI_REVISION_3_0)
		print_indented("- ONFI version 3.0");
	if (m_onfiData.rev.revision & ONFI_REVISION_3_1)
		print_indented("- ONFI version 3.1");
	if (m_onfiData.rev.revision & ONFI_REVISION_3_2)
		print_indented("- ONFI version 3.2");
	if (m_onfiData.rev.revision & ONFI_REVISION_4_0)
		print_indented("- ONFI version 4.0");
	if (m_onfiData.rev.revision & ONFI_REVISION_4_1)
		print_indented("- ONFI version 4.1");
	if (m_onfiData.rev.revision & ONFI_REVISION_4_2)
		print_indented("- ONFI version 4.2");
	if (m_onfiData.rev.revision & ONFI_REVISION_5_0)
		print_indented("- ONFI version 5.0");
	if (m_onfiData.rev.revision & ONFI_REVISION_RESERVED13)
		print_indented("* reserved bit 13");
	if (m_onfiData.rev.revision & ONFI_REVISION_RESERVED14)
		print_indented("* reserved bit 14");
	if (m_onfiData.rev.revision & ONFI_REVISION_RESERVED15)
		print_indented("* reserved bit 15");

	print16_hex("[6-7] Features", m_onfiData.rev.features);
	if (m_onfiData.rev.features & ONFI_FEATURES_16B_BUS)
		print_indented("- 16-bit data bus width");
	if (m_onfiData.rev.features & ONFI_FEATURES_MULTIPLE_LUN)
		print_indented("- multiple LUN operations");
	if (m_onfiData.rev.features & ONFI_FEATURES_NONSEQ_PAGE_PROG)
		print_indented("- non-sequential page programming");
	if (m_onfiData.rev.features & ONFI_FEATURES_MULTIPLANE_PROG_ERASE)
		print_indented("- multi-plane program and erase operations");
	if (m_onfiData.rev.features & ONFI_FEATURES_ODD_TO_EV_PG_COPYBACK)
		print_indented("- odd to even page Copyback");
	if (m_onfiData.rev.features & ONFI_FEATURES_NV_DDR)
		print_indented("- NV-DDR");
	if (m_onfiData.rev.features & ONFI_FEATURES_MULTIPLANE_READ)
		print_indented("- multi-plane read operations");
	if (m_onfiData.rev.features & ONFI_FEATURES_EXTENDED_PAR_PAGE)
		print_indented("- extended parameter page");
	if (m_onfiData.rev.features & ONFI_FEATURES_PROG_PG_REG_CLEAN_ENH)
		print_indented("- program page register clean enhancement");
	if (m_onfiData.rev.features & ONFI_FEATURES_NV_LPDDR4)
		print_indented("- NV-LPDDR4");
	if (m_onfiData.rev.features & ONFI_FEATURES_NV_DDR2)
		print_indented("- NV-DDR2");
	if (m_onfiData.rev.features & ONFI_FEATURES_VOLUME_ADDR)
		print_indented("- Volume addressing");
	if (m_onfiData.rev.features & ONFI_FEATURES_EXTERNAL_VPP)
		print_indented("- external Vpp");
	if (m_onfiData.rev.features & ONFI_FEATURES_NV_DDR3)
		print_indented("- NV-DDR3");
	if (m_onfiData.rev.features & ONFI_FEATURES_ZQ_CALIBRATION)
		print_indented("- ZQ calibration");
	if (m_onfiData.rev.features & ONFI_FEATURES_PACK_EL_SPEC)
		print_indented("- Package Electrical Specification");

	print16_hex("[8-9] Optional commands", m_onfiData.rev.optionalCmds);
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_PAGE_CACHE_PROG)
		print_indented("- Page Cache Program command");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_READ_CACHE)
		print_indented("- Read Cache commands");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_GET_FEAT_SET_FEAT)
		print_indented("- Get Features and Set Features");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_READ_STATUS_ENH)
		print_indented("- Read Status Enhanced");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_COPYBACK)
		print_indented("- Copyback");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_READ_UNIQUE_ID)
		print_indented("- Read Unique ID");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_CHANGE_RD_COL_ENH)
		print_indented("- Change Read Column Enhanced");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_CHANGE_ROW_ADDRESS)
		print_indented("- Change Row Address");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_SMALL_DATA_MOVE)
		print_indented("- Small Data Move");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_RESET_LUN)
		print_indented("- Reset LUN");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_VOLUME_SELECT)
		print_indented("- Volume Select");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_ODT_CONFIGURE)
		print_indented("- ODT Configure");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_LUN_GET_LUN_SET_FEAT)
		print_indented("- LUN Get and LUN Set Features");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_ZQ_CALIBRATION_SH_LON)
		print_indented("- ZQ Calibration (Long and Short)");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_RESERVED14)
		print_indented("* reserved bit 14");
	if (m_onfiData.rev.optionalCmds & ONFI_COMMANDS_RESERVED15)
		print_indented("* reserved bit 15");

	print8_hex("[10] Reserved {or ONFI-JEDEC JTG primary advanced command support}", m_onfiData.rev.jedecCmds);
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RANDOM_DATA_OUT)
		print_indented("- Random Data Out");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_MULTIPLANE_PAGE_PROGRAM)
		print_indented("- Multi-plane Page Program");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_MULTIPLANE_COPYBACK_PROG)
		print_indented("- Multi-plane Copyback Program");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_MULTIPLANE_BLOCK_ERASE)
		print_indented("- Multi-plane Block Erase");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RESERVED4)
		print_indented("* reserved bit 4");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RESERVED5)
		print_indented("* reserved bit 5");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RESERVED6)
		print_indented("* reserved bit 6");
	if (m_onfiData.rev.jedecCmds & ONFI_JEDEC_RESERVED7)
		print_indented("* reserved bit 7");

	print8_hex("[11] Reserved {or Training commands supported}", m_onfiData.rev.trainingCmds);
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_EXPLICIT_DCC)
		print_indented("- Explicit DCC Training");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_IMPLICIT_DCC)
		print_indented("- Implicit (command based) DCC Training");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_READ_DQ)
		print_indented("- Read DQ Training");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_WRITE_TX_DQ)
		print_indented("- Write TX DQ Training");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_WRITE_RX_DQ)
		print_indented("- Write RX DQ Training");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_RESERVED5)
		print_indented("* reserved bit 5");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_RESERVED6)
		print_indented("* reserved bit 6");
	if (m_onfiData.rev.trainingCmds & ONFI_TRAININGCMDS_RESERVED7)
		print_indented("* reserved bit 7");

	print16_hex("[12-13] Reserved {or Extended parameter page length}", m_onfiData.rev.extParamPageLen);
	print8_hex("[14] Reserved {or Number of parameter pages}", m_onfiData.rev.numOfParamPages);
	print8_bytes_hex("[15-31] Reserved", (char*) m_onfiData.rev.reserved, 31 - 15 + 1);
	print_chars("[32-43] Manufactured", (char*) m_onfiData.man.name, 43 - 32 + 1);
	print_chars("[44-63] Model", (char*) m_onfiData.man.model, 63 - 44 + 1);
	print8_hex("[64] JEDEC manufacturer ID", m_onfiData.man.jedecManID);

	printf(" [65-66] Date code: %04Xh = Year %04d, Week %02d\n", m_onfiData.man.dateCode, (2000 + (m_onfiData.man.dateCode & 0xFF)), ((m_onfiData.man.dateCode >> 8) + 1));
	if (0x00 == m_onfiData.man.dateCode)
		print_indented("or not implemented (value 0000h)");
	print8_bytes_hex("[67-79] Reserved", (char*) m_onfiData.man.reserved, 79 - 67 + 1);
	print32_hex_int("[80-83] Number of data bytes per page", m_onfiData.mem.numOfDataBytesPerPage);
	print16_hex_int("[84-85] Number of spare bytes per page", m_onfiData.mem.numOfSpareBytesPerPage);
	print32_hex_int("[86-89] Number of data bytes per partial page {or Reserved}", m_onfiData.mem.numOfDataBytesPerPartialPage);
	print16_hex_int("[90-91] Number of spare bytes per partial page {or Reserved}", m_onfiData.mem.numOfSpareBytesPerPartialPage);
	print32_hex_int("[92-95] Number of pages per block", m_onfiData.mem.numOfPagesPerBlock);
	print32_hex_int("[96-99] Number of blocks per LUN", m_onfiData.mem.numOfBlocksPerLUN);
	print8_hex_int("[100] Number of LUNs", m_onfiData.mem.numOfLUNs);

	printf(" [101] Number of address cycles: %02Xh = Columns: %d, Rows %d\n", m_onfiData.mem.numOfAddrCycles, m_onfiData.mem.numOfAddrCycles.percol, m_onfiData.mem.numOfAddrCycles.perrow);

	print8_hex_int("[102] Number of bits per cell", m_onfiData.mem.numOfBitsPerCell);
	print16_hex_int("[103-104] Bad blocks max per LUN", m_onfiData.mem.badBlocksMaxPerLUN);
	print16_hex_int("[105-106] Block endurance", m_onfiData.mem.blocksEndurance);
	print8_hex_int("[107] Guaranteed valid blocks at beginning of target", m_onfiData.mem.guaranteedValidBlocksAtBeginning);
	print16_hex_int("[108-109] Block endurance for guaranteed valid blocks", m_onfiData.mem.blocksEnduranceForGuaranteed);
	print8_hex_int("[110] Number of programs per page", m_onfiData.mem.numOfProgramsPerPage);

	print8_hex_int("[111] Partitial programming attributes {or Reserved}", m_onfiData.mem.partitialProgrammingAttr);
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_PARTIAL_HAS_CONTR)
		print_indented("- partial page programming has constraints\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED1)
		print_indented("* reserved bit 1\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED2)
		print_indented("* reserved bit 2\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED3)
		print_indented("* reserved bit 3\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_PG_LAYOUT_SPARE)
		print_indented("- partial page layout is partial page data followed by partial page spare\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED5)
		print_indented("* reserved bit 5\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED6)
		print_indented("* reserved bit 6\n");
	if (m_onfiData.mem.partitialProgrammingAttr & ONFI_PARTIAL_ATTR_RESERVED7)
		print_indented("* reserved bit 7");

	print8_hex_int("[112] Number of bits ECC correctability", m_onfiData.mem.numOfBitsECCCorrectability);
	print8_hex_int("[113] Number of interleaved {or planes} address bits", m_onfiData.mem.numOfInterleavedAddrBits);

	print8_hex("[114] Interleaved {or Multi-plane} operation attributes", m_onfiData.mem.interleavedOperationAttr);
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_OVERLAPPED)
		print_indented("- overlapped support");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_NO_BL_ADDR_RESTR)
		print_indented("- no block address restrictions");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_PROG_CACHE_SUP)
		print_indented("- program cache supported");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_ADDR_RESTR_CACHE)
		print_indented("- address restrictions for cache operations");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_READ_CACHE)
		print_indented("- read cache supported");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_LOW_BIT_XNOR_RESTR)
		print_indented("- lower bit XNOR block address restriction");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_RESERVED6)
		print_indented("* reserved bit 6");
	if (m_onfiData.mem.interleavedOperationAttr & ONFI_INTER_ATTR_RESERVED7)
		print_indented("* reserved bit 7");

	print8_hex("[115] Reserved", m_onfiData.mem.reserved1);
	print16_hex("[116-117] Reserved {or NV-DDR3 timing mode support}", m_onfiData.mem.nvDDR3TimingMode);
	print16_hex("[118-121] Reserved {or NV-LPDDR4 timing mode support}", m_onfiData.mem.nvLPDDR4TimingMode);
	print8_bytes_hex("[122-127] Reserved", (char*) m_onfiData.mem.reserved2, 127 - 122 + 1);
	print8_hex_int("[128] I/O pin capacitance, maximum {or Reserved}", m_onfiData.elec.ioPinCapacitance);
	print16_hex("[129-130] Async. {SDR} timing mode support", m_onfiData.elec.asyncTimingModeSupport);
	print16_hex("[131-132] Async. program cache timing mode support {or Reserved}", m_onfiData.elec.asyncProgramCacheTimingModeSupport);
	print16_hex_int("[133-134] tPROG Maximum page program time (us) {or Reserved}", m_onfiData.elec.maxPageProgramTime);
	print16_hex_int("[135-136] tBERS Maximum block erase time (us) {or Reserved}", m_onfiData.elec.maxBlockEraseTime);
	print16_hex_int("[137-138] tR Maximum page read time (us) {or Reserved}", m_onfiData.elec.maxPageReadTime);
	print16_hex_int("[139-140] tCCS Min. change column setup time (ns) {or Reserved}", m_onfiData.elec.minChangeColSetupTime);
	print8_hex("[141] Reserved {or NV-DDR timing mode support}", m_onfiData.elec.nvDDRTiming);
	print8_hex("[142] Reserved {or NV-DDR2 timing mode support}", m_onfiData.elec.nvDDR2Timing);
	print8_hex("[143] Reserved {or NV-DDR/NV-DDR2 features}", m_onfiData.elec.nvDDR12Features);
	print8_bytes_hex("[144-150] Reserved", (char*) m_onfiData.elec.reserved1, 150 - 144 + 1);
	print8_hex("[151] Reserved {or Driver strength support}", m_onfiData.elec.nvDDR12Features);
	print8_bytes_hex("[152-157] Reserved", (char*) m_onfiData.elec.reserved2, 157 - 152 + 1);
	print8_hex("[158] Reserved {or NV-DDR2/3 features}", m_onfiData.elec.nvDDR23Features);
	print8_hex("[159] Reserved {or NV-DDR2/3 warmup cycles}", m_onfiData.elec.nvDDR23WarmupCycles);
	print16_hex("[160-161] Reserved {or NV-DDR3 timing mode support}", m_onfiData.elec.nvDDR3TimingMode);
	print8_hex("[162] Reserved {or NV-DDR2 timing mode support}", m_onfiData.elec.nvDDR2TimingMode);
	print8_hex("[163] Reserved", m_onfiData.elec.reserved3);
	print16_hex_int("[164-165] Vendor revision", m_onfiData.vendor.revision);
	print8_bytes_hex("[166-253] Vendor specific", (char*) m_onfiData.vendor.specific, 253 - 166 + 1);

	print16_hex("[254-255] CRC", m_onfiData.vendor.integrityCrc);

	short calculatedCRC = ONFI_CRC_INIT;
	for (int i = 0; i < ONFI_CRC_RANGE; i++)
		calculatedCRC = doCrc16(calculatedCRC, ((char*)&m_onfiData)[i]);
	calculatedCRC ^= ONFI_CRC_FINAL_XOR;
	print16_hex("Calcul.   CRC", calculatedCRC);
	if ((calculatedCRC & 0xFFFF) != (m_onfiData.vendor.integrityCrc & 0xFFFF))
		print_indented("*CRC NOT CORRECT*\n");
	else
		print_indented("CRC correct\n");
}

int NandOnfi::readRaw(char* buffer, int max)
{
	if (ONFI_SIZE != max)
		return 0;
	memcpy(buffer, (char*) &m_onfiData, ONFI_SIZE);
	return ONFI_SIZE;
}

short NandOnfi::doCrc16(short input, char byte)
{
	//lame implementation, but uses CRC constant from standard
	for (int bit = 0; bit < 8; bit++)
	{
		if (((input & 0x8000)>>8) ^ (byte & 0x80))
			input = (input << 1) ^ ONFI_CRC_POLY;
		else
			input = input << 1;
		byte <<= 1;
	}
	return input;
}

void NandOnfi::print8_hex(char* text, int value)
{
	printf(" %s: %02Xh\n", text, value & 0xFF);
}

void NandOnfi::print8_hex_int(char* text, int value)
{
	printf(" %s: %02Xh = %d\n", text, value & 0xFF, value);
}

void NandOnfi::print16_hex(char* text, int value)
{
	printf(" %s: %04Xh\n", text, value & 0xFFFF);
}

void NandOnfi::print16_hex_int(char* text, int value)
{
	printf(" %s: %04Xh = %d\n", text, value & 0xFFFF, value);
}

void NandOnfi::print32_hex_int(char* text, int value)
{
	printf(" %s: %08Xh = %d\n", text, value, value);
}

void NandOnfi::print_header(char* text)
{
	printf(" %s:\n", text);

}

void NandOnfi::print_indented(char* text)
{
	printf("    ");
	printf("%s\n", text);
}

void NandOnfi::print8_bytes_hex(char* header,  char* buffer, int len)
{
	int index = 0;
	printf(" %s:\n", header);
	while (len != index)
	{
		if (index % 8 == 0)
			printf("    ");
		printf("%02Xh ", buffer[index] & 0x00FF);
		index++;
		if (index % 8 == 0)
			printf("\n");
	}
	if (index % 8 != 0)
		printf("\n");
}
void NandOnfi::print_chars(char* header, char* buffer, int len)
{
	char c;
	printf(" %s = \"", header);
	for (int i = 0; i < len; i++)
	{
		c = buffer[i]; //make sure not modify original buffer, otherway CRC will fail
		if (c < 0x20)
			c = '_';
		printf("%c", c);
	}
	printf("\"\n");
}
