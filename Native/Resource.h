#ifndef RESOURCE_H
#define RESOURCE_H

#include <Types/Basic\String.h>
#include <Types/Graphics\Image.h>
#include <Native\_Hinstance.h>

#include <windows.h>

class Resource
{
public:
	static Image*	LoadImage(WORD id);
	static HBITMAP	LoadBitmap(WORD id);
	static HCURSOR	LoadCursor(WORD id);
	static HCURSOR	LoadAnimatedCursor(WORD id);
	
	static String	LoadString(const String& resourceName);
	static String	CreateVersionString();
};

#endif
