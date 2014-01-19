#ifndef NANDCHIP_HPP
#define NANDCHIP_HPP

#include "FtdiNand.hpp"
#include "NandID.hpp"
#include "NandData.hpp"

using namespace std;

class NandChip {
public:
	NandChip(FtdiNand *fn);
	~NandChip();
	enum AccessType {
		accessNone=0,
		accessMain=1,
		accessOob=2,
		accessBoth=3
	};
	void showInfo();
	int readPage(int page, char *buff, int count, AccessType access);
	int writePage(int page, char *buff, int count, AccessType access);
	NandID *getIdPtr();
private:
	FtdiNand *m_fn;
	NandID *m_id;
	NandData *m_data;
};

#endif