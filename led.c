/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "remote.h"

static int set_report(libusb_device_handle *handle, uint8_t *data, int len)
{
	unsigned char *type = data;
	int val;

	val = 0x300 | *type;
	
	int status = success;

	status = libusb_control_transfer(handle, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS
		| LIBUSB_RECIPIENT_INTERFACE, 9, val, 0,
		(uint8_t *) data, len, 1000);

	if (status != len)
	{
		error("Error! status=%s\n", libusb_strerror(status));
	}

	return (status != len);
}

void set_led_brightness(libusb_device_handle *handle, int high)
{
	unsigned char buf[5];

	memset(buf, 0, sizeof(buf));
	buf[0] = 0xd;

	if (high) {
		buf[1] = 6;
		set_report(handle, buf, sizeof(buf));
		buf[1] = 5; buf[2] = 1;
		set_report(handle, buf, 3);
	} else {
		buf[1] = 5;
		set_report(handle, buf, sizeof(buf));
		set_report(handle, buf, 3);
	}
}

void set_led(libusb_device_handle *handle, int mode)
{
	unsigned char buf[5];
	
	memset(buf, 0, sizeof(buf));
	buf[0] = 0xd; buf[1] = mode;

	switch (mode) {
	case LEDMODE_OFF: // no light
		set_report(handle, buf, sizeof(buf));
		buf[1] = 3;
		set_report(handle, buf, 3);
		buf[1] = 4;
		set_report(handle, buf, 3);
		break;

	case LEDMODE_AMBER: // solid light
		set_report(handle, buf, sizeof(buf));
		buf[1] = 3; buf[2] = 1;
		set_report(handle, buf, 3);
		buf[1] = 4; buf[2] = 0;
		set_report(handle, buf, 3);
		break;

	case LEDMODE_AMBER_BLINK: // blinking amber light
		set_report(handle, buf, sizeof(buf));
		buf[1] = 3;
		set_report(handle, buf, 3);
		buf[1] = 4;
		set_report(handle, buf, 3);
		buf[1] = 3; buf[2] = 2;
		set_report(handle, buf, 3);
		break;

	case LEDMODE_WHITE:
		set_report(handle, buf, sizeof(buf));
		set_report(handle, buf, 3);
		buf[1] = 4; buf[2] = 1;
		set_report(handle, buf, 3);
		break;

	case LEDMODE_WHITE_BLINK:
		set_report(handle, buf, sizeof(buf));
		buf[1] = 3;
		set_report(handle, buf, 3);
		buf[1] = 4;
		set_report(handle, buf, 3);
		buf[1] = 4; buf[2] = 2;
		set_report(handle, buf, 3);
		break;

	case LEDMODE_BOTH:
		buf[1] = 7;
		set_report(handle, buf, sizeof(buf));
		buf[1] = 6; buf[2] = 1;
		set_report(handle, buf, 3);
		break;
	}
}

void run_led_test(libusb_device_handle *remote_handle)
{
	for (int i = 0; i < LEDMODE_MAX; i++)
	{
		printf("Setting LED to mode %d...\n", i);
		set_led(remote_handle, i);
		sleep(5);
	}
	
	printf("Returning to most recent LED mode...\n");
	set_led(remote_handle, led_mode);
}