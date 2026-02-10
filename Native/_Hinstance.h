#ifndef _HINSTANCE_H
#define _HINSTANCE_H

#include <windows.h>

extern "C" IMAGE_DOS_HEADER __ImageBase;	 // There might potentially be some issues with this instance trick.
#define HINST_PLAY ((HINSTANCE)&__ImageBase) // See http://blogs.msdn.com/oldnewthing/archive/2004/10/25/247180.aspx

#endif