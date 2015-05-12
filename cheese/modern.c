/*
 *  yummy/cheese/modern.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <skin.h>
#include <expat.h>
#include <stddef.h>
#include <stdio.h> /* TODO: change it to ops. */
#include <stdbool.h>
#include <string.h>

#include <gui.h>
#include <path.h>
#include <cheese/script.h>

#define BUFFER_SIZE 4096

#define STRING(a) {sizeof(a), a}

typedef void (*start_handler)(void);
typedef void (*text_handler)(void);

typedef int xml_int;
typedef int xml_bool;
typedef char * xml_string;
typedef float xml_float;
typedef char * xml_id;

enum elements {
	EL_ACCELERATOR,
	EL_ACCELERATORS,
	EL_ALBUMART,
	EL_ANIMATEDLAYER,
	EL_AUTHOR,
	EL_BITMAP,
	EL_BITMAPFONT,
	EL_BUTTON,
	EL_COLOR,
	EL_COLORTHEMES_LIST,
	EL_COLORTHEMES_MGR,
	EL_COMMENT,
	EL_COMPONENT,
	EL_COMPONENTBUCKET,
	EL_CONTAINER,
	EL_ELEMENTALIAS,
	EL_ELEMENTS,
	EL_EMAIL,
	EL_GAMMAGROUP,
	EL_GAMMASET,
	EL_GRID,
	EL_GROUP,
	EL_GROUPDEF,
	EL_GROUPXFADE,
	EL_GUIOBJECT,
	EL_HIDEOBJECT,
	EL_HOMEPAGE,
	EL_INCLUDE,
	EL_LAYER,
	EL_LAYOUT,
	EL_LAYOUTSTATUS,
	EL_MENU,
	EL_MENU_BUTTON_HOVER,
	EL_MENU_BUTTON_NORMAL,
	EL_MENU_BUTTON_PRESSED,
	EL_NAME,
	EL_NSTATESBUTTON,
	EL_PROGRESSGRID,
	EL_RECT,
	EL_ROOT,
	EL_SENDPARAMS,
	EL_SCREENSHOT,
	EL_SCRIPT,
	EL_SKININFO,
	EL_SLIDER,
	EL_SONGTICKER,
	EL_STATUS,
	EL_TEXT,
	EL_TOGGLEBUTTON,
	EL_VERSION,
	EL_VIS,
	EL_WASABI_MAINFRAME_NOSTATUS,
	EL_WASABI_MEDIALIBRARYFRAME_NOSTATUS,
	EL_WASABI_PLAYLISTFRAME_NOSTATUS,
	EL_WASABI_STANDARDFRAME_NOSTATUS,
	EL_WASABI_TITLEBAR,
	EL_WASABI_VISFRAME_NOSTATUS,
	EL_WASABIXML,
	EL_WINDOWHOLDER,
	NUM_ELEMENTS
};

enum xml_type {
	XML_ID, 
	XML_INT,
	XML_STRING,
	XML_FLOAT,
	XML_BOOL
};

struct skininfo {
	char *version;
	char *name;
	char *comment;
	char *author;
	char *email;
	char *homepage;
	char *screenshot;
};

struct image_entry {
	struct image_entry *next;

	char *path;
};

struct string {
	int length;
	char *data;
};

struct element_handler {
	const struct string name;
	const start_handler start;
	const text_handler text;
	const int *valid_children;
	const struct xml_attribute *attr_table;
};

struct xml_file {
	XML_Parser p; /* for creating a child parser context */
	const char *path; /* for getting the path relative to the parent xml */
};

struct context {
	struct skininfo *info;
	unsigned int element_stack[16];
	int cur_element;
	struct image_entry *images;
	struct xml_file xml_stack[16];
	int cur_xml;
};

struct xml_attribute {
	struct string name;
	enum xml_type type;
	unsigned long offset;
};

struct attr_wasabixml {
	xml_int version;
};

struct attr_container {
	xml_id id;
	xml_string name;
	xml_int default_x;
	xml_int default_y;
	xml_int default_w;
	xml_int default_h;
	xml_bool default_visible;
	xml_string component;
	xml_bool dynamic;
	xml_string primarycomponent;
	xml_bool canclose;
	xml_bool nofocusapponclose;
};

struct attr_layout {
	xml_id id;
	xml_string owner;
	xml_string lockto;
	xml_int alpha;
	xml_bool desktopalpha;
	xml_bool indesktop;
	xml_string linkwidth;
	xml_string linkheight;
	xml_bool taskbar;
	xml_bool move;
	xml_bool resizable;
	xml_bool scalable;
	xml_bool ontop;
};

struct attr_groupdef {
	xml_string background;
	xml_bool drawbackground;
	xml_int default_w;
	xml_int default_h;
	xml_int maximum_w;
	xml_int maximum_h;
	xml_int minimum_w;
	xml_int minimum_h;
	xml_bool propagatesize;
	xml_bool lockminmax;
	xml_int design_w;
	xml_int design_h;
	xml_string name;
	xml_string autowidthsource;
	xml_string autoheightsource;
	xml_bool register_autopopup;
	xml_string windowtype;
	xml_bool inherit_content;
	xml_string inherit_group;
	xml_bool inherit_params;
	xml_string xuitag;
	xml_string embed_xui;
};

struct attr_bitmap {
	xml_string id;
	xml_string file;
	xml_int x;
	xml_int y;
	xml_int w;
	xml_int h;
	xml_id gammagroup;
};

struct attr_bitmapfont {
	xml_string id;
	xml_string file;
	xml_int charwidth;
	xml_int charheight;
	xml_int hspacing;
	xml_int vspacing;
};

struct attr_truetypefont {
	xml_string id;
	xml_string file;
};

struct attr_color {
	xml_string id;
	xml_string value;
	xml_string gammagroup;
};

struct attr_elementalias {
	xml_string id;
	xml_string target;
};

struct attr_cursor {
	xml_string bitmap;
	xml_int hotspot_x;
	xml_int hotspot_y;
};

struct attr_gui_object {
	xml_int alpha;
	xml_int activealpha;
	xml_int inactivealpha;
	xml_id cursor;
	xml_string tooltip;
	xml_bool move;
	xml_bool renderbasetexture;
	xml_string cfgattrib;
	xml_bool visible;
	xml_int x;
	xml_int y;
	xml_int w;
	xml_int h;
	xml_int relatx;
	xml_int relaty;
	xml_int relatw;
	xml_int relath;
	xml_bool fitparent;
	xml_int x1;
	xml_int y1;
	xml_int x2;
	xml_int y2;
	xml_string anchor;
	xml_bool sysmetricsx;
	xml_bool sysmetricsy;
	xml_bool sysmetricsw;
	xml_bool sysmetricsh;
	xml_bool rectrgn;
	xml_int regionop;
	xml_int sysregion;
	xml_bool wantfocus;
	xml_bool focusonclick;
	xml_int taborder;
	xml_bool nodblclick;
	xml_bool noleftclick;
	xml_bool norightclick;
	xml_bool nomousemove;
	xml_bool nocontextmenu;
	xml_bool ghost;
	xml_string notify0;
	xml_string notify1;
	xml_string notify2;
	xml_string notify3;
	xml_string notify4;
	xml_string notify5;
	xml_string notify6;
	xml_string notify7;
	xml_string notify8;
	xml_string notify9;
	xml_id droptarget;
};

struct attr_button {
	xml_id image;
	xml_id downimage;
	xml_id hoverimage;
	xml_id activeimage;
	xml_string text;
	xml_string action;
	xml_string param;
	xml_bool borders;
	xml_string style;
	xml_int retcode;
	xml_id cbtarget;
	xml_id action_target;
	xml_bool center_image;
};

struct attr_nstatesbutton {
	xml_int nstates;
	xml_string cfgvals;
};

struct attr_images {
	xml_id images;
	xml_id source;
	xml_int imagesspacing;
};

struct attr_playlistdirectory {
};

struct attr_albumart {
	xml_id notfoundImage;
	xml_string align;
	xml_string valign;
	xml_string source;
	xml_bool stretched;
};

struct attr_winamp_browser {
	xml_string home;
};

struct attr_browser {
	xml_string url;
	xml_bool mainmb;
	xml_string targetname;
	xml_string scrollbars;
};

struct attr_wasabi_frame {
	xml_string orientation;
	xml_id left;
	xml_id right;
	xml_id top;
	xml_id bottom;
	xml_string from;
	xml_int width;
	xml_int height;
	xml_bool resizable;
	xml_int maxwidth;
	xml_int minwidth;
	xml_string vgrabber;
	xml_string vbitmap;
};

struct attr_wasabi_checkbox {
	xml_string text;
	xml_id radioid;
	xml_int radioval;
	xml_string action;
	xml_string param;
	xml_id action_target;
};

struct attr_wasabi_dropdownlist {
	xml_string items;
	xml_id feed;
	xml_int select;
	xml_int listheight;
	xml_int maxitems;
};

struct attr_wasabi_combobox {
	xml_string items;
	xml_id feed;
	xml_int select;
	xml_int listheight;
	xml_int maxitems;
};

struct attr_wasabi_historyeditbox {
	xml_bool navbuttons;
};

struct attr_componentbucket {
	xml_int leftmargin;
	xml_int rightmargin;
	xml_int spacing;
	xml_bool vertical;
};

