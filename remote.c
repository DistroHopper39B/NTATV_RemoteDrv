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

int main(int argc, char *argv[])
{
    int status 							= success;
    libusb_device_handle *remote_handle = NULL;
	int opt;
	
	apple_ir_command	ir_command;
	apple_ir_command	flush_cmd;
		
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
				exit(0);
			case 'v':
			{
				version();
				exit(0);
			}
			case 'm':
				// since '0' is a valid value, putting in non-integers will result in the LED mode
				// being successfully set to 0. There's no pretty way to fix this, maybe I will in the 
				// future
				led_mode = strtol(optarg, NULL, 0);
				if (led_mode < 0 || led_mode > LEDMODE_BOTH)
				{
					error("Invalid LED mode!\n");
					exit(1);
				}
				
				dprintf("LED mode will be set to %d (%s)\n", led_mode, led_modes_str[led_mode]);
				
				set_led(remote_handle, led_mode);
				printf("Successfully set LED mode to %d (%s)\n", led_mode, led_modes_str[led_mode]);
				
				exit(0);
			case 'b':
			{				
				// since '0' is a valid value, putting in non-integers will result in the LED brightness
				// being successfully set to 0. There's no pretty way to fix this, maybe I will in the 
				// future. 
				led_brightness = strtol(optarg, NULL, 0);
				dprintf("LED brightness will be set to %d\n", led_brightness);
				if (led_brightness != LED_BRIGHTNESS_HI
					&& led_brightness != LED_BRIGHTNESS_LO)
				{
					error("Brightness must be 0 or 1!\n");
					exit(1);
				}
				
				set_led_brightness(remote_handle, led_brightness);
				error("Successfully set LED brightness to %d\n", led_brightness);
				exit(0);
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
	
	// flush libusb cache
	int flush_len = sizeof(flush_cmd);
							
	do
	{
		dprintf("flushing, ");
		status = libusb_bulk_transfer(remote_handle,
							APPLE_REMOTE_ENDPOINT,
							(uint8_t *) &flush_cmd,
							sizeof(flush_cmd),
							&flush_len,
							10);
		dprintf("status = %d\n", status);
	} while (status == LIBUSB_SUCCESS);
	
	// do one more flush, this seems to fix things all of the time.
	status = libusb_bulk_transfer(remote_handle,
							APPLE_REMOTE_ENDPOINT,
							(uint8_t *) &flush_cmd,
							sizeof(flush_cmd),
							&flush_len,
							10);
	
							
	error("\nEntering remote test mode...\n");
	error("Press a button on your Apple remote to see the status or press Control-C to quit.\n");
	
	while (1)
	{
		int length = sizeof(ir_command);
		status = libusb_bulk_transfer(remote_handle,
									APPLE_REMOTE_ENDPOINT,
									(uint8_t *) &ir_command,
									sizeof(ir_command),
									&length,
									0);
		
		if (status == LIBUSB_SUCCESS)
		{
			process_signal(&ir_command, length);
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