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
#include <tlhelp32.h>

#include "libusb.h"

#include "ir.h"
#include "led.h"
#include "keymap.h"
 
#define VENDOR_APPLE 0x05ac
#define PRODUCT_APPLETV_REMOTE 0x8241

#define IS_APPLE_REMOTE(dev) ((dev->descriptor.idVendor == VENDOR_APPLE) && \
							 ((dev->descriptor.idProduct == PRODUCT_APPLETV_REMOTE)))


typedef enum
{
	success = 0,
	no_remote,
} error_codes;

extern bool debug;
#define dprintf(fmt, ...) if (debug) printf(fmt, ##__VA_ARGS__)
#define error(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)