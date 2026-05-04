#include <array>
#include <thread>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <windows.h>
#include <highgui.h>
#include <stdlib.h>   
#include <cxcore.h>
#include <cxmisc.h> 
#include <cvaux.h>
#include <ctype.h> 
#include <stdio.h>
#include <math.h>

#include <cv.h>
#include <ml.h>

using namespace cv;
using namespace std;

#define num 2
#define COMPORT "\\\\.\\COM8"

int main()
{
	//int angle[num] = { -360, -345, -234, -180, -154, -134, -90, -45, -4, 0, 4, 24, 45, 100, 123, 178, 213, 245, 300, 323, 360 };
	//int angle[num] = { 6, -360, -120, -100, 0, 180, 359 };
	//int angle[num] = { 8, -234, -345, -56, 78, -48, 123, 234, 356 };
	//int angle[num] = { 1, 15 };
	int angle[num];
	angle[0] = 1;

	//char byte[num*3];
	//char end_byte[] = { 'e' };
	//for (int i = 0; i < num; i++)
	//{
	//	int temp = angle[i] + 360;
	//	byte[3*i] = temp / 100;
	//	byte[3*i+1] = (temp % 100)/10;
	//	byte[3*i+2] = (temp % 100) % 10;
	//	//cout << temp << endl;
	//}
	
	
	char byte[num * 2];

	char start_byte[] = "s";
	char end_byte[] = "e";

	//cout << start_byte << endl;
	//for (int i = 0; i < num; i++)
	//{
	//	int temp = angle[i] + 360;
	//	byte[2 * i] = temp / 256;
	//	byte[2 * i + 1] = temp % 256;
	//	cout << temp - 360 << "\t" << temp << "\t" << temp / 256 << "," << temp % 256 << endl;
	//}
	//cout << end_byte << endl;

	// Declare variables and structures
	HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };

	// Open the highest available serial port number
	fprintf(stderr, "Opening serial port...");
	hSerial = CreateFile(
		COMPORT, GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	else fprintf(stderr, "OK\n");

	// Set device parameters (38400 baud, 1 start bit,
	// 1 stop bit, no parity)
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	//if (GetCommState(hSerial, &dcbSerialParams) == 0)
	//{
	//	fprintf(stderr, "Error getting device state\n");
	//	CloseHandle(hSerial);
	//	return 1;
	//}

	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	//if (SetCommState(hSerial, &dcbSerialParams) == 0)
	//{
	//	fprintf(stderr, "Error setting device parameters\n");
	//	CloseHandle(hSerial);
	//	return 1;
	//}

	//// Set COM port timeout settings
	//timeouts.ReadIntervalTimeout = 50;
	//timeouts.ReadTotalTimeoutConstant = 50;
	//timeouts.ReadTotalTimeoutMultiplier = 10;
	//timeouts.WriteTotalTimeoutConstant = 50;
	//timeouts.WriteTotalTimeoutMultiplier = 10;
	//if (SetCommTimeouts(hSerial, &timeouts) == 0)
	//{
	//	fprintf(stderr, "Error setting timeouts\n");
	//	CloseHandle(hSerial);
	//	return 1;
	//}

	getchar();
	// Send specified text (remaining command line arguments)
	DWORD bytes_written, e_bytes_written, s_bytes_written = 0;
	while (1)
	{
		if (hSerial != INVALID_HANDLE_VALUE)
		{
			cout << "\npress angle to run: ";
			cin >> angle[1];

			for (int i = 0; i < num; i++)
			{
				int temp = angle[i] + 360;
				byte[2 * i] = temp / 256;
				byte[2 * i + 1] = temp % 256;
				//cout << temp - 360 << "\t" << temp << "\t" << temp / 256 << "," << temp % 256 << endl;
			}

			//WriteFile(hSerial, bytes_to_send, 10, &bytes_written, NULL);
			WriteFile(hSerial, start_byte, 1, &s_bytes_written, NULL);
			WriteFile(hSerial, byte, num * 2, &bytes_written, NULL);
			WriteFile(hSerial, end_byte, 1, &e_bytes_written, NULL);
		}
		//if (!send)
		////if (!WriteFile(hSerial, bytes_to_send, strlen(bytes_to_send), &bytes_written, NULL))
		//{
		//	fprintf(stderr, "Error\n");
		//	CloseHandle(hSerial);
		//	/*return 0;*/
		//}
		fprintf(stderr, "%d start bytes written\n", s_bytes_written);
		fprintf(stderr, "%d bytes written\n", bytes_written);
		fprintf(stderr, "%d end bytes written\n", e_bytes_written);
		//std::this_thread::sleep_for(std::chrono::milliseconds(int(1 * 1000)));
		getchar();
	}


	// Close serial port
	fprintf(stderr, "Closing serial port...");
	if (CloseHandle(hSerial) == 0)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	fprintf(stderr, "OK\n");

	getchar();
	// exit normally
	return 0;
}