struct attr_customobject {

};

struct attr_edit {
	xml_string text;
	xml_string action;
	xml_bool multiline;
	xml_bool vscroll;
	xml_bool autohscroll;
	xml_bool autoenter;
	xml_bool password;
};

struct attr_grid {
	xml_id topleft;
	xml_id top;
	xml_id topright;
	xml_id left;
	xml_id middle;
	xml_id right;
	xml_id bottomleft;
	xml_id bottom;
	xml_id bottomright;
};

struct attr_group {
	xml_id id;
	xml_string instance_id;
};

struct attr_grouplist {

};

struct attr_layer {
	xml_id image;
	xml_id inactiveimage;
	xml_id region;
	xml_bool tile;
	xml_string resize;
	xml_string scale;
	xml_string dblclickaction;
};

struct attr_animatedlayer {
	xml_int frameheight;
	xml_int framewidth;
	xml_int elementframes;
	xml_int start;
	xml_int end;
	xml_int speed;
	xml_bool realtime;
	xml_bool autoreplay;
	xml_bool autoplay;
	xml_bool debug;
};

struct attr_layoutstatus {
	xml_string exclude;
	xml_string include_only;
};

struct attr_list {
	xml_string items;
	xml_bool multiselect;
	xml_bool autodeselect;
	xml_string select;
	xml_bool antialias;
	xml_id feed;
	xml_bool hoverselect;
	xml_bool sort;
	xml_bool selectonupdown;
	xml_int numcolumns;
	xml_int columwidths;
	xml_string columnlabels;
};

struct attr_mouseredir {
	xml_id target;
};

struct attr_wasabi_radiogroup {

};

struct attr_rect {
	xml_id color;
	xml_bool filled;
	xml_string edges;
};

struct attr_script {
	xml_string file;
	xml_string param;
};

struct attr_slider {
	xml_id barleft;
	xml_id barmiddle;
	xml_id barright;
	xml_id thumb;
	xml_id downthumb;
	xml_id hoverthumb;
	xml_string orientation;
	xml_int low;
	xml_int high;
};

struct attr_snappoint {
	xml_id id;
	xml_int x;
	xml_int y;
	xml_int relatx;
	xml_int relaty;
};

struct attr_wasabi_tabsheet {
	xml_string windowtype;
	xml_id children;
	xml_int type;
	xml_int content_margin_top;
	xml_int content_margin_left;
	xml_int content_margin_right;
	xml_int content_margin_bottom;
};

struct attr_text {
	xml_string display;
	xml_bool ticker;
	xml_bool antialias;
	xml_string text;
	xml_string _default;
	xml_id font;
	xml_int fontsize;
	xml_string align;
	xml_string valign;
	xml_int color;
	xml_int shadowcolor;
	xml_int shadowx;
	xml_int shadowy;
	xml_int timeroffstyle;
	xml_int timecolonwidth;
	xml_bool nograb;
	xml_bool showlen;
	xml_bool forcefixed;
	xml_bool forceupcase;
	xml_bool forceuppercase;
	xml_bool forcelocase;
	xml_bool forcelowercase;
	xml_bool bold;
	xml_bool wrap;
	xml_string dblclickaction;
	xml_int offsetx;
	xml_int offsety;
};

struct attr_wasabi_titlebox {
	xml_string title;
	xml_id content;
	xml_bool centered;
	xml_string suffix;
};

struct attr_wasabi_titlebar {

};

struct attr_tree {
	xml_string items;
	xml_string feed;
	xml_bool sorted;
	xml_bool childtabs;
	xml_bool expandroot;
};

struct attr_vis {
	xml_int colorallbands;
	xml_int colorband1;
	xml_int colorband2;
	xml_int colorband3;
	xml_int colorband4;
	xml_int colorband5;
	xml_int colorband6;
	xml_int colorband7;
	xml_int colorband8;
	xml_int colorband9;
	xml_int colorband10;
	xml_int colorband11;
	xml_int colorband12;
	xml_int colorband13;
	xml_int colorband14;
	xml_int colorband15;
	xml_int colorband16;
	xml_int colorbandpeak;
	xml_int colorallosc;
	xml_int colorosc1;
	xml_int colorosc2;
	xml_int colorosc3;
	xml_int colorosc4;
	xml_int colorosc5;
	xml_int channel;
	xml_bool fliph;
	xml_bool flipv;
	xml_int mode;
	xml_bool peaks;
	xml_int peakfalloff;
	xml_int falloff;
	xml_string coloring;
	xml_string bandwidth;
	xml_string oscstyle;
};

struct attr_eqvis {
	xml_string colortop;
	xml_string colormiddle;
	xml_string colorbottom;
	xml_string colorpreamp;
};

struct attr_gradient {
	xml_float gradient_x1;
	xml_float gradient_y1;
	xml_float gradient_x2;
	xml_float gradient_y2;
	xml_string points;
};

struct attr_wasabi_standardframe_status {
	xml_id content;
	xml_int padtitleleft;
	xml_int padtitleright;
	xml_id shade;
};

struct attr_wasabi_standardframe_nostatus {
	xml_id content;
	xml_int padtitleleft;
	xml_int padtitleright;
	xml_id shade;
};

struct attr_wasabi_standardframe_modal {
	xml_id content;
	xml_int padtitleleft;
	xml_int padtitleright;
	xml_id shade;
};

struct attr_wasabi_standardframe_static {
	xml_id content;
	xml_int padtitleleft;
	xml_int padtitleright;
	xml_id shade;
};

struct attr_wndholder {
	xml_id hold;
	xml_id component;
	xml_id param;
	xml_bool noshowcmdbar;
	xml_bool noanimatedrects;
	xml_bool disableanimatedrects;
	xml_bool autoopen;
	xml_bool autoclose;
};

struct attr_objdirview {
	xml_string dir;
	xml_id target;
	xml_id displaytarget;
	xml_id defaultdisplay;
};

struct attr_wasabi_pathpicker {

};

struct attr_querydrag {
	xml_id image;
	xml_id source;
};

struct attr_queryline {
	xml_id querylist;
	xml_string query;
	xml_bool _auto;
};

struct attr_queryresults {
};

struct attr_include {
	xml_string file;
};

union {
	struct attr_bitmap bitmap;
	struct attr_include include;
	struct attr_container container;
	struct attr_script script;
} attributes;

/*static const struct xml_attribute container[] = {
	{STRING("id"), XML_ID, offsetof(struct xml_container, id)},
	{STRING("name"), XML_STRING, offsetof(struct xml_container, name)},
	{STRING("default_x"), XML_INT, offsetof(struct xml_container, default_x)},
	{STRING("default_y"), XML_INT, offsetof(struct xml_container, default_y)},
	{STRING("default_w"), XML_INT, offsetof(struct xml_container, default_w)},
	{STRING("default_h"), XML_INT, offsetof(struct xml_container, default_h)},
	NULL
};

static const struct xml_attribute bitmap[] = {
	{STRING("id"), XML_STRING, offsetof(struct xml_bitmap, id)},
	{STRING("file"), XML_STRING, offsetof(struct xml_bitmap, file)},
	{STRING("x"), XML_INT, offsetof(struct xml_bitmap, x)},
	{STRING("y"), XML_INT, offsetof(struct xml_bitmap, y)},
	{STRING("w"), XML_INT, offsetof(struct xml_bitmap, w)},
	{STRING("h"), XML_INT, offsetof(struct xml_bitmap, h)},
	{STRING("gammagroup"), XML_ID, offsetof(struct xml_bitmap, gammagroup)},
	NULL
};

static const struct xml_attribute layer[] = {
	{STRING("x"), XML_INT, offsetof(struct xml_layer, x)},
	{STRING("y"), XML_INT, offsetof(struct xml_layer, y)},
	NULL
};*/

static const struct xml_attribute attr_table_include[] = {
	{STRING("file"), XML_STRING, offsetof(struct attr_include, file)},
	0
};

static const struct xml_attribute attr_table_wasabixml[] = {
	{STRING("version"), XML_INT, offsetof(struct attr_wasabixml, version)},
	0
};

static const struct xml_attribute attr_table_container[] = {
	{STRING("id"), XML_ID, offsetof(struct attr_container, id)},
	{STRING("name"), XML_STRING, offsetof(struct attr_container, name)},
	{STRING("default_x"), XML_INT, offsetof(struct attr_container, default_x)},
	{STRING("default_y"), XML_INT, offsetof(struct attr_container, default_y)},
	{STRING("default_w"), XML_INT, offsetof(struct attr_container, default_w)},
	{STRING("default_h"), XML_INT, offsetof(struct attr_container, default_h)},
	{STRING("default_visible"), XML_BOOL, offsetof(struct attr_container, default_visible)},
	{STRING("component"), XML_STRING, offsetof(struct attr_container, component)},
	{STRING("dynamic"), XML_BOOL, offsetof(struct attr_container, dynamic)},
	{STRING("primarycomponent"), XML_STRING, offsetof(struct attr_container, primarycomponent)},
	{STRING("canclose"), XML_BOOL, offsetof(struct attr_container, canclose)},
	{STRING("nofocusapponclose"), XML_BOOL, offsetof(struct attr_container, nofocusapponclose)},
	0
};

