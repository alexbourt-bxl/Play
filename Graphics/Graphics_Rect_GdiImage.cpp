#include "Graphics.h"

#include <Native\Gdi.h>

void Graphics::DrawRect_GdiImage(HDC hdc,
								 int x, int y, 
								 int width, int height, 
								 Color8& color)
{
	HRGN hrgnClip = null;
	
	if (m_clipRgn)
	{
		hrgnClip = Gdi::ToHRGN(m_clipRgn);
		::SelectClipRgn(hdc, hrgnClip);
	}

	//

	int rgb = color.ToGdi() & 0x00FFFFFF; // GDI doesn't handle the alpha byte correctly

	Rect rectOpacity(x, y, width, height);
	_PreOpacity_Gdi(hdc, &rectOpacity, (byte)color.A);

	HPEN hp = (HPEN)::CreatePen(PS_SOLID, 1, rgb);
	HPEN hpOld = (HPEN)::SelectObject(hdc, hp);
	HBRUSH hb = (HBRUSH)::GetStockObject(NULL_BRUSH);
	HBRUSH hbOld = (HBRUSH)::SelectObject(hdc, hb);

	::Rectangle(
		hdc, 
		x, y, 
		x + width, y + height);
	
	::DeleteObject(::SelectObject(hdc, hpOld));
	::SelectObject(hdc, hbOld);

	_PostOpacity_Gdi(hdc, &rectOpacity, (byte)color.A);

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdc, null);
		::DeleteObject(hrgnClip);
	}
}

void Graphics::FillRect_GdiImage(HDC hdc,
								 int x, int y, 
								 int width, int height, 
								 Color8& color)
{
	HRGN hrgnClip = null;
	
	if (m_clipRgn)
	{
		hrgnClip = Gdi::ToHRGN(m_clipRgn);
		::SelectClipRgn(hdc, hrgnClip);
	}

	int rgb = color.ToGdi() & 0x00FFFFFF; // GDI doesn't handle the alpha byte correctly

	Rect rectOpacity(x, y, width, height);
	_PreOpacity_Gdi(hdc, &rectOpacity, (byte)color.A);

	HBRUSH hb = ::CreateSolidBrush(rgb);
	
	RECT r = { x, y, x + width,	y + height };
	::FillRect(hdc, &r, hb);
	
	::DeleteObject(hb);

	_PostOpacity_Gdi(hdc, &rectOpacity, (byte)color.A);

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdc, null);
		::DeleteObject(hrgnClip);
	}
}

void Graphics::ClearRect_GdiImage(HDC hdc,
								  int x, int y, 
								  int width, int height)
{
	HRGN hrgnClip = null;
	
	if (m_clipRgn)
	{
		hrgnClip = Gdi::ToHRGN(m_clipRgn);
		::SelectClipRgn(hdc, hrgnClip);
	}

	//

	HBRUSH hb = ::CreateSolidBrush(0x00000000);
	
	RECT r = { x, y, x + width,	y + height };
	::FillRect(hdc, &r, hb);
	
	::DeleteObject(hb);

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdc, null);
		::DeleteObject(hrgnClip);
	}
}