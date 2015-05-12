/*
 *  yummy/include/out.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef OUT_H
#define OUT_H

#include <platform.h>

struct out_plugin
{
	/* out */ int version;
	/* out */ char *description;
	/* out */ int id;
	
	/* in */ window_t window;
	/* in */ library_t instance; /* TODO: port. */
	
	/* out */ void (*configure)(window_t parent);
	/* out */ void (*about)(window_t parent);
	
	/* out */ void (*init)();
	/* out */ void (*quit)();
	
	/* out */ int (*open)(int rate, int channels, int bits, int a, int b);
	/* out */ void (*close)();
	
	/* out */ int (*write)(char *buffer, int length);
	/* out */ int (*can_write)();
	/* out */ int (*is_playing)();
	/* out */ int (*pause)(int pause);
	
	/* out */ void (*set_volume)(int volume);
	/* out */ void (*set_panning)(int pan);
	
	/* out */ void (*flush)(int time);
	
	/* out */ int (*get_output_time)();
	/* out */ int (*get_written_time)();
};

#endif
