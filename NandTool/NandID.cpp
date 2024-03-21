/*
Software to interface to a NAND-flash chip connected to a FT2232H IC.
Org (C) 2012 Jeroen Domburg (jeroen AT spritesmods.com)
New code and modifications (c) 2014 Bjoern Kerler (info AT revskills.de)

This program is free software: you can redistribute it and/or modify
t under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "NandID.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "NandCmds.h"

#define NAND_MFR_TOSHIBA	0x98
#define NAND_MFR_SAMSUNG	0xec
#define NAND_MFR_FUJITSU	0x04
#define NAND_MFR_NATIONAL	0x8f
#define NAND_MFR_RENESAS	0x07
#define NAND_MFR_STMICRO	0x20
#define NAND_MFR_HYNIX		0xad
#define NAND_MFR_MICRON		0x2c
#define NAND_MFR_AMD		0x01
#define NAND_MFR_MACRONIX	0xc2
#define NAND_MFR_EON		0x92

#define LP_OPTIONS 1
#define NAND_CI_CELLTYPE_MSK    0x0C
#define NAND_BUSWIDTH_16		2

//Table is 'borrowed' from the Linux kernel: drivers/mtd/nandids.c
//Copyright (C) 2002 Thomas Gleixner (tglx@linutronix.de)
//Modified to get rid of the 16-bit devices: we don't support these.
//Name. ID code, pagesize, chipsize in MegaByte, eraseblock size, options
const NandID::DevCodes NandID::m_devCodes[]={
	{"NAND 1MiB 5V 8-bit",		0x6e, 256, 1, 0x1000, 0, 3},
	{"NAND 2MiB 5V 8-bit",		0x64, 256, 2, 0x1000, 0, 3},
	{"NAND 4MiB 5V 8-bit",		0x6b, 512, 4, 0x2000, 0, 3},
	{"NAND 1MiB 3,3V 8-bit",	0xe8, 256, 1, 0x1000, 0, 3},
	{"NAND 1MiB 3,3V 8-bit",	0xec, 256, 1, 0x1000, 0, 3},
	{"NAND 2MiB 3,3V 8-bit",	0xea, 256, 2, 0x1000, 0, 3},
	{"NAND 4MiB 3,3V 8-bit",	0xd5, 512, 4, 0x2000, 0, 3},
	{"NAND 4MiB 3,3V 8-bit",	0xe3, 512, 4, 0x2000, 0, 3},
	{"NAND 4MiB 3,3V 8-bit",	0xe5, 512, 4, 0x2000, 0, 3},
	{"NAND 8MiB 3,3V 8-bit",	0xd6, 512, 8, 0x2000, 0, 3},
	{"NAND 8MiB 1,8V 8-bit",	0x39, 512, 8, 0x2000, 0, 3},
	{"NAND 8MiB 3,3V 8-bit",	0xe6, 512, 8, 0x2000, 0, 3},
	{"NAND 16MiB 1,8V 8-bit",	0x33, 512, 16, 0x4000, 0, 3},
	{"NAND 16MiB 3,3V 8-bit",	0x73, 512, 16, 0x4000, 0, 3},
	{"NAND 32MiB 1,8V 8-bit",	0x35, 512, 32, 0x4000, 0, 3},
	{"NAND 32MiB 3,3V 8-bit",	0x75, 512, 32, 0x4000, 0, 3},
	{"NAND 64MiB 1,8V 8-bit",	0x36, 512, 64, 0x4000, 0, 4},
	{"NAND 64MiB 3,3V 8-bit",	0x76, 512, 64, 0x4000, 0, 4},
	{"NAND 128MiB 1,8V 8-bit",	0x78, 512, 128, 0x4000, 0, 3},
	{"NAND 128MiB 1,8V 8-bit",	0x39, 512, 128, 0x4000, 0, 3},
	{"NAND 128MiB 3,3V 8-bit",	0x79, 512, 128, 0x4000, 0, 4},
	{"NAND 256MiB 3,3V 8-bit",	0x71, 512, 256, 0x4000, 0, 4},

	/* 512 Megabit */
	{"NAND 64MiB 1,8V 8-bit",	0xA2, 0,  64, 0, LP_OPTIONS, 4},
	{"NAND 64MiB 1,8V 8-bit",	0xA0, 0,  64, 0, LP_OPTIONS, 4},
	{"NAND 64MiB 3,3V 8-bit",	0xF2, 0,  64, 0, LP_OPTIONS, 4},
	{"NAND 64MiB 3,3V 8-bit",	0xD0, 0,  64, 0, LP_OPTIONS, 4},
	{"NAND 64MiB 3,3V 8-bit",	0xF0, 0,  64, 0, LP_OPTIONS, 4},

	/* 1 Gigabit */
	{"NAND 128MiB 1,8V 8-bit",	0xA1, 0, 128, 0, LP_OPTIONS, 4},
	{"NAND 128MiB 3,3V 8-bit",	0xF1, 0, 128, 0, LP_OPTIONS, 4},
	{"NAND 128MiB 3,3V 8-bit",	0xD1, 0, 128, 0, LP_OPTIONS, 4},

	/* 2 Gigabit */
	{"NAND 256MiB 1,8V 8-bit",	0xAA, 0, 256, 0, LP_OPTIONS, 5},
	{"NAND 256MiB 3,3V 8-bit",	0xDA, 0, 256, 0, LP_OPTIONS, 5},

	/* 4 Gigabit */
	{"NAND 512MiB 1,8V 8-bit",	0xAC, 0, 512, 0, LP_OPTIONS, 5},
	{"NAND 512MiB 3,3V 8-bit",	0xDC, 0, 512, 0, LP_OPTIONS, 5},

	/* 8 Gigabit */
	{"NAND 1GiB 1,8V 8-bit",	0xA3, 0, 1024, 0, LP_OPTIONS, 5},
	{"NAND 1GiB 3,3V 8-bit",	0xD3, 0, 1024, 0, LP_OPTIONS, 5},

	/* 16 Gigabit */
	{"NAND 2GiB 1,8V 8-bit",	0xA5, 0, 2048, 0, LP_OPTIONS, 5},
	{"NAND 2GiB 3,3V 8-bit",	0xD5, 0, 2048, 0, LP_OPTIONS, 5},

	/* 32 Gigabit */
	{"NAND 4GiB 1,8V 8-bit",	0xA7, 0, 4096, 0, LP_OPTIONS, 5},
	{"NAND 4GiB 3,3V 8-bit",	0xD7, 0, 4096, 0, LP_OPTIONS, 5},

	/* 64 Gigabit */
	{"NAND 8GiB 1,8V 8-bit",	0xAE, 0, 8192, 0, LP_OPTIONS, 5},
	{"NAND 8GiB 3,3V 8-bit",	0xDE, 0, 8192, 0, LP_OPTIONS, 5},

	/* 128 Gigabit */
	{"NAND 16GiB 1,8V 8-bit",	0x1A, 0, 16384, 0, LP_OPTIONS, 5},
	{"NAND 16GiB 3,3V 8-bit",	0x3A, 0, 16384, 0, LP_OPTIONS, 5},

	/* 256 Gigabit */
	{"NAND 32GiB 1,8V 8-bit",	0x1C, 0, 32768, 0, LP_OPTIONS, 6},
	{"NAND 32GiB 3,3V 8-bit",	0x3C, 0, 32768, 0, LP_OPTIONS, 6},

	/* 512 Gigabit */
	{"NAND 64GiB 1,8V 8-bit",	0x1E, 0, (int)65536, 0, LP_OPTIONS, 6},
	{"NAND 64GiB 3,3V 8-bit",	0x3E, 0, (int)65536, 0, LP_OPTIONS, 6},
	{"", 0, 0, 0, 0, 0, 0}
};

