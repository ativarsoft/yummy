/*
 *  yummy/player/configurations.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <windows.h>
#ifdef DEBUG
#include <stdio.h>
#endif
#include <zlib.h>
#include "configurations.h"

#define CONFIG_BUFFER_SIZE 1024*32-1

#define CONFIG_UNKNOWN 0
#define CONFIG_BOOL 1

struct configs_s configs;

/* not keyboard keys. */
struct config_key_mapping_s
{
	int name_len; /* fast writing */
	int crc32; /* fast reading */
	char *name;
	int type;
	void *value;
};

/* every config value can be of a different type */
const struct config_key_mapping_s keys[] = 
{
	{4, 0x35f83b81, "utf8", CONFIG_UNKNOWN, &configs.utf8}, 
	{4, 0x279681e, "skin", CONFIG_UNKNOWN, &configs.skin}, 
	{14, 0xbf51906b, "config_extlist", CONFIG_UNKNOWN, &configs.config_extlist}, 
	{10, 0xe9dfe919, "newverchk2", CONFIG_UNKNOWN, &configs.newverchk2}, 
	{6, 0x93aaf31d, "defext", CONFIG_UNKNOWN, &configs.defext}, 
	{8, 0x58fe8cc2, "titlefmt", CONFIG_UNKNOWN, &configs.titlefmt}, 
	{7, 0xe86114f8, "proxy80", CONFIG_UNKNOWN, &configs.proxy80}, 
	{5, 0x7372c9be, "proxy", CONFIG_UNKNOWN, &configs.proxy}, 
	{14, 0xa5b305af, "visplugin_name", CONFIG_UNKNOWN, &configs.visplugin_name}, 
	{14, 0xdba9ef62, "dspplugin_name", CONFIG_UNKNOWN, &configs.dspplugin_name}, 
	{16, 0xfc16c578, "check_ft_startup", CONFIG_UNKNOWN, &configs.check_ft_startup}, 
	{18, 0x9cd70810, "updated_ft_startup", CONFIG_UNKNOWN, &configs.updated_ft_startup}, 
	{13, 0x98746331, "visplugin_num", CONFIG_UNKNOWN, &configs.visplugin_num}, 
	{11, 0x8cd2e4ef, "pe_fontsize", CONFIG_UNKNOWN, &configs.pe_fontsize}, 
	{12, 0xee406539, "pe_direction", CONFIG_UNKNOWN, &configs.pe_direction}, 
	{18, 0x514fac97, "visplugin_priority", CONFIG_UNKNOWN, &configs.visplugin_priority}, 
	{18, 0xe520cee2, "visplugin_autoexec", CONFIG_UNKNOWN, &configs.visplugin_autoexec}, 
	{13, 0x33e2133c, "dspplugin_num", CONFIG_UNKNOWN, &configs.dspplugin_num}, 
	{6, 0x74f66e04, "sticon", CONFIG_UNKNOWN, &configs.sticon}, 
	{6, 0xba3814f0, "splash", CONFIG_UNKNOWN, &configs.splash}, 
	{7, 0x9d08481c, "taskbar", CONFIG_UNKNOWN, &configs.taskbar}, 
	{9, 0x116bf6a5, "dropaotfs", CONFIG_UNKNOWN, &configs.dropaotfs}, 
	{8, 0xcebd6e80, "ascb_new", CONFIG_UNKNOWN, &configs.ascb_new}, 
	{5, 0x8f182f06, "ttips", CONFIG_UNKNOWN, &configs.ttips}, 
	{4, 0x1151dbf, "riol", CONFIG_UNKNOWN, &configs.riol}, 
	{5, 0xf09d8552, "minst", CONFIG_UNKNOWN, &configs.minst}, 
	{9, 0xc19bf35, "whichicon", CONFIG_UNKNOWN, &configs.whichicon}, 
	{10, 0x4c6a6391, "whichicon2", CONFIG_UNKNOWN, &configs.whichicon2}, 
	{9, 0x6814cfb1, "addtolist", CONFIG_UNKNOWN, &configs.addtolist}, 
	{4, 0x36641d9e, "snap", CONFIG_UNKNOWN, &configs.snap}, 
	{7, 0x4e32647a, "snaplen", CONFIG_UNKNOWN, &configs.snaplen}, 
	{6, 0x3d8e604f, "parent", CONFIG_UNKNOWN, &configs.parent}, 
	{6, 0x882051fd, "hilete", CONFIG_UNKNOWN, &configs.hilete}, 
	{6, 0xe240122b, "disvis", CONFIG_UNKNOWN, &configs.disvis}, 
	{6, 0xf4d8c299, "rofiob", CONFIG_UNKNOWN, &configs.rofiob}, 
	{12, 0x684dd24f, "shownumsinpl", CONFIG_UNKNOWN, &configs.shownumsinpl}, 
	{12, 0x78377dce, "keeponscreen", CONFIG_UNKNOWN, &configs.keeponscreen}, 
	{7, 0x3eaa48a3, "eqdsize", CONFIG_UNKNOWN, &configs.eqdsize}, 
	{10, 0x2e68b65d, "usecursors", CONFIG_UNKNOWN, &configs.usecursors}, 
	{9, 0xa9d4295c, "fixtitles", CONFIG_UNKNOWN, &configs.fixtitles}, 
	{8, 0x62a6dc27, "priority", CONFIG_UNKNOWN, &configs.priority}, 
	{18, 0xaa884652, "shuffle_morph_rate", CONFIG_UNKNOWN, &configs.shuffle_morph_rate}, 
	{12, 0x96d391bb, "useexttitles", CONFIG_UNKNOWN, &configs.useexttitles}, 
	{6, 0x910ccd00, "bifont", CONFIG_UNKNOWN, &configs.bifont}, 
	{10, 0x6de3b083, "dotitlenum", CONFIG_UNKNOWN, &configs.dotitlenum}, 
	{9, 0x4495c2b7, "dotasknum", CONFIG_UNKNOWN, &configs.dotasknum}, 
	{12, 0x1b3ac464, "plscrollsize", CONFIG_UNKNOWN, &configs.plscrollsize}, 
	{12, 0xf13037d4, "plmw2xscroll", CONFIG_UNKNOWN, &configs.plmw2xscroll}, 
	{9, 0x96587de0, "inet_mode", CONFIG_UNKNOWN, &configs.inet_mode}, 
	{4, 0x983597c2, "ospb", CONFIG_UNKNOWN, &configs.ospb}, 
	{17, 0xc7a82a6b, "embedwnd_freesize", CONFIG_UNKNOWN, &configs.embedwnd_freesize}, 
	{9, 0x937931c2, "no_visseh", CONFIG_UNKNOWN, &configs.no_visseh}, 
	{9, 0xb3265c12, "newverchk", CONFIG_UNKNOWN, &configs.newverchk}, 
	{10, 0x9ed8d98f, "newverchk3", CONFIG_UNKNOWN, &configs.newverchk3}, 
	{15, 0x45ca81e3, "prefs_last_page", CONFIG_UNKNOWN, &configs.prefs_last_page}, 
	{11, 0xb3f38474, "autoload_eq", CONFIG_UNKNOWN, &configs.autoload_eq}, 
	{19, 0x93e275c, "playlist_recyclebin", CONFIG_UNKNOWN, &configs.playlist_recyclebin}, 
	{6, 0xcf173a, "use_eq", CONFIG_UNKNOWN, &configs.use_eq}, 
	{5, 0x8ae16ee3, "eq_ws", CONFIG_UNKNOWN, &configs.eq_ws}, 
	{2, 0x7f7904c0, "wx", CONFIG_UNKNOWN, &configs.wx}, 
	{2, 0x87e3456, "wy", CONFIG_UNKNOWN, &configs.wy}, 
	{9, 0x5c4c7cb7, "minimized", CONFIG_UNKNOWN, &configs.minimized}, 
	{3, 0x359ba48, "aot", CONFIG_UNKNOWN, &configs.aot}, 
	{7, 0x33fe5971, "shuffle", CONFIG_UNKNOWN, &configs.shuffle}, 
	{6, 0xa857b3c0, "repeat", CONFIG_UNKNOWN, &configs.repeat}, 
	{6, 0xb99acdde, "volume", CONFIG_UNKNOWN, &configs.volume}, 
	{3, 0x7d5ca7fb, "pan", CONFIG_UNKNOWN, &configs.pan}, 
	{8, 0x67e62352, "easymove", CONFIG_UNKNOWN, &configs.easymove}, 
	{5, 0x7c14dde6, "dsize", CONFIG_UNKNOWN, &configs.dsize}, 
	{12, 0x7f567b3b, "timeleftmode", CONFIG_UNKNOWN, &configs.timeleftmode}, 
	{14, 0x345add16, "autoscrollname", CONFIG_UNKNOWN, &configs.autoscrollname}, 
	{2, 0x7f7e6904, "sa", CONFIG_UNKNOWN, &configs.sa}, 
	{6, 0xe1bc56e2, "safire", CONFIG_UNKNOWN, &configs.safire}, 
	{5, 0x10e41ad5, "saref", CONFIG_UNKNOWN, &configs.saref}, 
	{9, 0xe167b81d, "safalloff", CONFIG_UNKNOWN, &configs.safalloff}, 
	{8, 0x64de606a, "sa_peaks", CONFIG_UNKNOWN, &configs.sa_peaks}, 
	{15, 0x4407e67c, "sa_peak_falloff", CONFIG_UNKNOWN, &configs.sa_peak_falloff}, 
	{5, 0x1d33b76b, "eq_wx", CONFIG_UNKNOWN, &configs.eq_wx}, 
	{5, 0x6a3487fd, "eq_wy", CONFIG_UNKNOWN, &configs.eq_wy}, 
	{7, 0xbbeb66fa, "eq_open", CONFIG_UNKNOWN, &configs.eq_open}, 
	{7, 0x5e5dcb80, "mw_open", CONFIG_UNKNOWN, &configs.mw_open}, 
	{5, 0x6a486f51, "pe_wx", CONFIG_UNKNOWN, &configs.pe_wx}, 
	{5, 0x1d4f5fc7, "pe_wy", CONFIG_UNKNOWN, &configs.pe_wy}, 
	{7, 0x164a951e, "pe_open", CONFIG_UNKNOWN, &configs.pe_open}, 
	{8, 0xa71c25ab, "pe_width", CONFIG_UNKNOWN, &configs.pe_width}, 
	{9, 0x1fb3a456, "pe_height", CONFIG_UNKNOWN, &configs.pe_height}, 
	{12, 0xc1e1b6ee, "pe_height_ws", CONFIG_UNKNOWN, &configs.pe_height_ws}, 
	{10, 0xe82adb6, "eq_limiter", CONFIG_UNKNOWN, &configs.eq_limiter}, 
	{7, 0x9345b277, "eq_type", CONFIG_UNKNOWN, &configs.eq_type}, 
	{14, 0xb3e31bab, "eq_frequencies", CONFIG_UNKNOWN, &configs.eq_frequencies}, 
	{8, 0xe98163de, "video_wx", CONFIG_UNKNOWN, &configs.video_wx}, 
	{8, 0x9e865348, "video_wy", CONFIG_UNKNOWN, &configs.video_wy}, 
	{10, 0xa3e7de78, "video_open", CONFIG_UNKNOWN, &configs.video_open}, 
	{11, 0x3784c8d, "video_width", CONFIG_UNKNOWN, &configs.video_width}, 
	{12, 0xcd1a45c2, "video_height", CONFIG_UNKNOWN, &configs.video_height}, 
	{12, 0x7bcd436f, "video_ratio1", CONFIG_UNKNOWN, &configs.video_ratio1}, 
	{12, 0xe2c412d5, "video_ratio2", CONFIG_UNKNOWN, &configs.video_ratio2}, 
	{14, 0x96eb691c, "video_useratio", CONFIG_UNKNOWN, &configs.video_useratio}, 
	{11, 0xf09f3b4, "windowshade", CONFIG_UNKNOWN, &configs.windowshade}, 
	{6, 0xfc49dd3d, "preamp", CONFIG_UNKNOWN, &configs.preamp}, 
	{4, 0x9430dab8, "pilp", CONFIG_UNKNOWN, &configs.pilp}, 
	{8, 0xdbe70670, "randskin", CONFIG_UNKNOWN, &configs.randskin}, 
	{3, 0x9f71e61b, "cwd", CONFIG_UNKNOWN, &configs.cwd}, 
	{7, 0xc8489815, "outname", CONFIG_UNKNOWN, &configs.outname}, 
	{5, 0xbfe284d4, "pladv", CONFIG_UNKNOWN, &configs.pladv}, 
	{7, 0xb268163d, "eq_data", CONFIG_UNKNOWN, &configs.eq_data}, 
	{11, 0x32bdc1b6, "video_vsync", CONFIG_UNKNOWN, &configs.video_vsync}, 
	{15, 0x26cc99c9, "video_aspectadj", CONFIG_UNKNOWN, &configs.video_aspectadj}, 
	{14, 0x9fec9c00, "video_overlays", CONFIG_UNKNOWN, &configs.video_overlays}, 
	{13, 0x80612325, "video_gdiplus", CONFIG_UNKNOWN, &configs.video_gdiplus}, 
	{11, 0x74444d21, "video_ddraw", CONFIG_UNKNOWN, &configs.video_ddraw}, 
	{13, 0x77c5ee65, "video_updsize", CONFIG_UNKNOWN, &configs.video_updsize}, 
	{14, 0x6154418f, "video_autoopen", CONFIG_UNKNOWN, &configs.video_autoopen}, 
	{15, 0xbf785c46, "video_autoclose", CONFIG_UNKNOWN, &configs.video_autoclose}, 
	{10, 0x73bf845e, "video_noss", CONFIG_UNKNOWN, &configs.video_noss}, 
	{9, 0x89e1f7f, "video_osd", CONFIG_UNKNOWN, &configs.video_osd}, 
	{10, 0x6e122f06, "video_yv12", CONFIG_UNKNOWN, &configs.video_yv12}, 
	{15, 0x66bd4e55, "video_stopclose", CONFIG_UNKNOWN, &configs.video_stopclose}, 
	{13, 0xfcf3c980, "video_auto_fs", CONFIG_UNKNOWN, &configs.video_auto_fs}, 
	{24, 0xaa879ab7, "playback_thread_priority", CONFIG_UNKNOWN, &configs.playback_thread_priority}, 
	{10, 0x3a839013, "audio_bits", CONFIG_UNKNOWN, &configs.audio_bits}, 
	{10, 0xc2415a63, "audio_mono", CONFIG_UNKNOWN, &configs.audio_mono}, 
	{14, 0x33d12e83, "audio_surround", CONFIG_UNKNOWN, &configs.audio_surround}, 
	{12, 0xb092dad4, "audio_dither", CONFIG_UNKNOWN, &configs.audio_dither}, 
	{10, 0x213fe264, "replaygain", CONFIG_UNKNOWN, &configs.replaygain}, 
	{15, 0x7983d8c0, "replaygain_mode", CONFIG_UNKNOWN, &configs.replaygain_mode}, 
	{17, 0xb933ad83, "replaygain_source", CONFIG_UNKNOWN, &configs.replaygain_source}, 
	{25, 0x86511a18, "replaygain_preferred_only", CONFIG_UNKNOWN, &configs.replaygain_preferred_only}, 
	{22, 0x72380c2f, "replaygain_non_rg_gain", CONFIG_UNKNOWN, &configs.replaygain_non_rg_gain}, 
	{17, 0x1af00fcd, "replaygain_preamp", CONFIG_UNKNOWN, &configs.replaygain_preamp}, 
	{13, 0xceb974a7, "video_fliprgb", CONFIG_UNKNOWN, &configs.video_fliprgb}, 
	{23, 0x685ec401, "video_remove_fs_on_stop", CONFIG_UNKNOWN, &configs.video_remove_fs_on_stop}, 
	{13, 0x27e5abcf, "wav_do_header", CONFIG_UNKNOWN, &configs.wav_do_header}, 
	{11, 0xa2d3b8e6, "wav_convert", CONFIG_UNKNOWN, &configs.wav_convert}, 
	{7, 0x6c65a8af, "wav_ext", CONFIG_UNKNOWN, &configs.wav_ext}, 
	{20, 0xf42f4600, "playlist_custom_font", CONFIG_UNKNOWN, &configs.playlist_custom_font}, 
	{13, 0x235f35e4, "custom_plfont", CONFIG_UNKNOWN, &configs.custom_plfont}, 
	{11, 0x4ca81feb, "no_registry", CONFIG_UNKNOWN, &configs.no_registry}, 
	{22, 0x673c48dd, "last_classic_skin_page", CONFIG_UNKNOWN, &configs.last_classic_skin_page}, 
	{18, 0x56650e72, "last_playback_page", CONFIG_UNKNOWN, &configs.last_playback_page}, 
	{12, 0x5c99cbd5, "zeropadplnum", CONFIG_UNKNOWN, &configs.zeropadplnum}, 
	{23, 0xffdead0c, "accessibility_modalbeep", CONFIG_UNKNOWN, &configs.accessibility_modalbeep}, 
	{24, 0x3c8e8634, "accessibility_modalflash", CONFIG_UNKNOWN, &configs.accessibility_modalflash}, 
	{19, 0x1696d81c, "noml_ratings_prompt", CONFIG_UNKNOWN, &configs.noml_ratings_prompt}, 
	{12, 0x3808df2a, "noml_ratings", CONFIG_UNKNOWN, &configs.noml_ratings}, 
	{9, 0x65d6fb4, "whtsnewlp", CONFIG_UNKNOWN, &configs.whtsnewlp}, 
};

