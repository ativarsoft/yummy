/*
 *  yummy/player/classic.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <gui.h>
#include <skin.h>
#include <classic.h>
#include <menus.h>
#include "playback.h"

#if 0
#define TRACE() puts(__FUNCTION__)
#else
#define TRACE()
#endif

#define NUM_CONTROLS_MAIN 25
#define NUM_CONTROLS_EQUALIZER 18
#define NUM_CONTROLS_PLAYLIST 16

#define CLASSIC_WINDOW_WIDTH 275
#define CLASSIC_WINDOW_HEIGHT 116

struct subbitmap {
	int bmp;
	int x, y;
};

struct widget {
	int rect[4];
	int cursor;
	const struct handlers *control;
	/*void (*action)(void);*/
	void *data;
};

struct handlers {
	void (*mouse_down)(const struct widget *widget, window_t w, int x, int y);
	void (*mouse_up)(const struct widget *widget, window_t w, int x, int y);
	void (*mouse_move)(const struct widget *widget, window_t w, int x, int y);
	void (*draw)(const struct widget *widget, window_t w);
};

/*struct classic_window {
	const struct control *controls;
	const int num_controls;
};*/

struct background_data {
	const struct subbitmap *subbmp;
	bool move_window;
};

struct checkbox_data {
	const struct subbitmap *on_up;
	const struct subbitmap *on_down;
	const struct subbitmap *off_up;
	const struct subbitmap *off_down;
	bool checked;
	void (*action)(bool checked);
};

struct slider_data {
	const struct subbitmap **background;
	const struct subbitmap *up;
	const struct subbitmap *down;
	bool horizontal;
	int min, max;
	int value;
	void (*action)(int value);
};

struct button_data {
	const struct subbitmap *up;
	const struct subbitmap *down;
	void (*action)(void);
};

struct clutterbar_data {
	bool a, d;
	int mouse_down;
	void (*set_o)();
	void (*set_a)(window_t, bool);
	void (*set_i)();
	void (*set_d)(bool);
	void (*set_v)();
};

struct song_title_data {
	char *string;
	int offset;
};

struct resizeable_background_data {
	const struct subbitmap *top_left;
	const struct subbitmap *title;
	const struct subbitmap *top;
	const struct subbitmap *top_right;
	const struct subbitmap *left;
	const struct subbitmap *right;
	const struct subbitmap *bottom;
	const struct subbitmap *bottom_left;
	const struct subbitmap *bottom_right;
	int length_top, length_left, length_right, length_bottom;
};

struct pledit_data {
	char normal_color[3], current_color[3];
	int cur_line;
};

struct menu_data {
	int num_buttons;
	int cur_button;
	const struct subbitmap **buttons_up;
	const struct subbitmap **buttons_down;
	const struct subbitmap *bar;
};

struct grip_data {
	int min_width, min_height;
};

struct scroll_data {
	const struct subbitmap *background;
	const struct subbitmap *bar_up;
	const struct subbitmap *bar_down;
	int bar_length;
	int value;
};

enum {
	BMP_BALANCE,
	BMP_CBUTTONS,
	BMP_EQ_EX,
	BMP_EQMAIN,
	BMP_EQMAIN_ISO,
	BMP_GEN,
	BMP_GENEX,
	BMP_MAIN,
	BMP_MB,
	BMP_MONOSTER,
	BMP_NUMBERS,
	BMP_NUMS_EX,
	BMP_PLAYPAUS,
	BMP_PLEDIT,
	BMP_POSBAR,
	BMP_SHUFREP,
	BMP_TEXT,
	BMP_TITLEBAR,
	BMP_VIDEO,
	BMP_VOLUME,
	NUM_BMPS
};

enum {
	CURSOR_CLOSE,
	CURSOR_EQCLOSE,
	CURSOR_EQNORMAL,
	CURSOR_EQSLID,
	CURSOR_EQTITLE,
	CURSOR_MAINMENU,
	CURSOR_MIN,
	CURSOR_NORMAL,
	CURSOR_PCLOSE,
	CURSOR_PNORMAL,
	CURSOR_POSBAR,
	CURSOR_PSIZE,
	CURSOR_PTBAR,
	CURSOR_PVSCROLL,
	CURSOR_PWINBUT,
	CURSOR_PWSNORM,
	CURSOR_PWSSIZE,
	CURSOR_SONGNAME,
	CURSOR_TITLEBAR,
	CURSOR_VOLBAL,
	CURSOR_VOLBAR,
	CURSOR_WINBUT,
	CURSOR_WSCLOSE,
	CURSOR_WSMIN,
	CURSOR_WSNORMAL,
	CURSOR_WSPOSBAR,
	CURSOR_WSWINBUT,
	NUM_CURSORS
};

const static char *bmp_filenames[] = {
	"BALANCE.BMP",
	"CBUTTONS.BMP",
	"EQ_EX.BMP",
	"EQMAIN.BMP",
	"EQMAIN_ISO.BMP",
	"GEN.BMP",
	"GENEX.BMP",
	"MAIN.BMP",
	"MB.BMP",
	"MONOSTER.BMP",
	"NUMBERS.BMP",
	"NUMS_EX.BMP",
	"PLAYPAUS.BMP",
	"PLEDIT.BMP",
	"POSBAR.BMP",
	"SHUFREP.BMP",
	"TEXT.BMP",
	"TITLEBAR.BMP",
	"VIDEO.BMP",
	"VOLUME.BMP"
};

const static char *cursor_filenames[] = {
	"CLOSE.CUR",
	"EQCLOSE.CUR",
	"EQNORMAL.CUR",
	"EQSLID.CUR",
	"EQTITLE.CUR",
	"MAINMENU.CUR",
	"MIN.CUR",
	"NORMAL.CUR",
	"PCLOSE.CUR",
	"PNORMAL.CUR",
	"POSBAR.CUR",
	"PSIZE.CUR",
	"PTBAR.CUR",
	"PVSCROLL.CUR",
	"PWINBUT.CUR",
	"PWSNORM.CUR",
	"PWSSIZE.CUR",
	"SONGNAME.CUR",
	"TITLEBAR.CUR",
	"VOLBAL.CUR",
	"VOLBAR.CUR",
	"WINBUT.CUR",
	"WSCLOSE.CUR",
	"WSMIN.CUR",
	"WSNORMAL.CUR",
	"WSPOSBAR.CUR",
	"WSWINBUT.CUR"
};

window_t w1, w2, w3;
pixmap_t bmps[NUM_BMPS];
cursor_t cursors[NUM_CURSORS];
bool double_size = false;
bool easymove = true;

/* event */
/*window_t cur_window;
int *cur_rect;*/
static int last_x, last_y;
static const struct widget *capture;
static bool main_shade = false, eq_shade = false, pl_shade = false;

