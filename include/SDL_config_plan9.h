/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifndef _SDL_config_plan9_h
#define _SDL_config_plan9_h
#include <stdarg.h>

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;

#undef SDL_LOADSO_DISABLED
#define SDL_THREADS_DISABLED	1

#define SDL_AUDIO_DRIVER_DUMMY	1
#define SDL_AUDIO_DRIVER_PLAN9	1
#define SDL_CDROM_DUMMY			1
#define SDL_VIDEO_DRIVER_DUMMY	1
#define SDL_VIDEO_DRIVER_PLAN9	1
#define SDL_JOYSTICK_DUMMY		1
//#define SDL_TIMER_DUMMY			1
#define SDL_TIMER_PLAN9			1



#define LACKS_SYS_MMAN_H	1


#define C_ALLOCA 1
#define HAVE_ABS 1
#define HAVE_ATOF 1
#define HAVE_ATOI 1
#define HAVE_BCOPY 1
#define HAVE_CALLOC 1
#define HAVE_CTYPE_H 1
#define HAVE_FREE 1
#define HAVE_GETENV 1
#define HAVE_INTTYPES_H 1
#define HAVE_ITOA 1
#define HAVE_LIBC 1
#define HAVE_MALLOC 1
#define HAVE_MATH_H 1
#define HAVE_MEMCMP 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_MEMSET 1
#define HAVE_MMAP 0
#define HAVE_PUTENV 1
#define HAVE_QSORT 1
#define HAVE_REALLOC 1
#define HAVE_SETJMP 1
#define HAVE_SIGACTION 1
#define HAVE_SIGNAL_H 1
#define HAVE_SNPRINTF 1
#define HAVE_SSCANF 1
#define HAVE_STDARG_H 1
#define HAVE_STDDEF_H 1
#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRCASECMP 1
#define HAVE_STRCHR 1
#define HAVE_STRCMP 1
#define HAVE_STRDUP 1
#define HAVE_STRING_H 1
#define HAVE_STRING_H 1
#define HAVE_STRLEN 1
#define HAVE_STRNCASECMP 1
#define HAVE_STRNCMP 1
#define HAVE_STRRCHR 1
#define HAVE_STRSTR 1
#define HAVE_STRTOD 1
#define HAVE_STRTOL 1
#define HAVE_STRTOLL 1
#define HAVE_STRTOUL 1
#define HAVE_STRTOULL 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1
#define HAVE_VSNPRINTF 1
#define HAVE__ULTOA 1
#define PACKAGE_BUGREPORT ""
#define PACKAGE_NAME ""
#define PACKAGE_STRING ""
#define PACKAGE_TARNAME ""
#define PACKAGE_VERSION ""
#define SDL_BYTEORDER 1234
#define SDL_HAS_64BIT_TYPE 1
#define STACK_DIRECTION -1

#endif /* _SDL_config_plan9_h */
