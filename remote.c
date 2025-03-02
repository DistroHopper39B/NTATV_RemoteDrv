/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>

#include "libusb/lusb0_usb.h"
#include "remote.h"

bool debug = false;

#define dprintf(fmt, ...) if (debug) printf(fmt, ##__VA_ARGS__)

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 1

static usb_dev_handle *find_ir(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;

	for (bus = usb_busses; bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next)
			if (dev->descriptor.idVendor == VENDOR_APPLE)
			    if(IS_APPLE_REMOTE(dev))
 				return usb_open(dev);
	}

	return NULL;
}

static usb_dev_handle *get_ir(void)
{
	static usb_dev_handle *ir = NULL;

	if (!ir) {
		usb_init();
		usb_find_busses();
		usb_find_devices();
        
		ir = find_ir();
		if (!ir) {
			fprintf(stderr, "IR receiver not found, quitting\n");
			exit(1);
		}
        
        usb_set_configuration(ir, 1);
        int ret = usb_claim_interface(ir, 1);
		if (ret) {
			fprintf(stderr, "error claiming interface (%d): %s\n", ret, usb_strerror());
			exit(2);
		}
		//usb_reset(ir);
		//
	}

	return ir;
}

static int set_report(unsigned char* data, int len)
{
	unsigned char *type = data;
	int val;

	val = 0x300 | *type;

	return (usb_control_msg(get_ir(), USB_ENDPOINT_OUT | USB_TYPE_CLASS
				| USB_RECIP_INTERFACE, 9, val, 0,
				(char*) data, len, 1000) != len);
}

static void set_led_brightness(int high)
{
	unsigned char buf[5];

	memset(buf, 0, sizeof(buf));
	buf[0] = 0xd;

	if (high) {
		buf[1] = 6;
		set_report(buf, sizeof(buf));
		buf[1] = 5; buf[2] = 1;
		set_report(buf, 3);
	} else {
		buf[1] = 5;
		set_report(buf, sizeof(buf));
		set_report(buf, 3);
	}
}

static void set_led(int mode)
{
	unsigned char buf[5];

	memset(buf, 0, sizeof(buf));
	buf[0] = 0xd; buf[1] = mode;

	switch (mode) {
	case LEDMODE_OFF:
		set_report(buf, sizeof(buf));
		buf[1] = 3;
		set_report(buf, 3);
		buf[1] = 4;
		set_report(buf, 3);
		break;

	case LEDMODE_AMBER:
		set_report(buf, sizeof(buf));
		buf[1] = 3; buf[2] = 1;
		set_report(buf, 3);
		buf[1] = 4; buf[2] = 0;
		set_report(buf, 3);
		break;

	case LEDMODE_AMBER_BLINK:
		set_report(buf, sizeof(buf));
		buf[1] = 3;
		set_report(buf, 3);
		buf[1] = 4;
		set_report(buf, 3);
		buf[1] = 3; buf[2] = 2;
		set_report(buf, 3);
		break;

	case LEDMODE_WHITE:
		set_report(buf, sizeof(buf));
		set_report(buf, 3);
		buf[1] = 4; buf[2] = 1;
		set_report(buf, 3);
		break;

	case LEDMODE_WHITE_BLINK:
		set_report(buf, sizeof(buf));
		buf[1] = 3;
		set_report(buf, 3);
		buf[1] = 4;
		set_report(buf, 3);
		buf[1] = 4; buf[2] = 2;
		set_report(buf, 3);
		break;

	case LEDMODE_BOTH:
		buf[1] = 7;
		set_report(buf, sizeof(buf));
		buf[1] = 6; buf[2] = 1;
		set_report(buf, 3);
		break;
	}
}

void dumphex(unsigned char* buf, int size)
{
	int i;
	for(i=0; i < size; i++) {
	  printf("%02x ", buf[i]);
	}
	printf("\n");
}
  
void print_button(int button) {
	printf("Remote button press detected: ");
	switch (button) {
	  	case EVENT_UP: printf("Up\n"); break;
	  	case EVENT_DOWN: printf("Down\n"); break;
	  	case EVENT_LEFT: printf("Left\n"); break;
	  	case EVENT_RIGHT: printf("Right\n"); break;
	  	case EVENT_MENU: printf("Menu\n"); break;
	  	case EVENT_PLAY: printf("Play/pause\n"); break;
	}
}
  