static const struct subbitmap subbmp_main = {BMP_MAIN, 0, 0},
	subbmp_title_bar_on = {BMP_TITLEBAR, 27, 0},
	subbmp_title_bar_off = {BMP_TITLEBAR, 27, 15},
	subbmp_title_bar_shade_on = {BMP_TITLEBAR, 27, 29},
	subbmp_title_bar_shade_off = {BMP_TITLEBAR, 27, 42},
	subbmp_title_bar_easter_on = {BMP_TITLEBAR, 27, 57},
	subbmp_title_bar_easter_off = {BMP_TITLEBAR, 27, 72},
	subbmp_a = {BMP_TITLEBAR, 304, 0},
	subbmp_mono_on = {BMP_MONOSTER, 29, 0},
	subbmp_mono_off = {BMP_MONOSTER, 29, 12},
	subbmp_stereo_on = {BMP_MONOSTER, 0, 0},
	subbmp_stereo_off = {BMP_MONOSTER, 0, 12},
	subbmp_previous_up = {BMP_CBUTTONS, 0, 0},
	subbmp_previous_down = {BMP_CBUTTONS, 0, 18},
	subbmp_play_up = {BMP_CBUTTONS, 22+23*0, 0},
	subbmp_play_down = {BMP_CBUTTONS, 22+23*0, 18},
	subbmp_pause_up = {BMP_CBUTTONS, 22+23*1, 0},
	subbmp_pause_down = {BMP_CBUTTONS, 22+23*1, 18},
	subbmp_stop_up = {BMP_CBUTTONS, 22+23*2, 0},
	subbmp_stop_down = {BMP_CBUTTONS, 22+23*2, 18},
	subbmp_next_up = {BMP_CBUTTONS, 22+23*3, 0},
	subbmp_next_down = {BMP_CBUTTONS, 22+23*3, 18},
	subbmp_eject_up = {BMP_CBUTTONS, 22+23*4, 0},
	subbmp_eject_down = {BMP_CBUTTONS, 22+23*4, 16},
	subbmp_status = {BMP_PLAYPAUS, 9*2, 0},
	subbmp_volume_bg1 = {BMP_VOLUME, 0, 15*0},
	subbmp_volume_bg2 = {BMP_VOLUME, 0, 15*1},
	subbmp_volume_bg3 = {BMP_VOLUME, 0, 15*2},
	subbmp_volume_bg4 = {BMP_VOLUME, 0, 15*3},
	subbmp_volume_bg5 = {BMP_VOLUME, 0, 15*4},
	subbmp_volume_bg6 = {BMP_VOLUME, 0, 15*5},
	subbmp_volume_bg7 = {BMP_VOLUME, 0, 15*6},
	subbmp_volume_bg8 = {BMP_VOLUME, 0, 15*7},
	subbmp_volume_bg9 = {BMP_VOLUME, 0, 15*8},
	subbmp_volume_bg10 = {BMP_VOLUME, 0, 15*9},
	subbmp_volume_bg11 = {BMP_VOLUME, 0, 15*10},
	subbmp_volume_bg12 = {BMP_VOLUME, 0, 15*11},
	subbmp_volume_bg13 = {BMP_VOLUME, 0, 15*12},
	subbmp_volume_bg14 = {BMP_VOLUME, 0, 15*13},
	subbmp_volume_bg15 = {BMP_VOLUME, 0, 15*14},
	subbmp_volume_bg16 = {BMP_VOLUME, 0, 15*15},
	subbmp_volume_bg17 = {BMP_VOLUME, 0, 15*16},
	subbmp_volume_bg18 = {BMP_VOLUME, 0, 15*17},
	subbmp_volume_bg19 = {BMP_VOLUME, 0, 15*18},
	subbmp_volume_bg20 = {BMP_VOLUME, 0, 15*19},
	subbmp_volume_bg21 = {BMP_VOLUME, 0, 15*20},
	subbmp_volume_bg22 = {BMP_VOLUME, 0, 15*21},
	subbmp_volume_bg23 = {BMP_VOLUME, 0, 15*22},
	subbmp_volume_bg24 = {BMP_VOLUME, 0, 15*23},
	subbmp_volume_bg25 = {BMP_VOLUME, 0, 15*24},
	subbmp_volume_bg26 = {BMP_VOLUME, 0, 15*25},
	subbmp_volume_bg27 = {BMP_VOLUME, 0, 15*26},
	subbmp_volume_bg28 = {BMP_VOLUME, 0, 15*27},
	subbmp_volume_bar_up = {BMP_VOLUME, 15, 422},
	subbmp_volume_bar_down = {BMP_VOLUME, 0, 422},
	subbmp_balance_bg1 = {BMP_BALANCE, 9, 15*0},
	subbmp_balance_bg2 = {BMP_BALANCE, 9, 15*1},
	subbmp_balance_bg3 = {BMP_BALANCE, 9, 15*2},
	subbmp_balance_bg4 = {BMP_BALANCE, 9, 15*3},
	subbmp_balance_bg5 = {BMP_BALANCE, 9, 15*4},
	subbmp_balance_bg6 = {BMP_BALANCE, 9, 15*5},
	subbmp_balance_bg7 = {BMP_BALANCE, 9, 15*6},
	subbmp_balance_bg8 = {BMP_BALANCE, 9, 15*7},
	subbmp_balance_bg9 = {BMP_BALANCE, 9, 15*8},
	subbmp_balance_bg10 = {BMP_BALANCE, 9, 15*9},
	subbmp_balance_bg11 = {BMP_BALANCE, 9, 15*10},
	subbmp_balance_bg12 = {BMP_BALANCE, 9, 15*11},
	subbmp_balance_bg13 = {BMP_BALANCE, 9, 15*12},
	subbmp_balance_bg14 = {BMP_BALANCE, 9, 15*13},
	subbmp_balance_bg15 = {BMP_BALANCE, 9, 15*14},
	subbmp_balance_bg16 = {BMP_BALANCE, 9, 15*15},
	subbmp_balance_bg17 = {BMP_BALANCE, 9, 15*16},
	subbmp_balance_bg18 = {BMP_BALANCE, 9, 15*17},
	subbmp_balance_bg19 = {BMP_BALANCE, 9, 15*18},
	subbmp_balance_bg20 = {BMP_BALANCE, 9, 15*19},
	subbmp_balance_bg21 = {BMP_BALANCE, 9, 15*20},
	subbmp_balance_bg22 = {BMP_BALANCE, 9, 15*21},
	subbmp_balance_bg23 = {BMP_BALANCE, 9, 15*22},
	subbmp_balance_bg24 = {BMP_BALANCE, 9, 15*23},
	subbmp_balance_bg25 = {BMP_BALANCE, 9, 15*24},
	subbmp_balance_bg26 = {BMP_BALANCE, 9, 15*25},
	subbmp_balance_bg27 = {BMP_BALANCE, 9, 15*26},
	subbmp_balance_bg28 = {BMP_BALANCE, 9, 15*27},
	subbmp_balance_bar_up = {BMP_BALANCE, 15, 422},
	subbmp_balance_bar_down = {BMP_BALANCE, 0, 422},
	subbmp_eq_off_up = {BMP_SHUFREP, 0, 61},
	subbmp_eq_off_down = {BMP_SHUFREP, 46, 61},
	subbmp_eq_on_up = {BMP_SHUFREP, 0, 73},
	subbmp_eq_on_down = {BMP_SHUFREP, 46, 73},
	subbmp_pl_off_up = {BMP_SHUFREP, 23, 61},
	subbmp_pl_off_down = {BMP_SHUFREP, 70, 61},
	subbmp_pl_on_up = {BMP_SHUFREP, 23, 73},
	subbmp_pl_on_down = {BMP_SHUFREP, 70, 73},
	subbmp_shuffle_off_up = {BMP_SHUFREP, 29, 0},
	subbmp_shuffle_off_down = {BMP_SHUFREP, 29, 15},
	subbmp_shuffle_on_up = {BMP_SHUFREP, 29, 15*2},
	subbmp_shuffle_on_down = {BMP_SHUFREP, 29, 15*3},
	subbmp_repeat_off_up = {BMP_SHUFREP, 0, 0},
	subbmp_repeat_off_down = {BMP_SHUFREP, 0, 15},
	subbmp_repeat_on_up = {BMP_SHUFREP, 0, 15*2},
	subbmp_repeat_on_down = {BMP_SHUFREP, 0, 15*3},
	subbmp_clutterbar_off = {BMP_TITLEBAR, 304, 0},
	subbmp_clutterbar_disabled = {BMP_TITLEBAR, 312, 0},
	subbmp_clutterbar_off_o = {BMP_TITLEBAR, 304, 44},
	subbmp_clutterbar_off_a = {BMP_TITLEBAR, 312, 44},
	subbmp_clutterbar_off_i = {BMP_TITLEBAR, 320, 44},
	subbmp_clutterbar_off_d = {BMP_TITLEBAR, 328, 44},
	subbmp_clutterbar_off_v = {BMP_TITLEBAR, 336, 44},
	subbmp_clutterbar_o = {BMP_TITLEBAR, 304, 45},
	subbmp_clutterbar_a = {BMP_TITLEBAR, 312, 53},
	subbmp_clutterbar_i = {BMP_TITLEBAR, 320, 61},
	subbmp_clutterbar_d = {BMP_TITLEBAR, 328, 69},
	subbmp_clutterbar_v = {BMP_TITLEBAR, 336, 77},
	subbmp_status_play = {BMP_PLAYPAUS, 0, 0},
	subbmp_status_pause = {BMP_PLAYPAUS, 9, 0},
	subbmp_status_stop = {BMP_PLAYPAUS, 18, 0},
	subbmp_status_red_on = {BMP_PLAYPAUS, 0, 0},
	subbmp_status_red_off = {BMP_PLAYPAUS, 0, 0},
	subbmp_status_green_on = {BMP_PLAYPAUS, 0, 0},
	subbmp_status_green_off = {BMP_PLAYPAUS, 0, 0},
	subbmp_options_up = {BMP_TITLEBAR, 0, 0},
	subbmp_options_down = {BMP_TITLEBAR, 0, 9},
	subbmp_minimize_up = {BMP_TITLEBAR, 9, 0},
	subbmp_minimize_down = {BMP_TITLEBAR, 9, 9},
	subbmp_close_up = {BMP_TITLEBAR, 18, 0},
	subbmp_close_down = {BMP_TITLEBAR, 18, 9},
	subbmp_maximize_normal_up = {BMP_TITLEBAR, 0, 18},
	subbmp_maximize_normal_down = {BMP_TITLEBAR, 9, 18},
	subbmp_maximize_ws_up = {BMP_TITLEBAR, 0, 27},
	subbmp_maximize_ws_down = {BMP_TITLEBAR, 9, 27},
	subbmp_posbar_background = {BMP_POSBAR, 0, 0},
	subbmp_posbar_bar_up = {BMP_POSBAR, 248, 0},
	subbmp_posbar_bar_down = {BMP_POSBAR, 277, 0};

static const struct subbitmap subbmp_eq_background = {BMP_EQMAIN, 0, 0},
	subbmp_eq_title_bar_on = {BMP_EQMAIN, 0, 134},
	subbmp_eq_title_bar_off = {BMP_EQMAIN, 0, 149},
	subbmp_eq_on_on_up = {BMP_EQMAIN, 69, 119},
	subbmp_eq_on_on_down = {BMP_EQMAIN, 187, 119},
	subbmp_eq_on_off_up = {BMP_EQMAIN, 10, 119},
	subbmp_eq_on_off_down = {BMP_EQMAIN, 128, 119},
	subbmp_eq_auto_on_up = {BMP_EQMAIN, 95, 119},
	subbmp_eq_auto_on_down = {BMP_EQMAIN, 213, 119},
	subbmp_eq_auto_off_up = {BMP_EQMAIN, 36, 119},
	subbmp_eq_auto_off_down = {BMP_EQMAIN, 154, 119},
	subbmp_eq_presets_up = {BMP_EQMAIN, 224, 164},
	subbmp_eq_presets_down = {BMP_EQMAIN, 224, 176},
	subbmp_eq_preamp = {BMP_EQMAIN, 13, 164},
	subbmp_eq_slider_bg1 = {BMP_EQMAIN, 13+15*0, 164},
	subbmp_eq_slider_bg2 = {BMP_EQMAIN, 13+15*1, 164},
	subbmp_eq_slider_bg3 = {BMP_EQMAIN, 13+15*2, 164},
	subbmp_eq_slider_bg4 = {BMP_EQMAIN, 13+15*3, 164},
	subbmp_eq_slider_bg5 = {BMP_EQMAIN, 13+15*4, 164},
	subbmp_eq_slider_bg6 = {BMP_EQMAIN, 13+15*5, 164},
	subbmp_eq_slider_bg7 = {BMP_EQMAIN, 13+15*6, 164},
	subbmp_eq_slider_bg8 = {BMP_EQMAIN, 13+15*7, 164},
	subbmp_eq_slider_bg9 = {BMP_EQMAIN, 13+15*8, 164},
	subbmp_eq_slider_bg10 = {BMP_EQMAIN, 13+15*9, 164},
	subbmp_eq_slider_bg11 = {BMP_EQMAIN, 13+15*10, 164},
	subbmp_eq_slider_bg12 = {BMP_EQMAIN, 13+15*11, 164},
	subbmp_eq_slider_bg13 = {BMP_EQMAIN, 13+15*12, 164},
	subbmp_eq_slider_bg14 = {BMP_EQMAIN, 13+15*13, 164},
	subbmp_eq_slider_bg15 = {BMP_EQMAIN, 13+15*0, 229},
	subbmp_eq_slider_bg16 = {BMP_EQMAIN, 13+15*1, 229},
	subbmp_eq_slider_bg17 = {BMP_EQMAIN, 13+15*2, 229},
	subbmp_eq_slider_bg18 = {BMP_EQMAIN, 13+15*3, 229},
	subbmp_eq_slider_bg19 = {BMP_EQMAIN, 13+15*4, 229},
	subbmp_eq_slider_bg20 = {BMP_EQMAIN, 13+15*5, 229},
	subbmp_eq_slider_bg21 = {BMP_EQMAIN, 13+15*6, 229},
	subbmp_eq_slider_bg22 = {BMP_EQMAIN, 13+15*7, 229},
	subbmp_eq_slider_bg23 = {BMP_EQMAIN, 13+15*8, 229},
	subbmp_eq_slider_bg24 = {BMP_EQMAIN, 13+15*9, 229},
	subbmp_eq_slider_bg25 = {BMP_EQMAIN, 13+15*10, 229},
	subbmp_eq_slider_bg26 = {BMP_EQMAIN, 13+15*11, 229},
	subbmp_eq_slider_bg27 = {BMP_EQMAIN, 13+15*12, 229},
	subbmp_eq_slider_bg28 = {BMP_EQMAIN, 13+15*13, 229},
	subbmp_eq_slider_up = {BMP_EQMAIN, 0, 164},
	subbmp_eq_slider_down = {BMP_EQMAIN, 0, 176};

