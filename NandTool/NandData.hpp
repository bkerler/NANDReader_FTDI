#ifndef NANDDATA_HPP
#define NANDDATA_HPP

#include "FtdiNand.hpp"
#include "NandGeometry.hpp"

using namespace std;

class NandData {
public:
	NandData(FtdiNand *ftdi, NandGeometry *id);
	virtual int readPage(int pageno, char *buf, int max);
	virtual int readOob(int pageno, char *buf, int max);
	virtual int writePage(int pageno, char *buf, int len);
	virtual int eraseBlock(int block);
protected:
	FtdiNand *m_ft;
	NandGeometry *m_id;
};

#endif
