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

// IR Remote

typedef enum
{
	REMOTE_VENDOR_APPLE 	= 0x87,
	REMOTE_VENDOR_6B		= 0x6B, // Unknown vendor (0x6B)
	REMOTE_VENDOR_FE		= 0xFE, // Unknown vendor (0xFE)
} ir_vendors;

typedef struct
{
	uint8_t dat00;
	uint8_t vendor;
	uint8_t dat02;
	uint8_t dat03;
	uint8_t dat04;
} ir_command;

typedef struct
{
	uint8_t flags;
	uint8_t vendor;
	uint8_t event;
	uint8_t addr;
	uint8_t event_id;
} ir_command_vendor_apple;

typedef enum
{
	APPLE_REMOTE_PRESS		= 0x25,
	APPLE_REMOTE_REPEAT		= 0x26,
} ir_flags_apple;

typedef enum 
{
	APPLE_REMOTE_BUTTON		= 0xEE,
	APPLE_REMOTE_PAIRING	= 0xE0,
} ir_events_apple;

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
} ir_eventids_general_apple;

typedef enum
{
	APPLE_REMOTE_PAIR1		= 0x02,
	APPLE_REMOTE_PAIR2		= 0x03,
	APPLE_REMOTE_UNPAIR1	= 0x04,
	APPLE_REMOTE_UNPAIR2	= 0x05,
} ir_eventids_pairing_apple;

typedef struct
{
	uint8_t __unknown1; // Always 0x26
	uint8_t vendor;
	uint8_t __unknown2; // Always 0x86
	uint8_t __unknown3; // Changes depending on button
	uint8_t button;
} ir_command_vendor_6b;

typedef enum
{
	VEN6B_REMOTE_LEFT		= 0x04,
	VEN6B_REMOTE_CENTER		= 0x05,
	VEN6B_REMOTE_RIGHT		= 0x06,
	VEN6B_REMOTE_POWER		= 0x12,
	VEN6B_REMOTE_DOWN		= 0x1a,
	VEN6B_REMOTE_UP			= 0x1b,
} ir_buttons_vendor_6b;

#define APPLE_REMOTE_ENDPOINT 0x82

extern bool debug;
extern int led_brightness;
extern int led_mode;

#define dprintf(fmt, ...) if (debug) printf(fmt, ##__VA_ARGS__)

#define error(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

void set_led(libusb_device_handle *handle, int mode);
void set_led_brightness(libusb_device_handle *handle, int high);
void run_led_test(libusb_device_handle *remote_handle);
void process_signal(ir_command *ir, int len);