/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

#include <stdbool.h>

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

typedef struct
{
	unsigned char flags;
	unsigned char vendor;
	unsigned char dat02;
	unsigned char dat03;
	unsigned char dat04;
} ir_command;

/**
 * Specify every conceivable button that could be on a remote.
 * As more remotes are supported, this enum will grow.
 */

typedef enum
{
	REMOTE_BUTTON_NOT_PAIRED,

	REMOTE_BUTTON_APPLE_MENU,
	REMOTE_BUTTON_APPLE_PLAY_PAUSE,
	REMOTE_BUTTON_APPLE_FAST_FWD,
	REMOTE_BUTTON_APPLE_REWIND,
	REMOTE_BUTTON_APPLE_VOLUME_UP,
	REMOTE_BUTTON_APPLE_VOLUME_DOWN,
	REMOTE_BUTTON_APPLE_ALUMINUM_SELECT,

	REMOTE_BUTTON_0,
	REMOTE_BUTTON_1,
	REMOTE_BUTTON_2,
	REMOTE_BUTTON_3,
	REMOTE_BUTTON_4,
	REMOTE_BUTTON_5,
	REMOTE_BUTTON_6,
	REMOTE_BUTTON_7,
	REMOTE_BUTON_8,
	REMOTE_BUTON_9,
	REMOTE_BUTTON_A,
	REMOTE_BUTTON_B,
	REMOTE_BUTTON_C,
	REMOTE_BUTTON_D,
	REMOTE_BUTTON_E,
	REMOTE_BUTTON_F,

	REMOTE_BUTTON_UP,
	REMOTE_BUTTON_DOWN,
	REMOTE_BUTTON_LEFT,
	REMOTE_BUTTON_RIGHT,
	REMOTE_BUTTON_SELECT,
	REMOTE_BUTTON_RETURN,

	REMOTE_BUTTON_PLAY,
	REMOTE_BUTTON_PAUSE,
	REMOTE_BUTTON_STOP,
	REMOTE_BUTTON_SKIP,
	REMOTE_BUTTON_REPLAY,
	REMOTE_BUTTON_REWIND,
	REMOTE_BUTTON_FAST_FW,
	REMOTE_BUTTON_RECORD,
	REMOTE_BUTTON_VOL_UP,
	REMOTE_BUTTON_VOL_DOWN,
	REMOTE_BUTTON_MUTE,
	REMOTE_BUTTON_CHANNEL_UP,
	REMOTE_BUTTON_CHANNEL_DN,
	REMOTE_BUTTON_PREV_CH,

	REMOTE_BUTTON_POWER,
	REMOTE_BUTTON_MENU,
	REMOTE_BUTTON_SOURCE,
	REMOTE_BUTTON_SETUP,
	REMOTE_BUTTON_GUIDE,
	REMOTE_BUTTON_HOME,
	REMOTE_BUTTON_INFO,
	REMOTE_BUTTON_MY_TV,
	REMOTE_BUTTON_QUEUE,
	REMOTE_BUTTON_SLEEP,
	REMOTE_BUTTON_EXIT,
	REMOTE_BUTTON_MUSIC,
	REMOTE_BUTTON_MOVIES,
	REMOTE_BUTTON_PICTURES,
	REMOTE_BUTTON_HELP,

	REMOTE_BUTTON_F1,
	REMOTE_BUTTON_F2,
	REMOTE_BUTTON_F3,
	REMOTE_BUTTON_F4,
	REMOTE_BUTTON_F5,
	REMOTE_BUTTON_F6,
	REMOTE_BUTTON_F7,
	REMOTE_BUTTON_F8,
	REMOTE_BUTTON_F9,
	REMOTE_BUTTON_F10,
	REMOTE_BUTTON_F11,
	REMOTE_BUTTON_F12,
	REMOTE_BUTTON_F13,
	REMOTE_BUTTON_F14,

	REMOTE_BUTTON_RED,
	REMOTE_BUTTON_ORANGE,
	REMOTE_BUTTON_YELLOW,
	REMOTE_BUTTON_GREEN,
	REMOTE_BUTTON_BLUE,
	REMOTE_BUTTON_PURPLE,
	REMOTE_BUTTON_PINK,
	REMOTE_BUTTON_BLACK,
	REMOTE_BUTTON_WHITE,

	REMOTE_BUTTON_MAX,
	REMOTE_BUTTON_INVALID = 0xFF
} remote_button;

typedef struct
{
	volatile remote_button  button; // Intermediate button format
	volatile unsigned char 	modifiers; // Modifiers
	volatile unsigned char	key_code; // Virtual key_code code for SendKey
	volatile const char*	name;
} key_map;
// Public handle type for IR handle.
typedef void *appleir_device_handle;

// These will be exported in the DLL.
appleir_device_handle appleir_open(void);
void appleir_close(appleir_device_handle device);
void appleir_set_led(appleir_device_handle device, int mode);
void appleir_set_led_brightness(appleir_device_handle device, int high);
volatile key_map *appleir_get_keymap(void);
_Noreturn DWORD __stdcall appleir_remote_loop(appleir_device_handle device);
void appleir_debug(bool set);
