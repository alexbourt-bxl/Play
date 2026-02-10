#include "Graphics.h"

#include <Native\Gdi.h>

void Graphics::DrawImage_GdiImage(HDC hdcDst,
								  int xDst, int yDst, 
								  int wDst, int hDst, 
								  HDC hdcSrc,
								  int xSrc, int ySrc, 
								  int wSrc, int hSrc, 
								  int opacity)
{
	HRGN hrgnClip = null;
	
	if (m_clipRgn)
	{
		hrgnClip = Gdi::ToHRGN(m_clipRgn);
		::SelectClipRgn(hdcDst, hrgnClip);
	}

	//

	switch (m_interpolation)
	{
	case Nearest:
	default:
		::SetStretchBltMode(
			hdcDst, 
			COLORONCOLOR);
		break;

	case Bilinear:
		::SetStretchBltMode(
			hdcDst, 
			HALFTONE);
		break;
	}

	bool perPixel = opacity >= 0.0; // Negative opacity means "not per-pixel"
	opacity = abs(opacity);

	if (opacity == 0xFF && !perPixel)
	{
		::StretchBlt(
			hdcDst, 
			xDst, yDst, 
			wDst, hDst, 
			hdcSrc, 
			xSrc, ySrc, 
			wSrc, hSrc, 
			SRCCOPY);
	}
	else
	{
		BLENDFUNCTION blend = 
		{ 
			AC_SRC_OVER, 
			0, 
			(BYTE)opacity, 
			perPixel ? AC_SRC_ALPHA	: 0
		};

		::AlphaBlend(
			hdcDst, 
			xDst, yDst, 
			wDst, hDst, 
			hdcSrc, 
			xSrc, ySrc, 
			wSrc, hSrc, 
			blend);
	}

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdcDst, null);
		::DeleteObject(hrgnClip);
	}
}

void Graphics::CopyImage_GdiImage(HDC hdcDst,
								  int xDst, int yDst, 
								  HDC hdcSrc,
								  int xSrc, int ySrc, 
								  int wSrc, int hSrc)
{
	HRGN hrgnClip = null;
	
	if (m_clipRgn)
	{
		hrgnClip = Gdi::ToHRGN(m_clipRgn);
		::SelectClipRgn(hdcDst, hrgnClip);
	}

	//

	::SetStretchBltMode(
		hdcDst, 
		COLORONCOLOR);
	
	::BitBlt(
		hdcDst, 
		xDst, yDst, 
		wSrc, hSrc,
		hdcSrc, 
		xSrc, ySrc, 
		SRCCOPY);

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdcDst, null);
		::DeleteObject(hrgnClip);
	}
}