/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */
 
#define VENDOR_APPLE 0x05ac
#define PRODUCT_APPLETV_REMOTE 0x8241

#define IS_APPLE_REMOTE(dev) ((dev->descriptor.idVendor == VENDOR_APPLE) && \
                             ((dev->descriptor.idProduct == PRODUCT_APPLETV_REMOTE)))


typedef enum
{
    LEDMODE_OFF,
    LEDMODE_AMBER,
    LEDMODE_AMBER_BLINK,
    LEDMODE_WHITE,
    LEDMODE_WHITE_BLINK,
    LEDMODE_BOTH,
    LEDMODE_MAX
} led_modes;

const char *led_modes_str[LEDMODE_MAX] =
{
    "Off",
    "Amber",
    "Amber (blinking) (default)",
    "White",
    "White (blinking)",
    "Both blinking",
};

typedef enum
{
    LED_BRIGHTNESS_LO,
    LED_BRIGHTNESS_HI,
    LED_BRIGHTNESS_MAX
} led_brightnesses;

struct ir_command {
    unsigned char flags;
    unsigned char unused;
    unsigned char event;
    unsigned char address;
    unsigned char eventId;
};

#define BUTTON_TIMEOUT 150
#define HOLD_TIMEOUT   500


#define EVENT_UP 1
#define EVENT_DOWN 2
#define EVENT_LEFT 3
#define EVENT_RIGHT 4
#define EVENT_PLAY 5
#define EVENT_MENU 6
#define EVENT_HOLD_PLAY 7
#define EVENT_HOLD_MENU 8

#define EVENT_RELEASE 0x80