void handle_button(struct ir_command command) {	
	switch(command.eventId)
	{
	  	case 0x0a: case 0x0b:
			print_button(EVENT_UP);
			break;
	  	case 0x0c: case 0x0d:
			print_button(EVENT_DOWN);
			break;
	  	case 0x09: case 0x08:
			print_button(EVENT_LEFT);
			break;
	  	case 0x06: case 0x07:
			print_button(EVENT_RIGHT);
			break;
		case 0x03: case 0x02:
			print_button(EVENT_MENU);
			break;
		case 0x05: case 0x04:
			print_button(EVENT_PLAY);
			break;
	  	case 0x00:
			printf("Timed out\n");
			break;
	  	default: printf("unknown\n");
	}
}

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
	printf("-t, --led-test \t\tTest all LED modes.\n");
	printf("-b, --led-brightness \tChange the brightness of the front LED.\n");
	printf("Supported LED brightnesses:\n");
	printf("    0: Dim\n");
	printf("    1: Bright (default)\n");
	
}





static void run_led_test(void)
{
	set_led_brightness(LED_BRIGHTNESS_HI);
	dprintf("Running LED test with high brightness.\n");
	
	for (int i = 0; i < LEDMODE_MAX; i++)
    {
        dprintf("Setting LED to mode %d (%s)...\n", i, led_modes_str[i]);
        set_led(i);
        sleep(2);
    }
	
	set_led_brightness(LED_BRIGHTNESS_LO);
	dprintf("Running LED test with low brightness.\n");
	
	for (int i = 0; i < LEDMODE_MAX; i++)
    {
        dprintf("Setting LED to mode %d (%s)...\n", i, led_modes_str[i]);
        set_led(i);
        sleep(2);
    }
	
	dprintf("Going back to defaults.\n");
	set_led(LEDMODE_AMBER_BLINK);
	set_led_brightness(LED_BRIGHTNESS_HI);
}

int main(int argc, char *argv[])
{
	struct ir_command command;
	struct ir_command timeoutCommand;
	
	int keydown = 0;
	
	int opt;
	
	// Default at startup.
	int led_brightness = LED_BRIGHTNESS_HI;
	int led_mode = LEDMODE_AMBER_BLINK;
	
	while ((opt = getopt_long(argc, argv, "hvdm:tb:", long_options, NULL)) != -1)
	{
		// this is bad and will be rewritten later. (top 10 lies)
		switch (opt)
		{
			// we do this one first to ensure debug output ASAP
			case 'd':
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
					printf("Invalid LED mode!\n");
					exit(1);
				}
				
				dprintf("LED mode will be set to %d (%s)\n", led_mode, led_modes_str[led_mode]);
				
				set_led(led_mode);
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
					printf("Brightness must be 0 or 1!\n");
					exit(1);
				}
				
				set_led_brightness(led_brightness);
				printf("Successfully set LED brightness to %d\n", led_brightness);
				exit(0);
			}
			case 't':
			{
				debug = true;
				run_led_test();
				exit(0);
			}
			default:
				help();
				exit(1);
		}
	}
	
	set_led_brightness(LED_BRIGHTNESS_HI);
	set_led(LEDMODE_WHITE);
	
	// Actual remote implementation.
	version();
	printf("\nEntering remote test mode...\n");
	printf("Press a button on your Apple Remote or press Control-C to quit.\n");
	
	memset(&timeoutCommand, 0, sizeof(timeoutCommand));

	
	while (1)
	{
		int result = usb_interrupt_read(get_ir(), 0x82, (char*) &command, sizeof(command), keydown ? BUTTON_TIMEOUT : 0);  
		if(result > 0)
		{
			// we have an IR code!
			if (debug) dumphex((unsigned char*) &command, result);
			
			if (command.flags == 0x26) {
				// set
				command.event = 0xee;
			}
	
			switch(command.event)
			{
				case 0xee:
				case 0xe5: 
				  	handle_button(command);
				  	break;
				case 0xe0:
					// pairing??
				  	set_led(LEDMODE_AMBER);
					printf("You tried to pair your remote, this doesn't do anything\n");
				  	break;
				default:
				  	if(debug) printf("Unknown event %x\n", command.event);
			}
			keydown = 1;
			
		} else if (result == -110)
		{
		  	// timeout, reset led
		  	keydown = 0;                        
		  	set_led(LEDMODE_WHITE);
		  	handle_button(timeoutCommand);
		}
		else
		{
		  	// something else
		  	keydown = 0;
		  	if (debug) printf("Got nuffing: %s\n", usb_strerror());
		}
	}
	
	return 0;
}