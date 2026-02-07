/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

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

typedef enum
{
	LED_BRIGHTNESS_LO,
	LED_BRIGHTNESS_HI,
	LED_BRIGHTNESS_MAX
} led_brightnesses;