static const struct subbitmap subbmp_pl_top_left_on = {BMP_PLEDIT, 0, 0},
	subbmp_pl_title_on = {BMP_PLEDIT, 26, 0},
	subbmp_pl_top_on = {BMP_PLEDIT, 127, 0},
	subbmp_pl_top_right_on = {BMP_PLEDIT, 153, 0},
	subbmp_pl_top_left_off = {BMP_PLEDIT, 0, 21},
	subbmp_pl_title_off = {BMP_PLEDIT, 26, 21},
	subbmp_pl_top_off = {BMP_PLEDIT, 127, 21},
	subbmp_pl_top_right_off = {BMP_PLEDIT, 153, 21},
	subbmp_pl_left = {BMP_PLEDIT, 0, 42},
	subbmp_pl_right = {BMP_PLEDIT, 26, 42},
	subbmp_pl_bottom = {BMP_PLEDIT, 179, 0},
	subbmp_pl_bottom_left = {BMP_PLEDIT, 0, 72},
	subbmp_pl_bottom_right = {BMP_PLEDIT, 126, 72},
	subbmp_pl_menu_add_url_up = {BMP_PLEDIT, 0, 111},
	subbmp_pl_menu_add_dir_up = {BMP_PLEDIT, 0, 130},
	subbmp_pl_menu_add_file_up = {BMP_PLEDIT, 0, 149},
	subbmp_pl_menu_add_url_down = {BMP_PLEDIT, 23, 111},
	subbmp_pl_menu_add_dir_down = {BMP_PLEDIT, 23, 130},
	subbmp_pl_menu_add_file_down = {BMP_PLEDIT, 23, 149},
	subbmp_pl_menu_add_bar = {BMP_PLEDIT, 48, 111},
	subbmp_pl_menu_rem_all_up = {BMP_PLEDIT, 54, 111},
	subbmp_pl_menu_rem_crop_up = {BMP_PLEDIT, 54, 130},
	subbmp_pl_menu_rem_sel_up = {BMP_PLEDIT, 54, 149},
	subbmp_pl_menu_rem_misc_up = {BMP_PLEDIT, 54, 168},
	subbmp_pl_menu_rem_all_down = {BMP_PLEDIT, 77, 111},
	subbmp_pl_menu_rem_crop_down = {BMP_PLEDIT, 77, 130},
	subbmp_pl_menu_rem_sel_down = {BMP_PLEDIT, 77, 149},
	subbmp_pl_menu_rem_misc_down = {BMP_PLEDIT, 77, 168},
	subbmp_pl_menu_rem_bar = {BMP_PLEDIT, 100, 111},
	subbmp_pl_menu_sel_inv_up = {BMP_PLEDIT, 104, 111},
	subbmp_pl_menu_sel_zero_up = {BMP_PLEDIT, 104, 130},
	subbmp_pl_menu_sel_all_up = {BMP_PLEDIT, 104, 149},
	subbmp_pl_menu_sel_inv_down = {BMP_PLEDIT, 127, 111},
	subbmp_pl_menu_sel_zero_down = {BMP_PLEDIT, 127, 130},
	subbmp_pl_menu_sel_all_down = {BMP_PLEDIT, 127, 149},
	subbmp_pl_menu_sel_bar = {BMP_PLEDIT, 150, 111},
	subbmp_pl_menu_misc_sort_up = {BMP_PLEDIT, 154, 111},
	subbmp_pl_menu_misc_inf_up = {BMP_PLEDIT, 154, 130},
	subbmp_pl_menu_misc_opts_up = {BMP_PLEDIT, 154, 149},
	subbmp_pl_menu_misc_sort_down = {BMP_PLEDIT, 177, 111},
	subbmp_pl_menu_misc_inf_down = {BMP_PLEDIT, 177, 130},
	subbmp_pl_menu_misc_opts_down = {BMP_PLEDIT, 177, 149},
	subbmp_pl_menu_misc_bar = {BMP_PLEDIT, 200, 111},
	subbmp_pl_menu_list_new_up = {BMP_PLEDIT, 204, 111},
	subbmp_pl_menu_list_save_up = {BMP_PLEDIT, 204, 130},
	subbmp_pl_menu_list_load_up = {BMP_PLEDIT, 204, 149},
	subbmp_pl_menu_list_new_down = {BMP_PLEDIT, 227, 111},
	subbmp_pl_menu_list_save_down = {BMP_PLEDIT, 227, 130},
	subbmp_pl_menu_list_load_down = {BMP_PLEDIT, 227, 149},
	subbmp_pl_menu_list_bar = {BMP_PLEDIT, 250, 111};

static struct subbitmap a = {BMP_TEXT, 0, 0},
	b = {BMP_TEXT, 5, 0},
	c = {BMP_TEXT, 10, 0},
	d = {BMP_TEXT, 15, 0},
	e = {BMP_TEXT, 20, 0},
	f = {BMP_TEXT, 25, 0},
	g = {BMP_TEXT, 30, 0},
	h = {BMP_TEXT, 35, 0},
	i = {BMP_TEXT, 40, 0},
	j = {BMP_TEXT, 45, 0},
	k = {BMP_TEXT, 50, 0},
	l = {BMP_TEXT, 55, 0},
	m = {BMP_TEXT, 60, 0},
	n = {BMP_TEXT, 65, 0},
	o = {BMP_TEXT, 70, 0},
	p = {BMP_TEXT, 75, 0},
	q = {BMP_TEXT, 80, 0},
	r = {BMP_TEXT, 85, 0},
	s = {BMP_TEXT, 90, 0},
	t = {BMP_TEXT, 95, 0},
	u = {BMP_TEXT, 100, 0},
	v = {BMP_TEXT, 105, 0},
	w = {BMP_TEXT, 110, 0},
	x = {BMP_TEXT, 115, 0},
	y = {BMP_TEXT, 120, 0},
	z = {BMP_TEXT, 125, 0},
	double_quote = {BMP_TEXT, 130, 0},
	at = {BMP_TEXT, 135, 0},
	space = {BMP_TEXT, 150, 0},
	zero = {BMP_TEXT, 0, 6},
	one = {BMP_TEXT, 5, 6},
	two = {BMP_TEXT, 10, 6},
	three = {BMP_TEXT, 15, 6},
	four = {BMP_TEXT, 20, 6},
	five = {BMP_TEXT, 25, 6},
	six = {BMP_TEXT, 30, 6},
	seven = {BMP_TEXT, 35, 6},
	eight = {BMP_TEXT, 40, 6},
	nine = {BMP_TEXT, 45, 6},
	ellipsis = {BMP_TEXT, 50, 6},
	period = {BMP_TEXT, 55, 6},
	colon = {BMP_TEXT, 60, 6},
	left_parentheses = {BMP_TEXT, 65, 6},
	right_parentheses = {BMP_TEXT, 70, 6},
	minus = {BMP_TEXT, 75, 6},
	single_quote = {BMP_TEXT, 80, 6},
	exclamation = {BMP_TEXT, 85, 6},
	underscore = {BMP_TEXT, 90, 6},
	plus = {BMP_TEXT, 95, 6},
	slash = {BMP_TEXT, 100, 6},
	backslash = {BMP_TEXT, 105, 6},
	left_bracket = {BMP_TEXT, 110, 6},
	right_bracket = {BMP_TEXT, 115, 6},
	caret = {BMP_TEXT, 120, 6},
	and = {BMP_TEXT, 125, 6},
	percent = {BMP_TEXT, 130, 6},
	comma = {BMP_TEXT, 135, 6},
	equal = {BMP_TEXT, 140, 6},
	dollar = {BMP_TEXT, 145, 6},
	hash = {BMP_TEXT, 150, 6},
	a_ring = {BMP_TEXT, 0, 12},
	o_umlaut = {BMP_TEXT, 5, 12},
	a_umlaut = {BMP_TEXT, 10, 12},
	question = {BMP_TEXT, 15, 12},
	asterisk = {BMP_TEXT, 20, 12};