static const struct xml_attribute attr_table_layout[] = {
	{STRING("id"), XML_ID, offsetof(struct attr_layout, id)},
	{STRING("owner"), XML_STRING, offsetof(struct attr_layout, owner)},
	{STRING("lockto"), XML_STRING, offsetof(struct attr_layout, lockto)},
	{STRING("alpha"), XML_INT, offsetof(struct attr_layout, alpha)},
	{STRING("desktopalpha"), XML_BOOL, offsetof(struct attr_layout, desktopalpha)},
	{STRING("indesktop"), XML_BOOL, offsetof(struct attr_layout, indesktop)},
	{STRING("linkwidth"), XML_STRING, offsetof(struct attr_layout, linkwidth)},
	{STRING("linkheight"), XML_STRING, offsetof(struct attr_layout, linkheight)},
	{STRING("taskbar"), XML_BOOL, offsetof(struct attr_layout, taskbar)},
	{STRING("move"), XML_BOOL, offsetof(struct attr_layout, move)},
	{STRING("resizable"), XML_BOOL, offsetof(struct attr_layout, resizable)},
	{STRING("scalable"), XML_BOOL, offsetof(struct attr_layout, scalable)},
	{STRING("ontop"), XML_BOOL, offsetof(struct attr_layout, ontop)},
	0
};

static const struct xml_attribute attr_table_groupdef[] = {
	{STRING("background"), XML_STRING, offsetof(struct attr_groupdef, background)},
	{STRING("drawbackground"), XML_BOOL, offsetof(struct attr_groupdef, drawbackground)},
	{STRING("default_w"), XML_INT, offsetof(struct attr_groupdef, default_w)},
	{STRING("default_h"), XML_INT, offsetof(struct attr_groupdef, default_h)},
	{STRING("maximum_w"), XML_INT, offsetof(struct attr_groupdef, maximum_w)},
	{STRING("maximum_h"), XML_INT, offsetof(struct attr_groupdef, maximum_h)},
	{STRING("minimum_w"), XML_INT, offsetof(struct attr_groupdef, minimum_w)},
	{STRING("minimum_h"), XML_INT, offsetof(struct attr_groupdef, minimum_h)},
	{STRING("propagatesize"), XML_BOOL, offsetof(struct attr_groupdef, propagatesize)},
	{STRING("lockminmax"), XML_BOOL, offsetof(struct attr_groupdef, lockminmax)},
	{STRING("design_w"), XML_INT, offsetof(struct attr_groupdef, design_w)},
	{STRING("design_h"), XML_INT, offsetof(struct attr_groupdef, design_h)},
	{STRING("name"), XML_STRING, offsetof(struct attr_groupdef, name)},
	{STRING("autowidthsource"), XML_STRING, offsetof(struct attr_groupdef, autowidthsource)},
	{STRING("autoheightsource"), XML_STRING, offsetof(struct attr_groupdef, autoheightsource)},
	{STRING("register_autopopup"), XML_BOOL, offsetof(struct attr_groupdef, register_autopopup)},
	{STRING("windowtype"), XML_STRING, offsetof(struct attr_groupdef, windowtype)},
	{STRING("inherit_content"), XML_BOOL, offsetof(struct attr_groupdef, inherit_content)},
	{STRING("inherit_group"), XML_STRING, offsetof(struct attr_groupdef, inherit_group)},
	{STRING("inherit_params"), XML_BOOL, offsetof(struct attr_groupdef, inherit_params)},
	{STRING("xuitag"), XML_STRING, offsetof(struct attr_groupdef, xuitag)},
	{STRING("embed_xui"), XML_STRING, offsetof(struct attr_groupdef, embed_xui)},
	0
};

static const struct xml_attribute attr_table_bitmap[] = {
	{STRING("id"), XML_STRING, offsetof(struct attr_bitmap, id)},
	{STRING("file"), XML_STRING, offsetof(struct attr_bitmap, file)},
	{STRING("x"), XML_INT, offsetof(struct attr_bitmap, x)},
	{STRING("y"), XML_INT, offsetof(struct attr_bitmap, y)},
	{STRING("w"), XML_INT, offsetof(struct attr_bitmap, w)},
	{STRING("h"), XML_INT, offsetof(struct attr_bitmap, h)},
	{STRING("gammagroup"), XML_ID, offsetof(struct attr_bitmap, gammagroup)},
	0
};

static const struct xml_attribute attr_table_bitmapfont[] = {
	{STRING("id"), XML_STRING, offsetof(struct attr_bitmapfont, id)},
	{STRING("file"), XML_STRING, offsetof(struct attr_bitmapfont, file)},
	{STRING("charwidth"), XML_INT, offsetof(struct attr_bitmapfont, charwidth)},
	{STRING("charheight"), XML_INT, offsetof(struct attr_bitmapfont, charheight)},
	{STRING("hspacing"), XML_INT, offsetof(struct attr_bitmapfont, hspacing)},
	{STRING("vspacing"), XML_INT, offsetof(struct attr_bitmapfont, vspacing)},
	0
};

static const struct xml_attribute attr_table_truetypefont[] = {
	{STRING("id"), XML_STRING, offsetof(struct attr_truetypefont, id)},
	{STRING("file"), XML_STRING, offsetof(struct attr_truetypefont, file)},
	0
};

static const struct xml_attribute attr_table_color[] = {
	{STRING("id"), XML_STRING, offsetof(struct attr_color, id)},
	{STRING("value"), XML_STRING, offsetof(struct attr_color, value)},
	{STRING("gammagroup"), XML_STRING, offsetof(struct attr_color, gammagroup)},
	0
};

static const struct xml_attribute attr_table_elementalias[] = {
	{STRING("id"), XML_STRING, offsetof(struct attr_elementalias, id)},
	{STRING("target"), XML_STRING, offsetof(struct attr_elementalias, target)},
	0
};

static const struct xml_attribute attr_table_cursor[] = {
	{STRING("bitmap"), XML_STRING, offsetof(struct attr_cursor, bitmap)},
	{STRING("hotspot_x"), XML_INT, offsetof(struct attr_cursor, hotspot_x)},
	{STRING("hotspot_y"), XML_INT, offsetof(struct attr_cursor, hotspot_y)},
	0
};

static const struct xml_attribute attr_table_gui_object[] = {
	{STRING("alpha"), XML_INT, offsetof(struct attr_gui_object, alpha)},
	{STRING("activealpha"), XML_INT, offsetof(struct attr_gui_object, activealpha)},
	{STRING("inactivealpha"), XML_INT, offsetof(struct attr_gui_object, inactivealpha)},
	{STRING("cursor"), XML_ID, offsetof(struct attr_gui_object, cursor)},
	{STRING("tooltip"), XML_STRING, offsetof(struct attr_gui_object, tooltip)},
	{STRING("move"), XML_BOOL, offsetof(struct attr_gui_object, move)},
	{STRING("renderbasetexture"), XML_BOOL, offsetof(struct attr_gui_object, renderbasetexture)},
	{STRING("cfgattrib"), XML_STRING, offsetof(struct attr_gui_object, cfgattrib)},
	{STRING("visible"), XML_BOOL, offsetof(struct attr_gui_object, visible)},
	{STRING("x"), XML_INT, offsetof(struct attr_gui_object, x)},
	{STRING("y"), XML_INT, offsetof(struct attr_gui_object, y)},
	{STRING("w"), XML_INT, offsetof(struct attr_gui_object, w)},
	{STRING("h"), XML_INT, offsetof(struct attr_gui_object, h)},
	{STRING("relatx"), XML_INT, offsetof(struct attr_gui_object, relatx)},
	{STRING("relaty"), XML_INT, offsetof(struct attr_gui_object, relaty)},
	{STRING("relatw"), XML_INT, offsetof(struct attr_gui_object, relatw)},
	{STRING("relath"), XML_INT, offsetof(struct attr_gui_object, relath)},
	{STRING("fitparent"), XML_BOOL, offsetof(struct attr_gui_object, fitparent)},
	{STRING("x1"), XML_INT, offsetof(struct attr_gui_object, x1)},
	{STRING("y1"), XML_INT, offsetof(struct attr_gui_object, y1)},
	{STRING("x2"), XML_INT, offsetof(struct attr_gui_object, x2)},
	{STRING("y2"), XML_INT, offsetof(struct attr_gui_object, y2)},
	{STRING("anchor"), XML_STRING, offsetof(struct attr_gui_object, anchor)},
	{STRING("sysmetricsx"), XML_BOOL, offsetof(struct attr_gui_object, sysmetricsx)},
	{STRING("sysmetricsy"), XML_BOOL, offsetof(struct attr_gui_object, sysmetricsy)},
	{STRING("sysmetricsw"), XML_BOOL, offsetof(struct attr_gui_object, sysmetricsw)},
	{STRING("sysmetricsh"), XML_BOOL, offsetof(struct attr_gui_object, sysmetricsh)},
	{STRING("rectrgn"), XML_BOOL, offsetof(struct attr_gui_object, rectrgn)},
	{STRING("regionop"), XML_INT, offsetof(struct attr_gui_object, regionop)},
	{STRING("sysregion"), XML_INT, offsetof(struct attr_gui_object, sysregion)},
	{STRING("wantfocus"), XML_BOOL, offsetof(struct attr_gui_object, wantfocus)},
	{STRING("focusonclick"), XML_BOOL, offsetof(struct attr_gui_object, focusonclick)},
	{STRING("taborder"), XML_INT, offsetof(struct attr_gui_object, taborder)},
	{STRING("nodblclick"), XML_BOOL, offsetof(struct attr_gui_object, nodblclick)},
	{STRING("noleftclick"), XML_BOOL, offsetof(struct attr_gui_object, noleftclick)},
	{STRING("norightclick"), XML_BOOL, offsetof(struct attr_gui_object, norightclick)},
	{STRING("nomousemove"), XML_BOOL, offsetof(struct attr_gui_object, nomousemove)},
	{STRING("nocontextmenu"), XML_BOOL, offsetof(struct attr_gui_object, nocontextmenu)},
	{STRING("ghost"), XML_BOOL, offsetof(struct attr_gui_object, ghost)},
	{STRING("notify0"), XML_STRING, offsetof(struct attr_gui_object, notify0)},
	{STRING("notify1"), XML_STRING, offsetof(struct attr_gui_object, notify1)},
	{STRING("notify2"), XML_STRING, offsetof(struct attr_gui_object, notify2)},
	{STRING("notify3"), XML_STRING, offsetof(struct attr_gui_object, notify3)},
	{STRING("notify4"), XML_STRING, offsetof(struct attr_gui_object, notify4)},
	{STRING("notify5"), XML_STRING, offsetof(struct attr_gui_object, notify5)},
	{STRING("notify6"), XML_STRING, offsetof(struct attr_gui_object, notify6)},
	{STRING("notify7"), XML_STRING, offsetof(struct attr_gui_object, notify7)},
	{STRING("notify8"), XML_STRING, offsetof(struct attr_gui_object, notify8)},
	{STRING("notify9"), XML_STRING, offsetof(struct attr_gui_object, notify9)},
	{STRING("droptarget"), XML_ID, offsetof(struct attr_gui_object, droptarget)},
	0
};

