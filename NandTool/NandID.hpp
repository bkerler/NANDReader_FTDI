#ifndef NANDID_HPP
#define NANDID_HPP

#include <string>
using namespace std;
#include "FtdiNand.hpp"


class NandID {
public:
	NandID(FtdiNand *fn, unsigned char *idbytes);
	string getManufacturer();
	string getDesc();
	int getPageSize();
	int getSizeMB();
	int getOobSize();
	bool isLargePage();
	int getAddrByteCount();
	unsigned char* getID();
#pragma pack(push,1)
	struct ONFI_Param {
		struct rev_info
		{
			unsigned int magic;
			unsigned short revision;
			unsigned short features;
			unsigned short optionalcmds;
			unsigned short reserved[8];
		};
		rev_info rev;
		struct man_info
		{
			unsigned char name[12];
			unsigned char model[20];
			unsigned char jedec_man_id;
			unsigned short datecode;
			unsigned char reserved[13];
		};
		man_info man;
		struct mem_org
		{
			unsigned int databytes_per_page;
			unsigned short sparebytes_per_page;
			unsigned int databytes_per_partial;
			unsigned short sparebytes_per_partial;
			unsigned int pages_per_block;
			unsigned int blocks_per_lun;
			unsigned char luns;
			struct addr_cyc
			{
				unsigned char percol:4;
				unsigned char perrow:4;
			};
			addr_cyc addrcyc;
			unsigned char bitspercell;
			unsigned short badblocks_per_lun;
			unsigned short blocks_endurance;
			unsigned char valid_blocks_at_beginning;
			unsigned short blocks_endurance_valid_blocks;
			unsigned char programs_per_page;
			unsigned char partitial_programming_attr;
			unsigned char number_of_bits_ecc_correct;
			unsigned char number_of_interleaved_addr_bits;
			unsigned char interleaved_operation_attr;
			unsigned char reserved[13];
		};
		mem_org mem;
		struct electr_param
		{
			unsigned char io_pin_capacitance;
			unsigned short timingmodesupport;
			unsigned short programcachetimingmodesupport;
			unsigned short maximumpageprogramtime;
			unsigned short maximumblockerasetime;
			unsigned short maximumpagereadtime;
			unsigned short maximumchangecolsetuptime;
			unsigned char reserved[3];
		};
		electr_param elec_parm;
		struct vendor
		{
			unsigned short vendor_revision;
			unsigned char vendor_specific[88];
			unsigned short integrity_crc;
		};
		vendor vendor_info;
	};
#pragma pack(pop)
	ONFI_Param m_onfi;
private:
	typedef struct {
		const char *name;
		unsigned char id;
		int pagesize;
		int chipsizeMB;
		int erasesize;
		int options;
		int addrcycles;
	} DevCodes;

	static const DevCodes m_devCodes[];
	unsigned char m_idBytes[8];
	
	string m_nandManuf;
	string m_nandDesc;
	int m_nandPageSz;
	int m_nandbw;
	int m_nandOobSz;
	int m_nandEraseSz;
	int m_nandChipSzMB;
	int m_nandaddrcyc;
	bool m_nandIsLP;
	bool m_onfib;
};
#endif