const struct subbitmap *ascii_table[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	&space, &exclamation, &double_quote, &hash, &dollar, &percent, &and, &single_quote,
	&left_parentheses, &right_parentheses, &asterisk, &plus, &comma, &minus, &period, &slash,
	&zero, &one, &two, &three, &four, &five, &six, &seven, &eight,
	&nine, &colon, 0, 0, &equal, 0, &question,
	&at, &a, &b, &c, &d, &e, &f, &g,
	&h, &i, &j, &k, &l, &m, &n, &o,
	&p, &q, &r, &s, &t, &u, &v, &w,
	&x, &y, &z, &left_bracket, &backslash, &right_bracket, &caret, &underscore,
	0, &a, &b, &c, &d, &e, &f, &g,
	&h, &i, &j, &k, &l, &m, &n, &o,
	&p, &q, &r, &s, &t, &u, &v, &w,
	&x, &y, &z, &left_bracket, 0, &right_bracket, 0, 0,
	&c, &u, &e, &a, &a_umlaut, &a, &a_ring, &c,
	&e, &e, &e, &i, &i, &i, &a_umlaut, &a_ring,
	&e, &a, &a, &o, &o_umlaut, &o, &u, &u,
	&y, &o_umlaut, &u, 0, 0, 0, 0, 0,
	&a, &i, &o, &u, &n, &n, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

static void draw_time(int minutes, int seconds, bool remaining_time)
{
	pixmap_t bmp;
	
	bmp = bmps[BMP_NUMS_EX];
	gui->draw_image(bmp, 48, 26, 9, 13, 9 * (minutes / 10), 0);
	gui->draw_image(bmp, 60, 26, 9, 13, 9 * (minutes % 10), 0);
	gui->draw_image(bmp, 78, 26, 9, 13, 9 * (seconds / 10), 0);
	gui->draw_image(bmp, 90, 26, 9, 13, 9 * (seconds % 10), 0);
}

static void test_button(void)
{
	puts(__FUNCTION__);
}

static void test_checkbox(bool state) {
	printf("%s: %s\n", __FUNCTION__, state? "true" : "false");
}

static struct background_data data_main = {
	.subbmp = &subbmp_main
};

static struct background_data data_title_bar = {
	.subbmp = &subbmp_title_bar_off
};

static struct background_data data_mono = {
	.subbmp = &subbmp_mono_off
};

static struct background_data data_stereo = {
	.subbmp = &subbmp_stereo_off
};

static struct background_data data_status = {
	.subbmp = &subbmp_status_stop
};

static struct button_data data_options = {
	.up = &subbmp_options_up,
	.down = &subbmp_options_down,
	.action = &test_button
};

static void cmd_main_minimize(void)
{
	gui->minimize_window(w1);
	gui->hide_window(w2);
	gui->hide_window(w3);
}

static struct button_data data_minimize = {
	.up = &subbmp_minimize_up,
	.down = &subbmp_minimize_down,
	.action = &cmd_main_minimize
};

static void cmd_main_maximize(void)
{
	main_shade = !main_shade;
	if (main_shade)
		gui->resize_window(w1, 275, 14);
	else
		gui->resize_window(w1, 275, 116);
}

static struct button_data data_maximize = {
	.up = &subbmp_maximize_normal_up,
	.down = &subbmp_maximize_normal_down,
	.action = &cmd_main_maximize
};

static void cmd_main_close(void)
{
	TRACE();
	gui->destroy_window(w1);
}

static struct button_data data_close = {
	.up = &subbmp_close_up,
	.down = &subbmp_close_down,
	.action = &cmd_main_close
};

static void cmd_main_previous(void)
{
	playback_previous();
}

static struct button_data data_previous = {
	.up = &subbmp_previous_up,
	.down = &subbmp_previous_down,
	.action = &cmd_main_previous
};

static void cmd_main_play(void)
{
	playback_play();
}

static struct button_data data_play = {
	.up = &subbmp_play_up,
	.down = &subbmp_play_down,
	.action = &cmd_main_play /* playback->play(); */,
};

static void cmd_main_pause(void)
{
	playback_pause();
}

static struct button_data data_pause = {
	.up = &subbmp_pause_up,
	.down = &subbmp_pause_down,
	.action = &cmd_main_pause
};

static void cmd_main_stop(void)
{
	playback_stop();
}

static struct button_data data_stop = {
	.up = &subbmp_stop_up,
	.down = &subbmp_stop_down,
	.action = &cmd_main_stop
};

static void cmd_main_next(void)
{
	playback_next();
}

static struct button_data data_next = {
	.up = &subbmp_next_up,
	.down = &subbmp_next_down,
	.action = &cmd_main_next
};

static void cmd_main_eject(void)
{
	gui->open_file_dialog();
}

static struct button_data data_eject = {
	.up = &subbmp_eject_up,
	.down = &subbmp_eject_down,
	.action = &cmd_main_eject
};

static void cmd_main_eq(bool checked)
{
	if (checked)
		gui->show_window(w2);
	else
		gui->hide_window(w2);
}

static struct checkbox_data data_eq = {
	.on_up = &subbmp_eq_on_up,
	.on_down = &subbmp_eq_on_down,
	.off_up = &subbmp_eq_off_up,
	.off_down = &subbmp_eq_off_down,
	.checked = false,
	.action = &cmd_main_eq
};

static void cmd_main_pl(bool checked)
{
	if (checked)
		gui->show_window(w3);
	else
		gui->hide_window(w3);
}

static struct checkbox_data data_pl = {
	.on_up = &subbmp_pl_on_up,
	.on_down = &subbmp_pl_on_down,
	.off_up = &subbmp_pl_off_up,
	.off_down = &subbmp_pl_off_down,
	.checked = false,
	.action = &cmd_main_pl
};

static struct checkbox_data data_shuffle = {
	.on_up = &subbmp_shuffle_on_up,
	.on_down = &subbmp_shuffle_on_down,
	.off_up = &subbmp_shuffle_off_up,
	.off_down = &subbmp_shuffle_off_down,
	.checked = false,
	.action = &test_checkbox
};

static struct checkbox_data data_repeat = {
	.on_up = &subbmp_repeat_on_up,
	.on_down = &subbmp_repeat_on_down,
	.off_up = &subbmp_repeat_off_up,
	.off_down = &subbmp_repeat_off_down,
	.checked = false,
	.action = &test_checkbox
};

static const struct subbitmap *volume_backgrounds[28] = {
	&subbmp_volume_bg1,
	&subbmp_volume_bg2,
	&subbmp_volume_bg3,
	&subbmp_volume_bg4,
	&subbmp_volume_bg5,
	&subbmp_volume_bg6,
	&subbmp_volume_bg7,
	&subbmp_volume_bg8,
	&subbmp_volume_bg9,
	&subbmp_volume_bg10,
	&subbmp_volume_bg11,
	&subbmp_volume_bg12,
	&subbmp_volume_bg13,
	&subbmp_volume_bg14,
	&subbmp_volume_bg15,
	&subbmp_volume_bg16,
	&subbmp_volume_bg17,
	&subbmp_volume_bg18,
	&subbmp_volume_bg19,
	&subbmp_volume_bg20,
	&subbmp_volume_bg21,
	&subbmp_volume_bg22,
	&subbmp_volume_bg23,
	&subbmp_volume_bg24,
	&subbmp_volume_bg25,
	&subbmp_volume_bg26,
	&subbmp_volume_bg27,
	&subbmp_volume_bg28
};

static void cmd_main_set_volume(int msg, int param)
{
	/*set_volume(param);*/
}

static struct slider_data data_volume = {
	.background = volume_backgrounds,
	.up = &subbmp_volume_bar_up,
	.down = &subbmp_volume_bar_down,
	.horizontal = true,
	.min = 0,
	.max = 51
};

static const struct subbitmap *balance_backgrounds[28] = {
	&subbmp_balance_bg28,
	&subbmp_balance_bg26,
	&subbmp_balance_bg24,
	&subbmp_balance_bg22,
	&subbmp_balance_bg20,
	&subbmp_balance_bg18,
	&subbmp_balance_bg16,
	&subbmp_balance_bg14,
	&subbmp_balance_bg12,
	&subbmp_balance_bg10,
	&subbmp_balance_bg8,
	&subbmp_balance_bg6,
	&subbmp_balance_bg4,
	&subbmp_balance_bg1,
	&subbmp_balance_bg2,
	&subbmp_balance_bg4,
	&subbmp_balance_bg6,
	&subbmp_balance_bg8,
	&subbmp_balance_bg10,
	&subbmp_balance_bg12,
	&subbmp_balance_bg14,
	&subbmp_balance_bg16,
	&subbmp_balance_bg18,
	&subbmp_balance_bg20,
	&subbmp_balance_bg22,
	&subbmp_balance_bg24,
	&subbmp_balance_bg26,
	&subbmp_balance_bg28
};

static void cmd_main_set_ballance(int msg, int param)
{
}

static struct slider_data data_balance = {
	.background = balance_backgrounds,
	.up = &subbmp_balance_bar_up,
	.down = &subbmp_balance_bar_down,
	.horizontal = true,
	.min = 0,
	.max = 24
};

void set_o()
{
	TRACE();
}

void set_a(window_t w, bool activated)
{
	TRACE();
	if (activated)
		gui->set_topmost(w);
	else
		gui->set_not_topmost(w);
}

void set_i()
{
	TRACE();
}

void set_d(bool activated)
{
	int rect[4];

	TRACE();
	gui->get_window_rect(rect);
	if (activated) {
		gui->resize_window(w1, 275 * 2, 116 * 2);
		gui->resize_window(w2, 275 * 2, 116 * 2);
	} else {
		gui->resize_window(w1, 275, 116);
		gui->resize_window(w2, 275, 116);
	}
	double_size = activated;
	gui->redraw_window(w1);
	gui->redraw_window(w2);
}

void set_v()
{
	TRACE();
}

static struct clutterbar_data data_clutterbar = {
	.a = false,
	.d = false,
	.set_o = &set_o,
	.set_a = &set_a,
	.set_i = &set_i,
	.set_d = &set_d,
	.set_v = &set_v
};

static struct song_title_data data_song_title = {
	.string = "  ***  ",
	.offset = 0
};

static struct scroll_data data_posbar = {
	.background = &subbmp_posbar_background,
	.bar_up = &subbmp_posbar_bar_up,
	.bar_down = &subbmp_posbar_bar_down,
	.bar_length = 29,
	.value = 0
};

static void cmd_eq_close(void)
{
	data_eq.checked = false;
	gui->hide_window(w2);
	gui->redraw_window(w1);
}

static struct background_data data_eq_background = {
	.subbmp = &subbmp_eq_background
};

static struct background_data data_eq_title_bar = {
	.subbmp = &subbmp_eq_title_bar_off
};

static void cmd_eq_maximize(void)
{
	eq_shade = !eq_shade;
	if (eq_shade)
		gui->resize_window(w2, 275, 14);
	else
		gui->resize_window(w2, 275, 116);
}

static struct button_data data_eq_maximize = {
	.up = &subbmp_maximize_normal_up,
	.down = &subbmp_maximize_normal_down,
	.action = &cmd_eq_maximize
};

static struct button_data data_eq_close = {
	.up = &subbmp_close_up,
	.down = &subbmp_close_down,
	.action = &cmd_eq_close
};

static struct checkbox_data data_eq_on = {
	.on_up = &subbmp_eq_on_on_up,
	.on_down = &subbmp_eq_on_on_down,
	.off_up = &subbmp_eq_on_off_up,
	.off_down = &subbmp_eq_on_off_down,
	.action = &test_checkbox
};

static struct checkbox_data data_eq_auto = {
	.on_up = &subbmp_eq_auto_on_up,
	.on_down = &subbmp_eq_auto_on_down,
	.off_up = &subbmp_eq_auto_off_up,
	.off_down = &subbmp_eq_auto_off_down,
	.action = &test_checkbox
};

static void cmd_eq_presets(void)
{
	int rect[4];

	gui->get_window_rect(rect);
	menus_show(menu_presets, rect[0] + 217, rect[1] + 18);
}

static struct button_data data_eq_presets = {
	.up = &subbmp_eq_presets_up,
	.down = &subbmp_eq_presets_down,
	.action = &cmd_eq_presets
};

static const struct subbitmap *eq_slider_backgrounds[28] = {
	&subbmp_eq_slider_bg1,
	&subbmp_eq_slider_bg2,
	&subbmp_eq_slider_bg3,
	&subbmp_eq_slider_bg4,
	&subbmp_eq_slider_bg5,
	&subbmp_eq_slider_bg6,
	&subbmp_eq_slider_bg7,
	&subbmp_eq_slider_bg8,
	&subbmp_eq_slider_bg9,
	&subbmp_eq_slider_bg10,
	&subbmp_eq_slider_bg11,
	&subbmp_eq_slider_bg12,
	&subbmp_eq_slider_bg13,
	&subbmp_eq_slider_bg14,
	&subbmp_eq_slider_bg15,
	&subbmp_eq_slider_bg16,
	&subbmp_eq_slider_bg17,
	&subbmp_eq_slider_bg18,
	&subbmp_eq_slider_bg19,
	&subbmp_eq_slider_bg20,
	&subbmp_eq_slider_bg21,
	&subbmp_eq_slider_bg22,
	&subbmp_eq_slider_bg23,
	&subbmp_eq_slider_bg24,
	&subbmp_eq_slider_bg25,
	&subbmp_eq_slider_bg26,
	&subbmp_eq_slider_bg27,
	&subbmp_eq_slider_bg28
};

static struct slider_data data_eq_preamp = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider1 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider2 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider3 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider4 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider5 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider6 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider7 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider8 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider9 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static struct slider_data data_eq_slider10 = {
	.background = eq_slider_backgrounds,
	.up = &subbmp_eq_slider_up,
	.down = &subbmp_eq_slider_down,
	.horizontal = false,
	.min = 2,
	.max = 51
};

static void cmd_pl_close(void)
{
	data_pl.checked = false;
	gui->hide_window(w3);
	gui->redraw_window(w1);
}

static struct resizeable_background_data data_pl_background = {
	.top_left = &subbmp_pl_top_left_off,
	.title = &subbmp_pl_title_off,
	.top = &subbmp_pl_top_off,
	.top_right = &subbmp_pl_top_right_off,
	.left = &subbmp_pl_left,
	.right = &subbmp_pl_right,
	.bottom = &subbmp_pl_bottom,
	.bottom_left = &subbmp_pl_bottom_left,
	.bottom_right = &subbmp_pl_bottom_right,
	.length_top = 0,
	.length_left = 0,
	.length_right = 0,
	.length_bottom = 38
};

static void cmd_pl_maximize(void)
{
	pl_shade = !pl_shade;
	if (pl_shade)
		gui->resize_window(w3, 275, 14);
	else
		gui->resize_window(w3, 275, 116);
}

static struct button_data data_pl_maximize = {
	.up = &subbmp_maximize_normal_up,
	.down = &subbmp_maximize_normal_down,
	.action = &cmd_pl_maximize
};

static struct button_data data_pl_close = {
	.up = &subbmp_close_up,
	.down = &subbmp_close_down,
	.action = &cmd_pl_close
};

static struct pledit_data data_pledit = {
};

static const struct subbitmap *menu_add_up_backgrounds[3] = {
	&subbmp_pl_menu_add_url_up,
	&subbmp_pl_menu_add_dir_up,
	&subbmp_pl_menu_add_file_up
};

static const struct subbitmap *menu_add_down_backgrounds[3] = {
	&subbmp_pl_menu_add_url_down,
	&subbmp_pl_menu_add_dir_down,
	&subbmp_pl_menu_add_file_down
};

static const struct subbitmap *menu_rem_up_backgrounds[4] = {
	&subbmp_pl_menu_rem_all_up,
	&subbmp_pl_menu_rem_crop_up,
	&subbmp_pl_menu_rem_sel_up,
	&subbmp_pl_menu_rem_misc_up
};

static const struct subbitmap *menu_rem_down_backgrounds[4] = {
	&subbmp_pl_menu_rem_all_down,
	&subbmp_pl_menu_rem_crop_down,
	&subbmp_pl_menu_rem_sel_down,
	&subbmp_pl_menu_rem_misc_down
};

static const struct subbitmap *menu_sel_up_backgrounds[3] = {
	&subbmp_pl_menu_sel_inv_up,
	&subbmp_pl_menu_sel_zero_up,
	&subbmp_pl_menu_sel_all_up
};

static const struct subbitmap *menu_sel_down_backgrounds[3] = {
	&subbmp_pl_menu_sel_inv_down,
	&subbmp_pl_menu_sel_zero_down,
	&subbmp_pl_menu_sel_all_down
};

static const struct subbitmap *menu_misc_up_backgrounds[3] = {
	&subbmp_pl_menu_misc_sort_up,
	&subbmp_pl_menu_misc_inf_up,
	&subbmp_pl_menu_misc_opts_up
};

static const struct subbitmap *menu_misc_down_backgrounds[3] = {
	&subbmp_pl_menu_misc_sort_down,
	&subbmp_pl_menu_misc_inf_down,
	&subbmp_pl_menu_misc_opts_down
};

static const struct subbitmap *menu_list_up_backgrounds[3] = {
	&subbmp_pl_menu_list_new_up,
	&subbmp_pl_menu_list_save_up,
	&subbmp_pl_menu_list_load_up
};

static const struct subbitmap *menu_list_down_backgrounds[3] = {
	&subbmp_pl_menu_list_new_down,
	&subbmp_pl_menu_list_save_down,
	&subbmp_pl_menu_list_load_down
};

static struct menu_data data_menu_add = {
	.num_buttons = 3,
	.buttons_up = menu_add_up_backgrounds,
	.buttons_down = menu_add_down_backgrounds,
	.bar = &subbmp_pl_menu_add_bar
};

static struct menu_data data_menu_rem = {
	.num_buttons = 4,
	.buttons_up = menu_rem_up_backgrounds,
	.buttons_down = menu_rem_down_backgrounds,
	.bar = &subbmp_pl_menu_rem_bar
};

static struct menu_data data_menu_sel = {
	.num_buttons = 3,
	.buttons_up = menu_sel_up_backgrounds,
	.buttons_down = menu_sel_down_backgrounds,
	.bar = &subbmp_pl_menu_sel_bar
};

static struct menu_data data_menu_misc = {
	.num_buttons = 3,
	.buttons_up = menu_misc_up_backgrounds,
	.buttons_down = menu_misc_down_backgrounds,
	.bar = &subbmp_pl_menu_misc_bar
};

static struct menu_data data_menu_list = {
	.num_buttons = 3,
	.buttons_up = menu_list_up_backgrounds,
	.buttons_down = menu_list_down_backgrounds,
	.bar = &subbmp_pl_menu_list_bar
};

static struct button_data data_pl_previous = {
	.up = NULL,
	.down = NULL,
	.action = &test_button
};

static struct button_data data_pl_play = {
	.up = NULL,
	.down = NULL,
	.action = &test_button
};

static struct button_data data_pl_pause = {
	.up = NULL,
	.down = NULL,
	.action = &test_button
};

static struct button_data data_pl_stop = {
	.up = NULL,
	.down = NULL,
	.action = &test_button
};

static struct button_data data_pl_next = {
	.up = NULL,
	.down = NULL,
	.action = &test_button
};

static struct button_data data_pl_eject = {
	.up = NULL,
	.down = NULL,
	.action = &test_button
};

static struct grip_data data_pl_grip = {
	.min_width = 275,
	.min_height = 116
};

static void capture_mouse(window_t w, const struct widget *widget)
{
	gui->capture_mouse(w);
	capture = widget;
}

static void release_mouse(void)
{
	gui->release_mouse();
	capture = NULL;
}

static void draw_pixmap(const struct subbitmap *subbmp, int dst_x, int dst_y, int w, int h)
{
	int id;

	if (subbmp != NULL) {
		id = subbmp->bmp;
		gui->draw_image(bmps[id], dst_x, dst_y, w, h, subbmp->x, subbmp->y);
	}
}

static void draw_pixmap_double(const struct subbitmap *subbmp, int dst_x, int dst_y, int w, int h)
{
	int id;

	if (subbmp != NULL) {
		id = subbmp->bmp;
		if (double_size == false)
			gui->draw_image(bmps[id], dst_x, dst_y, w, h, subbmp->x, subbmp->y);
		else
			gui->draw_image_double(bmps[id], dst_x * 2, dst_y * 2, w, h, subbmp->x, subbmp->y);
	}
}

static void draw_pixmap_loop_horizontal(const struct subbitmap *subbmp, int dst_x, int dst_y, int src_w, int src_h, int dst_w)
{
	int i;

	for (i = 0; i < dst_w; i += src_w)
		draw_pixmap(subbmp, dst_x + i, dst_y, src_w, src_h);
}

static void draw_pixmap_loop_vertical(const struct subbitmap *subbmp, int dst_x, int dst_y, int src_w, int src_h, int dst_h)
{
	int i;

	for (i = 0; i < dst_h; i += src_h)
		draw_pixmap(subbmp, dst_x, dst_y + i, src_w, src_h);
}

static void background_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
	struct background_data *bg;

	TRACE();
	bg = (struct background_data *) widget->data;
	if (y < 16 || easymove) {
		capture_mouse(w, widget);
		last_x = x;
		last_y = y;
	}
}

