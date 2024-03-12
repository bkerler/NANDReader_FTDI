#include "FtdiDiag.hpp"
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <conio.h>
#include <Windows.h>
#include <ftd2xx.h>

FtdiDiag::~FtdiDiag(void)
{
	FT_Close(ftdi_handle_channel_a);
	FT_Close(ftdi_handle_channel_b);
}

FtdiDiag::FtdiDiag(int dev_id)
{
	DWORD number_of_ftdis;
	DWORD Flags;
	DWORD ID;
	DWORD Type;
	DWORD LocId;
	char SerialNumber[16];
	char Description[64];
	FT_HANDLE ftdi_temp;

	if (FT_OK != FT_CreateDeviceInfoList(&number_of_ftdis))
	{
		printf("Error: FT_CreateDeviceInfoList\n");
		exit(1);
	}
	printf("Number of FTDI devices %d\n", number_of_ftdis);

	for (unsigned int i = 0; i < number_of_ftdis; i++)
	{
		if (FT_OK != FT_GetDeviceInfoDetail(i, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftdi_temp))
		{
			printf("Error: FT_GetDeviceInfoDetail\n");
			exit(1);
		}
		printf("Device #%d:\n", i);
		printf(" -Flgs 0x%x\n", Flags);
		printf(" -Type 0x%x\n", Type);
		printf(" -ID   0x%x\n", ID);
		printf(" -SN   %s\n", SerialNumber);
		printf(" -Desc %s\n", Description);
	}

	if (FT_OK != FT_Open(dev_id, &ftdi_handle_channel_a))
	{
		printf("Error: FT_OPEN channel a\n");
		exit(1);
	}
	if (FT_OK != FT_Open(dev_id + 1, &ftdi_handle_channel_b))
	{
		printf("\nUnable to open FTDI device #%d (ussually channel B has id next to channel A)\n", dev_id + 1);
		printf("It could also be caused by channel B open by another process.\n");
		if (dev_id == 0) //will not try to open device number -1
		{
			printf("Channel A was selected as FTDI device #0, then channel B should be #1 but it failed to open.\n");
			printf("No alternative scenario is possible, application does not support manual selecting FTDI device number for channel B.\n");
			exit(1);
		}
		printf("Trying an reverse channel order.\n");
		printf("If channel A is #%d, then channel B can be #%d.\n", dev_id, dev_id - 1);
		if (FT_OK != FT_Open(dev_id - 1, &ftdi_handle_channel_b))
		{
			printf("Error: FT_OPEN channel b\n");
			exit(1);
		}
		printf("FTDI device #%d used as channel B.\n\n", dev_id - 1);
	}
	resetFtdi();
}

int FtdiDiag::startAllTests()
{
	warning();
	for (int i = 0; i < MAX_TESTS; i++) 
		executeTest(i);
	return 0;
}

int FtdiDiag::warning()
{
	printf("Executing test will set various combination of signal on outputs.\n");
	printf("Make sure NAND memory is not connected. Generated signals may affect the memory state or the memory may affect testing process.\n");
	printf("Please prepare a tool to observe signals (voltmeter, oscilloscope or just an LED) and something for making shorts.\n");
	printf("Be warned, sometimes FTDI a \"ticklish\" may stop reponding when touched by bare hands.\n");
	printf("Press S (capital letter) to start the tests, any other to exit.\n\n");
	if ('S' != _getch())
		exit(0);
	return 0;
}

int FtdiDiag::sleep1000untilEnterPressed()
{
	for (int i = 0; i < 10; i++)
	{
		Sleep(100);
		if (isEnterPressed())
			return 1;
	}
	return 0;
}

int FtdiDiag::isEnterPressed()
{
	if (!_kbhit())
		return 0; //no key in buffer
	if (13 == _getch())
		return 1; //enter detected
	return 0;
}

int FtdiDiag::startOneTest(int test_number)
{
	if (test_number > MAX_TESTS)
		{
			printf("no test number %d, exiting\n", test_number);
			exit(1);
		}
	warning();
	executeTest(test_number);
	return 0;
}

