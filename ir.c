/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "remote.h"
 
static
void handle_button(uint8_t button)
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
void handle_pairing(uint8_t addr, uint8_t pair_command)
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
void dumphex(uint8_t *buf, int len)
{
    for (int i = 0; i < len; i++)
    {
        dprintf("%02x ", buf[i]);
    }
    
    printf("\n");
}

void process_signal(apple_ir_command *ir, int len)
{
    if (len < 5) return;
    
    if (debug)
    {
        printf("Raw signal: ");
        dumphex((uint8_t *) &ir, len);
    }
    
    if (ir->flags == APPLE_REMOTE_REPEAT)
    {
        printf("Repeat: ");
    }
    
    switch (ir->event)
    {
        case APPLE_REMOTE_BUTTON:
            handle_button(ir->event_id);
            break;
        case APPLE_REMOTE_PAIRING:
            handle_pairing(ir->addr, ir->event_id);
            break;
        default:
            error("Unknown event %x\n", ir->event);
            break;
    }
}