/*
 *  yummy/player/w32/comdlg.cpp
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#define NTDDI_VERSION NTDDI_WIN7
#include <windows.h>
#include <shobjidl.h>

extern "C" {
void open_file_dialog(void)
{
#if 0 /* Requires Vista */
	IFileOpenDialog *f;
	HRESULT r;
	
	r = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (!SUCCEEDED(r))
		return;
	r = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void **) &f);
#endif
}
}
