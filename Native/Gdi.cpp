#include "Gdi.h"

#include <Types/Graphics\GdiImage8.h>

HRGN Gdi::ToHRGN(Region& rgn)
{
	HRGN hrgn = ::CreateRectRgn(0, 0, 0, 0);
	HRGN _hrgn = ::CreateRectRgn(0, 0, 0, 0);

	for (int i = 0; i < rgn.Rects.Count; i++)
	{
		::SetRectRgn(
			_hrgn, 
			rgn.Rects[i].Left, 
			rgn.Rects[i].Top, 
			rgn.Rects[i].Right, 
			rgn.Rects[i].Bottom);

		::CombineRgn(hrgn, hrgn, _hrgn, RGN_OR);
	}

	::DeleteObject(_hrgn);
	return hrgn;
}

void Gdi::InitBitmapHeader(BITMAPINFOHEADER* bi, int width, int height)
{
	memset(bi, 0, sizeof(BITMAPINFOHEADER));
	
	bi->biSize			= sizeof(BITMAPINFOHEADER);
	bi->biWidth			= width;
	bi->biHeight		= -height;
	bi->biPlanes		= 1;
	bi->biBitCount		= 32;
	bi->biCompression	= BI_RGB;
	//bi->biAlphaMask	= 0xFF000000;
	//bi->biRedMask		= 0x00FF0000;
	//bi->biGreenMask	= 0x0000FF00;
	//bi->biBlueMask	= 0x000000FF;
	//bi->biCSType		= LCS_WINDOWS_COLOR_SPACE;
	//bi->biIntent		= LCS_GM_ABS_COLORIMETRIC;
}

void Gdi::CopyToBuffer(int* buffer, HDC hdc, HBITMAP hbitmap)
{
#ifdef _DEBUG
	LeakTest gdiLeakTest(L"Gdi::CopyToBuffer()");
#endif

	BITMAP b;
	::GetObject(hbitmap, sizeof(b), &b);

	BITMAPINFO bi;
	InitBitmapHeader(
		&bi.bmiHeader, 
		b.bmWidth, 
		b.bmHeight);

	::GetDIBits(
		hdc,
		hbitmap,
		0,
		b.bmHeight,
		buffer,
		&bi,
		DIB_RGB_COLORS);
}

void Gdi::CopyFromBuffer(HDC hdc, HBITMAP hbitmap, int* buffer)
{
#ifdef _DEBUG
	LeakTest gdiLeakTest(L"Gdi::CopyToHdc()");
#endif

	BITMAP b;
	::GetObject(hbitmap, sizeof(b), &b);

	BITMAPINFO bi;
	InitBitmapHeader(
		&bi.bmiHeader, 
		b.bmWidth, 
		b.bmHeight);

	::SetDIBits(
		hdc,
		hbitmap,
		0,
		b.bmHeight,
		buffer,
		&bi,
		DIB_RGB_COLORS);
}

uint Gdi::GetObjectCount()
{
	return ::GetGuiResources(
		::GetCurrentProcess(), 
		GR_GDIOBJECTS);
}