static void background_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
	struct background_data *bg;

	TRACE();
	bg = (struct background_data *) widget->data;
	if (y < 16 || easymove) {
		release_mouse();
	}
}

static void background_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
	static int a, b;
	/*printf("%s: %d %d\n", __FUNCTION__, x, y);*/
	if (capture == widget) {
		if (gui->check_glue(w, w1, x - last_x, y - last_y) == false) {
			gui->move_window(w, x - last_x, y - last_y);
			a = x;
			b = y;
		}
		
		if (w == w1) {
			gui->move_window(w2, x - last_x, y - last_y);
			gui->move_window(w3, x - last_x, y - last_y);
		}
	}
}

static void background_draw(const struct widget *widget, window_t w)
{
	struct background_data *bg;
	const int *rect;

	TRACE();
	rect = widget->rect;
	bg = (struct background_data *) widget->data;
	draw_pixmap_double(bg->subbmp, rect[0], rect[1], rect[2], rect[3]);
}

static const struct handlers background_handlers = {
	.mouse_down = &background_mouse_down,
	.mouse_up = &background_mouse_up,
	.mouse_move = &background_mouse_move,
	.draw = &background_draw
};

static void checkbox_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
	struct checkbox_data *checkbox;

	TRACE();
	checkbox = (struct checkbox_data *) widget->data;
	capture_mouse(w, widget);
	gui->redraw_window(w);
}

static void checkbox_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
	struct checkbox_data *checkbox;
	const int *rect;

	TRACE();
	checkbox = (struct checkbox_data *) widget->data;
	if (capture == widget) {
		release_mouse();
		rect = widget->rect;
		if (x > rect[0] && x < rect[0] + rect[2] &&
			y > rect[1] && y < rect[1] + rect[3]) {
			checkbox->checked = !checkbox->checked;
			checkbox->action(checkbox->checked);
		}
		gui->redraw_window(w);
	}
}

static void checkbox_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
}

static void checkbox_draw(const struct widget *widget, window_t w)
{
	struct checkbox_data *checkbox;
	const struct subbitmap *subbmp;
	const int *rect;
	
	TRACE();
	rect = widget->rect;
	checkbox = (struct checkbox_data *) widget->data;
	if (checkbox->checked == true) {
		/*puts("checked=true");*/
		if (capture == widget)
			subbmp = checkbox->on_down;
		else
			subbmp = checkbox->on_up;
	} else {
		/*puts("checked=false");*/
		if (capture == widget)
			subbmp = checkbox->off_down;
		else
			subbmp = checkbox->off_up;
	}
	
	draw_pixmap_double(subbmp, rect[0], rect[1], rect[2], rect[3]);
}

static const struct handlers checkbox_handlers = {
	.mouse_down = &checkbox_mouse_down,
	.mouse_up = &checkbox_mouse_up,
	.mouse_move = &checkbox_mouse_move,
	.draw = &checkbox_draw
};

static void button_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
	struct button_data *button;

	TRACE();
	button = (struct button_data *) widget->data;
	capture_mouse(w, widget);
	gui->redraw_window(w);
}

static void button_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
	struct button_data *button;
	const int *rect;

	TRACE();
	button = (struct button_data *) widget->data;
	if (capture == widget) {
		release_mouse();
		gui->redraw_window(w);
		rect = widget->rect;
		if (x > rect[0] && x < rect[0] + rect[2] &&
			y > rect[1] && y < rect[1] + rect[3])
			button->action();
	}
}

