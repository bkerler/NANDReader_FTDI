#ifndef NANDDATALP_HPP
#define NANDDATALP_HPP

#include "NandData.hpp"

using namespace std;

class NandDataLP: public NandData {
public:
	NandDataLP(FtdiNand *ftdi, NandID *id);
	virtual int readPage(int pageno, char *buf, int max);
	virtual int readOob(int pageno, char *buf, int max);
	virtual int writePage(int pageno, char *buf, int len);
	virtual int eraseBlock(int pageno);
};


#endif