#include "OpComposite.h"

#include <Native\Gdi.h>

#include <windows.h>
#include <assert.h>

void OpComposite::Composite_GdiImage(HDC hdcOut,
									 int outX, int outY,
									 int outWidth, int outHeight,
									 //
									 HDC hdcIn,
									 int inX, int inY,
									 int inWidth, int inHeight,
									 //
									 int opacity,
									 Region& clip)
{
	HRGN hrgnClip = null;
	
	if (!clip.IsNull)
	{
		hrgnClip = Gdi::ToHRGN(clip);
		::SelectClipRgn(hdcOut, hrgnClip);
	}

	//

	::SetStretchBltMode(
		hdcOut, 
		COLORONCOLOR);

	BLENDFUNCTION blend = 
	{ 
		AC_SRC_OVER, 
		0, 
		(BYTE)opacity, 
		AC_SRC_ALPHA
	};

	::AlphaBlend(
		hdcOut, 
		outX, outY, 
		outWidth, outHeight,
		hdcIn, 
		inX, inY,
		inWidth, inHeight,
		blend);

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdcOut, null);
		::DeleteObject(hrgnClip);
	}
}