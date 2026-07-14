/*
 * Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "appleir.h"

bool debug = false;
static bool run_anyway = false;

static const char *appleir_error_str[] = {
	"Success",
	"LibUSB start failure",
	"Device initialization failure",
	"DLL and EXE version mismatch",
	"Unknown Error"
};

EXPORT const char *appleir_strerror(int error)
{
	if (error > appleir_error_max || error < success)
		return appleir_error_str[appleir_error_max];

	return appleir_error_str[error];
}

EXPORT int appleir_open(appleir_device_handle *device,
						uint8_t vMaj,
						uint8_t vMin,
						uint8_t vPatch)
{
	static libusb_device_handle *remote_handle = NULL;
	int status;

	if (run_anyway == false
        && vMaj != VERSION_MAJOR
        && vMin != VERSION_MINOR
        && vPatch != VERSION_PATCH)
	{
        /* We have to open() twice if the version is mismatched. */
		*device = NULL;
        run_anyway = true;
		return version_mismatch;
	}

	if (remote_handle)
	{
		*device = remote_handle;
		return success;
	}

	status = libusb_init(NULL);
	if (status < 0)
	{
		*device = NULL;
		return libusb_start_fail;
	}

	// Check to see if the IR receiver exists
	remote_handle = libusb_open_device_with_vid_pid(NULL,
													VENDOR_APPLE,
													PRODUCT_APPLETV_REMOTE);
	if (!remote_handle)
	{
		*device = NULL;
		return device_init_fail;
	}

	status = libusb_claim_interface(remote_handle, 0);
	if (status)
	{
		*device = NULL;
		return device_init_fail;
	}

	status = libusb_claim_interface(remote_handle, 1);
	if (status)
	{
		*device = NULL;
		return device_init_fail;
	}

	*device = remote_handle;
	return success;
}

EXPORT void appleir_close(appleir_device_handle device)
{
	libusb_device_handle *remote_handle = device;
	if (!device) return;

	libusb_release_interface(remote_handle, 0);
	libusb_release_interface(remote_handle, 1);
	libusb_close(remote_handle);

	libusb_exit(NULL);
}

EXPORT void appleir_debug(bool set)
{
	debug = set;
}
