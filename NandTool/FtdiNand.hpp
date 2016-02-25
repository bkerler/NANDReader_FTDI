#ifndef FTDINAND
#define FTDINAND

#include <ftd2xx.h>

using namespace std;

class FtdiNand {
public:
	FtdiNand();
	~FtdiNand();
	int open(int vid, int pid, bool doslow);
	int sendCmd(char cmd);
	int sendAddr(long long addr, int noBytes);
	int writeData(char *data, int count);
	int readData(char *data, int count);
	int waitReady();
	unsigned char status();
private:
	int error(const char *err);
	FT_STATUS nandRead(int cl, int al, char *buf, int count);
	int nandWrite(int cl, int al, char *buf, int count);
	FT_HANDLE m_ftdi;
	bool m_slowAccess;
	int m_rbErrorCount;
};

#endif