static const struct xml_attribute attr_table_button[] = {
	{STRING("image"), XML_ID, offsetof(struct attr_button, image)},
	{STRING("downimage"), XML_ID, offsetof(struct attr_button, downimage)},
	{STRING("hoverimage"), XML_ID, offsetof(struct attr_button, hoverimage)},
	{STRING("activeimage"), XML_ID, offsetof(struct attr_button, activeimage)},
	{STRING("text"), XML_STRING, offsetof(struct attr_button, text)},
	{STRING("action"), XML_STRING, offsetof(struct attr_button, action)},
	{STRING("param"), XML_STRING, offsetof(struct attr_button, param)},
	{STRING("borders"), XML_BOOL, offsetof(struct attr_button, borders)},
	{STRING("style"), XML_STRING, offsetof(struct attr_button, style)},
	{STRING("retcode"), XML_INT, offsetof(struct attr_button, retcode)},
	{STRING("cbtarget"), XML_ID, offsetof(struct attr_button, cbtarget)},
	{STRING("action_target"), XML_ID, offsetof(struct attr_button, action_target)},
	{STRING("center_image"), XML_BOOL, offsetof(struct attr_button, center_image)},
	0
};

static const struct xml_attribute attr_table_nstatesbutton[] = {
	{STRING("nstates"), XML_INT, offsetof(struct attr_nstatesbutton, nstates)},
	{STRING("cfgvals"), XML_STRING, offsetof(struct attr_nstatesbutton, cfgvals)},
	0
};

static const struct xml_attribute attr_table_images[] = {
	{STRING("images"), XML_ID, offsetof(struct attr_images, images)},
	{STRING("source"), XML_ID, offsetof(struct attr_images, source)},
	{STRING("imagesspacing"), XML_INT, offsetof(struct attr_images, imagesspacing)},
	0
};

static const struct xml_attribute attr_table_playlistdirectory[] = {
	0
};

static const struct xml_attribute attr_table_albumart[] = {
	{STRING("notfoundImage"), XML_ID, offsetof(struct attr_albumart, notfoundImage)},
	{STRING("align"), XML_STRING, offsetof(struct attr_albumart, align)},
	{STRING("valign"), XML_STRING, offsetof(struct attr_albumart, valign)},
	{STRING("source"), XML_STRING, offsetof(struct attr_albumart, source)},
	{STRING("stretched"), XML_BOOL, offsetof(struct attr_albumart, stretched)},
	0
};

static const struct xml_attribute attr_table_winamp_browser[] = {
	{STRING("home"), XML_STRING, offsetof(struct attr_winamp_browser, home)},
	0
};

static const struct xml_attribute attr_table_browser[] = {
	{STRING("url"), XML_STRING, offsetof(struct attr_browser, url)},
	{STRING("mainmb"), XML_BOOL, offsetof(struct attr_browser, mainmb)},
	{STRING("targetname"), XML_STRING, offsetof(struct attr_browser, targetname)},
	{STRING("scrollbars"), XML_STRING, offsetof(struct attr_browser, scrollbars)},
	0
};

static const struct xml_attribute attr_table_wasabi_frame[] = {
	{STRING("orientation"), XML_STRING, offsetof(struct attr_wasabi_frame, orientation)},
	{STRING("left"), XML_ID, offsetof(struct attr_wasabi_frame, left)},
	{STRING("right"), XML_ID, offsetof(struct attr_wasabi_frame, right)},
	{STRING("top"), XML_ID, offsetof(struct attr_wasabi_frame, top)},
	{STRING("bottom"), XML_ID, offsetof(struct attr_wasabi_frame, bottom)},
	{STRING("from"), XML_STRING, offsetof(struct attr_wasabi_frame, from)},
	{STRING("width"), XML_INT, offsetof(struct attr_wasabi_frame, width)},
	{STRING("height"), XML_INT, offsetof(struct attr_wasabi_frame, height)},
	{STRING("resizable"), XML_BOOL, offsetof(struct attr_wasabi_frame, resizable)},
	{STRING("maxwidth"), XML_INT, offsetof(struct attr_wasabi_frame, maxwidth)},
	{STRING("minwidth"), XML_INT, offsetof(struct attr_wasabi_frame, minwidth)},
	{STRING("vgrabber"), XML_STRING, offsetof(struct attr_wasabi_frame, vgrabber)},
	{STRING("vbitmap"), XML_STRING, offsetof(struct attr_wasabi_frame, vbitmap)},
	0
};

static const struct xml_attribute attr_table_wasabi_checkbox[] = {
	{STRING("text"), XML_STRING, offsetof(struct attr_wasabi_checkbox, text)},
	{STRING("radioid"), XML_ID, offsetof(struct attr_wasabi_checkbox, radioid)},
	{STRING("radioval"), XML_INT, offsetof(struct attr_wasabi_checkbox, radioval)},
	{STRING("action"), XML_STRING, offsetof(struct attr_wasabi_checkbox, action)},
	{STRING("param"), XML_STRING, offsetof(struct attr_wasabi_checkbox, param)},
	{STRING("action_target"), XML_ID, offsetof(struct attr_wasabi_checkbox, action_target)},
	0
};

static const struct xml_attribute attr_table_wasabi_dropdownlist[] = {
	{STRING("items"), XML_STRING, offsetof(struct attr_wasabi_dropdownlist, items)},
	{STRING("feed"), XML_ID, offsetof(struct attr_wasabi_dropdownlist, feed)},
	{STRING("select"), XML_INT, offsetof(struct attr_wasabi_dropdownlist, select)},
	{STRING("listheight"), XML_INT, offsetof(struct attr_wasabi_dropdownlist, listheight)},
	{STRING("maxitems"), XML_INT, offsetof(struct attr_wasabi_dropdownlist, maxitems)},
	0
};

static const struct xml_attribute attr_table_wasabi_combobox[] = {
	{STRING("items"), XML_STRING, offsetof(struct attr_wasabi_combobox, items)},
	{STRING("feed"), XML_ID, offsetof(struct attr_wasabi_combobox, feed)},
	{STRING("select"), XML_INT, offsetof(struct attr_wasabi_combobox, select)},
	{STRING("listheight"), XML_INT, offsetof(struct attr_wasabi_combobox, listheight)},
	{STRING("maxitems"), XML_INT, offsetof(struct attr_wasabi_combobox, maxitems)},
	0
};

static const struct xml_attribute attr_table_wasabi_historyeditbox[] = {
	{STRING("navbuttons"), XML_BOOL, offsetof(struct attr_wasabi_historyeditbox, navbuttons)},
	0
};

static const struct xml_attribute attr_table_componentbucket[] = {
	{STRING("leftmargin"), XML_INT, offsetof(struct attr_componentbucket, leftmargin)},
	{STRING("rightmargin"), XML_INT, offsetof(struct attr_componentbucket, rightmargin)},
	{STRING("spacing"), XML_INT, offsetof(struct attr_componentbucket, spacing)},
	{STRING("vertical"), XML_BOOL, offsetof(struct attr_componentbucket, vertical)},
	0
};

static const struct xml_attribute attr_table_customobject[] = {
	0
};

static const struct xml_attribute attr_table_edit[] = {
	{STRING("text"), XML_STRING, offsetof(struct attr_edit, text)},
	{STRING("action"), XML_STRING, offsetof(struct attr_edit, action)},
	{STRING("multiline"), XML_BOOL, offsetof(struct attr_edit, multiline)},
	{STRING("vscroll"), XML_BOOL, offsetof(struct attr_edit, vscroll)},
	{STRING("autohscroll"), XML_BOOL, offsetof(struct attr_edit, autohscroll)},
	{STRING("autoenter"), XML_BOOL, offsetof(struct attr_edit, autoenter)},
	{STRING("password"), XML_BOOL, offsetof(struct attr_edit, password)},
	0
};

