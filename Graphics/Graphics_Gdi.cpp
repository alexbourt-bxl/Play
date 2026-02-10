#include "Graphics.h"

void Graphics::UpdateGdiScratch(int width, int height)
{
	if (width + 1 > m_gdiScratch.Width 
		|| height + 1 > m_gdiScratch.Height)
	{
		m_gdiScratch.SetSize(Size(
			width  + 1 > m_gdiScratch.Width  ? width  + 1 : m_gdiScratch.Width,
			height + 1 > m_gdiScratch.Height ? height + 1 : m_gdiScratch.Height));
	}
}

void Graphics::_PreOpacity_Gdi(HDC hdc, Rect* rect, byte opacity)
{
	/*	This is a workaround for the lack of good alpha support in GDI. */

	if (opacity == 0xFF)
		return;

	// Get bitmap dimensions

	HBITMAP hbitmap = (HBITMAP)::GetCurrentObject(hdc, OBJ_BITMAP);
	
	BITMAP b;
	::GetObject(hbitmap, sizeof(b), &b);

	// Make sure the scratch buffer doesn't fall outside of the destination image

	Size size(
		min(rect->Width + 1, b.bmWidth - rect->X), 
		min(rect->Height + 1, b.bmHeight - rect->Y));

	UpdateGdiScratch(size.Width, size.Height);

	// Before the drawing starts, we copy the destination rectangle to the scratch image

	assert(typeof(m_image) == TYPEOF(GdiImage));

	::BitBlt(
		m_gdiScratch.Hdc, 
		0, 
		0, 
		size.Width, 
		size.Height, 
		hdc, 
		rect->X, 
		rect->Y, 
		SRCCOPY);
}

void Graphics::_PostOpacity_Gdi(HDC hdc, Rect* rect, byte opacity)
{
	/*	NOTE: This function assumes that the scratch image 
		has already been created and adequately sized. */

	if (opacity == 0xFF)
		return;

	// Get bitmap dimensions

	HBITMAP hbitmap = (HBITMAP)::GetCurrentObject(hdc, OBJ_BITMAP);
	
	BITMAP b;
	::GetObject(hbitmap, sizeof(b), &b);

	// Make sure the scratch buffer doesn't fall outside of the destination image

	Size size(
		min(rect->Width + 1, b.bmWidth - rect->X), 
		min(rect->Height + 1, b.bmHeight - rect->Y));

	/*	Paint the scratch rectangle onto the destination 
		rectangle with the inverse of the drawing opacity. */

	BLENDFUNCTION blend = 
	{ 
		AC_SRC_OVER, 
		0, 
		0xFF - opacity, // Constant source alpha
		0				// No per-pixel alpha
	};

	assert(typeof(m_image) == TYPEOF(GdiImage));

	::SetStretchBltMode(
		hdc, 
		COLORONCOLOR);

	::AlphaBlend(
		hdc, 
		rect->X, 
		rect->Y, 
		size.Width, 
		size.Height, 
		m_gdiScratch.Hdc, 
		0, 
		0,
		size.Width, 
		size.Height, 
		blend);
}