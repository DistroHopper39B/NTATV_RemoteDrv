/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "appleir.h"

bool debug = false;
bool initialized = false;
libusb_device_handle *remote_handle = NULL;

/**
 * appleir_open(appleir_device_handle)
 * @return The device handle, or NULL if the device is not open.
 */
__declspec(dllexport) appleir_device_handle appleir_open(void)
{
	int status;

	if (initialized) // New instance
		return remote_handle;

	status = libusb_init(NULL);
	if (status < 0)
	{
		error("LibUSB failed to start: %s (%d)\n", libusb_strerror(status), status);
		return NULL;
	}

	// Check to see if the IR receiver exists
	remote_handle = libusb_open_device_with_vid_pid(NULL,
													VENDOR_APPLE,
													PRODUCT_APPLETV_REMOTE);
	if (!remote_handle)
	{
		error("No IR receiver found!\n");
		return NULL;
	}

	status = libusb_claim_interface(remote_handle, 0);
	if (status)
	{
		error("Cannot claim interface 0: %s (%d)\n", libusb_strerror(status), status);
		return NULL;
	}

	status = libusb_claim_interface(remote_handle, 1);
	if (status)
	{
		error("Cannot claim interface 1: %s (%d)\n", libusb_strerror(status), status);
		return NULL;
	}

	initialized = true;

	return remote_handle;
}

/**
 * appleir_close(appleir_device_handle)
 * @param device The device to close.
 */
__declspec(dllexport) void appleir_close(appleir_device_handle device)
{
	remote_handle = device;

	if (remote_handle)
	{
		libusb_release_interface(remote_handle, 0);
		libusb_release_interface(remote_handle, 1);
		libusb_close(remote_handle);
	}

	libusb_exit(NULL);
}

__declspec(dllexport) void appleir_debug(bool set)
{
	debug = set;
}