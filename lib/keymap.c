/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "appleir.h"

bool map_initialized = false;
volatile key_map map[REMOTE_BUTTON_MAX];
key_map pressed_key;

static const char *remote_button_name[REMOTE_BUTTON_MAX] =
{
		"Unpaired Button",

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

bool press_key(remote_button button)
{
	int inputs_count = 0;
	INPUT inputs[5];

	if (!map_initialized)
	{
		error("Keymap not initialized\n");
		return FALSE;
	}

	if (button >= REMOTE_BUTTON_MAX)
	{
		error("Button %d too high\n", button);
		return FALSE;
	}

	pressed_key = map[button];

	printf("Executing action: %s\n", pressed_key.name);

	if (pressed_key.key_code == 0)
	{
		error("Button %d does not correspond with key code\n", button);
		return FALSE;
	}

	// Key down for modifiers
	if (pressed_key.modifiers & MOD_WIN)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_LWIN;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= 0;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	if (pressed_key.modifiers & MOD_CONTROL)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_CONTROL;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= 0;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	if (pressed_key.modifiers & MOD_SHIFT)
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
	inputs[inputs_count].ki.wVk			= pressed_key.key_code;
	inputs[inputs_count].ki.wScan		= 0;
	inputs[inputs_count].ki.dwFlags		= 0;
	inputs[inputs_count].ki.time		= 0;
	inputs[inputs_count].ki.dwExtraInfo	= 0;
	inputs_count++;

	SendInput(inputs_count, inputs, sizeof(INPUT));
	return TRUE;
}

bool release_key(void)
{
	int inputs_count = 0;
	INPUT inputs[5];

	// Key up for main pressed_key
	inputs[inputs_count].type 			= INPUT_KEYBOARD;
	inputs[inputs_count].ki.wVk			= pressed_key.key_code;
	inputs[inputs_count].ki.wScan		= 0;
	inputs[inputs_count].ki.dwFlags		= KEYEVENTF_KEYUP;
	inputs[inputs_count].ki.time		= 0;
	inputs[inputs_count].ki.dwExtraInfo	= 0;
	inputs_count++;

	// Key up for modifiers
	if (pressed_key.modifiers & MOD_WIN)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_LWIN;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= KEYEVENTF_KEYUP;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	if (pressed_key.modifiers & MOD_CONTROL)
	{
		inputs[inputs_count].type 			= INPUT_KEYBOARD;
		inputs[inputs_count].ki.wVk			= VK_CONTROL;
		inputs[inputs_count].ki.wScan		= 0;
		inputs[inputs_count].ki.dwFlags		= KEYEVENTF_KEYUP;
		inputs[inputs_count].ki.time		= 0;
		inputs[inputs_count].ki.dwExtraInfo	= 0;
		inputs_count++;
	}

	if (pressed_key.modifiers & MOD_SHIFT)
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
	return TRUE;
}

__declspec(dllexport) volatile key_map *appleir_get_keymap(void)
{
	if (!map_initialized)
	{
		// we could fill this in statically, but eh...
		memset((void *) &map, 0, sizeof(map));
		for (int i = 0; i < REMOTE_BUTTON_MAX; i++)
		{
			map[i].button = i;
			map[i].name = remote_button_name[i];
		}
	}

	map_initialized = true;

	return map;
}