static int nand_id_has_period(unsigned char *id_data, int arrlen, int period)
{
	int i, j;
	for (i = 0; i < period; i++)
		for (j = i + period; j < arrlen; j += period)
			if (id_data[i] != id_data[j])
				return 0;
	return 1;
}

static int nand_id_len(unsigned char *id_data, int arrlen)
{
	int last_nonzero, period;

	/* Find last non-zero byte */
	for (last_nonzero = arrlen - 1; last_nonzero >= 0; last_nonzero--)
		if (id_data[last_nonzero])
			break;

	/* All zeros */
	if (last_nonzero < 0)
		return 0;

	/* Calculate wraparound period */
	for (period = 1; period < arrlen; period++)
		if (nand_id_has_period(id_data, arrlen, period))
			break;

	/* There's a repeated pattern */
	if (period < arrlen)
		return period;

	/* There are trailing zeros */
	if (last_nonzero < arrlen - 1)
		return last_nonzero + 1;

	/* No pattern detected */
	return arrlen;
}

//Constructor: construct NAND ID info from the 5 ID bytes read from the NAND.
NandID::NandID(FtdiNand *fn, unsigned char *idBytes) {
	int x;
	for (x=0; x<8; x++) m_idBytes[x]=idBytes[x];
	
	x=0;
	while (m_devCodes[x].id!=0 && m_devCodes[x].id!=idBytes[1]) x++;
	if (m_devCodes[x].id==0) {
		printf("Sorry, unknown nand chip with id code %hhx.\n", idBytes[1]);
		exit(0);
	}
	
	m_nandDesc=m_devCodes[x].name;

	unsigned char onfi[4]={0};
	fn->sendCmd(NAND_CMD_READID);
	fn->sendAddr(0x20, 1);
	fn->readData((char *)onfi, 4);
	m_onfib=(onfi[0]=='O' && onfi[1]=='N' && onfi[2]=='F' && onfi[3]=='I')?true:false;
	if (m_onfib)
	{
		fn->sendCmd(NAND_CMD_ONFI);
		fn->sendAddr(0, 1);
		fn->waitReady();
		fn->readData((char*)&m_onfi, sizeof(m_onfi));
		if (m_onfi.rev.magic==0x49464E4F) m_onfib=true;
		else m_onfib=false;
	}
	m_nandChipSzMB=m_devCodes[x].chipsizeMB;
	m_nandIsLP=((m_devCodes[x].options&LP_OPTIONS)!=0);
	/*if (m_devCodes[x].pagesize!=0) {
		//Page/erasesize is device-specific
		m_nandPageSz=m_devCodes[x].pagesize;
		m_nandEraseSz=m_devCodes[x].erasesize;
		m_nandOobSz=(m_nandPageSz==512)?16:8;
	} else {
		//Page/erasesize is encoded in 3/4/5th ID-byte
		int i;
		i=idBytes[3]&3;
		m_nandPageSz=1024<<i;
		i=(idBytes[3]>>4)&3;
		m_nandEraseSz=(64*1024)<<i;
		m_nandOobSz=((idBytes[3]&4)?16:8)*(m_nandPageSz/512);
	}*/

	if (m_devCodes[x].pagesize==0)
	{
		int ext_id=idBytes[3];
		/*
		 * Field definitions are in the following datasheets:
		 * Old style (4,5 byte ID): Samsung K9GAG08U0M (p.32)
		 * New Samsung (6 byte ID): Samsung K9GAG08U0F (p.44)
		 * Hynix MLC   (6 byte ID): Hynix H27UBG8T2B (p.22)
		 *
		 * Check for ID length, non-zero 6th byte, cell type, and Hynix/Samsung
		 * ID to decide what to do.
		 */
		int id_len=nand_id_len(idBytes,8); //(idBytes[0] == idBytes[6]) && (idBytes[1] == idBytes[7])?6:4;

		if ((id_len==6) && (idBytes[0] == NAND_MFR_SAMSUNG) && (idBytes[2] & NAND_CI_CELLTYPE_MSK) && (idBytes[5] != 0x00))
		{
			m_nandPageSz=2048UL<<(ext_id&3);
			ext_id>>=2;
			
			switch (ext_id & 0x03) {
				case 1:
				m_nandOobSz = 128UL;
				break;
				case 2:
				m_nandOobSz = 218UL;
				break;
				case 3:
				m_nandOobSz = 400UL;
				break;
				default:
				m_nandOobSz = 436UL;
				break;
			}
			ext_id>>=2;
			m_nandEraseSz = (128UL * 1024UL) << (((ext_id >> 1) & 0x04) | (ext_id & 0x03));
			m_nandbw = 0;
		}
		else if ((id_len == 6) && (idBytes[0] == NAND_MFR_HYNIX) && (idBytes[2] & NAND_CI_CELLTYPE_MSK))
		{
			unsigned int tmp;
			/* Calc pagesize */
			m_nandPageSz=2048UL<<(ext_id&3);
			ext_id >>= 2;
			/* Calc oobsize */
			switch (((ext_id >> 2) & 0x04) | (ext_id & 0x03)) 
			{
				case 0:
					m_nandOobSz = 128;
					break;
				case 1:
					m_nandOobSz = 224;
					break;
				case 2:
					m_nandOobSz = 448;
					break;
				case 3:
					m_nandOobSz = 64;
					break;
				case 4:
					m_nandOobSz = 32;
					break;
				case 5:
					m_nandOobSz = 16;
					break;
				default:
					m_nandOobSz = 640;
					break;
			}
			ext_id >>= 2;
			/* Calc blocksize */
			tmp = ((ext_id >> 1) & 0x04) | (ext_id & 0x03);
			if (tmp < 0x03)
				m_nandEraseSz = (128 * 1024) << tmp;
			else if (tmp == 0x03)
				m_nandEraseSz = 768 * 1024;
			else
				m_nandEraseSz = (64 * 1024) << tmp;
			m_nandbw = 0;
		}
		else
		{
			m_nandPageSz=1024UL<<(ext_id&3);
			ext_id>>=2;
			m_nandOobSz = (8 << (ext_id & 0x01)) * (m_nandPageSz >> 9);
			ext_id>>=2;
			m_nandEraseSz = (64UL * 1024UL) << (ext_id & 0x03);
			m_nandbw = (ext_id & NAND_BUSWIDTH_16) ? 1 : 0;
		}	
	}
	else	// Old nands have info hardcoded
	{
			m_nandPageSz = m_devCodes[x].pagesize;
			m_nandEraseSz = m_devCodes[x].erasesize;
			m_nandOobSz = m_nandPageSz / 32;
			m_nandbw = m_devCodes[x].options & NAND_BUSWIDTH_16;
	}	
	
	m_nandaddrcyc = m_devCodes[x].addrcycles;

	char buff[100];
	sprintf(buff, "Unknown (%hhx)", idBytes[0]);
	m_nandManuf=buff;
	if (idBytes[0]==0x98) m_nandManuf="Toshiba";
	if (idBytes[0]==0xec) m_nandManuf="Samsung";
	if (idBytes[0]==0x04) m_nandManuf="Fujitsu";
	if (idBytes[0]==0x8f) m_nandManuf="National Semiconductors";
	if (idBytes[0]==0x07) m_nandManuf="Renesas";
	if (idBytes[0]==0x20) m_nandManuf="ST Micro";
	if (idBytes[0]==0xad) m_nandManuf="Hynix";
	if (idBytes[0]==0x2c) m_nandManuf="Micron";
	if (idBytes[0]==0x01) m_nandManuf="AMD";
	if (idBytes[0]==0xc2) m_nandManuf="Macronix";
}

string NandID::getDesc() {
	return m_nandDesc;
}

string NandID::getManufacturer() {
	return m_nandManuf;
}

int NandID::getPageSize() {
	return m_nandPageSz;
}

int NandID::getOobSize() {
	return m_nandOobSz;
}

int NandID::getSizeMB() {
	return m_nandChipSzMB;
}

bool NandID::isLargePage() {
	return m_nandIsLP;
}

unsigned char* NandID::getID() {
	return m_idBytes;
}

//Get the amount of bytes the address needs to be sent as.
int NandID::getAddrByteCount() {
	return m_nandaddrcyc;
	/*int cyc;
	if (m_nandIsLP) {
		if (m_nandChipSzMB>=32768) {
			cyc=6;
		} else if (m_nandChipSzMB>=128) {
			cyc=5;
		} else {
			cyc=4;
		}
	} else {
		if (m_nandChipSzMB>128) { //>=64 ???
			cyc=3; 
		} else {
			cyc=4;
		}
	}
	return cyc;*/
}
