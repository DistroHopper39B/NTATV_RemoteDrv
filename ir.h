/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

/* IR Remote */

typedef enum
{
	REMOTE_VENDOR_APPLE 	= 0x87,
	REMOTE_VENDOR_6B		= 0x6B, // Unknown vendor (0x6B)
	REMOTE_VENDOR_FE		= 0xFE, // Unknown vendor (0xFE)
} ir_vendors;

typedef struct
{
	uint8_t flags;
	uint8_t vendor;
	uint8_t dat02;
	uint8_t dat03;
	uint8_t dat04;
} ir_command;

/*
 * REMOTE DEVICE
 * Vendor: 	Apple
 * Device:	Apple Plastic Remote
 */

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

/*
 * REMOTE DEVICE
 * Vendor: 	Unknown/multiple
 * Device:	Vendor 6B
 */

typedef struct
{
	uint8_t flags; // Always 0x26
	uint8_t vendor;
	uint8_t __unknown2; // Always 0x86
	uint8_t button_1;
	uint8_t button_2;
} ir_command_vendor_6b;

/*
 * Power: ed 12
 * Up: e4 1b
 * Down: e5 1a
 * Left: fb 04
 * Right: f9 06
 * Center: fa 05
 */

typedef enum
{
	VEN6B_REMOTE_LEFT		= 0xfb04,
	VEN6B_REMOTE_CENTER		= 0xfa05,
	VEN6B_REMOTE_RIGHT		= 0xf906,
	VEN6B_REMOTE_POWER		= 0xed12,
	VEN6B_REMOTE_DOWN		= 0xe51a,
	VEN6B_REMOTE_UP			= 0xe41b,
} ir_buttons_vendor_6b;

/*
 * REMOTE DEVICE
 * Vendor: 	Unknown/multiple
 * Device:	Vendor FE
 */

typedef struct
{
	uint8_t flags; // Always 0x26
	uint8_t vendor;
	uint8_t __unknown; // Always 0x01
	uint8_t button_1;
	uint8_t button_2;
} ir_command_vendor_fe;

typedef enum
{
	VENFE_BUTTON_UP			= 0x6699,
	VENFE_BUTTON_DOWN		= 0x31ce,
	VENFE_BUTTON_LEFT		= 0x3ec1,
	VENFE_BUTTON_RIGHT		= 0x2dd2,
	VENFE_BUTTON_SELECT		= 0x35ca,
	VENFE_BUTTON_BACK		= 0x3ac5,
	VENFE_BUTTON_NUM1		= 0x6d92,
	VENFE_BUTTON_NUM2		= 0x6c93,
	VENFE_BUTTON_NUM3		= 0x33cc,
	VENFE_BUTTON_NUM4		= 0x718e,
	VENFE_BUTTON_NUM5		= 0x708f,
	VENFE_BUTTON_NUM6		= 0x37c8,
	VENFE_BUTTON_NUM7		= 0x758a,
	VENFE_BUTTON_NUM8		= 0x748b,
	VENFE_BUTTON_NUM9		= 0x3bc4,
	VENFE_BUTTON_NUM0		= 0x7887,
	VENFE_BUTTON_CHANNEL_UP	= 0x639c,
	VENFE_BUTTON_CHANNEL_DN	= 0x32cd,
	VENFE_BUTTON_VOLUME_UP	= 0x22dd,
	VENFE_BUTTON_MUTE		= 0x7689,
	VENFE_BUTTON_PLAY		= 0x27d8,
	VENFE_BUTTON_STOP		= 0x738c,
	VENFE_BUTTON_SKIP		= 0x659a,
	VENFE_BUTTON_REPLAY		= 0xbc43,
	VENFE_BUTTON_FASTFWD	= 0xad52,
	VENFE_BUTTON_REWIND		= 0xab54,
	VENFE_BUTTON_POWER		= 0xac53,
	VENFE_BUTTON_HOME		= 0xba45,
	VENFE_BUTTON_MYTV		= 0x6a95,
	VENFE_BUTTON_MUSIC		= 0xa956,
	VENFE_BUTTON_VIDEOS		= 0xa55a,
	VENFE_BUTTON_PICTURES	= 0xa659,
	VENFE_BUTTON_INFO		= 0x2fd0,
} ir_buttons_vendor_fe;

#define APPLE_REMOTE_ENDPOINT 0x2

bool process_signal(void *signal, int len);