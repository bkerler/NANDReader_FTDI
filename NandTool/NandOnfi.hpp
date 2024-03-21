#ifndef NANDONFI_HPP
#define NANDONFI_HPP

#include <string>
using namespace std;
#include "FtdiNand.hpp"
#include "onfi.h"

class NandOnfi {
public:
	NandOnfi(FtdiNand *fn);
	bool hasOnfi(void);
	void showOnfi(void);
	void printPrintableChar(char c);
	int readRaw(char* buffer, int max);
private:
	bool m_hasOnfi;
	bool m_readOnfi;
	bool m_printItemCommaFlag;
	ONFI_Param m_onfiData;

};

#endif