#include "Graphics.h"

#include <Native\Gdi.h>
#include <Types/Basic\String.h>
#include <Types/Graphics\Font.h>

void Graphics::DrawText_GdiImage(HDC hdc,
								 const String& _text, 
								 Font& font, 
								 int x, int y, 
								 Color8& color, 
								 Rect* clipRect, 
								 const Align& align)
{
	String& text = (String&)_text;

	if (text == L"")
		return;

	if (color.A == 0)
		return;

	HRGN hrgnClip = null;
	
	if (m_clipRgn)
	{
		hrgnClip = Gdi::ToHRGN(m_clipRgn);
		::SelectClipRgn(hdc, hrgnClip);
	}

	//

	int rgb = color.ToGdi() & 0x00FFFFFF; // GDI doesn't handle the alpha byte correctly

	Size size = MeasureText_GdiImage(hdc, text, font/*, clipRect*/);

	TEXTMETRIC tm;
	::GetTextMetrics(hdc, &tm);

	Rect textRect = GetTextRect(x, y, size, align);

	//textRect.Y -= tm.tmInternalLeading + tm.tmExternalLeading; // offset by the leading

	_PreOpacity_Gdi(hdc, &textRect, (byte)color.A);

	HFONT hfOld = (HFONT)::SelectObject(hdc, font.m_hfont);

	//::SetMapMode(hdc, font.m_mapMode);

	//::SetBkMode(hdc, TRANSPARENT);
	//::SetTextColor(hdc, rgb);

	//RECT r = {
	//	textRect.Left,
	//	textRect.Top,
	//	textRect.Right,
	//	textRect.Bottom };

	//::DrawText(
	//	hdc,
	//	text,
	//	-1,
	//	&r,
	//	DT_LEFT 
	//	| DT_TOP 
	//	| DT_NOPREFIX 
	//	| DT_WORDBREAK 
	//	| DT_END_ELLIPSIS
	//	| (m_wrapText ? null : DT_SINGLELINE));

	::SetMapMode(hdc, font.m_mapMode);
	::SetTextAlign(hdc, TA_LEFT | TA_TOP);

	::SetBkMode(hdc, TRANSPARENT);
	::SetTextColor(hdc, rgb);

	RECT r;

	if (clipRect)
	{ 
		r.left   = clipRect->Left;
		r.top    = clipRect->Top; 
		r.right  = clipRect->Right;
		r.bottom = clipRect->Bottom;
	}

	::ExtTextOut(
		hdc, 
		textRect.X, 
		textRect.Y, 
		ETO_CLIPPED | ETO_NUMERICSLOCAL, 
		clipRect ? &r : null, 
		text,
		text.Length,
		null);

	::SelectObject(hdc, hfOld);

	_PostOpacity_Gdi(hdc, &textRect, (byte)color.A);

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdc, null);
		::DeleteObject(hrgnClip);
	}
}

Size Graphics::MeasureText_GdiImage(HDC hdc, const String& _text, Font& font/*, Rect* clipRect*/)
{
	String& text = (String&)_text;

	if (text == L"")
		return Size(0, 0);

	HFONT hfOld = (HFONT)::SelectObject(hdc, font.m_hfont);

	//::SetMapMode(hdc, font.m_mapMode);

	//RECT r = { 0, 0, 0, 0 }; 

	//if (clipRect)
	//{ 
	//	r.left   = clipRect->Left;
	//	r.top    = clipRect->Top; 
	//	r.right  = clipRect->Right;
	//	r.bottom = clipRect->Bottom;
	//}

	//::DrawText(
	//	hdc,
	//	text,
	//	-1,
	//	&r,
	//	DT_CALCRECT 
	//	| DT_LEFT 
	//	| DT_TOP 
	//	| DT_NOPREFIX 
	//	| DT_WORDBREAK 
	//	| DT_END_ELLIPSIS
	//	| (m_wrapText ? null : DT_SINGLELINE));

	SIZE sz;
	::GetTextExtentPoint32(hdc, text, text.Length, &sz);

	//TEXTMETRIC tm;
	//::GetTextMetrics(hdc, &tm);

	::SelectObject(hdc, hfOld);

	//return Size(
	//	r.right - r.left, 
	//	r.bottom - r.top);

	return Size(
		sz.cx, 
		sz.cy); // - tm.tmInternalLeading - tm.tmExternalLeading); // ignore the leading
}

int Graphics::MeasureTextFit_GdiImage(HDC hdc, const String& _text, Font& font, int widthInPixels)
{
	String& text = (String&)_text;

	if (text == L"" || widthInPixels < 0)
		return 0;

	HFONT hfOld = (HFONT)::SelectObject(hdc, font.m_hfont);

	int caretPos;
	SIZE sz;

	::GetTextExtentExPoint(
		hdc, 
		text, 
		text.Length, 
		widthInPixels, 
		&caretPos, 
		null, 
		&sz);

	::SelectObject(hdc, hfOld);

	 return caretPos;
}