static void button_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
	TRACE();
}

static void button_draw(const struct widget *widget, window_t w)
{
	const struct subbitmap *subbmp;
	struct button_data *button;
	const int *rect;

	TRACE();
	rect = widget->rect;
	button = (struct button_data *) widget->data;
	if (capture == widget)
		subbmp = button->down;
	else
		subbmp = button->up;
	draw_pixmap_double(subbmp, rect[0], rect[1], rect[2], rect[3]);
}

static const struct handlers button_handlers = {
	.mouse_down = &button_mouse_down,
	.mouse_up = &button_mouse_up,
	.mouse_move = &button_mouse_move,
	.draw = &button_draw
};

static void slider_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
	struct slider_data *slider;

	TRACE();
	slider = (struct slider_data *) widget->data;
	capture_mouse(w, widget);
	last_x = x;
	last_y = y;
	gui->redraw_window(w);
}

static void slider_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
	struct slider_data *slider;

	TRACE();
	slider = (struct slider_data *) widget->data;
	release_mouse();
	gui->redraw_window(w);
}

static void slider_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
	struct slider_data *slider;
	int diff;
	
	slider = (struct slider_data *) widget->data;
	if (capture == widget) {
		/* set value */
		if (slider->horizontal == true) {
			diff = x - last_x;
			slider->value += diff;
		} else {
			diff = y - last_y;
			slider->value -= diff;
		}

		/* check bounds */
		if (slider->value < slider->min) {
			slider->value = slider->min;
		} else if (slider->value > slider->max) {
			slider->value = slider->max;
		} else {
			/* prevent the slider from moving when cursor is not in slider range */
			last_x = x;
			last_y = y;
		}

		printf("value: %d\n", slider->value);
		gui->redraw_window(w);
	}
}

static void slider_draw(const struct widget *widget, window_t w)
{
	struct slider_data *slider;
	const struct subbitmap *bg;
	const struct subbitmap *bar;
	const int *rect;
	int n;
	int range;
	
	TRACE();
	rect = widget->rect;
	slider = (struct slider_data *) widget->data;
	/* select the background */
	/* there are 28 possible bg images */
	if (slider->horizontal == true) {
		range = slider->max - slider->min;
		n = slider->value * 28 / range;
		if (n > 27)
			n = 27;
	} else {
		range = slider->max - slider->min;
		n = slider->value * 28 / range;
		if (n > 27)
			n = 27;
	}
	bg = slider->background[n];
	if (capture == widget)
		bar = slider->down;
	else
		bar = slider->up;
	/* draw the background */
	draw_pixmap_double(bg, rect[0], rect[1], rect[2], rect[3]);
	/* draw the bar */
	if (slider->horizontal == true) {
		draw_pixmap_double(bar, rect[0] + slider->value + slider->min, rect[1] + 1, 14, 11);
	} else {
		draw_pixmap_double(bar, rect[0] + 1, rect[1] + rect[3] - 13 - slider->value, 11, 11);
	}
}

static const struct handlers slider_handlers = {
	.mouse_down = &slider_mouse_down,
	.mouse_up = &slider_mouse_up,
	.mouse_move = &slider_mouse_move,
	.draw = &slider_draw
};

static void clutterbar_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
	struct clutterbar_data *clutterbar;
	int widget_y;

	clutterbar = (struct clutterbar_data *) widget->data;
	capture_mouse(w, widget);
	last_x = x;
	last_y = y;
	widget_y = widget->rect[1];
	if (y > widget_y && y <= widget_y + 8)
		clutterbar->mouse_down = 1;
	else if (y > widget_y + 8 && y <= widget_y + 8*2)
		clutterbar->mouse_down = 2;
	else if (y > widget_y + 8*2 && y <= widget_y + 8*3)
		clutterbar->mouse_down = 3;
	else if (y > widget_y + 8*3 && y <= widget_y + 8*4)
		clutterbar->mouse_down = 4;
	else if (y > widget_y + 8*4 && y <= widget_y + 8*5)
		clutterbar->mouse_down = 5;
	gui->redraw_window(w);
}

static void clutterbar_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
	struct clutterbar_data *clutterbar;

	clutterbar = (struct clutterbar_data *) widget->data;
	release_mouse();
	switch (clutterbar->mouse_down) {
	case 1:
		clutterbar->set_o();
		break;
	case 2:
		clutterbar->a = !clutterbar->a;
		clutterbar->set_a(w, clutterbar->a);
		break;
	case 3:
		clutterbar->set_i();
		break;
	case 4:
		clutterbar->d = !clutterbar->d;
		clutterbar->set_d(clutterbar->d);
		break;
	case 5:
		clutterbar->set_v();
		break;
	}
	clutterbar->mouse_down = 0;
	gui->redraw_window(w);
}

static void clutterbar_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
}

static void clutterbar_draw(const struct widget *widget, window_t w)
{
	const struct subbitmap *bg;
	const int *rect;
	struct clutterbar_data *clutterbar;
	int mouse_down;

	TRACE();
	bg = &subbmp_clutterbar_off;
	rect = widget->rect;
	clutterbar = (struct clutterbar_data *) widget->data;
	mouse_down = clutterbar->mouse_down;
	if (clutterbar->a || clutterbar->d) {
		/* draw the inactive characters */
		bg = &subbmp_clutterbar_off;
		draw_pixmap_double(bg, rect[0], rect[1], rect[2], rect[3]);
		/* draw only the pressed character and the active characters */
		bg = &subbmp_clutterbar_a;
		if (clutterbar->a)
			draw_pixmap_double(bg, rect[0], 31, rect[2], 8);
		bg = &subbmp_clutterbar_d;
		if (clutterbar->d)
			draw_pixmap_double(bg, rect[0], 47, rect[2], 8);
		/*mouse_down--;*/
		switch (mouse_down) {
		case 0:
			bg = &subbmp_clutterbar_off;
			break;
		case 1:
			bg = &subbmp_clutterbar_off_o;
			break;
		case 2:
			bg = &subbmp_clutterbar_off_a;
			break;
		case 3:
			bg = &subbmp_clutterbar_off_i;
			break;
		case 4:
			bg = &subbmp_clutterbar_off_d;
			break;
		case 5:
			bg = &subbmp_clutterbar_off_v;
			break;
		}
		draw_pixmap_double(bg, rect[0], rect[1] + 8 * mouse_down, rect[2], 8);
	} else {
		switch (mouse_down) {
		case 0:
			bg = &subbmp_clutterbar_off;
			break;
		case 1:
			bg = &subbmp_clutterbar_off_o;
			break;
		case 2:
			bg = &subbmp_clutterbar_off_a;
			break;
		case 3:
			bg = &subbmp_clutterbar_off_i;
			break;
		case 4:
			bg = &subbmp_clutterbar_off_d;
			break;
		case 5:
			bg = &subbmp_clutterbar_off_v;
			break;
		}
		draw_pixmap_double(bg, rect[0], rect[1], rect[2], rect[3]);
	}
}

static const struct handlers clutterbar_handlers = {
	.mouse_down = &clutterbar_mouse_down,
	.mouse_up = &clutterbar_mouse_up,
	.mouse_move = &clutterbar_mouse_move,
	.draw = &clutterbar_draw
};

static void song_title_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
	struct song_title_data *song_title;

	song_title = (struct song_title_data *) widget->data;
	capture_mouse(w, widget);

	last_x = x;
}

static void song_title_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
	struct song_title_data *song_title;

	song_title = (struct song_title_data *) widget->data;
	release_mouse();
}

static void song_title_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
	struct song_title_data *song_title;

	song_title = (struct song_title_data *) widget->data;
	if (capture == widget) {
		song_title->offset = (last_x - x) / 5;
		/*if (song_title->offset < 0)
			song_title->offset = 0;
		else if (song_title->offset > 50)
			song_title->offset = 50;*/
		gui->redraw_window(w);
	}
}

static void song_title_draw(const struct widget *widget, window_t w)
{
	/*static void draw_string(int x, int y, int w, int offset)*/ /*FIXME: remove.*/
	struct song_title_data *song_title;
	
	int x, y, width;
	
	int a;
	int c;
	int i;
	int len;
	
	song_title = (struct song_title_data *) widget->data;
	
	x = widget->rect[0];
	y = widget->rect[1];
	width = widget->rect[2];
	
	len = strlen(song_title->string);
	a = song_title->offset % 5;
	/* TODO: draw the beginning and ending character pieces. */
	/*gui->draw_image(bmps[BMP_TEXT], x+a, y, w-a, 6, ascii_table[c]->x+a, ascii_table[c]->y);*/
	i = song_title->offset % 5;
	while (width > 5) {
		c = song_title->string[i];
		if (ascii_table[c] == NULL)
			c = ' ';
		draw_pixmap_double(ascii_table[c], x - a, y, 5, 6);
		i++;
		if (i >= len)
			i = 0;
		width -= 5;
		x += 5;
	}
}

static const struct handlers song_title_handlers = {
	.mouse_down = &song_title_mouse_down,
	.mouse_up = &song_title_mouse_up,
	.mouse_move = &song_title_mouse_move,
	.draw = &song_title_draw
};

static void resizeable_background_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
	TRACE();
	if (y < 16 || easymove) {
		capture_mouse(w, widget);
		last_x = x;
		last_y = y;
	}
}

static void resizeable_background_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
	TRACE();
	if (y < 16 || easymove)
		release_mouse();
}

static void resizeable_background_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
	if (capture == widget) {
		gui->move_window(w, x-last_x, y-last_y);
	}
}

static void resizeable_background_draw(const struct widget *widget, window_t w)
{
	struct resizeable_background_data *bg;
	const int *rect;

	TRACE();
	rect = widget->rect;
	bg = (struct resizeable_background_data *) widget->data;
	draw_pixmap(bg->top_left, 0, 0, 25, 20);
	draw_pixmap(bg->top_right, rect[2] - 25, 0, 25, 20);
	draw_pixmap_loop_horizontal(bg->top, 25, 0, 25, 20, rect[2]-50);
	draw_pixmap_loop_horizontal(bg->bottom, 0, rect[3] - bg->length_bottom, 25, bg->length_bottom, rect[2]);
	draw_pixmap_loop_vertical(bg->left, 0, 20, 25, 29, rect[3]-19*2);
	draw_pixmap_loop_vertical(bg->right, rect[2]-25, 20, 25, 29, rect[3]-19*2);
	draw_pixmap(bg->bottom_left, 0, rect[3] - bg->length_bottom, 125, bg->length_bottom);
	draw_pixmap(bg->bottom_right, rect[2] - 150, rect[3] - bg->length_bottom, 150, bg->length_bottom);
}

static const struct handlers resizeable_background_handlers = {
	.mouse_down = &resizeable_background_mouse_down,
	.mouse_up = &resizeable_background_mouse_up,
	.mouse_move = &resizeable_background_mouse_move,
	.draw = &resizeable_background_draw
};

