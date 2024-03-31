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

#include "NandChip.hpp"
#include "NandData.hpp"
#include "NandDataLP.hpp"
#include "NandDataSP.hpp"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "NandCmds.h"
#include "NandGeometryFromUser.hpp"
#include "NandGeometryByID.hpp"

NandChip::NandChip(FtdiNand *fn, char* geometry) {
	unsigned char id[8];
	m_fn = fn;
	//Try to read the 5 NAND ID-bytes and create the ID-object
	m_fn->sendCmd(NAND_CMD_READID);
	m_fn->sendAddr(0, 1);
	m_fn->readData((char *)id, 8);
	if (NULL == geometry)
		m_id = new NandGeometryByID(fn, id);
	else
		m_id = new NandGeometryFromUser(fn, id, geometry);
	m_onfi = new NandOnfi(fn);
	//We use a different data object for large- and small-page devices
	if (m_id->isLargePage()) {
		m_data = new NandDataLP(fn, m_id);
	}
	else {
		m_data = new NandDataSP(fn, m_id);
	}
}

NandChip::~NandChip() {
	delete m_id;
}

void NandChip::showCommon()
{
	//Dump some info.
	unsigned char *id_bytes = m_id->getID();
	printf("Nand type: %s\n", m_id->getDesc().c_str());
	printf("Manufacturer: %s\n", m_id->getManufacturer().c_str());
	printf("ID: %02X\n", id_bytes[1]);
	printf("Chip: %02X\n", id_bytes[2]);
	printf("Raw ID bytes: %02X %02X %02X %02X %02X (%02X)\n", id_bytes[0], id_bytes[1], id_bytes[2], id_bytes[3], id_bytes[4], id_bytes[5]);
	printf("Size: %iMB, pagesize %i bytes, OOB size %i bytes\n", m_id->getSizeMB(), m_id->getPageSize(), m_id->getOobSize());
	printf("%s page, needs %i addr bytes.\n", m_id->isLargePage()?"Large":"Small",  m_id->getAddrByteCount());
}

void NandChip::showInfo() {
	showCommon();
	if (m_onfi->hasOnfi())
		printf("Nand has ONFI structure\n");
}

void NandChip::showInfoLong() {
	showCommon();
	if (m_onfi->hasOnfi())
		m_onfi->showOnfi();
}

int NandChip::readPage(int page, char *buff, int count, NandChip::AccessType access) {
	int r=0;
	if ((NandChip::accessMain == access) || (NandChip::accessBoth == access)) {
		r=m_data->readPage(page, buff, count);
		count-=r;
	}
	if ((NandChip::accessOob == access) || (NandChip::accessBoth == access)) {
		r=m_data->readOob(page, &buff[r], count);
		count-=r;
	}
	if (NandChip::accessOnfi == access)
		r = m_onfi->readRaw(buff, count);
	return r;
}

int NandChip::writePage(int page, char *buff, int count, NandChip::AccessType access) {
	int r;
	//Can't read/writeback main or OOB data here because the erase size usually is bigger than the page size...
	if (access != NandChip::accessBoth) {
		printf("Writing of only main / OOB data isn't supported yet.\n");
		exit(0);
	}
	r=m_data->writePage(page, buff, count);
	return r;
}

NandGeometry *NandChip::getIdPtr() {
	return m_id;
}

bool NandChip::hasOnfi()
{
	return m_onfi->hasOnfi();
}
