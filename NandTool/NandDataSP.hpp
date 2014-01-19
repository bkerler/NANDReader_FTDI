#ifndef NANDDATASP_HPP
#define NANDDATASP_HPP

#include "NandData.hpp"

using namespace std;

class NandDataSP: public NandData {
public:
	NandDataSP(FtdiNand *ftdi, NandID *id);
	virtual int readPage(int pageno, char *buf, int max);
	virtual int readOob(int pageno, char *buf, int max);
	virtual int writePage(int pageno, char *buf, int len);
	virtual int eraseBlock(int block);
};


#endif