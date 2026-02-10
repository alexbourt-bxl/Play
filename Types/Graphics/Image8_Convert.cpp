#include "Image8.h"

#include <Types/Graphics\GdiImage.h>
#include <Types/Graphics\GdiImage8.h>
#include <Native\Gdi.h>

Image* Image8::ConvertTo(Type type)
{
	if (type == TYPEOF(Image8))
		return (Image*)Copy();

	/*	GdiImage8 has to come before GdiImage
		because the former derives from the latter. */

	/*	Image8 -> GdiImage8 */

	else if (type == TYPEOF(GdiImage8))
	{
		GdiImage8* gdi8 = new GdiImage8(Width, Height);

#ifdef _DEBUG
		Gdi::LeakTest gdiLeakTest(L"Image8::ConvertTo(TYPEOF(GdiImage8))");
#endif

		Gdi::CopyFromBuffer(
			gdi8->Hdc,
			gdi8->Hbitmap,
			(int*)Buffer);

		return gdi8;
	}

	/*	Image8 -> GdiImage */

	else if (type == TYPEOF(GdiImage))
	{
		GdiImage* gdi = new GdiImage(Width, Height);

#ifdef _DEBUG
		Gdi::LeakTest gdiLeakTest(L"Image8::ConvertTo(TYPEOF(GdiImage))");
#endif

		Gdi::CopyFromBuffer(
			gdi->Hdc,
			gdi->Hbitmap,
			(int*)Buffer);

		return gdi;
	}

	return null;
}