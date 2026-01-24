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
        dprintf("%02x ", buf[i]);
    }
    
    printf("\n");
}

static
void handle_button_ven6b(uint8_t button)
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
    }
}

static
void process_signal_ven6b(ir_command_vendor_6b *ir, int len)
{
    handle_button_ven6b(ir->button);
}

static
void handle_button_apple(uint8_t button)
{
    switch (button)
	{
		case APPLE_REMOTE_TIMEOUT:
			printf("Timeout\n");
			break;
		case APPLE_REMOTE_RESVD:
			printf("Reserved\n");
			break;
		case APPLE_REMOTE_MENU1:
		case APPLE_REMOTE_MENU2:
			printf("Menu\n");
			break;
		case APPLE_REMOTE_PLAY1:
		case APPLE_REMOTE_PLAY2:
			printf("Play/Pause\n");
			break;
		case APPLE_REMOTE_RIGHT1:
		case APPLE_REMOTE_RIGHT2:
			printf("Fast Forward\n");
			break;
		case APPLE_REMOTE_LEFT1:
		case APPLE_REMOTE_LEFT2:
			printf("Rewind\n");
			break;
		case APPLE_REMOTE_UP1:
		case APPLE_REMOTE_UP2:
			printf("Volume Up\n");
			break;
		case APPLE_REMOTE_DOWN1:
		case APPLE_REMOTE_DOWN2:
			printf("Volume Down\n");
			break;
		default:
			printf("Unknown button %x\n", button);
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
void process_signal_apple(ir_command_vendor_apple *ir, int len)
{
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

void process_signal(ir_command *ir, int len)
{
    if (len < 5) return;
    
    if (debug)
    {
        printf("Raw signal: ");
        dumphex((uint8_t *) ir, len);
    }
    
    if (ir->vendor == REMOTE_VENDOR_APPLE)
    {
        dprintf("Apple remote\n");
        process_signal_apple((ir_command_vendor_apple *) ir, len);
    }
    else if (ir->vendor == REMOTE_VENDOR_6B)
    {
        dprintf("Generic remote (vendor = 6b)\n");
        process_signal_ven6b((ir_command_vendor_6b *) ir, len);
    }
    else
    {
        error("Unsupported remote vendor %x\n", ir->vendor);
    }
}