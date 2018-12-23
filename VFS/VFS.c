// VFS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Device.h"

int main(int argc, char** argv)
{
	// TODO: implement driver to emulate real FS
	int Status = DeviceIoControl(argc, argv);
	return Status;
}
