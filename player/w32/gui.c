/*
 *  yummy/player/w32/gui.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> /* TODO: remove */
#include <menus.h>
#include "gui.h"

void open_file_dialog(void);

const LOGFONT logfont = {
	.lfHeight = 10,
	.lfWidth = 0,
	.lfEscapement = 0,
	.lfOrientation = 0,
	.lfWeight = FW_DONTCARE,
	.lfItalic = 0,
	.lfUnderline = 0,
	.lfStrikeOut = 0,
	.lfCharSet = DEFAULT_CHARSET,
	.lfOutPrecision = OUT_DEFAULT_PRECIS,
	.lfClipPrecision = CLIP_DEFAULT_PRECIS,
	.lfQuality = DEFAULT_QUALITY,
	.lfPitchAndFamily = DEFAULT_PITCH,
	.lfFaceName = "Arial"
};

HINSTANCE instance;
static const char modern_window_class_name[] = "modern_window";
static const char class_name[] = "classic_window";
static HDC mem_dc, bmp_dc;
static HBITMAP mem_bmp;
static PAINTSTRUCT p;
static HWND window;
static HCURSOR cursor;

static LRESULT move_window2(HWND hwnd, UINT msg, WPARAM a, LPARAM b)
{
	LRESULT hit;

	hit = DefWindowProc(hwnd, msg, a, b);

	return hit == HTCLIENT? HTCAPTION : hit;
}

