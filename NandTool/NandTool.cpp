          // NandTool.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef WIN32
#include <io.h>
#endif
#include "FtdiNand.hpp"
#include "FtdiDiag.hpp"
#include "NandChip.hpp"

#define ALL_TESTS	-1

//Windows needs O_BINARY to open binary files, but the flag is undefined
//on Unix. Hack around that. (Thanks, Shawn Hoffman)
#ifndef O_BINARY
#define O_BINARY 0
#endif 

void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

DWORD GetTickCountDiff(DWORD offset)
{
	//GetTickCount() could roll-over but binary math should do the magic and return correct value
	return GetTickCount() - offset;
}


#define PROGRESS_MESSAGE_SIZE	100
int printProgressIndicator(int page_number, int pages, float throughput_ratio)
{
	char progress_message[PROGRESS_MESSAGE_SIZE];
	int printout_strlen;
	snprintf(progress_message, PROGRESS_MESSAGE_SIZE, "Processing page %d of %d (%1.1f%%), throughput %1.2fKiB/s", page_number, pages, 100.0f*page_number / pages, throughput_ratio);
	printf("%s", progress_message); //"%s" to make sure % char will be not be removed
	printout_strlen = strlen(progress_message);
	for (int i = 0; i < printout_strlen; i++)
		printf("\b");
	return printout_strlen;
}

