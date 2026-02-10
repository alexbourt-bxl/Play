#include "Clipboard.h"

#include <Types/Basic\String.h>
#include <Types/Graphics\Image8.h>
#include <Types/Graphics\GdiImage8.h>
//#include "Graphics.h"
#include <Controls\Window\Window.h>
//#include "Error.h"

#include <windows.h>

Clipboard::Data Clipboard::s_object;
Window*	Clipboard::s_ownerWindow = null;

Window* Clipboard::GetOwnerWindow()
{
	return s_ownerWindow;
}

void Clipboard::SetOwnerWindow(Window* window)
{
	s_ownerWindow = window;
}

Object* Clipboard::Get(Type objType)
{
	if (objType == TYPEOF(String))
	{
		if (s_object && typeof((Object*)s_object) == TYPEOF(String))
			return ((Object*)s_object)->Copy();

		if (!::IsClipboardFormatAvailable(CF_UNICODETEXT))
			return null;

		if (!::OpenClipboard(null))
			return null;

		HGLOBAL hGlobal = ::GetClipboardData(CF_UNICODETEXT);

		if (!hGlobal)
		{
			::CloseClipboard();
			return null;
		}

		wchar_t* pGlobal = (wchar_t*)::GlobalLock(hGlobal);
		
		String* text = new String(pGlobal);

		::GlobalUnlock(hGlobal);
		::CloseClipboard();

		return null; // TODO: return text;
	}
	else if (objType == TYPEOF(Image))
	{
		if (s_object && typeof(s_object) == TYPEOF(Image))
			return ((Object*)s_object)->Copy();

		if (!::IsClipboardFormatAvailable(CF_DIB))
			return null;

		if (!::OpenClipboard(s_ownerWindow->Hwnd))
			return null;

		HGLOBAL hGlobal = ::GetClipboardData(CF_DIB);

		if (!hGlobal)
		{
			::CloseClipboard();
			return null;
		}

		BITMAPINFO* bi = (BITMAPINFO*)::GlobalLock(hGlobal);

		if (!bi)
		{
			::CloseClipboard();
			return null;
		}

		int nColors = bi->bmiHeader.biClrUsed
			? bi->bmiHeader.biClrUsed 
			: (1 << bi->bmiHeader.biBitCount);

		void* bits = bi->bmiHeader.biBitCount > 8
			? (void*)((DWORD*)(bi->bmiColors + bi->bmiHeader.biClrUsed) + (bi->bmiHeader.biCompression == BI_BITFIELDS ? 3 : 0))
			: (void*)(bi->bmiColors + nColors);
			
		GdiImage8* image8 = new GdiImage8(
			bi->bmiHeader.biWidth, 
			bi->bmiHeader.biHeight);
        
		::StretchDIBits(
			image8->Hdc,
			0,
			0, 
			image8->Width,
			image8->Height,
			0, 
			0, 
			bi->bmiHeader.biWidth,
			bi->bmiHeader.biHeight,
			bits, 
			bi, 
			DIB_RGB_COLORS, 
			SRCCOPY);

		// Make opaque

		if (bi->bmiHeader.biBitCount < 32)
		{
			byte* buffer = (byte*)image8->Buffer + 3;
			byte* bufferEnd = (byte*)image8->Buffer + image8->Width * image8->Height * sizeof(int);

			while (buffer < bufferEnd)
			{
				*buffer = (byte)0xFF;
				buffer += sizeof(int);
			}
		}

		::GlobalUnlock(hGlobal);
		::CloseClipboard();

		//s_object.data = img;
		return image8; //s_object.data;
	}

	return null;
}

bool Clipboard::Set(::Object* obj)
{
	if (!obj)
		return false;

	if (typeof(obj) == TYPEOF(String))
	{
		String* text = (String*)obj;

		if (!::OpenClipboard(null))
			return false;

		HGLOBAL hGlobal = ::GlobalAlloc(
			GHND | GMEM_SHARE, 
			text->SizeInBytes);

		if (!hGlobal)
		{
			::CloseClipboard();
			return false;
		}

		wchar_t* pGlobal = (wchar_t*)::GlobalLock(hGlobal);
		
		wcsncpy_s(
			pGlobal, text->SizeInBytes / sizeof(wchar_t),
			(wchar_t*)text, text->Length);
		
		::GlobalUnlock(hGlobal);

		::EmptyClipboard();
		::SetClipboardData(CF_UNICODETEXT, hGlobal);
		
		::CloseClipboard();
		::GlobalFree(hGlobal);

		//s_object = text; // TODO:

		return true;
	}
	else if (typeof(obj) == TYPEOF(Image8))
	{
		//Image8* image = (Image8*)obj;

		//Image8 img(image->Width, image->Height);
		//Graphics g(&img);

		//// Paint a white background for copying to other applications
		//g.FillRect(0, 0, img.Width, img.Height, Color::White);//image->BackColor);

		//g.BlendImage(
		//	0,
		//	0,
		//	image,
		//	0,
		//	0,
		//	image->Width,
		//	image->Height,
		//	1.0,
		//	Over);

		//if (!::OpenClipboard(s_ownerWindow->Hwnd))
		//	return false;

		//HGLOBAL hGlobal = ::GlobalAlloc(
		//	GHND | GMEM_SHARE, 
		//	sizeof(BITMAPV5HEADER) + img.Width * img.Height * sizeof(int));

		//if (!hGlobal)
		//{
		//	::CloseClipboard();
		//	return false;
		//}

		//char* pGlobal = (char*)::GlobalLock(hGlobal);

		//BITMAPINFO* bi = (BITMAPINFO*)pGlobal;
		//
		//GdiImage8::InitBitmapHeader(
		//	&bi->bmiHeader, 
		//	 img.Width, 
		//	-img.Height);

		//// The bitmap has to be created upside-down

		//for (int y = 0; y < img.Height; y++)
		//{
		//	memcpy_s(
		//		pGlobal + sizeof(BITMAPINFOHEADER) + y * img.Width * sizeof(int), // start of row
		//		img.Width * sizeof(int), // size of row
		//		(int*)img.Buffer + (img.Height - 1 - y) * img.Width, // start of opposite row
		//		img.Width * sizeof(int)); // size of row
		//}

		//::GlobalUnlock(hGlobal);

		//::EmptyClipboard();
		//::SetClipboardData(CF_DIB, hGlobal);
		//
		//::CloseClipboard();
		//::GlobalFree(hGlobal);

		//s_object = image;

		//return true;

		return false;
	}

	return false;
}

void Clipboard::Update()
{
	//Clear();
	//GetData();
}

void Clipboard::Clear()
{
	s_object.Clear();
}