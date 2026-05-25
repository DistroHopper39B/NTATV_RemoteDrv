/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

#define _WIN32_WINNT        0x0501
#define WINVER              0x0501

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include <windows.h>

#include "libusb.h"
typedef libusb_device_handle *appleir_device_handle;

#define APPLEIR_DLL
#include <appleirapi.h>

//#include "keymap.h"
//#include "ir.h"

#define VENDOR_APPLE 0x05ac
#define PRODUCT_APPLETV_REMOTE 0x8241

#define SHARED __attribute__((section(".shr"), shared))

extern bool debug;
extern SHARED libusb_device_handle *remote_handle;

#define error(fmt, ...) fprintf(stderr, "(%s:%d) ", __FILE__, __LINE__); \
						fprintf(stderr, fmt, ##__VA_ARGS__)
