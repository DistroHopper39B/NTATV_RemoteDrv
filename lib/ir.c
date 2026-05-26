/*
 * Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "appleir.h"

#define APPLE_REMOTE_ENDPOINT 0x2

bool cache_flushed = false;

static
void flush_cache(appleir_device_handle device)
{
    int status;
    remote_handle = device;
    ir_command dummy;

    if (!device)
        return;

    do
    {
        status = libusb_interrupt_transfer(remote_handle,
                                           LIBUSB_ENDPOINT_IN | APPLE_REMOTE_ENDPOINT,
                                           (uint8_t *) &dummy,
                                           sizeof(dummy),
                                           NULL,
                                           10);
    } while (status == LIBUSB_SUCCESS);

    cache_flushed = true;
}

EXPORT bool appleir_get_raw_data(appleir_device_handle device, ir_command *command)
{
    int status, length;
    remote_handle = device;

    if (!cache_flushed)
        flush_cache(device);

    status = libusb_interrupt_transfer(remote_handle,
                                       LIBUSB_ENDPOINT_IN | APPLE_REMOTE_ENDPOINT,
                                       (uint8_t *) command,
                                       sizeof(*command),
                                       &length,
                                       128);

    return status == LIBUSB_SUCCESS ? true : false;
}
