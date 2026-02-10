#include "Label.h"

void Label::OnPaint(Graphics::Message& msg)
{
	_Rect paintRect = PaintRect;
	_Rect fullPaintRect = FullPaintRect;

	msg.Graphics.FillRect(
		paintRect,
		BackColor);

	_Size size = msg.Graphics.MeasureText(m_text, m_font);

	/////////////////////////////////////////////////////
	// TODO: replace with platform-neutral wrapped code

	HDC hdc = ((GdiImage*)msg.Graphics.TargetImage)->Hdc;

	HFONT hfOld = (HFONT)::SelectObject(hdc, TextFont.m_hfont);

	TEXTMETRIC tm;
	::GetTextMetrics(hdc, &tm);

	::SelectObject(hdc, hfOld);

	/////////////////////////////////////////////////////

	int x = 0;
	int y = 0;

	if (TextAlign == Align::Center)		x = paintRect.Width / 2;
	else if (TextAlign == Align::Right)	x = paintRect.Width;

	if (TextAlign == Align::Top)		y = -(tm.tmInternalLeading + tm.tmExternalLeading);
	if (TextAlign == Align::Middle)		y = (paintRect.Height - (tm.tmInternalLeading + tm.tmExternalLeading)) / 2;
	else if (TextAlign == Align::Bottom 
		|| TextAlign == Align::Baseline) y = paintRect.Height;

	msg.Graphics.WrapText = m_wrap;
	msg.Graphics.ClipRegion = Region(FullPaintRect);

	msg.Graphics.DrawText(
		Text, 
		TextFont,
		paintRect.X + x, 
		paintRect.Y + y, 
		TextColor, 
		&fullPaintRect, 
		TextAlign);

	//msg.Graphics.DrawRect(
	//	PaintRect,
	//	Color::Red);
}