static const struct xml_attribute attr_table_grid[] = {
	{STRING("topleft"), XML_ID, offsetof(struct attr_grid, topleft)},
	{STRING("top"), XML_ID, offsetof(struct attr_grid, top)},
	{STRING("topright"), XML_ID, offsetof(struct attr_grid, topright)},
	{STRING("left"), XML_ID, offsetof(struct attr_grid, left)},
	{STRING("middle"), XML_ID, offsetof(struct attr_grid, middle)},
	{STRING("right"), XML_ID, offsetof(struct attr_grid, right)},
	{STRING("bottomleft"), XML_ID, offsetof(struct attr_grid, bottomleft)},
	{STRING("bottom"), XML_ID, offsetof(struct attr_grid, bottom)},
	{STRING("bottomright"), XML_ID, offsetof(struct attr_grid, bottomright)},
	0
};

static const struct xml_attribute attr_table_group[] = {
	{STRING("id"), XML_ID, offsetof(struct attr_group, id)},
	{STRING("instance_id"), XML_STRING, offsetof(struct attr_group, instance_id)},
	0
};

static const struct xml_attribute attr_table_grouplist[] = {
	0
};

static const struct xml_attribute attr_table_layer[] = {
	{STRING("image"), XML_ID, offsetof(struct attr_layer, image)},
	{STRING("inactiveimage"), XML_ID, offsetof(struct attr_layer, inactiveimage)},
	{STRING("region"), XML_ID, offsetof(struct attr_layer, region)},
	{STRING("tile"), XML_BOOL, offsetof(struct attr_layer, tile)},
	{STRING("resize"), XML_STRING, offsetof(struct attr_layer, resize)},
	{STRING("scale"), XML_STRING, offsetof(struct attr_layer, scale)},
	{STRING("dblclickaction"), XML_STRING, offsetof(struct attr_layer, dblclickaction)},
	0
};

static const struct xml_attribute attr_table_animatedlayer[] = {
	{STRING("frameheight"), XML_INT, offsetof(struct attr_animatedlayer, frameheight)},
	{STRING("framewidth"), XML_INT, offsetof(struct attr_animatedlayer, framewidth)},
	{STRING("elementframes"), XML_INT, offsetof(struct attr_animatedlayer, elementframes)},
	{STRING("start"), XML_INT, offsetof(struct attr_animatedlayer, start)},
	{STRING("end"), XML_INT, offsetof(struct attr_animatedlayer, end)},
	{STRING("speed"), XML_INT, offsetof(struct attr_animatedlayer, speed)},
	{STRING("realtime"), XML_BOOL, offsetof(struct attr_animatedlayer, realtime)},
	{STRING("autoreplay"), XML_BOOL, offsetof(struct attr_animatedlayer, autoreplay)},
	{STRING("autoplay"), XML_BOOL, offsetof(struct attr_animatedlayer, autoplay)},
	{STRING("debug"), XML_BOOL, offsetof(struct attr_animatedlayer, debug)},
	0
};

static const struct xml_attribute attr_table_layoutstatus[] = {
	{STRING("exclude"), XML_STRING, offsetof(struct attr_layoutstatus, exclude)},
	{STRING("include_only"), XML_STRING, offsetof(struct attr_layoutstatus, include_only)},
	0
};

static const struct xml_attribute attr_table_list[] = {
	{STRING("items"), XML_STRING, offsetof(struct attr_list, items)},
	{STRING("multiselect"), XML_BOOL, offsetof(struct attr_list, multiselect)},
	{STRING("autodeselect"), XML_BOOL, offsetof(struct attr_list, autodeselect)},
	{STRING("select"), XML_STRING, offsetof(struct attr_list, select)},
	{STRING("antialias"), XML_BOOL, offsetof(struct attr_list, antialias)},
	{STRING("feed"), XML_ID, offsetof(struct attr_list, feed)},
	{STRING("hoverselect"), XML_BOOL, offsetof(struct attr_list, hoverselect)},
	{STRING("sort"), XML_BOOL, offsetof(struct attr_list, sort)},
	{STRING("selectonupdown"), XML_BOOL, offsetof(struct attr_list, selectonupdown)},
	{STRING("numcolumns"), XML_INT, offsetof(struct attr_list, numcolumns)},
	{STRING("columwidths"), XML_INT, offsetof(struct attr_list, columwidths)},
	{STRING("columnlabels"), XML_STRING, offsetof(struct attr_list, columnlabels)},
	0
};

static const struct xml_attribute attr_table_mouseredir[] = {
	{STRING("target"), XML_ID, offsetof(struct attr_mouseredir, target)},
	0
};

static const struct xml_attribute attr_table_wasabi_radiogroup[] = {
	0
};

static const struct xml_attribute attr_table_rect[] = {
	{STRING("color"), XML_ID, offsetof(struct attr_rect, color)},
	{STRING("filled"), XML_BOOL, offsetof(struct attr_rect, filled)},
	{STRING("edges"), XML_STRING, offsetof(struct attr_rect, edges)},
	0
};

static const struct xml_attribute attr_table_script[] = {
	{STRING("file"), XML_STRING, offsetof(struct attr_script, file)},
	{STRING("param"), XML_STRING, offsetof(struct attr_script, param)},
	0
};

static const struct xml_attribute attr_table_slider[] = {
	{STRING("barleft"), XML_ID, offsetof(struct attr_slider, barleft)},
	{STRING("barmiddle"), XML_ID, offsetof(struct attr_slider, barmiddle)},
	{STRING("barright"), XML_ID, offsetof(struct attr_slider, barright)},
	{STRING("thumb"), XML_ID, offsetof(struct attr_slider, thumb)},
	{STRING("downthumb"), XML_ID, offsetof(struct attr_slider, downthumb)},
	{STRING("hoverthumb"), XML_ID, offsetof(struct attr_slider, hoverthumb)},
	{STRING("orientation"), XML_STRING, offsetof(struct attr_slider, orientation)},
	{STRING("low"), XML_INT, offsetof(struct attr_slider, low)},
	{STRING("high"), XML_INT, offsetof(struct attr_slider, high)},
	0
};

static const struct xml_attribute attr_table_snappoint[] = {
	{STRING("id"), XML_ID, offsetof(struct attr_snappoint, id)},
	{STRING("x"), XML_INT, offsetof(struct attr_snappoint, x)},
	{STRING("y"), XML_INT, offsetof(struct attr_snappoint, y)},
	{STRING("relatx"), XML_INT, offsetof(struct attr_snappoint, relatx)},
	{STRING("relaty"), XML_INT, offsetof(struct attr_snappoint, relaty)},
	0
};

static const struct xml_attribute attr_table_wasabi_tabsheet[] = {
	{STRING("windowtype"), XML_STRING, offsetof(struct attr_wasabi_tabsheet, windowtype)},
	{STRING("children"), XML_ID, offsetof(struct attr_wasabi_tabsheet, children)},
	{STRING("type"), XML_INT, offsetof(struct attr_wasabi_tabsheet, type)},
	{STRING("content_margin_top"), XML_INT, offsetof(struct attr_wasabi_tabsheet, content_margin_top)},
	{STRING("content_margin_left"), XML_INT, offsetof(struct attr_wasabi_tabsheet, content_margin_left)},
	{STRING("content_margin_right"), XML_INT, offsetof(struct attr_wasabi_tabsheet, content_margin_right)},
	{STRING("content_margin_bottom"), XML_INT, offsetof(struct attr_wasabi_tabsheet, content_margin_bottom)},
	0
};

static const struct xml_attribute attr_table_text[] = {
	{STRING("display"), XML_STRING, offsetof(struct attr_text, display)},
	{STRING("ticker"), XML_BOOL, offsetof(struct attr_text, ticker)},
	{STRING("antialias"), XML_BOOL, offsetof(struct attr_text, antialias)},
	{STRING("text"), XML_STRING, offsetof(struct attr_text, text)},
	{STRING("default"), XML_STRING, offsetof(struct attr_text, _default)},
	{STRING("font"), XML_ID, offsetof(struct attr_text, font)},
	{STRING("fontsize"), XML_INT, offsetof(struct attr_text, fontsize)},
	{STRING("align"), XML_STRING, offsetof(struct attr_text, align)},
	{STRING("valign"), XML_STRING, offsetof(struct attr_text, valign)},
	{STRING("color"), XML_INT, offsetof(struct attr_text, color)},
	{STRING("shadowcolor"), XML_INT, offsetof(struct attr_text, shadowcolor)},
	{STRING("shadowx"), XML_INT, offsetof(struct attr_text, shadowx)},
	{STRING("shadowy"), XML_INT, offsetof(struct attr_text, shadowy)},
	{STRING("timeroffstyle"), XML_INT, offsetof(struct attr_text, timeroffstyle)},
	{STRING("timecolonwidth"), XML_INT, offsetof(struct attr_text, timecolonwidth)},
	{STRING("nograb"), XML_BOOL, offsetof(struct attr_text, nograb)},
	{STRING("showlen"), XML_BOOL, offsetof(struct attr_text, showlen)},
	{STRING("forcefixed"), XML_BOOL, offsetof(struct attr_text, forcefixed)},
	{STRING("forceupcase"), XML_BOOL, offsetof(struct attr_text, forceupcase)},
	{STRING("forceuppercase"), XML_BOOL, offsetof(struct attr_text, forceuppercase)},
	{STRING("forcelocase"), XML_BOOL, offsetof(struct attr_text, forcelocase)},
	{STRING("forcelowercase"), XML_BOOL, offsetof(struct attr_text, forcelowercase)},
	{STRING("bold"), XML_BOOL, offsetof(struct attr_text, bold)},
	{STRING("wrap"), XML_BOOL, offsetof(struct attr_text, wrap)},
	{STRING("dblclickaction"), XML_STRING, offsetof(struct attr_text, dblclickaction)},
	{STRING("offsetx"), XML_INT, offsetof(struct attr_text, offsetx)},
	{STRING("offsety"), XML_INT, offsetof(struct attr_text, offsety)},
	0
};

