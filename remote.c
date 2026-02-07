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

#include <windows.h>


#include "remote.h"

#ifdef __linux__
#printf "This program is designed for Windows. Please use atvclient on Linux."
#endif


#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 1

// defaults on startup

static const char *led_modes_str[LEDMODE_MAX] =
{
	"Off",
	"Amber",
	"Amber (blinking) (default)",
	"White",
	"White (blinking)",
	"Both blinking",
};

static const struct option long_options[] = 
{
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'v'},
	{"debug", no_argument, NULL, 'd'},
	{"led-mode", required_argument, NULL, 'm'},
	{"led-brightness", required_argument, NULL, 'b'}
};

static void version(void)
{
	printf("Apple TV Remote Driver for Windows NT version %d.%d.%d\n",
		VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	printf("This program is part of NTATV (https://github.com/DistroHopper39B/NTATV)\n");
}

static void help(void)
{
	version();
	printf("\n");
	
	printf("The following options are available:\n");
	printf("-h, --help \t\tShow this help screen.\n");
	printf("-v, --version \t\tShow version number.\n");
	printf("-d, --debug \t\tEnable verbose output.\n");
	printf("-m, --led-mode \t\tChange the front LED's mode of operation.\n");
	printf("Supported LED modes:\n");
	for (int i = 0; i < LEDMODE_MAX; i++)
	{
		printf("    %i: %s\n", i, led_modes_str[i]);
	}
	printf("-b, --led-brightness \tChange the brightness of the front LED.\n");
	printf("Supported LED brightnesses:\n");
	printf("    0: Dim\n");
	printf("    1: Bright (default)\n");
}

int handle_led_mode(appleir_device_handle device, char *arg)
{
	if (strlen(arg) != 1)
	{
		return EINVAL;
	}

	if (!isdigit(arg[0]))
	{
		return EINVAL;
	}

	int led_mode = strtol(arg, NULL, 0);
	if (led_mode < LEDMODE_OFF || led_mode > LEDMODE_BOTH)
	{
		return EINVAL;
	}
	appleir_set_led(device, led_mode);
	return 0;
}

int handle_led_brightness(appleir_device_handle device, char *arg)
{
	if (strlen(arg) != 1)
	{
		return EINVAL;
	}

	if (!isdigit(arg[0]))
	{
		return EINVAL;
	}

	int led_brightness = strtol(arg, NULL, 0);
	if (led_brightness != LED_BRIGHTNESS_HI
		&& led_brightness != LED_BRIGHTNESS_LO)
	{
		return EINVAL;
	}
	appleir_set_led_brightness(device, led_brightness);
	return 0;
}

int main(int argc, char *argv[])
{
	int status 						= success;
	appleir_device_handle device	= NULL;
	volatile key_map *map			= NULL;
	int opt;

	device = appleir_open();
	if (!device)
	{
		printf("Cannot find remote!\n");
		status = no_remote;
		goto done;
	}

	while ((opt = getopt_long(argc, argv, "hvdm:b:", long_options, NULL)) != -1)
	{
		switch (opt)
		{
			// we do this one first to ensure debug output ASAP
			case 'd':
				appleir_debug(true);
				break;
			case 'h':
				help();
				return 0;
			case 'v':
			{
				version();
				return 0;
			}
			case 'm':
				status = handle_led_mode(device, optarg);
				if (status != 0)
				{
					printf("Failed to set LED mode: Errno %d (%s)\n", status, strerror(status));
				}
				return status;

			case 'b':
			{				
				status = handle_led_brightness(device, optarg);
				{
					if (status != 0)
					{
						printf("Failed to set LED brightness: Errno %d (%s)\n", status, strerror(status));
					}
					return status;
				}
			}
			default:
				help();
				exit(1);
		}
	}
	
	version();

	map = appleir_get_keymap();

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

	printf("\nEntering remote test mode...\n");
	printf("Press a button on your Apple remote to see the status or press Control-C to quit.\n");

	HANDLE thread = CreateThread(NULL, 0, appleir_remote_loop, device, 0, NULL);
	WaitForSingleObject(thread, INFINITE);

done:
	appleir_close(device);
	
	return status;
}