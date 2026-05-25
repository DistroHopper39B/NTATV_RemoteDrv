#pragma once

#ifdef APPLEIR_DLL
#define PROGRAM_TYPE "Library"
#else
#define PROGRAM_TYPE "Application"
#endif
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define VERSION_MAJOR 0
#define VERSION_MINOR 3
#define VERSION_PATCH 0

#if VERSION_PATCH != 0
#define VERSION_STRING "ATVClient " PROGRAM_TYPE " version " STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_PATCH)
#else
#define VERSION_STRING "ATVClient " PROGRAM_TYPE " version " STR(VERSION_MAJOR) "." STR(VERSION_MINOR)
#endif
