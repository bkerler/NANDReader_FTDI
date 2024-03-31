/*
Software to interface to a NAND-flash chip connected to a FT2232H IC.
Org (C) 2012 Jeroen Domburg (jeroen AT spritesmods.com)
New code and modifications (c) 2014 Bjoern Kerler (info AT revskills.de)
New code and modifications (c) 2024 nord-data-recall

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

#include "NandGeometry.hpp"
#include <stdio.h>
#include <stdlib.h>

NandGeometry::NandGeometry(unsigned char *idBytes){
	for (int x = 0; x < 8; x++) //copy to class property 
		m_idBytes[x] = idBytes[x];

	char buff[100];
	sprintf(buff, "Unknown (%hhx)", idBytes[0]);
	m_nandManuf = buff;
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

string NandGeometry::getManufacturer() {
	return m_nandManuf;
}

unsigned char* NandGeometry::getID() {
	return m_idBytes;
}

string NandGeometry::getDesc() {
	return 0;
}

int NandGeometry::getPageSize() {
	return 0;
}

int NandGeometry::getOobSize() {
	return 0;
}

int NandGeometry::getSizeMB() {
	return 0;
}

long NandGeometry::getPagesCount() {
	return 0;
}

bool NandGeometry::isLargePage() {
	return false;
}

int NandGeometry::getAddrByteCount() {
	return 0;
}
