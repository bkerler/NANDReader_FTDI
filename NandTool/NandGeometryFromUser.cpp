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

#include "NandGeometryFromUser.hpp"

//Constructor: construct NandGeometry info from argument provided by command line.
NandGeometryFromUser::NandGeometryFromUser(FtdiNand *fn, unsigned char *idBytes, char* geometry) : NandGeometry(idBytes)
{
	char *firstSubArgPtr, *secondSubArgPtr;
	bool pagemode_correct = false;
	bool addrmode_correct = false;
	//example user param 10000:simple4B:long:2048:16
	m_nandDesc = "user defined, geometry " + string(geometry);

	firstSubArgPtr = geometry;
	for (int param = 0; param < 4; param++)
	{
		secondSubArgPtr = strchr(firstSubArgPtr, ':');
		if ((NULL == secondSubArgPtr)&&(3 != param)) //no next subarg
		{
			printf("missing parameter in geometry definition\n");
			printf("template: %s\n", getTextTemplate());
			exit(1);
		}
		if (NULL != secondSubArgPtr)
			*secondSubArgPtr = '\0'; //convert separator to end of string
		//process parameter
		switch (param)
		{
			case 0:
				m_pagesCount = strtol(firstSubArgPtr, NULL, 0);
				if (0 >= m_pagesCount)
				{
					printf("Number of pages less or equal then zero\n");
					exit(1);
				}
				break;
			case 1:
				if (0 == strcmp(firstSubArgPtr, "LPsimple5B"))
				{
					m_nandaddrcyc = 5;
					m_nandIsLP = true;
					addrmode_correct = true;
				}
				if (0 == strcmp(firstSubArgPtr, "LPsimple4B"))
				{
					m_nandaddrcyc = 4;
					m_nandIsLP = true;
					addrmode_correct = true;
				}
				if (0 == strcmp(firstSubArgPtr, "LPsimple3B"))
				{
					m_nandaddrcyc = 3;
					m_nandIsLP = true;
					addrmode_correct = true;
				}
				if (0 == strcmp(firstSubArgPtr, "SPsimple5B"))
				{
					m_nandaddrcyc = 5;
					m_nandIsLP = false;
					addrmode_correct = true;
				}
				if (0 == strcmp(firstSubArgPtr, "SPsimple4B"))
				{
					m_nandaddrcyc = 4;
					m_nandIsLP = false;
					addrmode_correct = true;
				}
				if (0 == strcmp(firstSubArgPtr, "SPsimple3B"))
				{
					m_nandaddrcyc = 3;
					m_nandIsLP = false;
					addrmode_correct = true;
				}
				if (false == addrmode_correct)
				{
					printf("Supported addressing modes are 'SPsimple3B', 'SPsimple4B' 'SPsimple5B', 'LPsimple3B', 'LPsimple4B' or 'LPsimple5B'\n");
					exit(1);
				}
				break;
			case 2:
				m_nandPageSz = strtol(firstSubArgPtr, NULL, 0);
				if (0 >= m_nandPageSz)
				{
					printf("Size of main area of page less or equal then zero\n");
					exit(1);
				}
				break;
			case 3:
				m_nandOobSz = strtol(firstSubArgPtr, NULL, 0);
				if (0 >= m_nandOobSz)
				{
					printf("Size of spare area of page less or equal then zero\n");
					exit(1);
				}
				break;
		}
		firstSubArgPtr = secondSubArgPtr+1; //one char after semicolon replaced by null char
	}
}

string NandGeometryFromUser::getDesc() {
	return m_nandDesc;
}

int NandGeometryFromUser::getPageSize() {
	return m_nandPageSz;
}

int NandGeometryFromUser::getOobSize() {
	return m_nandOobSz;
}

long NandGeometryFromUser::getPagesCount() {
	return m_pagesCount;
}

bool NandGeometryFromUser::isLargePage() {
	return m_nandIsLP;
}

int NandGeometryFromUser::getAddrByteCount() {
	return m_nandaddrcyc;
}

int NandGeometryFromUser::getSizeMB() {
	return m_pagesCount*m_nandPageSz / 1024 / 1024;
}

string NandGeometryFromUser::getTextTemplate() {
	return "pages:addressing:main_size:spare_size";
}
