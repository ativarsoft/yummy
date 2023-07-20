/*
 *  yummy/player/w32/preferences.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <windows.h>
#include <commctrl.h>

extern HWND main_window;

static char *nodes[] = {
	"Donation",
	"General Preferences",
	"Plugins",
	"Skins",
	"Bookmarks",
	"Prefs"
};

static LRESULT CALLBACK pref_callback(HWND hwnd, UINT msg, WPARAM a, LPARAM b)
{
	switch(msg) {
		case WM_COMMAND:
		break;
	};

	return DefWindowProc(hwnd, msg, a, b);
}

void preferences()
{
	MSG msg;
	HWND pref;
	HWND tree;
	HMENU tree_menu = 0;
	HINSTANCE instance;
	WNDCLASS pref_class;
	const char pref_class_name[] = "pref_class";
	TVINSERTSTRUCT insert;
	HTREEITEM previous;
	int i;

	instance = GetModuleHandle(0);

	memset(&pref_class, 0, sizeof(WNDCLASS));
	pref_class.hbrBackground = (HBRUSH) (COLOR_BTNFACE+1);
	pref_class.lpszClassName = pref_class_name;
	pref_class.lpfnWndProc = &pref_callback;
	pref_class.hInstance = instance;
	pref_class.hIcon = LoadIcon(instance, MAKEINTRESOURCE(1));
	pref_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	if(!RegisterClass(&pref_class))
		error(1, GetLastError(), "register class");

	pref=CreateWindow(pref_class_name, "Preferences", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0, 0, 587, 456, main_window, 0, instance, 0);
	tree=CreateWindow(WC_TREEVIEW, "", WS_VISIBLE | WS_CHILD, 10, 10, 200, 370, pref, tree_menu, instance, 0);
	CreateWindow(WC_BUTTON, "Close", WS_VISIBLE | WS_CHILD, 10, 380, 200, 20, pref, 0, instance, 0);

	previous = TVI_FIRST;
	insert.item.mask = TVIF_TEXT;
	insert.item.cchTextMax = 10;
	insert.hParent = TVI_ROOT;
	for (i=0; i<5; i++) {
		insert.item.pszText = nodes[i];
		insert.hInsertAfter = previous;
		previous = (HTREEITEM) SendMessage(tree, TVM_INSERTITEM, 0, (LPARAM) &insert);
	}

	while(GetMessage(&msg, pref, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(pref_class_name, instance);
}
