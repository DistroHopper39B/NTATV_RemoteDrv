/*
 * Apple TV Remote and LED Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <getopt.h>
#include <windows.h>
#include <unistd.h>

#include <appleirapi.h>

#include "ir.h"

int atvclient_open_device(appleir_device_handle *device);
bool atvclient_set_led_mode(char *arg);
bool atvclient_set_led_brightness(char *arg);
DWORD __stdcall atvclient_remote_loop(appleir_device_handle device);
volatile key_map *atvclient_init_keymap(void);

extern bool client_debug;