static void scroll_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
}

static void scroll_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
}

static void scroll_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
}

static void scroll_draw(const struct widget *widget, window_t w)
{
	struct scroll_data *scroll;
	const int *rect;
	const struct subbitmap *bar;
	
	scroll = (struct scroll_data *) widget->data;
	rect = widget->rect;
	if (scroll->value >= 0 || 1) {
		/* the background is only drawn if the bar is being showed. */
		draw_pixmap_double(scroll->background, rect[0], rect[1], rect[2], rect[3]);
		if (capture == widget)
			bar = scroll->bar_down;
		else
			bar = scroll->bar_up;
		draw_pixmap_double(bar, rect[0] + scroll->value, rect[1], scroll->bar_length, rect[3]);
	}
}

static const struct handlers scroll_handlers = {
	.mouse_down = &scroll_mouse_down,
	.mouse_up = &scroll_mouse_up,
	.mouse_move = &scroll_mouse_move,
	.draw = &scroll_draw
};

static void pledit_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
}

static void pledit_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
}

static void pledit_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
}

static void pledit_draw(const struct widget *widget, window_t w)
{
	struct lpedit_data *pledit;
	const int *rect;
	char black[3] = {255, 0, 0};
	int line_max_chars;
	int num_lines;
	int i, j;
	int x, y;
	unsigned char c;
	char *title, *line;
	int num_len, title_len, time_len, line_len;
	char buffer[32];

	rect = widget->rect;
	/* draw background */
	gui->draw_filled_rectangle(rect[0], rect[1], rect[2], rect[3], black);
	/* draw text */
	line_max_chars = rect[2] / 5;
	num_lines = rect[3] / (6 + 1); /* consider line spacing */
	x = rect[0];
	y = rect[1];
	pledit = widget->data;
	for (i = 0; i < num_lines; i++) {
		num_len = sprintf(buffer, "%d", 10);
		time_len = sprintf(buffer, "%d:%2d", 10, 10);
		title = "mateus";
		title_len = strlen(title);
		line = title;
		line_len = strlen(line);
		for (j = 0; j < line_max_chars; j++) {
			if (j >= line_len)
				break;
			c = line[j];
			if (ascii_table[c]) /* TODO: put this condition in song title control. */
				gui->draw_image(bmps[BMP_TEXT], x, y, 5, 6, ascii_table[c]->x, ascii_table[c]->y);
			x += 5;
		}
		x = rect[0];
		y += 7;
	}
}

static const struct handlers pledit_handlers = {
	.mouse_down = &pledit_mouse_down,
	.mouse_up = &pledit_mouse_up,
	.mouse_move = &pledit_mouse_move,
	.draw = &pledit_draw
};

static void menu_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
	if (x > widget->rect[0] + 3) {
		capture_mouse(w, widget);
		gui->redraw_window(w);
	}
}

static void menu_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
	release_mouse();
	gui->redraw_window(w);
}

static void menu_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
	int id;
	struct menu_data *menu;
	const int *rect;

	menu = (struct menu_data *) widget->data;
	rect = widget->rect;
	if (capture == widget) {
		if (x > rect[0] + 3 && x < rect[0] + 3 + rect[2] &&
			y > rect[1] - menu->num_buttons * 18 && y < rect[1] + rect[3]) {
			menu->cur_button = (widget->rect[1] + widget->rect[3] - y) / 18;
			gui->redraw_window(w);
		} else {
			menu->cur_button = -1;
		}
	}
}

static void menu_draw(const struct widget *widget, window_t w)
{
	char color[3] = {255, 120, 120};
	const int *rect;
	struct menu_data *menu;
	int num_buttons;
	int cur_y;
	int i;

	rect = widget->rect;
	gui->draw_filled_rectangle(rect[0], rect[1], rect[2], rect[3], color);

	menu = (struct menu_data *) widget->data;
	if (capture == widget) {
		num_buttons = menu->num_buttons;
		cur_y = rect[1];
		for (i = 0; i < num_buttons; i++) {
			if (menu->cur_button != i)
				draw_pixmap(menu->buttons_up[i], rect[0]+3, cur_y, 25-3, 18);
			else
				draw_pixmap(menu->buttons_down[i], rect[0]+3, cur_y, 25-3, 18);
			cur_y -= 18;
		}
		draw_pixmap(menu->bar, rect[0], rect[1] - (num_buttons - 1) * 18, 3, 18 * num_buttons);
	}
}

static const struct handlers menu_handlers = {
	.mouse_down = &menu_mouse_down,
	.mouse_up = &menu_mouse_up,
	.mouse_move = &menu_mouse_move,
	.draw = &menu_draw
};

static void grip_mouse_down(const struct widget *widget, window_t w, int x, int y)
{
	struct grip_data *grip;

	grip = (struct grip_data *) widget->data;
	capture_mouse(w, widget);
	last_x = x;
	last_y = y;
}

static void grip_mouse_up(const struct widget *widget, window_t w, int x, int y)
{
	struct grip_data *grip;

	grip = (struct grip_data *) widget->data;
	release_mouse();
}

static void grip_mouse_move(const struct widget *widget, window_t w, int x, int y)
{
	struct grip_data *grip;
	int diff_x, diff_y;
	int step_x = 25, step_y = 29;
	int rect[4];
	int width, height;

	grip = (struct grip_data *) widget->data;
	if (capture == widget) {
		diff_x = x - last_x;
		diff_x /= step_x;
		last_x += diff_x * step_x;
		diff_y = y - last_y;
		diff_y /= step_y;
		last_y += diff_y * step_y;
		gui->get_window_rect(rect);
		if (rect[2] >= grip->min_width)
			width = rect[2];
		else
			width = grip->min_width;
		if (rect[3] >= grip->min_height)
			height = rect[3];
		else
			height = grip->min_height;
		gui->resize_window(w, width + diff_x * step_x, height + diff_y * step_y);
	}
}

static void grip_draw(const struct widget *widget, window_t w)
{
	char color[3] = {0, 255, 0};
	const int *rect;

	rect = widget->rect;
	gui->draw_filled_rectangle(rect[0], rect[1], rect[2], rect[3], color);
}

static const struct handlers grip_handlers = {
	.mouse_down = &grip_mouse_down,
	.mouse_up = &grip_mouse_up,
	.mouse_move = &grip_mouse_move,
	.draw = &grip_draw
};

static const struct widget main_template[] = {
	{{0, 0, 275, 116}, CURSOR_NORMAL, &background_handlers, &data_main},
	{{0, 0, 275, 14}, CURSOR_TITLEBAR, &background_handlers, &data_title_bar},
	{{212, 41, 29, 12}, CURSOR_NORMAL, &background_handlers, &data_mono},
	{{239, 41, 29, 12}, CURSOR_NORMAL, &background_handlers, &data_stereo},
	{{26, 28, 9, 9}, CURSOR_NORMAL, &background_handlers, &data_status},
	{{6, 3, 9, 9}, CURSOR_NORMAL, &button_handlers, &data_options},
	{{244, 3, 9, 9}, CURSOR_MIN, &button_handlers, &data_minimize},
	{{254, 3, 9, 9}, CURSOR_NORMAL, &button_handlers, &data_maximize},
	{{264, 3, 9, 9}, CURSOR_CLOSE, &button_handlers, &data_close},
	{{16+23*0, 88, 23, 18}, CURSOR_NORMAL, &button_handlers, &data_previous},
	{{16+23*1-1, 88, 23, 18}, CURSOR_NORMAL, &button_handlers, &data_play},
	{{16+23*2-1, 88, 23, 18}, CURSOR_NORMAL, &button_handlers, &data_pause},
	{{16+23*3-1, 88, 23, 18}, CURSOR_NORMAL, &button_handlers, &data_stop},
	{{16+23*4-1, 88, 23, 18}, CURSOR_NORMAL, &button_handlers, &data_next},
	{{136, 89, 22, 16}, CURSOR_NORMAL, &button_handlers, &data_eject},
	{{26, 28, 26+9, 28+9}, CURSOR_NORMAL, &background_handlers, 0},
	{{107, 57, 68, 14}, CURSOR_NORMAL, &slider_handlers, &data_volume},
	{{177, 57, 38, 14}, CURSOR_NORMAL, &slider_handlers, &data_balance},
	{{219, 58, 23, 12}, CURSOR_NORMAL, &checkbox_handlers, &data_eq},
	{{219+23, 58, 23, 12}, CURSOR_NORMAL, &checkbox_handlers, &data_pl},
	{{165, 89, 46, 15}, CURSOR_NORMAL, &checkbox_handlers, &data_shuffle},
	{{210, 89, 29, 15}, CURSOR_NORMAL, &checkbox_handlers, &data_repeat},
	{{10, 22, 8, 43}, CURSOR_NORMAL, &clutterbar_handlers, &data_clutterbar},
	{{112, 27, 152, 6}, CURSOR_SONGNAME, &song_title_handlers, &data_song_title},
	{{16, 72, 248, 10}, CURSOR_NORMAL, &scroll_handlers, &data_posbar}
};

