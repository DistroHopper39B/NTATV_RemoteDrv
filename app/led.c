/*
 * Apple TV Remote and LED Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "atvclient.h"

bool atvclient_set_led_mode(char *arg)
{
    bool success = false;

    appleir_device_handle device = NULL;

    if (strlen(arg) != 1 || !isdigit(arg[0]))
        return false;

    if (atvclient_open_device(&device))
        return false;

    int led_mode = strtol(arg, NULL, 0);

    success = appleir_set_led(device, led_mode);
    if (success) appleir_close(device);
    return success;
}

bool atvclient_set_led_brightness(char *arg)
{
    bool success = false;

    appleir_device_handle device = NULL;

    if (strlen(arg) != 1 || !isdigit(arg[0]))
        return false;

    if (atvclient_open_device(&device))
        return false;

    int led_brightness = strtol(arg, NULL, 0);

    success = appleir_set_led_brightness(device, led_brightness);
    if (success) appleir_close(device);
    return success;
}
