#include "Graphics.h"

#include <Native\Gdi.h>

void Graphics::DrawLine_GdiImage(HDC hdc,
								 int x1, int y1, 
								 int x2, int y2, 
								 Color8& color)
{
	if (color.A == 0)
		return;

	HRGN hrgnClip = null;
	
	if (m_clipRgn)
	{
		hrgnClip = Gdi::ToHRGN(m_clipRgn);
		::SelectClipRgn(hdc, hrgnClip);
	}

	//

	Rect rect;
	rect.Expand(Point(x1, y1));
	rect.Expand(Point(x2, y2));
	
	int rgb = color.ToGdi() & 0x00FFFFFF; // GDI doesn't handle the alpha byte correctly

	_PreOpacity_Gdi(hdc, &rect, (byte)color.A);

	HPEN hp = (HPEN)::CreatePen(PS_SOLID, 1, rgb);
	HPEN hpOld = (HPEN)::SelectObject(hdc, hp);

	::MoveToEx(hdc, x1, y1, null);
	::LineTo(hdc, x2, y2);

	::DeleteObject(::SelectObject(hdc, hpOld));

	_PostOpacity_Gdi(hdc, &rect, (byte)color.A);

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdc, null);
		::DeleteObject(hrgnClip);
	}
}