static const struct xml_attribute attr_table_wasabi_titlebox[] = {
	{STRING("title"), XML_STRING, offsetof(struct attr_wasabi_titlebox, title)},
	{STRING("content"), XML_ID, offsetof(struct attr_wasabi_titlebox, content)},
	{STRING("centered"), XML_BOOL, offsetof(struct attr_wasabi_titlebox, centered)},
	{STRING("suffix"), XML_STRING, offsetof(struct attr_wasabi_titlebox, suffix)},
	0
};

static const struct xml_attribute attr_table_wasabi_titlebar[] = {
	0
};

static const struct xml_attribute attr_table_tree[] = {
	{STRING("items"), XML_STRING, offsetof(struct attr_tree, items)},
	{STRING("feed"), XML_STRING, offsetof(struct attr_tree, feed)},
	{STRING("sorted"), XML_BOOL, offsetof(struct attr_tree, sorted)},
	{STRING("childtabs"), XML_BOOL, offsetof(struct attr_tree, childtabs)},
	{STRING("expandroot"), XML_BOOL, offsetof(struct attr_tree, expandroot)},
	0
};

static const struct xml_attribute attr_table_vis[] = {
	{STRING("colorallbands"), XML_INT, offsetof(struct attr_vis, colorallbands)},
	{STRING("colorband1"), XML_INT, offsetof(struct attr_vis, colorband1)},
	{STRING("colorband2"), XML_INT, offsetof(struct attr_vis, colorband2)},
	{STRING("colorband3"), XML_INT, offsetof(struct attr_vis, colorband3)},
	{STRING("colorband4"), XML_INT, offsetof(struct attr_vis, colorband4)},
	{STRING("colorband5"), XML_INT, offsetof(struct attr_vis, colorband5)},
	{STRING("colorband6"), XML_INT, offsetof(struct attr_vis, colorband6)},
	{STRING("colorband7"), XML_INT, offsetof(struct attr_vis, colorband7)},
	{STRING("colorband8"), XML_INT, offsetof(struct attr_vis, colorband8)},
	{STRING("colorband9"), XML_INT, offsetof(struct attr_vis, colorband9)},
	{STRING("colorband10"), XML_INT, offsetof(struct attr_vis, colorband10)},
	{STRING("colorband11"), XML_INT, offsetof(struct attr_vis, colorband11)},
	{STRING("colorband12"), XML_INT, offsetof(struct attr_vis, colorband12)},
	{STRING("colorband13"), XML_INT, offsetof(struct attr_vis, colorband13)},
	{STRING("colorband14"), XML_INT, offsetof(struct attr_vis, colorband14)},
	{STRING("colorband15"), XML_INT, offsetof(struct attr_vis, colorband15)},
	{STRING("colorband16"), XML_INT, offsetof(struct attr_vis, colorband16)},
	{STRING("colorbandpeak"), XML_INT, offsetof(struct attr_vis, colorbandpeak)},
	{STRING("colorallosc"), XML_INT, offsetof(struct attr_vis, colorallosc)},
	{STRING("colorosc1"), XML_INT, offsetof(struct attr_vis, colorosc1)},
	{STRING("colorosc2"), XML_INT, offsetof(struct attr_vis, colorosc2)},
	{STRING("colorosc3"), XML_INT, offsetof(struct attr_vis, colorosc3)},
	{STRING("colorosc4"), XML_INT, offsetof(struct attr_vis, colorosc4)},
	{STRING("colorosc5"), XML_INT, offsetof(struct attr_vis, colorosc5)},
	{STRING("channel"), XML_INT, offsetof(struct attr_vis, channel)},
	{STRING("fliph"), XML_BOOL, offsetof(struct attr_vis, fliph)},
	{STRING("flipv"), XML_BOOL, offsetof(struct attr_vis, flipv)},
	{STRING("mode"), XML_INT, offsetof(struct attr_vis, mode)},
	{STRING("peaks"), XML_BOOL, offsetof(struct attr_vis, peaks)},
	{STRING("peakfalloff"), XML_INT, offsetof(struct attr_vis, peakfalloff)},
	{STRING("falloff"), XML_INT, offsetof(struct attr_vis, falloff)},
	{STRING("coloring"), XML_STRING, offsetof(struct attr_vis, coloring)},
	{STRING("bandwidth"), XML_STRING, offsetof(struct attr_vis, bandwidth)},
	{STRING("oscstyle"), XML_STRING, offsetof(struct attr_vis, oscstyle)},
	0
};

static const struct xml_attribute attr_table_eqvis[] = {
	{STRING("colortop"), XML_STRING, offsetof(struct attr_eqvis, colortop)},
	{STRING("colormiddle"), XML_STRING, offsetof(struct attr_eqvis, colormiddle)},
	{STRING("colorbottom"), XML_STRING, offsetof(struct attr_eqvis, colorbottom)},
	{STRING("colorpreamp"), XML_STRING, offsetof(struct attr_eqvis, colorpreamp)},
	0
};

static const struct xml_attribute attr_table_gradient[] = {
	{STRING("gradient_x1"), XML_FLOAT, offsetof(struct attr_gradient, gradient_x1)},
	{STRING("gradient_y1"), XML_FLOAT, offsetof(struct attr_gradient, gradient_y1)},
	{STRING("gradient_x2"), XML_FLOAT, offsetof(struct attr_gradient, gradient_x2)},
	{STRING("gradient_y2"), XML_FLOAT, offsetof(struct attr_gradient, gradient_y2)},
	{STRING("points"), XML_STRING, offsetof(struct attr_gradient, points)},
	0
};

static const struct xml_attribute attr_table_wasabi_standardframe_status[] = {
	{STRING("content"), XML_ID, offsetof(struct attr_wasabi_standardframe_status, content)},
	{STRING("padtitleleft"), XML_INT, offsetof(struct attr_wasabi_standardframe_status, padtitleleft)},
	{STRING("padtitleright"), XML_INT, offsetof(struct attr_wasabi_standardframe_status, padtitleright)},
	{STRING("shade"), XML_ID, offsetof(struct attr_wasabi_standardframe_status, shade)},
	0
};

static const struct xml_attribute attr_table_wasabi_standardframe_nostatus[] = {
	{STRING("content"), XML_ID, offsetof(struct attr_wasabi_standardframe_nostatus, content)},
	{STRING("padtitleleft"), XML_INT, offsetof(struct attr_wasabi_standardframe_nostatus, padtitleleft)},
	{STRING("padtitleright"), XML_INT, offsetof(struct attr_wasabi_standardframe_nostatus, padtitleright)},
	{STRING("shade"), XML_ID, offsetof(struct attr_wasabi_standardframe_nostatus, shade)},
	0
};

static const struct xml_attribute attr_table_wasabi_standardframe_modal[] = {
	{STRING("content"), XML_ID, offsetof(struct attr_wasabi_standardframe_modal, content)},
	{STRING("padtitleleft"), XML_INT, offsetof(struct attr_wasabi_standardframe_modal, padtitleleft)},
	{STRING("padtitleright"), XML_INT, offsetof(struct attr_wasabi_standardframe_modal, padtitleright)},
	{STRING("shade"), XML_ID, offsetof(struct attr_wasabi_standardframe_modal, shade)},
	0
};

static const struct xml_attribute attr_table_wasabi_standardframe_static[] = {
	{STRING("content"), XML_ID, offsetof(struct attr_wasabi_standardframe_static, content)},
	{STRING("padtitleleft"), XML_INT, offsetof(struct attr_wasabi_standardframe_static, padtitleleft)},
	{STRING("padtitleright"), XML_INT, offsetof(struct attr_wasabi_standardframe_static, padtitleright)},
	{STRING("shade"), XML_ID, offsetof(struct attr_wasabi_standardframe_static, shade)},
	0
};

static const struct xml_attribute attr_table_wndholder[] = {
	{STRING("hold"), XML_ID, offsetof(struct attr_wndholder, hold)},
	{STRING("component"), XML_ID, offsetof(struct attr_wndholder, component)},
	{STRING("param"), XML_ID, offsetof(struct attr_wndholder, param)},
	{STRING("noshowcmdbar"), XML_BOOL, offsetof(struct attr_wndholder, noshowcmdbar)},
	{STRING("noanimatedrects"), XML_BOOL, offsetof(struct attr_wndholder, noanimatedrects)},
	{STRING("disableanimatedrects"), XML_BOOL, offsetof(struct attr_wndholder, disableanimatedrects)},
	{STRING("autoopen"), XML_BOOL, offsetof(struct attr_wndholder, autoopen)},
	{STRING("autoclose"), XML_BOOL, offsetof(struct attr_wndholder, autoclose)},
	0
};