int FtdiDiag::executeTest(int test_number)
{
	printf("Starting the test %d\n", test_number);
	switch (test_number)
	{
		case 0:testHalfHertzOnPin(0, ftdi_handle_channel_a, "AD0 (NAND D0)");
			break;
		case 1:testHalfHertzOnPin(1, ftdi_handle_channel_a, "AD1 (NAND D1)");
			break;
		case 2:testHalfHertzOnPin(2, ftdi_handle_channel_a, "AD2 (NAND D2)");
			break;
		case 3:testHalfHertzOnPin(3, ftdi_handle_channel_a, "AD3 (NAND D3)");
			break;
		case 4:testHalfHertzOnPin(4, ftdi_handle_channel_a, "AD4 (NAND D4)");
			break;
		case 5:testHalfHertzOnPin(5, ftdi_handle_channel_a, "AD5 (NAND D5)");
			break;
		case 6:testHalfHertzOnPin(6, ftdi_handle_channel_a, "AD6 (NAND D6)");
			break;
		case 7:testHalfHertzOnPin(7, ftdi_handle_channel_a, "AD7 (NAND D7)");
			break;
		case 8:testDataRead(); 
			break;
		case 9:testHalfHertzOnPin(2, ftdi_handle_channel_b, "#RD pin (NAND ~RE)");
			break;
		case 10:testHalfHertzOnPin(3, ftdi_handle_channel_b, "#WE pin (NAND ~WE)");
			break;
		case 11:testRB();
			break;
		case 12:testHalfHertzOnControlPin(5, ftdi_handle_channel_a, "A13 pin (NAND WP)");
			break;
		case 13:testHalfHertzOnControlPin(6, ftdi_handle_channel_a, "A14 pin (NAND CLE)");
			break;
		case 14:testHalfHertzOnControlPin(7, ftdi_handle_channel_a, "A15 pin (NAND ALE)");
			break;
		case 15:testIORDY();
			break;
		}
	resetFtdi();
	printf("Test %d completed\n\n", test_number);
	return 0;
}

int FtdiDiag::resetFtdi()
{
	if (FT_OK != FT_SetBitMode(ftdi_handle_channel_a, 0, FT_BITMODE_RESET)) //reset FTDI after test
	{
		printf("Error: FT_SetBitMode ChA reset\n");
		exit(1);
	}
	if (FT_OK != FT_SetBitMode(ftdi_handle_channel_b, 0, FT_BITMODE_RESET)) //reset FTDI after test
	{
		printf("Error: FT_SetBitMode ChB reset\n");
		exit(1);
	}
	return 0;
}

int FtdiDiag::testHalfHertzOnPin(int data_bit, FT_HANDLE handle, char* pin_name)
{
	DWORD data_sent;
	BYTE data_out_1[1], data_out_0[1];

	printf("The test will generate a ~0.5Hz signal on %s, other pins configured as input.\n", pin_name);
	printf("To end the test, press ENTER.\n");
	if (FT_OK != FT_SetBitMode(handle, 1<<data_bit, FT_BITMODE_ASYNC_BITBANG))
	{
		printf("Error: FT_SetBitMode #1\n");
		exit(1);
	}

	data_out_0[0] = 0x00;
	data_out_1[0] = 1 << data_bit;
	while (1)
	{
		if (FT_OK != FT_Write(handle, data_out_1, 1, &data_sent))
		{
			printf("Error: FT_Write #1\n");
			exit(1);
		}
		if (sleep1000untilEnterPressed())
			break;
		if (FT_OK != FT_Write(handle, data_out_0, 1, &data_sent))
		{
			printf("Error: FT_Write #2\n");
			exit(1);
		}
		if (sleep1000untilEnterPressed())
			break;
	}
	if (FT_OK != FT_Write(handle, data_out_0, 1, &data_sent))
	{
		printf("Error: FT_Write #3\n");
		exit(1);
	}
	return 0;
}

int FtdiDiag::testDataRead()
{
	printf("The test will read data pins and display them on the screen.\n");
	printf("The FTDI chip has internal pull-up, without any stimulation all pins should be read as '1'.\n");
	printf("Short the data pin to ground and check if specific bit is '0' and only this bit.\n");
	printf("To end the test, press ENTER.\n");
	if (FT_OK != FT_SetBitMode(ftdi_handle_channel_a, 0, FT_BITMODE_ASYNC_BITBANG))
	{
		printf("Error: FT_SetBitMode #1\n");
		exit(1);
	}
	BYTE data_read;
	char bit_values[8];
	char output_str [50];
	while (1)
	{
		if (FT_OK != FT_GetBitMode(ftdi_handle_channel_a, &data_read))
		{
			printf("Error: FT_GetBitMode\n");
			exit(1);
		}
		for (int i = 0; i < 8; i++)
		{
			if (data_read & 0x01)
				bit_values[i] = '1';
			else
				bit_values[i] = '0';
			data_read = data_read >> 1;
		}
		snprintf(output_str, 50, "Data bus values (D0) %c%c%c%c%c%c%c%c (D7)", bit_values[0], bit_values[1], bit_values[2], bit_values[3], bit_values[4], bit_values[5], bit_values[6], bit_values[7]);
		printf(output_str);
		for (unsigned int i = 0; i < strlen(output_str); i++)
			printf("\b");
		if (isEnterPressed())
			break;
	}
	printf("\n"); 
	return 0;
}

