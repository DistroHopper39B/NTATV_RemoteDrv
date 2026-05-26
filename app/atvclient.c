/*
 * Apple TV Remote and LED Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "atvclient.h"

#ifdef __linux__
#error "This program is designed for Windows. Please use atvclient on Linux."
#endif

bool client_debug = false;

const char program_name[] = "Apple TV Client";
char message_buf[200] = {0};

static const struct option long_options[] =
{
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"led-mode", required_argument, NULL, 'm'},
		{"led-brightness", required_argument, NULL, 'b'}
};

static
void version(void)
{
	puts(VERSION_STRING);
    puts(COPYRIGHT_STRING);
}

static
void usage(char *name)
{
	printf("Usage: %s [OPTION]\n", name);
	printf("When this program is run with no arguments, it will display the buttons pressed on the remote.\n");
	printf("\n");

	printf("Command line arguments:\n");
	printf("  -m, --led-mode\tChange the LED mode. Valid modes:\n");
	printf("\t\t\t  0: Off\n");
	printf("\t\t\t  1: Amber\n");
	printf("\t\t\t  2: Amber (blinking) (default)\n");
	printf("\t\t\t  3: White (many TVs do not have white LEDs)\n");
	printf("\t\t\t  4: White (blinking)\n");
	printf("\t\t\t  5: Both blinking\n");
	printf("  -b, --led-brightness\tToggle between bright (1) and dim (0) brightness\n");
	printf("  -d, --debug\t\tEnable debug output\n");
	printf("  -h, --help\t\tShow this help screen and exit\n");
	printf("  -v, --version\t\tShow the program version and exit\n");
}

int message_boxf(HWND hwnd, uint32_t type, const char *title, char *message, ...)
{
	va_list list;

	va_start(list, message);
	vsnprintf(message_buf, 200, message, list);
	va_end(list);

	return MessageBox(hwnd, message_buf, title, type);
}

int atvclient_open_device(appleir_device_handle *device)
{
	int status = success;

	status = appleir_open(device,
						  VERSION_MAJOR,
						  VERSION_MINOR,
						  VERSION_PATCH);
	if (status == version_mismatch)
	{
		if (message_boxf(NULL,
						 MB_ICONWARNING | MB_YESNO,
						 program_name,
						 "DLL version does not match EXE version.\n"
						 "This may result in undefined behavior. Run anyway?") == IDYES)
		{
			status = appleir_open(device,
								  VERSION_MAJOR,
								  VERSION_MINOR,
								  VERSION_PATCH);
		}
		else
		{
			return status;
		}
	}

	if (status)
	{
		message_boxf(NULL,
					 MB_ICONERROR | MB_OK,
					 program_name,
					 "Cannot open remote: Error %d (%s)",
					 status, appleir_strerror(status));
		return status;
	}

	return success;
}

int main(int argc, char *argv[])
{
	appleir_device_handle 	device = NULL;
	int 					opt;
	volatile key_map        *map;

	while ((opt = getopt_long(argc, argv, "dhvm:b:", long_options, NULL)) != -1)
	{
		switch (opt)
		{
			case 'd':
				appleir_debug(true);
				client_debug = true;
				break;

			case 'h':
				version();
				usage(argv[0]);
				return success;

			case 'v':
				version();
				return success;

			case 'm':
				if (!atvclient_set_led_mode(optarg))
				{
					message_boxf(NULL,
								 MB_ICONERROR | MB_OK,
								 program_name,
								 "Invalid LED mode!");
					return -1;
				}

				return success;

			case 'b':
				if (!atvclient_set_led_brightness(optarg))
				{
					message_boxf(NULL,
								 MB_ICONERROR | MB_OK,
								 program_name,
								 "LED brightness setting must be 0 or 1!");
					return -1;
				}

				return success;

			case '?':
				usage(argv[0]);
				return -1;

			default:
				break;
		}
	}

	if (atvclient_open_device(&device))
		return -1;

	map = atvclient_init_keymap();

    /* TODO: This is where we're gonna get config from the registry. For now we hardcode */
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

    version();
    printf("\nEntering remote test mode...\n");
    printf("Press a button on your Apple remote to see the status or press Control-C to quit.\n");

    HANDLE thread = CreateThread(NULL, 0, atvclient_remote_loop, device, 0, NULL);
    while (thread)
	{
		sleep(5);
	}

	return success;
}
