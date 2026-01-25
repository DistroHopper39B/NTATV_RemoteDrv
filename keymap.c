/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "remote.h"

bool map_initialized = false;

static const char *remote_button_name[REMOTE_BUTTON_MAX] =
{
		"Apple_Menu",
		"Apple_Play/Pause",
		"Apple_Fast Forward",
		"Apple_Rewind",
		"Apple_Volume Up",
		"Apple_Volume Down",
		"Apple_Select",

		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",

		"Up",
		"Down",
		"Left",
		"Right",
		"Select",
		"Return",

		"Play",
		"Pause",
		"Stop",
		"Skip",
		"Replay",
		"Rewind",
		"Fast Forward",
		"Record",
		"Volume Up",
		"Volume Down",
		"Mute",
		"Channel Up",
		"Channel Down",
		"Previous Channel",

		"Power",
		"Menu",
		"Source",
		"Setup",
		"Guide",
		"Home",
		"Info",
		"My TV",
		"Queue",
		"Sleep",
		"Exit",
		"Music",
		"Movies",
		"Pictures",
		"Help",

		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"F13",
		"F14",

		"Red",
		"Orange",
		"Yellow",
		"Green",
		"Blue",
		"Purple",
		"Pink",
		"Black",
		"White",
};

key_map map[REMOTE_BUTTON_MAX];
/*
uint8_t get_modifiers(remote_button button)
{
	return map[button].modifiers;
}

uint8_t get_key(remote_button button)
{
	return map[button].key_code;
}

const char *get_name(remote_button button)
{
	return map[button].name;
}
 */

void press_key(remote_button button)
{
	int inputs_count = 0;
	INPUT inputs[10];

	if (!map_initialized)
	{
		error("Keymap not initialized\n");
		return;
	}

	if (button >= REMOTE_BUTTON_MAX)
	{
		error("Button %d too high\n", button);
		return;
	}

	key_map key = map[button];

	printf("Executing action: %s\n", key.name);

	if (key.key_code == 0)
	{
		error("Button %d does not correspond with key code\n", button);
		return;
	}

	// Key down for modifiers
	if (key.modifiers & MOD_WIN)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_LWIN;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= 0;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	if (key.modifiers & MOD_CONTROL)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_CONTROL;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= 0;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	if (key.modifiers & MOD_SHIFT)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_SHIFT;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= 0;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	// Key down for main key
	inputs[inputs_count].type 			= INPUT_KEYBOARD;
	inputs[inputs_count].ki.wVk			= key.key_code;
	inputs[inputs_count].ki.wScan		= 0;
	inputs[inputs_count].ki.dwFlags		= 0;
	inputs[inputs_count].ki.time		= 0;
	inputs[inputs_count].ki.dwExtraInfo	= 0;
	inputs_count++;

	// Key up for main key
	inputs[inputs_count].type 			= INPUT_KEYBOARD;
	inputs[inputs_count].ki.wVk			= key.key_code;
	inputs[inputs_count].ki.wScan		= 0;
	inputs[inputs_count].ki.dwFlags		= KEYEVENTF_KEYUP;
	inputs[inputs_count].ki.time		= 0;
	inputs[inputs_count].ki.dwExtraInfo	= 0;
	inputs_count++;

	// Key up for modifiers
	if (key.modifiers & MOD_WIN)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_LWIN;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= KEYEVENTF_KEYUP;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	if (key.modifiers & MOD_CONTROL)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_CONTROL;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= KEYEVENTF_KEYUP;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	if (key.modifiers & MOD_SHIFT)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_SHIFT;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= KEYEVENTF_KEYUP;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	SendInput(inputs_count, inputs, sizeof(INPUT));
}

void setup_keymap(void)
{
	if (!map_initialized)
	{
		// we could fill this in statically, but eh...
		memset(&map, 0, sizeof(map));
		for (int i = 0; i < REMOTE_BUTTON_MAX; i++)
		{
			map[i].button = i;
			map[i].name = remote_button_name[i];
		}
	}

	// TODO: This is where we're gonna get config from the registry. for now we hardcode

	map[REMOTE_BUTTON_APPLE_MENU].modifiers 		= 0;
	map[REMOTE_BUTTON_APPLE_MENU].key_code 			= VK_LWIN;
	map[REMOTE_BUTTON_APPLE_PLAY_PAUSE].modifiers	= 0;
	map[REMOTE_BUTTON_APPLE_PLAY_PAUSE].key_code	= VK_RETURN;
	map[REMOTE_BUTTON_APPLE_FAST_FWD].modifiers 	= 0;
	map[REMOTE_BUTTON_APPLE_FAST_FWD].key_code		= VK_RIGHT;
	map[REMOTE_BUTTON_APPLE_REWIND].modifiers		= 0;
	map[REMOTE_BUTTON_APPLE_REWIND].key_code		= VK_LEFT;
	map[REMOTE_BUTTON_APPLE_VOLUME_UP].modifiers	= 0;
	map[REMOTE_BUTTON_APPLE_VOLUME_UP].key_code		= VK_UP,
	map[REMOTE_BUTTON_APPLE_VOLUME_DOWN].modifiers	= 0;
	map[REMOTE_BUTTON_APPLE_VOLUME_DOWN].key_code	= VK_DOWN;

	map_initialized = true;
}