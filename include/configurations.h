/*
 *  yummy/plugins/include/configurations.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

/* these configs are shared between classic and modern skins. */
struct configs_s
{
	int utf8;
	int skin;
	int config_extlist;
	int newverchk2;
	int defext;
	int titlefmt;
	int proxy80;
	int proxy;
	int visplugin_name;
	int dspplugin_name;
	int check_ft_startup;
	int updated_ft_startup;
	int visplugin_num;
	int pe_fontsize;
	int pe_direction;
	int visplugin_priority;
	int visplugin_autoexec;
	int dspplugin_num;
	int sticon;
	int splash;
	int taskbar;
	int dropaotfs;
	int ascb_new;
	int ttips;
	int riol;
	int minst;
	int whichicon;
	int whichicon2;
	int addtolist;
	int snap;
	int snaplen;
	int parent;
	int hilete;
	int disvis;
	int rofiob;
	int shownumsinpl;
	int keeponscreen;
	int eqdsize;
	int usecursors;
	int fixtitles;
	int priority;
	int shuffle_morph_rate;
	int useexttitles;
	int bifont;
	int dotitlenum;
	int dotasknum;
	int plscrollsize;
	int plmw2xscroll;
	int inet_mode;
	int ospb;
	int embedwnd_freesize;
	int no_visseh;
	int newverchk;
	int newverchk3;
	int prefs_last_page;
	int autoload_eq;
	int playlist_recyclebin;
	int use_eq;
	int eq_ws;
	int wx;
	int wy;
	int minimized;
	int aot;
	int shuffle;
	int repeat;
	int volume;
	int pan;
	int easymove;
	int dsize;
	int timeleftmode;
	int autoscrollname;
	int sa;
	int safire;
	int saref;
	int safalloff;
	int sa_peaks;
	int sa_peak_falloff;
	int eq_wx;
	int eq_wy;
	int eq_open;
	int mw_open;
	int pe_wx;
	int pe_wy;
	int pe_open;
	int pe_width;
	int pe_height;
	int pe_height_ws;
	int eq_limiter;
	int eq_type;
	int eq_frequencies;
	int video_wx;
	int video_wy;
	int video_open;
	int video_width;
	int video_height;
	int video_ratio1;
	int video_ratio2;
	int video_useratio;
	int windowshade;
	int preamp;
	int pilp;
	int randskin;
	int cwd;
	int outname;
	int pladv;
	int eq_data;
	int video_vsync;
	int video_aspectadj;
	int video_overlays;
	int video_gdiplus;
	int video_ddraw;
	int video_updsize;
	int video_autoopen;
	int video_autoclose;
	int video_noss;
	int video_osd;
	int video_yv12;
	int video_stopclose;
	int video_auto_fs;
	int playback_thread_priority;
	int audio_bits;
	int audio_mono;
	int audio_surround;
	int audio_dither;
	int replaygain;
	int replaygain_mode;
	int replaygain_source;
	int replaygain_preferred_only;
	int replaygain_non_rg_gain;
	int replaygain_preamp;
	int video_fliprgb;
	int video_remove_fs_on_stop;
	int wav_do_header;
	int wav_convert;
	int wav_ext;
	int playlist_custom_font;
	int custom_plfont;
	int no_registry;
	int last_classic_skin_page;
	int last_playback_page;
	int zeropadplnum;
	int accessibility_modalbeep;
	int accessibility_modalflash;
	int noml_ratings_prompt;
	int noml_ratings;
	int whtsnewlp;
};

extern struct config config;

void load_config_file(char *path);

#ifdef DEBUG
void debug_check_config_length();
#endif

#endif
