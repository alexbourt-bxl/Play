#include "Graphics.h"

#include <Native\Gdi.h>

void Graphics::FillEllipse_GdiImage(HDC hdcDst,
									int x, int y, 
									int rx, int ry, // radii
									Color8& color)
{
	if (color.A == 0)
		return;

	HRGN hrgnClip = null;
	
	if (m_clipRgn)
	{
		hrgnClip = Gdi::ToHRGN(m_clipRgn);
		::SelectClipRgn(hdcDst, hrgnClip);
	}

	//

	int rgb = color.ToGdi() & 0x00FFFFFF; // GDI doesn't handle the alpha byte correctly

	Rect rectGdi(
		x - rx, 
		y - ry, 
		rx * 2 + 1,
		ry * 2 + 1);

	_PreOpacity_Gdi(hdcDst, &rectGdi, (byte)color.A);

	HPEN hp = (HPEN)::CreatePen(PS_SOLID, 1, rgb);
	HBRUSH hb = ::CreateSolidBrush(rgb);

	HPEN hpOld = (HPEN)::SelectObject(hdcDst, hp);
	HBRUSH hbOld = (HBRUSH)::SelectObject(hdcDst, hb);

	::Ellipse(
		hdcDst, 
		rectGdi.Left, 
		rectGdi.Top, 
		rectGdi.Right, 
		rectGdi.Bottom);
	
	::DeleteObject(::SelectObject(hdcDst, hpOld));
	::DeleteObject(::SelectObject(hdcDst, hbOld));

	_PostOpacity_Gdi(hdcDst, &rectGdi, (byte)color.A);

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdcDst, null);
		::DeleteObject(hrgnClip);
	}
}