void load_config(char *path)
{
	char *fullpath;
	char *buffer;
	int len;
	fullpath=malloc(4096);
	GetFullPathName(path, 4096, fullpath, 0);
	buffer=malloc(CONFIG_BUFFER_SIZE); /* 32K */
	if(!buffer)
		error(0, 0, "ENOMEM");
	if(!GetPrivateProfileSection("Winamp", buffer, CONFIG_BUFFER_SIZE, fullpath))
		error(0, 0, "cannot read \"%s\"", path);
	len=strlen(buffer)+1;
	/*printf("%s\n", buffer);*/
	/*while(*buffer)
	{
	}*/
}

void save_config(char *path)
{
}

#ifdef DEBUG
void debug_check_config_length()
{
	int num_keys;
	int i;
	int len;
	int crc;
	char *type;
	
	printf("sizeof(keys)==%d\n", sizeof(keys));
	num_keys=sizeof(keys)/sizeof(struct config_key_mapping_s);
	printf("num_keys==%d\n", num_keys);
	for(i=0; i<num_keys; i++)
	{
		len=strlen(keys[i].name);
		crc=crc32(0, 0, 0);
		crc=crc32(crc, keys[i].name, len);
		switch(keys[i].type)
		{
			case CONFIG_UNKNOWN:
			type="CONFIG_UNKNOWN";
			break;
		}
		printf("\t{%d, %#x, \"%s\", %s, &configs.%s}, \n", 
			len, crc, keys[i].name, type, keys[i].name);
	}
	fflush(stdout);
}
#endif