int _tmain(int argc, _TCHAR* argv[])
{
int x, r;
	int vid=0, pid=0;
	int dev_id = 0;
	int test_number = ALL_TESTS;//default
	bool err=false;
	bool doSlow=false;
	string file="";
	enum Action {
		actionNone=0,
		actionID,
		actionRead,
		actionWrite,
		actionVerify,
		actionDiagnostics
	};

#define PROGRESS_MESSAGE_SIZE	100
	char progress_message[PROGRESS_MESSAGE_SIZE];
	int progress_message_len;
	int progress_message_refresh_needed;
	DWORD progress_message_time_last_print;
#define PROGRESS_MESSAGE_REFRESH_TIME_500MS	500
	float throughput_ratio;
	DWORD throughput_time_start;
	float throughput_bytes;

	printf("FT2232H-based NAND reader\n");
	//Parse command line options
	Action action=actionNone;
	NandChip::AccessType access=NandChip::accessBoth;
	for (x=1; x<argc; x++) {
		if (strcmp(argv[x], "-i")==0) {
			action=actionID;
			}
		else if (strcmp(argv[x], "-d") == 0){
				action = actionDiagnostics;
				if (x<argc-1)
					{//there is something after -d parameter
						test_number = strtol(argv[++x], NULL, 0);
					}
			}
		else if (strcmp(argv[x],"-r")==0 && x<=(argc-2)) {
			action=actionRead;
			file=argv[++x];
//		} else if (strcmp(argv[x],"-w")==0 && x<=(argc-2)) {
//			action=actionWrite;
//			file=argv[++x];
		} else if (strcmp(argv[x], "-f") == 0 && x <= (argc - 2)) {
			dev_id = strtol(argv[++x], NULL, 0);
		} else if (strcmp(argv[x],"-v")==0 && x<=(argc-2)) {
			action=actionVerify;
			file=argv[++x];
		} else if (strcmp(argv[x],"-u")==0 && x<=(argc-2)) {
			action=actionVerify;
			char *endp;
			x++;
			vid=strtol(argv[x], &endp, 16);
			if (*endp!=':') {
				err=true;
			} else {
				endp++;
				pid=strtol(endp, NULL, 16);
			}
		} else if (strcmp(argv[x],"-t")==0 && x<=(argc-2)) {
			x++;
			if (strcmp(argv[x],"main")==0) {
				access=NandChip::accessMain;
			} else if (strcmp(argv[x], "oob")==0) {
				access=NandChip::accessOob;
			} else if (strcmp(argv[x], "both")==0) {
				access=NandChip::accessBoth;
			} else {
				printf("Must be 'main' 'oob' or 'both': %s\n", argv[x]);
				err=true;
			}
		} else if (strcmp(argv[x],"-s")==0) {
			doSlow=true;
		} else {
			printf("Unknown option or missing argument: %s\n", argv[x]);
			err=true;
		}
	}

	if (action==actionNone || err || argc==1) {
		printf("Usage: [-i|-r file|-v file|-d|-d test_no] [-t main|oob|both] [-s] [-f ftdi_id]\n");
		printf("  -i      - Identify chip\n");
		printf("  -r file - Read chip to file\n");
//		printf("  -w file - Write chip from file\n");
		printf("  -v file - Verify chip from file data\n");
		printf("  -t reg  - Select region to read/write (main mem, oob ('spare') data or both, interleaved)\n");
		printf("  -s      - clock FTDI chip at 12MHz instead of 60MHz\n");
		printf("  -u vid:pid - use different FTDI USB vid/pid. Vid and pid are in hex.\n");
		printf("  -d      - FTDI test mode, do all tests\n");
		printf("  -d test_no - FTDI test mode, one test\n");
		printf("  -f ftdi_id - number of FTDI device (default 0 = first detected)\n");
		exit(0);
	}

	if (actionDiagnostics == action)
	{
		FtdiDiag diag(dev_id);
		if (ALL_TESTS == test_number)
			diag.startAllTests();
		else
			diag.startOneTest(test_number);
		return 0;
	}

	FtdiNand ftdi;
	ftdi.open(vid, pid, doSlow, dev_id);
	NandChip nand(&ftdi);

	if (action==actionID) {
		nand.showInfo();
	} else if (action==actionRead || action==actionVerify) {
		int f;
		if (action==actionRead) {
			f=open(file.c_str(), O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
		} else {
			f=open(file.c_str(), O_RDONLY|O_BINARY);
		}
		if (f<0) {
			perror(file.c_str());
			exit(1);
		}
		NandID *id=nand.getIdPtr();
		long pages=(id->getSizeMB()*1024LL*1024LL)/id->getPageSize();
		int size=0;
		if (access==NandChip::accessMain) size=id->getPageSize();
		if (access==NandChip::accessOob) size=id->getOobSize();
		if (access==NandChip::accessBoth) size=id->getOobSize()+id->getPageSize();
		char *pageBuf=new char[size];
		char *verifyBuf=new char[size];
		int verifyErrors=0;
		nand.showInfo();
		printf("%sing %i pages of %i bytes...\n", action==actionRead?"Read":"Verify", pages, id->getPageSize());
		int oldpos=0;
		throughput_time_start = GetTickCountDiff(0);
		throughput_bytes = 0;
		throughput_ratio = 0.0;//questionable if it should 0 or INF when ratio is not known
		progress_message_refresh_needed = 1; //to make sure it is printed for first time
		for (x=0; x<pages; x++) {
			if (0 != progress_message_refresh_needed)
			{
				progress_message_len  = printProgressIndicator(x, pages, throughput_ratio);
				progress_message_time_last_print = GetTickCountDiff(0);
				progress_message_refresh_needed = 0;
			}
			nand.readPage(x, pageBuf, size, access);
			throughput_bytes += size;
			if (action==actionRead) {
				r=write(f, pageBuf, size);
				if (r!=size) {
					perror("writing data to file");
					exit(1);
				}
			} else {
				r=read(f, verifyBuf, size);
				if (r!=size) {
					perror("reading data from file");
					exit(1);
				}
				for (int y=0; y<size; y++) {
					if (verifyBuf[y]!=pageBuf[y]) {
						verifyErrors++;
						snprintf(progress_message, PROGRESS_MESSAGE_SIZE, "Verify error: Page %i, byte %i: file 0x%02hhX flash 0x%02hhX", x, y, verifyBuf[y], pageBuf[y]);
						//reusing progress_message buffer
						printf("%s", progress_message);
						if (progress_message_len > strlen(progress_message)) //if status message was longer than error message, which likely to happend
							for (int i = 0; i < progress_message_len - strlen(progress_message); i++)
								printf(" "); //overwrite the protrunding characters with spaces
						printf("\n");//new line
					}
				}
			}
			throughput_ratio = 1000.0f*throughput_bytes / (GetTickCountDiff(throughput_time_start)) / 1024;
			if (GetTickCountDiff(progress_message_time_last_print) > PROGRESS_MESSAGE_REFRESH_TIME_500MS)
				progress_message_refresh_needed = 1;
#ifdef OLD_STATUS
			int pos=((float)x/(float)pages*(float)100);
			if (pos>oldpos) {
				printf("%i/%i\n", x, pages);
				gotoxy(0,7);
				oldpos=pos;
			}
#endif
		}
		printProgressIndicator(pages, pages, throughput_ratio); //print summary at the end
		if (action==actionVerify) {
			printf("Verify: %i bytes differ between NAND and file.\n", verifyErrors);
		}
	}
	
	printf("\nAll done.\n"); 
	return 0;
}