static LRESULT CALLBACK callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	struct control *controls;
	struct skin_callbacks *w;

	w = (struct skin_callbacks *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_ERASEBKGND: /* prevent flickering */
		return 1;
	case WM_SETCURSOR:
		SetCursor(cursor);
		return 1;
	}
	/* sometimes w is NULL */
	if (w != NULL) {
		switch (uMsg) {
		case WM_PAINT:
			w->draw();
			break;
		case WM_LBUTTONDOWN:
			puts("WM_LBUTTONDOWN");
			w->mouse_down(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_LBUTTONUP:
			puts("WM_LBUTTONUP");
			w->mouse_up(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MOUSEMOVE:
			/*puts("WM_MOUSEMOVE");*/
			w->mouse_move(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_SIZE:
			if (w->resize != NULL)
				w->resize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_SETFOCUS:
			w->focus(true);
			break;
		case WM_KILLFOCUS:
			w->focus(false);
			break;
		case WM_RBUTTONUP:
			menus_show(menu_main, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static void init(void)
{
	WNDCLASS class;
#if 0
	INITCOMMONCONTROLSEX icc;
#endif

#if 1
	InitCommonControls();
#else /* Requires Vista */
	/* Init common controls */
	memset(&icc, 0, sizeof(icc));
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&icc);
#endif

	instance = GetModuleHandle(0);

	/* Register classic window class */
	memset(&class, 0, sizeof(class));
	class.hbrBackground = (HBRUSH) (COLOR_BTNFACE+1); /* NOTE: set to NULL on release. */
	class.lpszClassName = class_name;
	class.lpfnWndProc = &callback;
	class.hInstance = instance;
	class.hIcon = LoadIcon(instance, MAKEINTRESOURCE(1));
	class.hCursor = LoadCursor(NULL, IDC_ARROW);

	if (!RegisterClass(&class))
		error(1, GetLastError(), "Could not initialize GUI.");

	mem_dc = CreateCompatibleDC(NULL);
	bmp_dc = CreateCompatibleDC(NULL);
}

static void quit(void)
{
	DeleteDC(mem_dc);
	DeleteDC(bmp_dc);

	UnregisterClass(class_name, NULL);
}

static window_t create_window(int x, int y, int w, int h, char *title, const struct skin_callbacks *handler)
{
	HWND hwnd;

	hwnd = CreateWindowEx(0 /*WS_EX_TOOLWINDOW*/, class_name, title, WS_MINIMIZEBOX | WS_POPUP, x, y, w, h, main_window /*0*/, 0, instance, NULL);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) handler);

	return hwnd;
}

/*static inline void gui_create_button(rectangle_t *r, char *title, void *callback)
{
	CreateWindow(class, title, WM_CHILD, r->x, r->y, r->w, r->h, 0, 0, instance, callback);
}*/

static void destroy_window(window_t window)
{
	SendMessage(window, WM_CLOSE, 0, 0);
}

static void move_window(window_t window, int x, int y)
{
	POINT pos;
	RECT rect;

	pos.x = x;
	pos.y = y;
	ClientToScreen(window, &pos);
	GetWindowRect(window, &rect);
	rect.left += x;
	rect.top += y;
	SetWindowPos(window, NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

static void event_handler(uintptr_t a)
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

static void show_window(window_t window)
{
	ShowWindow((HWND) window, SW_SHOW);
}

static void hide_window(window_t window)
{
	ShowWindow((HWND) window, SW_HIDE);
}

static void set_topmost(window_t window)
{
	SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

static void set_not_topmost(window_t window)
{
	SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

static void resize_window(window_t window, int w, int h)
{
	SetWindowPos(window, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
}

static void get_window_rect(int *out)
{
	RECT in;
	int w, h;

	GetWindowRect(window, &in);
	w = in.right - in.left;
	h = in.bottom - in.top;
	out[0] = in.left;
	out[1] = in.top;
	out[2] = w;
	out[3] = h;
}

static void minimize_window(window_t w)
{
	ShowWindow(w, SW_MINIMIZE);
}

static pixmap_t load_image(const char *filename)
{
	return LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

static void unload_image(pixmap_t image)
{
	DeleteObject(image);
}

static void begin_drawing(window_t w)
{
	RECT rect;
	int width, height;

	BeginPaint(w, &p);
	window = w;

	GetWindowRect(w, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	mem_bmp = CreateCompatibleBitmap(p.hdc, width, height);
	SelectObject(mem_dc, mem_bmp);
}

static void end_drawing()
{
	RECT rect;
	int width, height;

	GetWindowRect(window, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	BitBlt(p.hdc, 0, 0, width, height, mem_dc, 0, 0, SRCCOPY);
	EndPaint(window, &p);
}

static void draw_image(pixmap_t pixmap, int dst_x, int dst_y, int w, int h, int src_x, int src_y)
{
	(void) SelectObject(bmp_dc, pixmap);
	BitBlt(mem_dc, dst_x, dst_y, w, h, bmp_dc, src_x, src_y, SRCCOPY);
}

static void draw_image_double(pixmap_t pixmap, int dst_x, int dst_y, int w, int h, int src_x, int src_y)
{
	(void) SelectObject(bmp_dc, pixmap);
	StretchBlt(mem_dc, dst_x, dst_y, w * 2, h * 2, bmp_dc, src_x, src_y, w, h, SRCCOPY);
}

static void draw_filled_rectangle(int x, int y, int w, int h, char *color)
{
	(void) SelectObject(mem_dc, GetStockObject(DC_PEN));
	SetDCPenColor(mem_dc, RGB(color[0], color[1], color[2]));
	(void) SelectObject(mem_dc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(mem_dc, RGB(color[0], color[1], color[2]));
	Rectangle(mem_dc, x, y, x + w, y + h);
}

static void capture_mouse(window_t w)
{
	SetCapture(w);
}

static void release_mouse(void)
{
	ReleaseCapture();
}

static void redraw_window(window_t w)
{
	RedrawWindow(w, NULL, NULL, RDW_INVALIDATE);
}

cursor_t load_cursor(char *filename)
{
	return LoadImage(NULL, filename, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
}

void unload_cursor(cursor_t c)
{
	DeleteObject(c);
}

static void set_cursor(cursor_t c)
{
	cursor = c;
}

static bool check_glue(window_t a, window_t b, int x, int y)
{
	RECT rect_a, rect_b;
	int diff;
	int w, h;
	int r = false;
	int new_x, new_y;

	GetWindowRect(a, &rect_a);
	GetWindowRect(b, &rect_b);
	w = rect_a.right - rect_a.left;
	h = rect_a.bottom - rect_a.top;

	printf("x: %d\ty: %d\n", x, y);

	/* right */
	diff = rect_b.left - rect_a.right - x;
	printf("x %d\tdiff %d\n", x, diff);
	if (diff < 10 && diff >= -10) {
		new_x = rect_b.left - w;
		new_y = rect_a.top + y;
		SetWindowPos(a, NULL, new_x, new_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		r = true;
	}

	/* left */
	diff = rect_b.right - rect_a.left - x;
	printf("x %d\tdiff %d\n", x, diff);
	if (diff < 10 && diff >= -10) {
		new_x = rect_b.right;
		new_y = rect_a.top + y;
		SetWindowPos(a, NULL, new_x, new_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		r = true;
	}

	/* bottom */
	diff = rect_b.bottom - rect_a.top - y;
	printf("x %d\tdiff %d\n", x, diff);
	if (diff < 10 && diff >= -10) {
		new_x = rect_a.left + x;
		new_y = rect_b.bottom;
		SetWindowPos(a, NULL, new_x, new_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		r = true;
	}

	/* left */
	diff = rect_b.top - rect_a.bottom - y;
	printf("x %d\tdiff %d\n", x, diff);
	if (diff < 10 && diff >= -10) {
		new_x = rect_a.left + x;
		new_y = rect_b.top - h;
		SetWindowPos(a, NULL, new_x, new_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		r = true;
	}

	return r;
}

static void open_dir_dialog(void)
{
}

struct gui gui_w32 = {
	.init = &init,
	.quit = &quit,

	.create_window = &create_window,
	.destroy_window = &destroy_window,
	.event_handler = &event_handler,
	.show_window = &show_window,
	.hide_window = &hide_window,
	.move_window = &move_window,
	.redraw_window = &redraw_window,
	.set_topmost = &set_topmost,
	.set_not_topmost = &set_not_topmost,
	.resize_window = &resize_window,
	.get_window_rect = &get_window_rect,
	.minimize_window = &minimize_window,

	.load_image = &load_image,
	.unload_image = &unload_image,
	.begin_drawing = &begin_drawing,
	.draw_image = &draw_image,
	.draw_image_double = &draw_image_double,
	.draw_filled_rectangle = &draw_filled_rectangle,
	.end_drawing = &end_drawing,
	.capture_mouse = &capture_mouse,
	.release_mouse = &release_mouse,
	.load_cursor = &load_cursor,
	.unload_cursor = &unload_cursor,
	.set_cursor = &set_cursor,

	.check_glue = &check_glue,

	.open_file_dialog = &open_file_dialog,
	.open_dir_dialog = &open_dir_dialog
};
