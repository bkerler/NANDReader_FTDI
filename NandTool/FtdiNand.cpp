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

#include <stdio.h>
#include "FtdiNand.hpp"
#ifdef WIN32
#include <windows.h>
#endif

//Masks for the address lines (for addrh) the various nand latch lines are connected to.
#define ADR_WP 0x20
#define ADR_CL 0x40
#define ADR_AL 0x80

//Error handling routine.
int FtdiNand::error(const char *err) {
	printf("Error at %s: %s\n", err, ftdi_get_error_string(&m_ftdi));
	exit(0); //Dirty. Disable to continue after errors.
	return 0;
}

//Read bytes from the nand, with the cl and al lines set as indicated.
int FtdiNand::nandRead(int cl, int al, char *buf, int count) {
	unsigned char *cmds=new unsigned char[count*2+2];
	unsigned char *ftdata=new unsigned char[count*2];
	int x, i, ret;
	i=0;
	//Construct read commands. First one sets the cl and al lines too, rest just reads.
	for (x=0; x<count; x++) {
		if (x==0) {
			cmds[i++]=READ_EXTENDED;
			cmds[i++]=(cl?ADR_CL:0)|(al?ADR_AL:0);
			cmds[i++]=0;
		} else {
			cmds[i++]=READ_SHORT;
			cmds[i++]=0;
		}
	}
	cmds[i++]=SEND_IMMEDIATE;

//	printf("Cmd:\n");
//	for (x=0; x<i; x++) printf("%02hhx %s", cmds[x], ((x&15)==15)?"\n":"");
//	printf("\n\n");


	if (ftdi_write_data(&m_ftdi, cmds, i)<0) return error("writing cmd");
	if (m_slowAccess) {
		//Div by 5 mode makes the ftdi-chip return all databytes double. Compensate for that.
		ret=ftdi_read_data(&m_ftdi, ftdata, count*2);
		for (x=0; x<count; x++) buf[x]=ftdata[x*2];
		ret/=2;
	} else {
		ret=ftdi_read_data(&m_ftdi, ftdata, count);
		for (x=0; x<count; x++) buf[x]=ftdata[x];
	}

	if (ret<0) return error("reading data");
	if (ret<count) return error("short read");
//	printf("%i bytes read.\n", ret);

	delete[] cmds;
	delete[] ftdata;
	return ret;
}

//Write bytes to the nand, with al/cl set as indicated.
int FtdiNand::nandWrite(int cl, int al, char *buf, int count) {
	unsigned char *cmds=new unsigned char[count*3+1];
	int x, i;
	i=0;

	//Construct write commands. First one sets the cl and al lines too, rest just reads.
	for (x=0; x<count; x++) {
		if (x==0) {
			cmds[i++]=WRITE_EXTENDED;
			cmds[i++]=(cl?ADR_CL:0)|(al?ADR_AL:0);
			cmds[i++]=0;
		} else {
			cmds[i++]=WRITE_SHORT;
			cmds[i++]=0;
		}
		cmds[i++]=buf[x];
	}

//	printf("Cmd:\n");
//	for (x=0; x<i; x++) printf("%02hhx %s", cmds[x], ((x&15)==15)?"\n":"");
//	printf("\n\n");

	if (ftdi_write_data(&m_ftdi, cmds, i)<0) return error("writing cmd");
	delete[] cmds;
	return count;
}

FtdiNand::FtdiNand() {
	m_rbErrorCount=0;
}

//Try to find the ftdi chip and open it.
int FtdiNand::open(int vid, int pid, bool doslow) {
	unsigned char slow=DIS_DIV_5;
	if (doslow) slow=EN_DIV_5;
	m_slowAccess=doslow;
	//If vid/pid is zero, use default FT2232H vid/pid.
	if (vid==0) vid=0x0403;
	if (pid==0) pid=0x6010;
	//Open FTDI communications
	if (ftdi_init(&m_ftdi)<0) return error("init");
	if (ftdi_usb_open(&m_ftdi, vid, pid)<0) return error("open");
	if (ftdi_set_bitmode(&m_ftdi, 0, BITMODE_MCU)<0) error("bitmode");
	if (ftdi_write_data(&m_ftdi, &slow, 1)<0) return error("writing div5 cmd");
	if (ftdi_set_latency_timer(&m_ftdi, 1)<0) return error("setting latency");
	ftdi_usb_purge_buffers(&m_ftdi);
	return 1;
}

//Destructor: Close everything.
FtdiNand::~FtdiNand(void) {
	ftdi_usb_close(&m_ftdi);
	ftdi_deinit(&m_ftdi);
}

unsigned char FtdiNand::status() {
	unsigned char status;
	sendCmd(0x70); //NAND_CMD_STATUS
	readData((char*)&status,1);
	return status;
}

//Send a command to the NAND chip
int FtdiNand::sendCmd(char cmd) {
	return nandWrite(1, 0, &cmd, 1);
}

//Send an address to the NAND. addr is the address and it is send
//as noBytes bytes. (the amount of bytes differs between flash types.)
int FtdiNand::sendAddr(long long addr, int noBytes) {
	unsigned char buff[10];
	int x;
	for (x=0; x<noBytes; x++) {
		buff[x]=addr&0xff;
		addr=addr>>8;
	}
	return nandWrite(0, 1, (char*) buff, noBytes);
}

//Write data to the flash.
int FtdiNand::writeData(char *data, int count) {
	return nandWrite(0, 0, data, count);
}

//Read data from the flash.
int FtdiNand::readData(char *data, int count) {
	return nandRead(0, 0, data, count);
}

//Timeout in ms. Due to use of usleep(), not exact, but ballpark.
#define TIMEOUT_MSEC 100
#define TIMEOUT_RETRIES 15

//Waits till the R-/B-line to go high
int FtdiNand::waitReady() {
	unsigned char cmd=GET_BITS_HIGH;
	unsigned char resp;
	int x;
	if (m_rbErrorCount==-1) return 1; //Too many timeouts -> don't check R/B-pin

	for (x=0; x<TIMEOUT_MSEC; x++) {
		//Send the command to read the IO-lines
		if (ftdi_write_data(&m_ftdi, &cmd, 1)<0) return error("writing cmd");
		//Read response
		if (ftdi_read_data(&m_ftdi, &resp, 1)<=0) return error("writing cmd");
		//Return if R/B-line is high (=ready)
		if (resp&2) return 1;
	#ifdef WIN32
		Sleep(1);
	#else
		usleep(1000);
	#endif
	}
	printf("Timeout on R/B-pin; chip seems busy for too long!\n");
	m_rbErrorCount++;
	if (m_rbErrorCount>TIMEOUT_RETRIES) {
		printf("WARNING: Too many R/B-pin timeouts. Going to ignore this pin for now.\n");
		printf("DOUBLE CHECK IF THE CHIP IS READ OR WRITTEN CORRECTLY!\n");
		m_rbErrorCount=-1;
	}
}
