/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "appleir.h"

bool cache_flushed = false;
static
void dumphex(uint8_t *buf, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%02x ", buf[i]);
	}

	printf("\n");
}

static
void handle_button_venfe(uint16_t button)
{
	switch (button)
	{
		case VENFE_BUTTON_UP:
			printf("Up\n");
			break;
		case VENFE_BUTTON_DOWN:
			printf("Down\n");
			break;
		case VENFE_BUTTON_LEFT:
			printf("Left\n");
			break;
		case VENFE_BUTTON_RIGHT:
			printf("Right\n");
			break;
		case VENFE_BUTTON_SELECT:
			printf("Select\n");
			break;
		case VENFE_BUTTON_BACK:
			printf("Back\n");
			break;
		case VENFE_BUTTON_NUM1:
			printf("1\n");
			break;
		case VENFE_BUTTON_NUM2:
			printf("2\n");
			break;
		case VENFE_BUTTON_NUM3:
			printf("3\n");
			break;
		case VENFE_BUTTON_NUM4:
			printf("4\n");
			break;
		case VENFE_BUTTON_NUM5:
			printf("5\n");
			break;
		case VENFE_BUTTON_NUM6:
			printf("6\n");
			break;
		case VENFE_BUTTON_NUM7:
			printf("7\n");
			break;
		case VENFE_BUTTON_NUM8:
			printf("8\n");
			break;
		case VENFE_BUTTON_NUM9:
			printf("9\n");
			break;
		case VENFE_BUTTON_NUM0:
			printf("0\n");
			break;
		case VENFE_BUTTON_CHANNEL_UP:
			printf("Channel Up\n");
			break;
		case VENFE_BUTTON_CHANNEL_DN:
			printf("Channel Down\n");
			break;
		case VENFE_BUTTON_VOLUME_UP:
			printf("Volume Up\n");
			break;
		case VENFE_BUTTON_MUTE:
			printf("Volume Down\n");
			break;
		case VENFE_BUTTON_PLAY:
			printf("Play/Pause\n");
			break;
		case VENFE_BUTTON_STOP:
			printf("Stop\n");
			break;
		case VENFE_BUTTON_SKIP:
			printf("Skip\n");
			break;
		case VENFE_BUTTON_REPLAY:
			printf("Replay\n");
			break;
		case VENFE_BUTTON_FASTFWD:
			printf("Fast Forward\n");
			break;
		case VENFE_BUTTON_REWIND:
			printf("Rewind\n");
			break;
		case VENFE_BUTTON_POWER:
			printf("Power\n");
			break;
		case VENFE_BUTTON_HOME:
			printf("Home\n");
			break;
		case VENFE_BUTTON_MYTV:
			printf("My TV\n");
			break;
		case VENFE_BUTTON_MUSIC:
			printf("Music\n");
			break;
		case VENFE_BUTTON_VIDEOS:
			printf("Videos\n");
			break;
		case VENFE_BUTTON_PICTURES:
			printf("Pictures\n");
			break;
		case VENFE_BUTTON_INFO:
			printf("Info\n");
			break;
		default:
			error("Unknown button %x\n", button);
			break;
	}
}

static
void process_signal_venfe(void *signal, int len)
{
	ir_command_vendor_fe *ir = signal;

	uint16_t button = (ir->button_1 << 8) | ir->button_2;

	handle_button_venfe(button);
}

static
void handle_button_ven6b(uint16_t button)
{
	switch (button)
	{
		case VEN6B_REMOTE_LEFT:
			printf("Left\n");
			break;
		case VEN6B_REMOTE_RIGHT:
			printf("Right\n");
			break;
		case VEN6B_REMOTE_CENTER:
			printf("Center\n");
			break;
		case VEN6B_REMOTE_POWER:
			printf("Power\n");
			break;
		case VEN6B_REMOTE_DOWN:
			printf("Down\n");
			break;
		case VEN6B_REMOTE_UP:
			printf("Up\n");
			break;
		default:
			printf("Unknown button %x\n", button);
			break;
	}
}

static
void process_signal_ven6b(void *signal, int len)
{
	ir_command_vendor_6b *ir = signal;

	uint16_t button = (ir->button_1 << 8) | ir->button_2;
	handle_button_ven6b(button);
}

