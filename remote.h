/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>

#include "libusb.h"
 
#define VENDOR_APPLE 0x05ac
#define PRODUCT_APPLETV_REMOTE 0x8241

#define IS_APPLE_REMOTE(dev) ((dev->descriptor.idVendor == VENDOR_APPLE) && \
                             ((dev->descriptor.idProduct == PRODUCT_APPLETV_REMOTE)))


							 
							 
typedef enum
{
    success = 0,
    no_remote,
} error_codes;
                             
typedef enum
{
    LEDMODE_OFF,
    LEDMODE_AMBER,
    LEDMODE_AMBER_BLINK,
    LEDMODE_WHITE,
    LEDMODE_WHITE_BLINK,
    LEDMODE_BOTH,
    LEDMODE_MAX
} led_modes;

typedef enum
{
    LED_BRIGHTNESS_LO,
    LED_BRIGHTNESS_HI,
    LED_BRIGHTNESS_MAX
} led_brightnesses;

typedef enum
{
	APPLE_REMOTE_PRESS		= 0x25,
	APPLE_REMOTE_REPEAT		= 0x26,
} apple_ir_flags;

typedef enum 
{
	APPLE_REMOTE_BUTTON		= 0xEE,
	APPLE_REMOTE_PAIRING	= 0xE0,
} apple_ir_events;


typedef enum
{
	APPLE_REMOTE_TIMEOUT	= 0x00,
	APPLE_REMOTE_RESVD		= 0x01,
	APPLE_REMOTE_MENU1		= 0x02,
	APPLE_REMOTE_MENU2		= 0x03,
	APPLE_REMOTE_PLAY1		= 0x04,
	APPLE_REMOTE_PLAY2		= 0x05,
	APPLE_REMOTE_RIGHT1		= 0x06,
	APPLE_REMOTE_RIGHT2		= 0x07,
	APPLE_REMOTE_LEFT1		= 0x08,
	APPLE_REMOTE_LEFT2		= 0x09,
	APPLE_REMOTE_UP1		= 0x0A,
	APPLE_REMOTE_UP2		= 0x0B,
	APPLE_REMOTE_DOWN1		= 0x0C,
	APPLE_REMOTE_DOWN2		= 0x0D,
} apple_ir_eventids_general;

typedef enum
{
	APPLE_REMOTE_PAIR1		= 0x02,
	APPLE_REMOTE_PAIR2		= 0x03,
	APPLE_REMOTE_UNPAIR1	= 0x04,
	APPLE_REMOTE_UNPAIR2	= 0x05,
} apple_ir_eventids_pairing;

typedef struct
{
	uint8_t flags;
	uint8_t unused;
	uint8_t event;
	uint8_t addr;
	uint8_t event_id;
} apple_ir_command;

#define APPLE_REMOTE_ENDPOINT 0x82

extern bool debug;
extern int led_brightness;
extern int led_mode;

#define dprintf(fmt, ...) if (debug) printf(fmt, ##__VA_ARGS__)

#define error(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

void set_led(libusb_device_handle *handle, int mode);
void set_led_brightness(libusb_device_handle *handle, int high);
void run_led_test(libusb_device_handle *remote_handle);
void process_signal(apple_ir_command *ir, int len);