static const struct widget equalizer_template[] = {
	/* background */
	{{0, 0, 275, 116}, CURSOR_NORMAL, &background_handlers, &data_eq_background},
	/* title bar */
	{{0, 0, 275, 14}, CURSOR_NORMAL, &background_handlers, &data_eq_title_bar},
	{{254, 3, 9, 9}, CURSOR_NORMAL, &button_handlers, &data_eq_maximize},
	{{264, 3, 9, 9}, CURSOR_CLOSE, &button_handlers, &data_eq_close},
	/* on auto buttons */
	{{14, 18, 26, 12}, CURSOR_NORMAL, &checkbox_handlers, &data_eq_on},
	{{40, 18, 32, 12}, CURSOR_NORMAL, &checkbox_handlers, &data_eq_auto},
	/* presets button */
	{{217, 18, 44, 12}, CURSOR_NORMAL, &button_handlers, &data_eq_presets},
	/* preamp */
	{{21, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_preamp},
	{{78+18*0, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider1},
	{{78+18*1, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider2},
	{{78+18*2, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider3},
	{{78+18*3, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider4},
	{{78+18*4, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider5},
	{{78+18*5, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider6},
	{{78+18*6, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider7},
	{{78+18*7, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider8},
	{{78+18*8, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider9},
	{{78+18*9, 38, 14, 64}, CURSOR_NORMAL, &slider_handlers, &data_eq_slider10}
};

static struct widget playlist_template[] = {
	{{0, 0, 275, 116}, CURSOR_NORMAL, &resizeable_background_handlers, &data_pl_background}, /* 0 */
	{{12, 20, 243, 58}, CURSOR_NORMAL, &pledit_handlers, &data_pledit}, /* 1 */
	/* menus */
	{{11, 86, 25, 18}, CURSOR_NORMAL, &menu_handlers, &data_menu_add}, /* 2 */
	{{40, 86, 25, 18}, CURSOR_NORMAL, &menu_handlers, &data_menu_rem}, /* 3 */
	{{69, 86, 25, 18}, CURSOR_NORMAL, &menu_handlers, &data_menu_sel}, /* 4 */
	{{98, 86, 25, 18}, CURSOR_NORMAL, &menu_handlers, &data_menu_misc}, /* 5 */
	{{228, 86, 25, 18}, CURSOR_NORMAL, &menu_handlers, &data_menu_list}, /* 6 */
	/* playback */
	{{0, 0, 0, 0}, CURSOR_NORMAL, &button_handlers, &data_pl_previous}, /* 7 */
	{{131, 101, 8, 8}, CURSOR_NORMAL, &button_handlers, &data_pl_play}, /* 8 */
	{{0, 0, 0, 0}, CURSOR_NORMAL, &button_handlers, &data_pl_pause}, /* 9 */
	{{0, 0, 0, 0}, CURSOR_NORMAL, &button_handlers, &data_pl_stop}, /* 10 */
	{{0, 0, 0, 0}, CURSOR_NORMAL, &button_handlers, &data_pl_next}, /* 11 */
	{{0, 0, 0, 0}, CURSOR_NORMAL, &button_handlers, &data_pl_eject}, /* 12 */
	
	{{256, 97, 19, 19}, CURSOR_NORMAL, &grip_handlers, &data_pl_grip}, /* 13 */
	{{254, 3, 9, 9}, CURSOR_NORMAL, &button_handlers, &data_pl_maximize}, /* 14 */
	{{264, 3, 9, 9}, CURSOR_CLOSE, &button_handlers, &data_pl_close}, /* 15 */
};

static int pl_anchor_bottom[] = {
	2, 3, 4, 5, 6, 12, 13, -1
};

static int pl_anchor_right[] = {
	6, 13, 14, 15, -1
};

static int pl_anchor_resize[] = {
	0, 1, -1
};

static const struct widget * collision_detection(int x, int y, const struct widget *template, int num_controls)
{
	int i;
	const int *rect;
	const struct widget *widget;

	widget = NULL;
	for (i = num_controls - 1; i >= 0; i--) {
		rect = template[i].rect;
		if (x >= rect[0] && y >= rect[1] &&
			x <= (rect[0] + rect[2]) && y <= (rect[1] + rect[3])) {
			widget = &template[i];
			break;
		}
	}
	return widget;
}

static void template_mouse_down(window_t w, const struct widget *template, int num_controls, int x, int y)
{
	const struct widget *widget;

	TRACE();
	printf("x=%d y=%d\n", x, y);
	widget = collision_detection(x, y, template, num_controls);
	if (widget != NULL)
		widget->control->mouse_down(widget, w, x, y);
}

static void template_mouse_up(window_t w, const struct widget *template, int num_controls, int x, int y)
{
	const struct widget *widget;

	TRACE();
	printf("x=%d y=%d\n", x, y);
	if (capture != NULL)
		widget = capture;
	else
		widget = collision_detection(x, y, template, num_controls);
	if (widget != NULL)
		widget->control->mouse_up(widget, w, x, y);
}

static void template_mouse_move(window_t w, const struct widget *template, int num_controls, int x, int y)
{
	const struct widget *widget;

	/*TRACE();*/
	if (capture != NULL)
		widget = capture;
	else
		widget = collision_detection(x, y, template, num_controls);
	if (widget != NULL) {
		gui->set_cursor(cursors[widget->cursor]);
		widget->control->mouse_move(widget, w, x, y);
	}
}

static void template_draw(window_t w, const struct widget *template, int num_controls)
{
	int i;
	const struct widget *widget;

	TRACE();
	widget = template;
	gui->begin_drawing(w);
	for (i = 0; i < num_controls; i++) {
		if (widget->data) /* FIXME: remove. */
		widget->control->draw(widget, w);
		widget++;
	}
	gui->end_drawing();
}

static void main_mouse_down(int x, int y)
{
	if (double_size) {
		x /= 2;
		y /= 2;
	}

	template_mouse_down(w1, main_template, NUM_CONTROLS_MAIN, x, y);
}

static void main_mouse_up(int x, int y)
{
	if (double_size) {
		x /= 2;
		y /= 2;
	}

	template_mouse_up(w1, main_template, NUM_CONTROLS_MAIN, x, y);
}

static void main_mouse_move(int x, int y)
{
	if (double_size) {
		x /= 2;
		y /= 2;
	}

	template_mouse_move(w1, main_template, NUM_CONTROLS_MAIN, x, y);
}

static void main_draw(void)
{
	template_draw(w1, main_template, NUM_CONTROLS_MAIN);
}

static void main_focus(bool focus)
{
	if (focus)
		data_title_bar.subbmp = &subbmp_title_bar_on;
	else
		data_title_bar.subbmp = &subbmp_title_bar_off;
	gui->redraw_window(w1);
}

static void equalizer_mouse_down(int x, int y)
{
	if (double_size) {
		x /= 2;
		y /= 2;
	}

	template_mouse_down(w2, equalizer_template, NUM_CONTROLS_EQUALIZER, x, y);
}

static void equalizer_mouse_up(int x, int y)
{
	if (double_size) {
		x /= 2;
		y /= 2;
	}

	template_mouse_up(w2, equalizer_template, NUM_CONTROLS_EQUALIZER, x, y);
}

static void equalizer_mouse_move(int x, int y)
{
	if (double_size) {
		x /= 2;
		y /= 2;
	}

	template_mouse_move(w2, equalizer_template, NUM_CONTROLS_EQUALIZER, x, y);
}

static void equalizer_draw(void)
{
	template_draw(w2, equalizer_template, NUM_CONTROLS_EQUALIZER);
}

static void equalizer_focus(bool focus)
{
	if (focus)
		data_eq_title_bar.subbmp = &subbmp_eq_title_bar_on;
	else
		data_eq_title_bar.subbmp = &subbmp_eq_title_bar_off;
	gui->redraw_window(w2);
}

static void resizeable_window_draw(window_t w, const struct widget *template, int num_controls)
{
	/*static void template_draw(window_t w, const struct widget *template, int num_controls)*/
	int i;
	const struct widget *widget;

	TRACE();
	widget = template;
	gui->begin_drawing(w);
	for (i = 0; i < num_controls; i++) {
		if (widget->data) /* FIXME: remove. */
		widget->control->draw(widget, w);
		widget++;
	}
	gui->end_drawing();
}

static void playlist_mouse_down(int x, int y)
{
	template_mouse_down(w3, playlist_template, NUM_CONTROLS_PLAYLIST, x, y);
}

static void playlist_mouse_up(int x, int y)
{
	template_mouse_up(w3, playlist_template, NUM_CONTROLS_PLAYLIST, x, y);
}

static void playlist_mouse_move(int x, int y)
{
	template_mouse_move(w3, playlist_template, NUM_CONTROLS_PLAYLIST, x, y);
}

static void playlist_draw(void)
{
	template_draw(w3, playlist_template, NUM_CONTROLS_PLAYLIST);
}

static void playlist_resize(int w, int h)
{
	int *rect;
	int i;
	int id;
	static int last_w = 275, last_h = 116;
	int diff;

	for (i = 0; (id = pl_anchor_right[i]) >= 0; i++) {
		rect = playlist_template[id].rect;
		diff = w - last_w;
		rect[0] += diff;
	}

	for (i = 0; (id = pl_anchor_bottom[i]) >= 0; i++) {
		rect = playlist_template[id].rect;
		diff = h - last_h;
		rect[1] += diff;
	}

	for (i = 0; (id = pl_anchor_resize[i]) >= 0; i++) {
		rect = playlist_template[id].rect;
		diff = w - last_w;
		rect[2] += diff;
		diff = h - last_h;
		rect[3] += diff;
	}

	last_w = w;
	last_h = h;

	gui->redraw_window(w3);
}

static void playlist_focus(bool focus)
{
	if (focus) {
		data_pl_background.top_left = &subbmp_pl_top_left_on;
		data_pl_background.title = &subbmp_pl_title_on;
		data_pl_background.top = &subbmp_pl_top_on;
		data_pl_background.top_right = &subbmp_pl_top_right_on;
	} else {
		data_pl_background.top_left = &subbmp_pl_top_left_off;
		data_pl_background.title = &subbmp_pl_title_off;
		data_pl_background.top = &subbmp_pl_top_off;
		data_pl_background.top_right = &subbmp_pl_top_right_off;
	}
	gui->redraw_window(w3);
}

static void video_draw(void)
{
}

static struct skin_callbacks main_callbacks = {
	.mouse_down = &main_mouse_down,
	.mouse_up = &main_mouse_up,
	.mouse_move = &main_mouse_move,
	.draw = &main_draw,
	.focus = &main_focus
};

static struct skin_callbacks equalizer_callbacks = {
	.mouse_down = &equalizer_mouse_down,
	.mouse_up = &equalizer_mouse_up,
	.mouse_move = &equalizer_mouse_move,
	.draw = &equalizer_draw,
	.focus = &equalizer_focus
};

static struct skin_callbacks playlist_callbacks = {
	.mouse_down = &playlist_mouse_down,
	.mouse_up = &playlist_mouse_up,
	.mouse_move = &playlist_mouse_move,
	.draw = &playlist_draw,
	.resize = &playlist_resize,
	.focus = &playlist_focus
};

static void classic_init(void)
{
	int i;

	w1 = gui->create_window(0, 0, 275, 116, "Main", &main_callbacks);
	main_window = w1;
	w2 = gui->create_window(0, 116, 275, 116, "Equalizer", &equalizer_callbacks);
	w3 = gui->create_window(0, 116*2, 275, 116, "Playlist", &playlist_callbacks);
	for (i = 0; i < NUM_BMPS; i++)
		bmps[i] = gui->load_image(bmp_filenames[i]);
	for (i = 0; i < NUM_CURSORS; i++)
		cursors[i] = gui->load_cursor(cursor_filenames[i]);
	gui->show_window(w1);
	gui->show_window(w2);
	gui->show_window(w3);
}

static void classic_quit(void)
{
	int i;

	for (i=0; i < NUM_BMPS; i++)
		gui->unload_image(bmps[i]);
	for (i=0; i < NUM_CURSORS; i++)
		gui->unload_cursor(cursors[i]);
	/* FIXME: window is destroyed twice. */
	gui->destroy_window(w1);
}

static void classic_set_title(char *title)
{
}

static void classic_set_time(int minutes, int seconds)
{
}

static void classic_set_bitrate(int value)
{
}

static void classic_set_freq(int value)
{
}

struct skin classic = {
	.init = &classic_init,
	.quit = &classic_quit,
	.set_title = &classic_set_title,
	.set_time = &classic_set_time,
	.set_bitrate = &classic_set_bitrate,
	.set_freq = &classic_set_freq
};
