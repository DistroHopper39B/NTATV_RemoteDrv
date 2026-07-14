/*
 * Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

#include "version.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#ifdef APPLEIR_DLL
#define DLLFUNC EXPORT
#else
#define DLLFUNC IMPORT
#endif

#pragma pack(push, 1)
typedef enum
{
	success = 0,
	libusb_start_fail,
	device_init_fail,
	version_mismatch,
	appleir_error_max,
} appleir_status_codes;

typedef struct
{
	uint8_t flags;
	uint8_t vendor;
	uint8_t dat02;
	uint8_t dat03;
	uint8_t dat04;
} ir_command;
#pragma pack(pop) 

// Public handle type for IR handle.
#ifndef APPLEIR_DLL
typedef void *appleir_device_handle;
#endif

DLLFUNC int appleir_open(appleir_device_handle *device,
						 uint8_t vMaj,
						 uint8_t vMin,
						 uint8_t vPatch);
DLLFUNC void appleir_close(appleir_device_handle device);
DLLFUNC bool appleir_set_led(appleir_device_handle device, int mode);
DLLFUNC bool appleir_set_led_brightness(appleir_device_handle device, int high);
DLLFUNC bool appleir_get_raw_data(appleir_device_handle device, ir_command *command, bool keydown);
DLLFUNC void appleir_debug(bool set);
DLLFUNC const char *appleir_strerror(int error);

#ifdef __cplusplus
};
#endif
