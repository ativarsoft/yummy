#include <platform.h>
#include <gui.h>
#include <skin.h>

extern struct gui gui_w32;
extern struct gui gui_x11;
extern struct skin classic;
extern struct skin modern;

/* This is used by classic and modern skins. */
window main_window;

struct gui *gui;
struct skin *skin;

void player_init(void)
{
#ifdef _WIN32
	gui = &gui_w32;
#elif defined __linux__
	gui = &gui_x11;
#endif
	skin = &classic;
	/*skin = &modern;*/
	
	gui->init();
	skin->init();
	/* these depend on the main window */
	tray_init(); /* Requires main window. */
	menus_init(); /* The menus are the same in classic and modern skins. */
	discovery_init(); /* Requires main window. */
	gui->event_handler();
	/*skin_classic(gui);*/
	/*preferences();*/
	/*pe_load("plugins/in_libav.dll");*/
	/*modern_skin("skins/Winamp Modern");*/
	/*discovery_init();*/
	
	/* TODO: destroy menus. */
	gui->quit();
	/*Sleep(10000);*/
	puts("bye");
}