int FtdiDiag::testRB()
{
	printf("The test will read RB pin and display it on the screen.\n");
	printf("The FTDI chip has internal pull-up, without any stimulation all pins should be read as '1'.\n");
	printf("Short RB pin to ground and check if the reading goes to '0'.\n");
	printf("To end the test, press ENTER.\n");
	if (FT_OK != FT_SetBitMode(ftdi_handle_channel_b, 0, FT_BITMODE_ASYNC_BITBANG))
	{
		printf("Error: FT_SetBitMode #1\n");
		exit(1);
	}
	BYTE data_read;
	while (1)
	{
		if (FT_OK != FT_GetBitMode(ftdi_handle_channel_b, &data_read))
		{
			printf("Error: FT_GetBitMode\n");
			exit(1);
		}
		if (data_read & 0x80)
			printf("RB pin state 1");
		else
			printf("RB pin state 0");
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		if (isEnterPressed())
			break;
	}
	printf("\n");
	return 0;
}

int FtdiDiag::testHalfHertzOnControlPin(int data_bit, FT_HANDLE handle, char* pin_name)
{
	DWORD data_sent;
	BYTE data_out_1[3], data_out_0[3];

	printf("The test will generate a ~0.5Hz signal on %s, other pins configured as input.\n", pin_name);
	printf("To end the test, press ENTER.\n");
	if (FT_OK != FT_SetBitMode(handle, 1 << data_bit, FT_BITMODE_MPSSE))
	{
		printf("Error: FT_SetBitMode #1\n");
		exit(1);
	}

	data_out_0[0] = 0x82;
	data_out_0[1] = 0; 
	data_out_0[2] = 1 << data_bit;
	data_out_1[0] = 0x82;
	data_out_1[1] = 1 << data_bit;
	data_out_1[2] = 1 << data_bit;
	while (1)
	{
		if (FT_OK != FT_Write(handle, data_out_1, 3, &data_sent))
		{
			printf("Error: FT_Write #1\n");
			exit(1);
		}
		if (sleep1000untilEnterPressed())
			break;
		if (FT_OK != FT_Write(handle, data_out_0, 3, &data_sent))
		{
			printf("Error: FT_Write #2\n");
			exit(1);
		}
		if (sleep1000untilEnterPressed())
			break;
	}
	if (FT_OK != FT_Write(handle, data_out_0, 3, &data_sent))
	{
		printf("Error: FT_Write #3\n");
		exit(1);
	}
	return 0;
}

int FtdiDiag::testIORDY()
{
	int number_of_ones, number_of_zeros;
	printf("The test will monitor state of IORDY pin for a while.\n");
	printf("Wait for 10000 samples or press ENTER to end the test and get the results.\n");
	if (FT_OK != FT_SetBitMode(ftdi_handle_channel_b, 0, FT_BITMODE_ASYNC_BITBANG))
	{
		printf("Error: FT_SetBitMode\n");
		exit(1);
	}
	number_of_ones = 0;
	number_of_zeros = 0;
	BYTE data_read;
	for (int i = 0; i < 10000; i++)
	{
		if(FT_OK != FT_GetBitMode(ftdi_handle_channel_b, &data_read))
		{
			printf("Error: FT_GetBitMode\n");
			exit(1);
		}
		if (i>100)	//at least 100 samples
			if (isEnterPressed())
				break;
		if (data_read & 0x10)
			number_of_ones++;
		else
			number_of_zeros++;
	}
	printf("Number of samples with value '0' %d\n", number_of_zeros);
	printf("Number of samples with value '1' %d\n", number_of_ones);
	if (number_of_ones == 0) 
		printf("No samples with value '1', looks like IORDY is connected correctly.\n");
	else
	{
		if (number_of_zeros == 0)
			printf("Pull-down or short to ground not active at all.\n");
		else
			printf("The connection to the IORDY line is not stable.\n");
		printf("!!!!!!!!!!!! It will make communication unstable. It is adviced to fix it. !!!!!!!!!!!!\n");
	}
	return 0;
}

