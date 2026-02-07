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

#include "keymap.h"
#include "led.h"
#include "ir.h"
#include "libusb.h"


#define VENDOR_APPLE 0x05ac
#define PRODUCT_APPLETV_REMOTE 0x8241

typedef libusb_device_handle *appleir_device_handle;
#define error(fmt, ...) fprintf(stderr, "(%s:%d) ", __FILE__, __LINE__); \
						fprintf(stderr, fmt, ##__VA_ARGS__)
