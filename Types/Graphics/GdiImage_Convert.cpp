#include "GdiImage.h"

#include <Types/Graphics\Image8.h>
#include <Types/Graphics\GdiImage8.h>
#include <Native\Gdi.h>

Image* GdiImage::ConvertTo(Type type)
{
	if (type == TYPEOF(GdiImage))
		return (Image*)Copy();

	/*	GdiImage -> GdiImage8 */

	else if (type == TYPEOF(GdiImage8))
	{
		GdiImage8* gdi8 = new GdiImage8(Width, Height);

#ifdef _DEBUG
		Gdi::LeakTest gdiLeakTest(L"GdiImage::ConvertTo(TYPEOF(GdiImage8))");
#endif

		::BitBlt(
			gdi8->Hdc,
			0,
			0,
			Width,
			Height,
			Hdc,
			0,
			0,
			SRCCOPY);

		return gdi8;
	}

	/*	GdiImage -> Image8 */

	else if (type == TYPEOF(Image8))
	{
		Image8* mem8 = new Image8(Width, Height);

#ifdef _DEBUG
		Gdi::LeakTest gdiLeakTest(L"GdiImage::ConvertTo(TYPEOF(Image8))");
#endif

		Gdi::CopyToBuffer(
			(int*)mem8->Buffer,
			Hdc,
			Hbitmap);

		return mem8;
	}

	return null;
}