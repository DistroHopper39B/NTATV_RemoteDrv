/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "remote.h"

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
void handle_button_apple(uint8_t button)
{
	switch (button)
	{
		case APPLE_REMOTE_MENU1:
		case APPLE_REMOTE_MENU2:
            press_key(REMOTE_BUTTON_APPLE_MENU);
			break;
		case APPLE_REMOTE_PLAY1:
		case APPLE_REMOTE_PLAY2:
            press_key(REMOTE_BUTTON_APPLE_PLAY_PAUSE);
			break;
		case APPLE_REMOTE_RIGHT1:
		case APPLE_REMOTE_RIGHT2:
			press_key(REMOTE_BUTTON_APPLE_FAST_FWD);
			break;
		case APPLE_REMOTE_LEFT1:
		case APPLE_REMOTE_LEFT2:
            press_key(REMOTE_BUTTON_APPLE_REWIND);
			break;
		case APPLE_REMOTE_UP1:
		case APPLE_REMOTE_UP2:
            press_key(REMOTE_BUTTON_APPLE_VOLUME_UP);
			break;
		case APPLE_REMOTE_DOWN1:
		case APPLE_REMOTE_DOWN2:
            press_key(REMOTE_BUTTON_APPLE_VOLUME_DOWN);
			break;
		default:
			printf("Unknown button %x\n", button);
			break;
	}
}

static
void handle_pairing_apple(uint8_t addr, uint8_t pair_command)
{
	switch (pair_command)
	{
		case APPLE_REMOTE_PAIR1:
		case APPLE_REMOTE_PAIR2:
			error("Remote pairing unimplemented! (Address: %x)\n", addr);
			break;
		case APPLE_REMOTE_UNPAIR1:
		case APPLE_REMOTE_UNPAIR2:
			error("Remote unpairing unimplemented! (Address: %x)\n", addr);
			break;
		default:
			error("Unknown pairing command %x\n", pair_command);
			break;
	}
}

static
void process_signal_apple(void *signal, int len)
{
	ir_command_vendor_apple *ir = signal;

	if (ir->flags == APPLE_REMOTE_REPEAT)
	{
		printf("Repeat: ");
	}

	switch (ir->event)
	{
		case APPLE_REMOTE_BUTTON:
			handle_button_apple(ir->event_id);
			break;
		case APPLE_REMOTE_PAIRING:
			handle_pairing_apple(ir->addr, ir->event_id);
			break;
		default:
			error("Unknown event %x\n", ir->event);
			break;
	}
}

void process_signal(void *signal, int len)
{
	ir_command *ir = signal;

	if (len < 5) return;

	if (debug)
	{
		printf("Raw signal: ");
		dumphex(signal, len);
	}

	if (ir->vendor == REMOTE_VENDOR_APPLE)
	{
		dprintf("Apple remote\n");
		process_signal_apple(signal, len);
	}
	else if (ir->vendor == REMOTE_VENDOR_6B)
	{
		dprintf("Generic remote (vendor = 6b)\n");
		process_signal_ven6b(signal, len);
	}
	else if (ir->vendor == REMOTE_VENDOR_FE)
	{
		dprintf("Generic remote (vendor = fe)\n");
		process_signal_venfe(signal, len);
	}
	else
	{
		if (ir->vendor == 0)
		{
			error("Vender is 0. Remote either unsupported, low battery, or too far away from device.\n");
		}
		else
		{
			error("Unsupported remote vendor %x\n", ir->vendor);
		}
		printf("Hex output: ");
		dumphex(signal, len);
	}
}