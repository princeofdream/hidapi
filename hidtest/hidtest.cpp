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

#if 1
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

	// Set up the command buffer.
	memset(buf,0x00,sizeof(buf));
	buf[0] = 0x01;
	buf[1] = 0x81;


	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	//handle = hid_open(0x5131, 0x2007, NULL);
	handle = hid_open(0x5131, 0x2007, L"13579");

	if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}

	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read manufacturer string\n");
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read product string\n");
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read serial number string\n");
	printf("Serial Number String: (%d) %ls", wstr[0], wstr);
	printf("\n");

	// Read Indexed String 1
#if 1
	wstr[0] = 0x0000;
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read indexed string 1\n");
	printf("Indexed String 1: %ls\n", wstr);
#endif

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);

	// Try to read from the device. There shoud be no
	// data here, but execution should not block.
	res = hid_read(handle, buf, 17);

#if 0
	//Sleep(3000);
	// Send a Feature Report to the device
	buf[0] = 0x2;
	buf[1] = 0xa0;
	buf[2] = 0x0a;
	buf[3] = 0x00;
	buf[4] = 0x00;
	i0=0;
	while (i0<17)
	{
		printf("0x%02x ",buf[i0]);
		i0++;
	}
	printf("\n");
	res = hid_send_feature_report(handle, buf, 17);
	if (res < 0) {
		printf("Unable to send a feature report.\n");
	}
#endif
	//Sleep(3000);
	memset(buf,0,sizeof(buf));

#if 0
	// Read a Feature Report from the device
	buf[0] = 0x2;
	res = hid_get_feature_report(handle, buf, sizeof(buf));
	if (res < 0) {
		printf("Unable to get a feature report.\n");
		printf("%ls", hid_error(handle));
	}
	else {
		// Print out the returned buffer.
		printf("Feature Report\n   ");
		for (i = 0; i < res; i++)
			printf("%02hhx ", buf[i]);
		printf("\n");
	}
#endif
	memset(buf,0,sizeof(buf));

	//Sleep(3000);
	// Toggle LED (cmd 0x80). The first byte is the report number (0x1).
#if 1
	unsigned char cmd_buf[4];
	memset(buf,0x0,sizeof(buf));
	memset(cmd_buf,0x0,sizeof(cmd_buf));
	i0=0;
	while(i0<32)
	{
		buf[i0] = i0;
		i0++;
	}
	buf[0] = 0x01;

#if 0
	//protocol nec
	buf[1] = 0x00;
	buf[2] = 0x01;
	//vendor code
	buf[3] = 0x7A;
	buf[4] = 0xC5;
	//keycode
	buf[5] = 0x3C;
	buf[6] = 0xD6;
	//repeat
	buf[7] = 0x00;
	buf[8] = 0x00;
#else
	buf[1] = 0x00;
	buf[2] = 0x01;

	buf[3] = 0x00;
	buf[4] = 0xBD;
	buf[5] = 0xDB;
	buf[6] = 0x4F;

	buf[7] = 0x00;
	buf[8] = 0x00;
#endif


	i0=0;
	// while(i0<32/2)
	{
		res = hid_write(handle, buf, 16);
		if (res < 0) {
			printf("write Fail!");
		}
		// buf+=2;
		usleep(4*1000);
		i0++;
	}
#endif
#if 0
	memset(buf,0x0,sizeof(buf));
	buf[0] = 0x1;
	buf[1] = 0x80;
	i0=0+2;
	while(i0<32)
	{
		buf[i0] = i0;
		i0++;
	}
	res = hid_write(handle, buf, 32);
	if (res < 0) {
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
	}
	else
		printf("write %d bytes\n",res );
#endif

#if 0
	// Request state (cmd 0x81). The first byte is the report number (0x1).
	buf[0] = 0x1;
	buf[1] = 0x81;
	/* usleep(500000); */
	res = hid_write(handle, buf, 17);
	if (res < 0)
		printf("Unable to write() (2)\n");
	else
		printf("write %d bytes\n",res );
#endif
#if 0
	// Read requested state. hid_read() has been set to be
	// non-blocking by the call to hid_set_nonblocking() above.
	// This loop demonstrates the non-blocking nature of hid_read().
	res = 0;
	memset(buf,0x0,sizeof(buf));
	//Sleep(3000);
	while (res == 0) {
		res = hid_read(handle, buf, sizeof(buf));
		if (res == 0)
			printf("waiting...\n");
		if (res < 0)
			printf("Unable to read()\n");
		#ifdef WIN32
		Sleep(500);
		#else
		usleep(500*1000);
		#endif
	}
	printf("Data read:\n   ");
	// Print out the returned buffer.
	for (i = 0; i < res; i++)
		printf("%02hhx ", buf[i]);
#endif
	printf("\n");

	hid_close(handle);

	/* Free static HIDAPI objects. */
	hid_exit();

#ifdef WIN32
	system("pause");
#endif

	return 0;
}
