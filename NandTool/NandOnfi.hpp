#ifndef NANDONFI_HPP
#define NANDONFI_HPP

#include <string>
using namespace std;
#include "FtdiNand.hpp"
#include "onfi.h"

class NandOnfi {
public:
	NandOnfi(FtdiNand *fn);
	NandOnfi(char* raw_data);
	bool hasOnfi(void);
	void showOnfi(void);
	int readRaw(char* buffer, int max);
private:
	bool m_hasOnfi;
	bool m_readOnfi;
	ONFI_Param m_onfiData;
	short doCrc16(short input, char byte);
	void print8_hex(char* text, int value);
	void print8_hex_int(char* text, int value);
	void print16_hex(char* text, int value);
	void print16_hex_int(char* text, int value);
	void print32_hex_int(char* text, int value);
	void print_header(char* text);
	void print_indented(char* text);
	void print8_bytes_hex(char* header, char* buffer, int len);
	void print_chars(char* header, char* buffer, int len);
};

#endif