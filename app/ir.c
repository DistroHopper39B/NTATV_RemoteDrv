/*
 * Apple TV Remote and LED Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "atvclient.h"

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

static
remote_event get_event_apple(ir_command_vendor_apple *command)
{
    switch (command->event)
    {
        case APPLE_REMOTE_BUTTONPRESS:
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

/* Main remote loop. Can be spawned in a Windows thread. */
DECLSPEC_NORETURN
DWORD __stdcall atvclient_remote_loop(appleir_device_handle device)
{
    bool key_down = false;
    internal_irctx irctx = {0};

    while (1)
    {
        if (appleir_get_raw_data(device, &irctx.command, key_down))
        {
            if (client_debug)
            {
                printf("Raw signal: ");
                dumphex((uint8_t *) &irctx.command, sizeof(irctx.command));
            }

            irctx.event = get_event(&irctx.command);

            switch (irctx.event)
            {
                case REMOTE_EVENT_BUTTONPRESS:
                    irctx.button = get_button(&irctx.command);

                    /* Handle very fast button switching */
                    if (key_down == true && irctx.button != irctx.previous_button)
                    {
                        atvclient_release_key();
                    }

                    atvclient_press_key(irctx.button);
                    key_down = true;
                    irctx.previous_button = irctx.button;
                    break;

                case REMOTE_EVENT_PAIR:
                    fprintf(stderr, "Remote pairing not implemented!\n");
                    if (key_down == true)
                    {
                        atvclient_release_key();
                        key_down = false;
                    }

                    break;
                case REMOTE_EVENT_INVALID:
                default:
                    fprintf(stderr, "Unknown event from remote!\nEvent: ");
                    dumphex((uint8_t *) &irctx.command, sizeof(irctx.command));
                    if (key_down == true)
                    {
                        atvclient_release_key();
                        key_down = false;
                    }

                    break;
            }
        }
        else
        {
            if (key_down)
                atvclient_release_key();

            key_down = false;
        }
    }
}