static const struct xml_attribute attr_table_objdirview[] = {
	{STRING("dir"), XML_STRING, offsetof(struct attr_objdirview, dir)},
	{STRING("target"), XML_ID, offsetof(struct attr_objdirview, target)},
	{STRING("displaytarget"), XML_ID, offsetof(struct attr_objdirview, displaytarget)},
	{STRING("defaultdisplay"), XML_ID, offsetof(struct attr_objdirview, defaultdisplay)},
	0
};

static const struct xml_attribute attr_table_wasabi_pathpicker[] = {
	0
};

static const struct xml_attribute attr_table_querydrag[] = {
	{STRING("image"), XML_ID, offsetof(struct attr_querydrag, image)},
	{STRING("source"), XML_ID, offsetof(struct attr_querydrag, source)},
	0
};

static const struct xml_attribute attr_table_queryline[] = {
	{STRING("querylist"), XML_ID, offsetof(struct attr_queryline, querylist)},
	{STRING("query"), XML_STRING, offsetof(struct attr_queryline, query)},
	{STRING("auto"), XML_BOOL, offsetof(struct attr_queryline, _auto)},
	0
};

static const struct xml_attribute attr_table_queryresults[] = {
	0
};

static void parse_xml_file(XML_Parser p, const char *path);

static struct context context;
static const int *valid_children;
static XML_Parser p;
static int (*script_exec)(struct script *script, char *function);

static void main_mouse_down(int x, int y)
{
}

static void main_mouse_up(int x, int y)
{
	gui->destroy_window(main_window);
}

static void main_mouse_move(int x, int y)
{
}

static void main_draw(void)
{
}

static void main_focus(bool focus)
{
}

static struct skin_callbacks main_callbacks = {
	.mouse_down = &main_mouse_down,
	.mouse_up = &main_mouse_up,
	.mouse_move = &main_mouse_move,
	.draw = &main_draw,
	.focus = &main_focus
};

static const int skininfo_children[] = {
		EL_VERSION, EL_NAME, EL_COMMENT, EL_AUTHOR,
		EL_EMAIL, EL_HOMEPAGE, EL_SCREENSHOT, -1
};

static const int accelerators_children[] = {
	EL_ACCELERATOR, -1
};

static const int gammaset_children[] = {
	EL_GAMMAGROUP, -1
};

static const int elements_children[] = {
	EL_COLOR, EL_BITMAP, EL_ELEMENTALIAS, EL_BITMAPFONT, -1
};

static const int layout_children[] = {
	EL_WASABI_MAINFRAME_NOSTATUS, EL_SENDPARAMS, EL_HIDEOBJECT, EL_LAYER,
	EL_BUTTON, EL_GROUP, EL_SCRIPT, EL_WASABI_PLAYLISTFRAME_NOSTATUS,
	EL_WASABI_MEDIALIBRARYFRAME_NOSTATUS, EL_WASABI_VISFRAME_NOSTATUS, EL_WASABI_STANDARDFRAME_NOSTATUS, EL_GRID,
	EL_TEXT, -1
};

static const int groupdef_children[] = {
	EL_LAYER, EL_WASABI_TITLEBAR, EL_BUTTON, EL_GROUP,
	EL_SCRIPT, EL_LAYOUTSTATUS, EL_SENDPARAMS, EL_HIDEOBJECT,
	EL_TEXT, EL_MENU, EL_MENU_BUTTON_HOVER, EL_MENU_BUTTON_NORMAL,
	EL_MENU_BUTTON_PRESSED, EL_NSTATESBUTTON, EL_TOGGLEBUTTON, EL_COLORTHEMES_MGR,
	EL_STATUS, EL_ANIMATEDLAYER, EL_VIS, EL_SONGTICKER,
	EL_SLIDER, EL_WINDOWHOLDER, EL_RECT, EL_PROGRESSGRID,
	EL_GRID, EL_GROUPXFADE, EL_COMPONENTBUCKET, EL_COLORTHEMES_LIST,
	EL_COMPONENT, EL_ALBUMART, EL_GUIOBJECT, -1
};

static const int container_children[] = {
	EL_INCLUDE, EL_LAYOUT, EL_GROUPDEF, -1
};

static const int wasabixml_children[] = {
	EL_ACCELERATORS, EL_INCLUDE, EL_SKININFO, EL_GAMMASET,
	EL_ELEMENTS, EL_GROUPDEF, EL_CONTAINER, EL_SCRIPT, -1
};

static const int root_children[] = {
	EL_WASABIXML, -1
};

static const int null_children[] = {
	-1
};

static const char * current_file(void)
{
	return context.xml_stack[context.cur_xml - 1].path;
}

static int current_line(void)
{
	return XML_GetCurrentLineNumber(context.xml_stack[context.cur_xml - 1].p);
}

static void push_element(int handler_id)
{
	if (context.cur_element <= 16) {
		context.element_stack[context.cur_element] = handler_id;
		context.cur_element++;
	} else {
		fprintf(stderr, "WARNING: element stack overflow.\n");
		fflush(stderr);
	}
}

static void push_xml(XML_Parser p, const char *path)
{
	struct xml_file *xml;

	if (context.cur_xml <= 16) {
		xml = &context.xml_stack[context.cur_xml];
		xml->p = p;
		xml->path = path;
		context.cur_xml++;
	} else {
		fprintf(stderr, "ERROR: xml stack overflow.\n");
		fflush(stderr);
	}
}

static void pop_element(void)
{
	context.cur_element--;
	
	if (context.cur_element < 1) {
		fprintf(stderr, "ERROR: element stack underflow.\n");
		fflush(stderr);
		context.cur_element = 1;
	}
}

static void read_text_version()
{
}

static void read_text_name()
{
}

static void read_text_comment()
{
}

static void read_text_author()
{
}

static void read_text_email()
{
}

static void read_text_homepage()
{
}

static void read_text_screenshot()
{
}

static void read_attributes(const char **attr, const struct xml_attribute *table)
{
	int i, j;
	int len;

	memset(&attributes, 0, sizeof(attributes));
	for (i = 0; attr[i] != NULL; i++) { /* for each attribute */
		/* look up table for matching attribute */
		len = strlen(attr[i]);
		for (j = 0; table[j].name.data != NULL; j++) {
			if (table[j].name.length == len && strcasecmp(table[j].name.data, attr[i]) == 0) {
				switch (table[j].type) {
					case XML_STRING:
					/* *((char **) table[j].data) = strdup(attr[i + 1]); */
					break;
					case XML_INT:
					/* sscanf(attr[i + 1], "%d", (int *) table[j].data); */
					break;
				}
				break; /* found attribute, quit loop */
			}
		}
	}
}

static void read_element_accelerator(void)
{
	fprintf(stdout, "%s\n", __FUNCTION__);
	fflush(stdout);
}

static void read_element_bitmap(void)
{
	/* add bitmap to linked list */
	fprintf(stdout, "linking bitmap '%s'.\n", attributes.bitmap.file);
	fflush(stdout);
}

static void read_element_container(void)
{
	const struct attr_container *p = &attributes.container;
	window_t window;

	window = gui->create_window(p->default_x, p->default_y, p->default_w, p->default_h, "test", &main_callbacks);
}

static void read_element_script(void)
{
	const char *old_path;
	char *new_path;
	struct script *script;

	fprintf(stdout, "%s\n", __FUNCTION__);
	fflush(stdout);
	
	fprintf(stdout, "Loading script '%s'.\n", attributes.script.file);
	fflush(stdout);
	old_path = context.xml_stack[0].path;
	new_path = path_append(old_path, attributes.script.file);
	script = script_load(new_path);
	script_exec(script, "onscriptload");
	free(new_path);
}

static char * alloc_xml_path(char *parent, char *child)
{
	char *parent_dir;
	char *new_path;
	
	parent_dir = strdup(parent);
	new_path = path_append(parent_dir, child);
	free(parent_dir);
	return new_path;
}

static void read_element_include()
{
	int i;
	XML_Parser new_p, old_p;
	const char *old_path;
	char *new_path;

	fprintf(stdout, "%s\n", __FUNCTION__);
	fflush(stdout);

	if (attributes.include.file) {
		fprintf(stdout, "including file '%s'.\n", attributes.include.file);
		fflush(stdout);
		old_p = context.xml_stack[context.cur_xml - 1].p;
		new_p = XML_ExternalEntityParserCreate(old_p, "", NULL);
		old_path = context.xml_stack[context.cur_xml - 1].path;
		new_path = path_append(old_path, attributes.include.file);
		parse_xml_file(new_p, new_path);
		free(new_path);
	}
}

