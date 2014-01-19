#ifndef FTDINAND
#define FTDINAND

#include <ftdi.h>

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
	int nandRead(int cl, int al, char *buf, int count);
	int nandWrite(int cl, int al, char *buf, int count);
	struct ftdi_context m_ftdi;
	bool m_slowAccess;
	int m_rbErrorCount;
};

#endif
