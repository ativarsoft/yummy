/*
 *  yummy/include/debug.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef DEBUG_H
#define DEBUG_H

#if defined DEBUG || 1

#include <stdio.h>
#include <cheese.h>
#include <config.h>
#include <guid.h>

/* if this program is being tested on a virtual or remote machine, then 
 * a descriptor that redirects to the host will be very useful. */
#ifndef DEBUG_OUT_DESCRIPTOR
#define DEBUG_OUT_DESCRIPTOR stdout
#endif

#define DEBUG_PRINTF(a, ...) \
	fprintf(DEBUG_OUT_DESCRIPTOR, a, __VA_ARGS__); \
	fflush(DEBUG_OUT_DESCRIPTOR);
/* FIXME: windows quirk? */
#define DEBUG_PUTS(a) \
	puts(a); \
	fflush(DEBUG_OUT_DESCRIPTOR);
/*#define DEBUG_PUTS(a) \
	fputs(a, DEBUG_OUT_DESCRIPTOR); \
	fflush(DEBUG_OUT_DESCRIPTOR);*/
#define DEBUG_TRACE() DEBUG_PRINTF("%s()\n", __FUNCTION__);
#define DEBUG_CHECK_NUM_ARGS(a, b) \
	if(a!=b) \
		DEBUG_PRINTF("WARNING: num args is %d but should be %d.\n", a, b);
#define DEBUG_UNKNOWN_MESSAGE() \
	DEBUG_PRINTF("WARNING: unknown message %d in %s\n", a, __FUNCTION__);
#define DEBUG_PRINT_GUID(a) debug_print_guid(a);
#define DEBUG_PRINT_GUID_C(a) debug_print_guid_c(a);

/* TODO: check if this outputs the correct guid. */
static inline void debug_print_guid(struct guid_s *guid)
{
	printf("{%x-%hx-%hx-%hhx%hhx-%hhx%hhx%hhx%hhx%hhx%hhx}\n", 
		guid->a, 
		guid->b, 
		guid->c, 
		guid->d[0], 
		guid->d[1], 
		guid->d[2], 
		guid->d[3], 
		guid->d[4], 
		guid->d[5], 
		guid->d[6], 
		guid->d[7]);
}

static inline void debug_print_guid_c(struct guid_s *guid)
{
	printf("{%#.8x, %#.4hx, %#.4hx, {%#.2hhx, %#.2hhx, %#.2hhx, %#.2hhx, %#.2hhx, %#.2hhx, %#.2hhx, %#.2hhx}}\n", 
		guid->a, 
		guid->b, 
		guid->c, 
		guid->d[0], 
		guid->d[1], 
		guid->d[2], 
		guid->d[3], 
		guid->d[4], 
		guid->d[5], 
		guid->d[6], 
		guid->d[7]);
}

#else

/* TODO: fill this. */
#define DEBUG_PRINTF(fmt, ...)
#define DEBUG_PUTS(a)
#define DEBUG_TRACE()
#define DEBUG_CHECK_NUM_ARGS(a, b)
#define DEBUG_PRINT_GUID(a)

#endif

#endif