/* name element text children attr_table */
static struct element_handler handlers[] = {
	{STRING("accelerator"), &read_element_accelerator, NULL, NULL, NULL},
	{STRING("accelerators"), NULL, NULL, accelerators_children, NULL},
	{STRING("albumart"), NULL, NULL, NULL, NULL},
	{STRING("animatedlayer"), NULL, NULL, NULL, NULL},
	{STRING("author"), NULL, &read_text_author, NULL, NULL},
	{STRING("bitmap"), &read_element_bitmap, NULL, NULL, attr_table_bitmap},
	{STRING("bitmapfont"), NULL, NULL, NULL, NULL},
	{STRING("button"), NULL, NULL, NULL, NULL},
	{STRING("color"), NULL, NULL, NULL, NULL},
	{STRING("colorthemes:list"), NULL, NULL, NULL, NULL},
	{STRING("colorthemes:mgr"), NULL, NULL, NULL, NULL},
	{STRING("comment"), NULL, &read_text_comment, NULL, NULL},
	{STRING("component"), NULL, NULL, NULL, NULL},
	{STRING("componentbucket"), NULL, NULL, NULL, NULL},
	{STRING("container"), &read_element_container, NULL, container_children, NULL},
	{STRING("elementalias"), NULL, NULL, NULL, NULL},
	{STRING("elements"), NULL, NULL, elements_children, NULL},
	{STRING("email"), NULL, &read_text_email, NULL, NULL},
	{STRING("gammagroup"), NULL, NULL, NULL, NULL},
	{STRING("gammaset"), NULL, NULL, gammaset_children, NULL},
	{STRING("grid"), NULL, NULL, NULL, NULL},
	{STRING("group"), NULL, NULL, NULL, NULL},
	{STRING("groupdef"), NULL, NULL, groupdef_children, NULL},
	{STRING("groupxfade"), NULL, NULL, NULL, NULL},
	{STRING("guiobject"), NULL, NULL, NULL, NULL},
	{STRING("hideobject"), NULL, NULL, NULL, NULL},
	{STRING("homepage"), NULL, &read_text_homepage, NULL, NULL},
	{STRING("include"), &read_element_include, NULL, NULL, attr_table_include},
	{STRING("layer"), NULL, NULL, NULL, NULL},
	{STRING("layout"), NULL, NULL, layout_children, NULL},
	{STRING("layoutstatus"), NULL, NULL, NULL, NULL},
	{STRING("menu"), NULL, NULL, NULL, NULL},
	{STRING("menu:button_hover"), NULL, NULL, NULL, NULL},
	{STRING("menu:button_normal"), NULL, NULL, NULL, NULL},
	{STRING("menu:button_pressed"), NULL, NULL, NULL, NULL},
	{STRING("name"), NULL, &read_text_name, NULL, NULL},
	{STRING("nstatesbutton"), NULL, NULL, NULL, NULL},
	{STRING("progressgrid"), NULL, NULL, NULL, NULL},
	{STRING("rect"), NULL, NULL, NULL, NULL},
	{STRING("root"), NULL, NULL, root_children, NULL},
	{STRING("sendparams"), NULL, NULL, NULL, NULL},
	{STRING("screenshot"), NULL, &read_text_screenshot, NULL, NULL},
	{STRING("script"), &read_element_script, NULL, NULL, attr_table_script},
	{STRING("skininfo"), NULL, NULL, skininfo_children, NULL},
	{STRING("slider"), NULL, NULL, NULL, NULL},
	{STRING("songticker"), NULL, NULL, NULL, NULL},
	{STRING("status"), NULL, NULL, NULL, NULL},
	{STRING("text"), NULL, NULL, NULL, NULL},
	{STRING("togglebutton"), NULL, NULL, NULL, NULL},
	{STRING("version"), NULL, &read_text_version, NULL, NULL},
	{STRING("vis"), NULL, NULL, NULL, NULL},
	{STRING("wasabi:mainframe:nostatus"), NULL, NULL, NULL, NULL},
	{STRING("wasabi:medialibraryframe:nostatus"), NULL, NULL, NULL, NULL},
	{STRING("wasabi:playlistframe:nostatus"), NULL, NULL, NULL, NULL},
	{STRING("wasabi:standardframe:nostatus"), NULL, NULL, NULL, NULL},
	{STRING("wasabi:titlebar"), NULL, NULL, NULL, NULL},
	{STRING("wasabi:visframe:nostatus"), NULL, NULL, NULL, NULL},
	{STRING("wasabixml"), NULL, NULL, wasabixml_children, NULL},
	{STRING("windowholder"), NULL, NULL, NULL, NULL}
};

static void start(void *data, const char *el, const char **attr)
{
	start_handler start;
	int i, id = -1, len;

	fprintf(stdout, "%s\n", el);
	fflush(stdout);
	
	/* get element id */
	len = strlen(handlers[i].name.data);
	for (i = 0; i < NUM_ELEMENTS; i++)
		if (handlers[i].name.length == len && strcasecmp(el, handlers[i].name.data) == 0) {
			id = i;
			break;
		}
	if (i == NUM_ELEMENTS) {
		fprintf(stderr, "WARNING: %s:%d: unknown element '%s'.\n", current_file(), current_line(), el);
		fflush(stderr);
		return;
	}

	/* verify if it is a valid child element */
	if (valid_children == NULL) {
		fprintf(stderr, "WARNING: %s:%d: unexpected element '%s'.", current_file(), current_line(), el);
		fprintf(stderr, " Last open element was '%s'.\n", handlers[context.element_stack[context.cur_element - 1]].name);
		fflush(stderr);
		goto out;
	}
	for (i = 0; valid_children[i] >= 0; i++)
		if (valid_children[i] == id)
			break;
	if (valid_children[i] < 0) {
		fprintf(stderr, "WARNING: %s:%d: unexpected element2 '%s'.", current_file(), current_line(), el);
		fprintf(stderr, " Last open element was '%s'.\n", handlers[context.element_stack[context.cur_element - 1]].name);
		fflush(stderr);
		goto out;
	}

	/* execute handler */
	start = handlers[id].start;
	if (start != NULL) {
		if (handlers[id].attr_table)
			read_attributes(attr, handlers[id].attr_table);
		start();
	}
	
out:
	/* push it onto the stack */
	/* if the element is unknown it is pushed anyway to match the closing tag */
	push_element(id);
	if (id >= 0)
		valid_children = handlers[id].valid_children;
	else
		valid_children = null_children;
}

static void end(void *data, const char *el)
{
	int id;

	/* all handlers pop the stack on element end */
	pop_element();
	if (id >= 0) {
		id = context.element_stack[context.cur_element - 1];
		valid_children = handlers[id].valid_children;
	} else {
		valid_children = null_children;
	}
}

static void text(void *data, const XML_Char *s, int len)
{
	/* execute handler on stack top */
}

static void parse_xml_file(XML_Parser p, const char *path)
{
	FILE *file;
	int done;
	int len;
	char *buffer;

	buffer = malloc(BUFFER_SIZE);
	if (buffer == NULL) {
		fprintf(stderr, "ERROR: no mem.\n");
		fflush(stderr);
		return;
	}
	file = fopen(path, "rb");
	if (file == NULL) {
		fprintf(stderr, "WARNING: missing file '%s'.\n", path);
		fflush(stderr);
		goto out;
	}
	
	/* push the root xml onto the stack */
	push_xml(p, path);

	for (;;) {
		len = fread(buffer, 1, BUFFER_SIZE, file);
		if (ferror(file)) {
			fprintf(stderr, "ERROR: Read error\n");
			fflush(stderr);
			exit(-1);
		}

		done = feof(file);

		if (!XML_Parse(p, buffer, len, done)) {
			fprintf(stderr, "ERROR: parse error at line %d: %s\n",
			XML_GetCurrentLineNumber(p),
			XML_ErrorString(XML_GetErrorCode(p)));
			fflush(stderr);
			goto out;
		}

		if (done)
			break;
	}
	
	context.cur_xml--;

out2:
	fclose(file);
out:
	free(buffer);
}

/* TODO: fall back to built-in classic on error. */
static void modern_init(void)
{
	const char *path = "Winamp Modern/skin.xml";
	/* TODO: main window should be the same for both types of skin. */
	main_window = gui->create_window(0, 0, 275, 116, "Main", &main_callbacks);

	/* push the root element onto the stack */
	context.cur_element = 0;
	push_element(EL_ROOT);
	valid_children = root_children;
	
	/* initialize xml stack */
	context.cur_xml = 0;

	p = XML_ParserCreate(NULL);
	if (!p) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		fflush(stderr);
		return;
	}
	XML_SetElementHandler(p, &start, &end);
	XML_SetCharacterDataHandler(p, text);
	parse_xml_file(p, path);
	fprintf(stdout, "done loading xml files.\n");
	fflush(stdout);
	
	/* linked lists should be transformed to arrays,
	 * pngs should be transformed to raw or bmp data
	 * and MAKI scripts should be recompiled. */
	 /* MAKI can trap events and, hook events
	  * and create GUI objects on the fly. */
	/*rebuild_data();*/

	gui->show_window(main_window);
}

static void modern_quit(void)
{
	/* FIXME: window is destroyed twice. */
	gui->destroy_window(main_window);
}

static void modern_set_title(char *title)
{
}

static void modern_set_time(int minutes, int seconds)
{
}

static void modern_set_bitrate(int value)
{
}

static void modern_set_freq(int value)
{
}

struct skin modern = {
	.init = &modern_init,
	.quit = &modern_quit,
	.set_title = &modern_set_title,
	.set_time = &modern_set_time,
	.set_bitrate = &modern_set_bitrate,
	.set_freq = &modern_set_freq
};
