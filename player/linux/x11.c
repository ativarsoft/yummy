/*
 *  yummy/player/linux/x11.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <unistd.h> /* sleep() */
#include <sys/ipc.h> /* shmget() */
#include <sys/shm.h> /* shmget() */
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

extern int w;
extern int h;
extern int bpp;

int init_gui()
{
	Display *display;
	Window window;
	Window root;
	int ignore;
	int default_visual;
	XShmSegmentInfo shminfo;
	int gc; /* TODO: check type and create a gc. */
	
	display = XOpenDisplay(0);
	/* check for mit shared memory extension */
	/* this provides faster rendering */
	/* because ipc memory transfers of bitmaps is slow */
	if(XQueryExtension(display, "MIT-SHM", &ignore, &ignore, &ignore))
		puts("MIT-SHM");
	root = RootWindow(display, 0);
	window = XCreateSimpleWindow(display, root, 1, 1, w, h, 0, 0, 0);
	XShmCreateImage(display, default_visual, zpixmap, 0, &shminfo, 0, 0);
	shminfo.shmid = shmget(IPC_PRIVATE, w*h*bpp, IPC_CREAT | 0777); /* TODO: dunno what 0777 stands for */
	if(shminfo.shmid<0)
		return 1;
	image->data = (char *) shmat(shminfo.shmidm 0, 0);
	shminfo.shmaddr = image->data;
	shminfo.readOnly = 0;
	XShmAttach(display, &shminfo);
	XShmPutImage(display, window, gc, image, 0, 0, 0, 0, image->width, image->height, 0);
	XMapWindow(display, window);
	XFlush(display);
	sleep(5);
	XShmDetach(display, &shminfo);
	XDestroyImage(image);
	shmdt(shminfo.shmaddr);
	return 0;
}
