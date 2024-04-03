#ifndef NANDGEOMETRY_HPP
#define NANDGEOMETRY_HPP

#include <string>
using namespace std;
#include "FtdiNand.hpp"

class NandGeometry {
public:
	NandGeometry(unsigned char *idbytes);
	string getManufacturer();
	unsigned char* getID();
	virtual string getDesc();
	virtual long getPagesCount();
	virtual int getSizeMB();
	virtual int getPageSize();
	virtual int getOobSize();
	virtual bool isLargePage();
	virtual int getAddrByteCount();
protected:
	unsigned char m_idBytes[8];
	string m_nandManuf;
};

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

#endif
