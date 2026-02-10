#include "GdiImage8.h"

#include <Types/Graphics\Image8.h>
#include <Types/Graphics\GdiImage.h>
#include <Native\Gdi.h>

Image* GdiImage8::ConvertTo(Type type)
{
	if (type == TYPEOF(GdiImage8))
		return (Image*)Copy();

	/*	GdiImage8 -> GdiImage */

	else if (type == TYPEOF(GdiImage))
	{
		GdiImage* gdi = new GdiImage(Width, Height);

#ifdef _DEBUG
		Gdi::LeakTest gdiLeakTest(L"GdiImage8::ConvertTo(TYPEOF(GdiImage))");
#endif

		::BitBlt(
			gdi->Hdc,
			0,
			0,
			Width,
			Height,
			Hdc,
			0,
			0,
			SRCCOPY);

		return gdi;
	}

	/*	GdiImage8 -> Image8 */

	else if (type == TYPEOF(Image8))
	{
		Image8* mem8 = new Image8(Width, Height);

#ifdef _DEBUG
		Gdi::LeakTest gdiLeakTest(L"GdiImage8::ConvertTo(TYPEOF(Image8))");
#endif

		Gdi::CopyToBuffer(
			(int*)mem8->Buffer,
			Hdc,
			Hbitmap);

		return mem8;
	}

	return null;
}