/*******************************************************
 Windows HID simplification

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009

 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"

// Headers needed for sleeping.
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[256];
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;
	int i0,i1;

#ifdef WIN32
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
#endif

	struct hid_device_info *devs, *cur_dev;

	if (hid_init())
		return -1;

#if 0
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
#endif

	handle = hid_open(0x5131, 0x2007, L"13579");

	if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}
	//Sleep(3000);
	memset(buf,0x0,sizeof(buf));
	i0=0;
	while(i0<32)
	{
		buf[i0] = i0;
		i0++;
	}

	buf[1] = 0x00;
	buf[2] = 0x01;

	buf[3] = 0x00;
	buf[4] = 0xBD;
	buf[5] = 0xDB;
	buf[6] = 0x4F;

	buf[7] = 0x00;
	buf[8] = 0x00;


	i0=0;
	while(i0<4)
	{
		res = hid_write(handle, buf, 2);
		if (res < 0) {
			printf("write Fail!");
		}
		buf+=2;
		usleep(400);
		i0++;
	}
	printf("\n");
	hid_close(handle);
	hid_exit();

#ifdef WIN32
	system("pause");
#endif

	return 0;
}
