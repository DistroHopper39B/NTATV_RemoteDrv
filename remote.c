/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "remote.h"

#ifdef __linux__
#error "This program is designed for Windows. Please use atvclient on Linux."
#endif


#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 1

bool debug = false;

// defaults on startup
int led_brightness 					= LED_BRIGHTNESS_HI;
int led_mode						= LEDMODE_AMBER_BLINK;

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
	{"led-test", no_argument, NULL, 't'},
	{"led-brightness", required_argument, NULL, 'b'}
};

static void version(void)
{
	error("Apple TV Remote Driver for Windows NT version %d.%d.%d\n",
		VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	error("This program is part of NTATV (https://github.com/DistroHopper39B/NTATV)\n");
}

static void help(void)
{
	version();
	error("\n");
	
	error("The following options are available:\n");
	error("-h, --help \t\tShow this help screen.\n");
	error("-v, --version \t\tShow version number.\n");
	error("-d, --debug \t\tEnable verbose output.\n");
	error("-m, --led-mode \t\tChange the front LED's mode of operation.\n");
	error("Supported LED modes:\n");
	for (int i = 0; i < LEDMODE_MAX; i++)
	{
		error("    %i: %s\n", i, led_modes_str[i]);
	}
	error("-t, --led-test \t\tTest all LED modes.\n");
	error("-b, --led-brightness \tChange the brightness of the front LED.\n");
	error("Supported LED brightnesses:\n");
	error("    0: Dim\n");
	error("    1: Bright (default)\n");
}

int handle_led_mode(libusb_device_handle *remote, char *arg)
{
	if (strlen(arg) != 1)
	{
		return EINVAL;
	}

	if (!isdigit(arg[0]))
	{
		return EINVAL;
	}

	led_mode = strtol(arg, NULL, 0);
	if (led_mode < LEDMODE_OFF || led_mode > LEDMODE_BOTH)
	{
		return EINVAL;
	}
	set_led(remote, led_mode);
	return 0;
}

int handle_led_brightness(libusb_device_handle *remote, char *arg)
{
	if (strlen(arg) != 1)
	{
		return EINVAL;
	}

	if (!isdigit(arg[0]))
	{
		return EINVAL;
	}

	led_brightness = strtol(arg, NULL, 0);
	if (led_brightness != LED_BRIGHTNESS_HI
		&& led_brightness != LED_BRIGHTNESS_LO)
	{
		return EINVAL;
	}
	set_led_brightness(remote, led_brightness);
	return 0;
}

int main(int argc, char *argv[])
{
    int status 							= success;
    libusb_device_handle *remote_handle = NULL;
	int opt;
	
	ir_command	gen_ir_command;
	ir_command	flush_cmd;
		
	status = libusb_init(NULL);
    if (status < 0)
    {
        error("FATAL ERROR: libusb failed to start! (%d)\n", status);
        return status;
    }
    
    // Check to see if the device exists
    remote_handle = libusb_open_device_with_vid_pid(NULL,
        VENDOR_APPLE,
        PRODUCT_APPLETV_REMOTE);
    if (!remote_handle)
    {
        error("No remote found!\n");
        status = no_remote;
        goto done;
    }
    	
	status = libusb_claim_interface(remote_handle, 0);
    if (status)
    {
        error("Cannot claim interface 0: Errno %d (%s)", status, libusb_strerror(status));
        goto done;
    }
	
	status = libusb_claim_interface(remote_handle, 1);
    if (status)
    {
        error("Cannot claim interface 1: Errno %d (%s)", status, libusb_strerror(status));
        goto done;
    }
	
	while ((opt = getopt_long(argc, argv, "hvdm:tb:", long_options, NULL)) != -1)
	{
		switch (opt)
		{
			// we do this one first to ensure debug output ASAP
			case 'd':
				libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
				debug = true;
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
				status = handle_led_mode(remote_handle, optarg);
				if (status != 0)
				{
					error("Failed to set LED mode: Errno %d (%s)\n", status, strerror(status));
				}
				return status;

			case 'b':
			{				
				status = handle_led_brightness(remote_handle, optarg);
				{
					if (status != 0)
					{
						error("Failed to set LED brightness: Errno %d (%s)\n", status, strerror(status));
					}
					return status;
				}
			}
			case 't':
			{
				run_led_test(remote_handle);
				exit(0);
			}
			default:
				help();
				exit(1);
		}
	}
	
	version();
	/**
	 * LibUSB likes to cache the last few button presses since the driver was loaded.
	 * To avoid incorrect button presses, flush these caches.
	 */

	do
	{
		dprintf("flushing, ");
		status = libusb_bulk_transfer(remote_handle,
							APPLE_REMOTE_ENDPOINT,
							(uint8_t *) &flush_cmd,
							sizeof(flush_cmd),
							NULL,
							10);
		dprintf("status = %d\n", status);
	} while (status == LIBUSB_SUCCESS);
	
	// do one more flush, this seems to fix things
	libusb_bulk_transfer(remote_handle,
						 APPLE_REMOTE_ENDPOINT,
						 (uint8_t *) &flush_cmd,
						 sizeof(flush_cmd),
						 NULL,
						 10);
	
							
	error("\nEntering remote test mode...\n");
	error("Press a button on your Apple remote to see the status or press Control-C to quit.\n");
	
	while (1)
	{
		int length;
		status = libusb_bulk_transfer(remote_handle,
									APPLE_REMOTE_ENDPOINT,
									(uint8_t *) &gen_ir_command,
									sizeof(ir_command),
									&length,
									0);
		
		if (status == LIBUSB_SUCCESS)
		{
			process_signal(&gen_ir_command, length);
		}
	}
	    
    done:
    if (remote_handle)
    {
		libusb_release_interface(remote_handle, 0);
        libusb_release_interface(remote_handle, 1);
        libusb_close(remote_handle);
    }
	
    libusb_exit(NULL);
	
    return status;
}