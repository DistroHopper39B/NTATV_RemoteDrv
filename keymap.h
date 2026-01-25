/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

/* Specify every conceivable button that could be on a remote. */

typedef struct {
	uint32_t	button; // Intermediate button format
	uint8_t     modifiers; //
} key_map;