static
void flush_cache(appleir_device_handle device)
{
	int status;
	remote_handle = device;
	ir_command dummy;

	if (!device)
		return;

	do
	{
		status = libusb_interrupt_transfer(remote_handle,
										   LIBUSB_ENDPOINT_IN | APPLE_REMOTE_ENDPOINT,
										   (uint8_t *) &dummy,
										   sizeof(dummy),
										   NULL,
										   10);
	} while (status == LIBUSB_SUCCESS);

	cache_flushed = true;
}

static
bool get_raw_data(appleir_device_handle device, ir_command *command)
{
	int status, length;
	remote_handle = device;

	if (!cache_flushed)
		flush_cache(device);

	status = libusb_interrupt_transfer(remote_handle,
										   LIBUSB_ENDPOINT_IN | APPLE_REMOTE_ENDPOINT,
										   (uint8_t *) command,
										   sizeof(*command),
										   &length,
										   100);

	return (status == LIBUSB_SUCCESS ? true : false);
}

static
int appleir_cmdcmp(ir_command *cmd1, ir_command *cmd2)
{
	return memcmp(cmd1, cmd2, sizeof(ir_command));
}

static remote_event get_event_apple(ir_command_vendor_apple *command)
{
	switch (command->event)
	{
		case APPLE_REMOTE_BUTTON:
			return REMOTE_EVENT_BUTTONPRESS;
		case APPLE_REMOTE_PAIRING:
			return REMOTE_EVENT_PAIR;
		default:

			return REMOTE_EVENT_INVALID;
	}
}

static
remote_event get_event(ir_command *command)
{
	switch (command->vendor)
	{
		case REMOTE_VENDOR_APPLE:
			return get_event_apple((ir_command_vendor_apple *) command);
		case REMOTE_VENDOR_6B:
		case REMOTE_VENDOR_FE:
			return REMOTE_EVENT_BUTTONPRESS;
		default:
			return REMOTE_EVENT_INVALID;
	}
}

static
remote_button get_button_apple(ir_command_vendor_apple *command)
{
	switch (command->event_id)
	{
		case APPLE_REMOTE_MENU1:
		case APPLE_REMOTE_MENU2:
			return REMOTE_BUTTON_APPLE_MENU;
		case APPLE_REMOTE_PLAY1:
		case APPLE_REMOTE_PLAY2:
			return REMOTE_BUTTON_APPLE_PLAY_PAUSE;
		case APPLE_REMOTE_RIGHT1:
		case APPLE_REMOTE_RIGHT2:
			return REMOTE_BUTTON_APPLE_FAST_FWD;
		case APPLE_REMOTE_LEFT1:
		case APPLE_REMOTE_LEFT2:
			return REMOTE_BUTTON_APPLE_REWIND;
		case APPLE_REMOTE_UP1:
		case APPLE_REMOTE_UP2:
			return REMOTE_BUTTON_APPLE_VOLUME_UP;
		case APPLE_REMOTE_DOWN1:
		case APPLE_REMOTE_DOWN2:
			return REMOTE_BUTTON_APPLE_VOLUME_DOWN;
		default:
			return REMOTE_BUTTON_INVALID;
	}
}

static
remote_button get_button(ir_command *command)
{
	switch (command->vendor)
	{
		case REMOTE_VENDOR_APPLE:
			return get_button_apple((ir_command_vendor_apple *) command);
		case REMOTE_VENDOR_6B:
		case REMOTE_VENDOR_FE:
			return REMOTE_BUTTON_INVALID;
		default:
			return REMOTE_BUTTON_INVALID;
	}
}

__declspec(dllexport) _Noreturn DWORD __stdcall appleir_remote_loop(appleir_device_handle device)
{
	bool key_down = false;
	internal_irctx irctx;

	while (1)
	{
		if (get_raw_data(device, &irctx.command))
		{
			if (debug)
			{
				printf("Raw signal: ");
				dumphex((uint8_t *) &irctx.command, sizeof(irctx.command));
			}
			// Handle very quick button switches
			if (key_down == true)
			{
				if (appleir_cmdcmp(&irctx.command, &irctx.previous_command) != 0)
					release_key();
			}

			irctx.event = get_event(&irctx.command);
			if (irctx.event == REMOTE_EVENT_BUTTONPRESS)
			{
				irctx.button = get_button(&irctx.command);
				press_key(irctx.button);
				key_down = true;
			}
			else if (irctx.event == REMOTE_EVENT_PAIR)
			{
				error("Remote pairing not implemented!\n");
			}
			else
			{
				error("Unknown event from remote!\n");
			}


		}
		else
		{
			if (key_down == true)
				release_key();
			key_down = false;
		}
	}
}