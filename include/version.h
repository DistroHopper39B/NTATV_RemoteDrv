/*
 * Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

#ifdef APPLEIR_DLL
#define PROGRAM_NAME "Apple IR"
#define PROGRAM_TYPE "Library"
#else
#define PROGRAM_NAME "Apple TV Client"
#define PROGRAM_TYPE "Application"
#endif
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define VERSION_MAJOR 0
#define VERSION_MINOR 3
#define VERSION_PATCH 0

#if VERSION_PATCH != 0
#define VERSION_STRING PROGRAM_NAME PROGRAM_TYPE " version " STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_PATCH)
#define VERSION_NUM_STR STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_PATCH)
#else
#define VERSION_STRING PROGRAM_NAME " " PROGRAM_TYPE " version " STR(VERSION_MAJOR) "." STR(VERSION_MINOR)
#define VERSION_NUM_STR STR(VERSION_MAJOR) "." STR(VERSION_MINOR)
#endif

#define AUTHOR "Sylas Hollander"
#define AUTHOR_ORG "Distro's Projects"
#define COPYRIGHT_YEAR 2026

#define COPYRIGHT_STRING "Copyright (C) " STR(COPYRIGHT_YEAR) " " AUTHOR ". All rights reserved."
