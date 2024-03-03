#pragma once
#ifndef FTDIDIAG
#define FTDIDIAG

//for FT_HANDLE typedef
#include <ftd2xx.h>

using namespace std;

class FtdiDiag {
public:
	FtdiDiag();
	~FtdiDiag();
	int startAllTests();
	int startOneTest(int test_number);
private:
	int executeTest(int test_number);
	int sleep1000untilEnterPressed();
	int isEnterPressed();
	int warning();
	int resetFtdi();
	int MAX_TESTS = 16;
	int testHalfHertzOnPin(int data_bit, FT_HANDLE handle, char* pin_name);
	int testHalfHertzOnControlPin(int data_bit, FT_HANDLE handle, char* pin_name);
	int testDataRead();
	int testRB();
	int testIORDY();
	FT_HANDLE ftdi_handle_channel_a;
	FT_HANDLE ftdi_handle_channel_b;
};

#endif