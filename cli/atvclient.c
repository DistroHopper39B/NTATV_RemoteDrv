/*
 * Apple TV Remote and LED Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

/**
 * When 'atvclient' is run with no arguments, it will display the remote buttoms.
 *
 * Command line arguments:
 * -m, --led-mode			Change the LED mode. Valid modes:
 *								0: Off
 *								1: Amber
 *								2: Amber (blinking)
 *								3: White (note: many Apple TVs do not have a white LED)
 *								4: White (blinking)
 *								5: Both blinking
 *
 * -b, --led-brightness		Change LED brightness (0 = dim, 1 = bright)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <getopt.h>
#include <windows.h>

#include <appleirapi.h>

#ifdef __linux__
#error "This program is designed for Windows. Please use atvclient on Linux."
#endif

static
void dumphex(uint8_t *buf, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%02x ", buf[i]);
    }

    printf("\n");
}

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
	printf(VERSION_STRING "\n");
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

int msgbox_error(const char *message, const char *title)
{
	return MessageBox(NULL, message, title, MB_ICONERROR | MB_OK);
}

int message_boxf(HWND hwnd, uint32_t type, const char *title, char *message, ...)
{
	va_list list;

	va_start(list, message);
	vsnprintf(message_buf, 200, message, list);
	va_end(list);

	return MessageBox(hwnd, message_buf, title, type);
}

static
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

static
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

static
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

int main(int argc, char *argv[])
{
	appleir_device_handle device	= NULL;
	int opt;
    ir_command command;

	opt = getopt_long(argc, argv, "dhvm:b:", long_options, NULL);
	switch (opt)
	{
		case 'd':
			appleir_debug(true);
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

	if (atvclient_open_device(&device))
		return -1;

    while (1)
    {
        if (appleir_get_raw_data(device, &command))
        {
            dumphex((uint8_t *) &command, 5);
        }
    }

	return success;
}
