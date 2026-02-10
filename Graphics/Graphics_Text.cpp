#include "Graphics.h"

#include <Types/Basic\String.h>
#include <Types/Graphics\Font.h>

void Graphics::DrawText(const String& text, Font& font, int x, int y, const Color& color, Rect* clipRect, const Align& align)
{
	if (typeof(m_image) == TYPEOF(GdiImage))
	{
		Color8 color8 = (Color&)color;

		DrawText_GdiImage(
			((GdiImage*)m_image)->Hdc,
			text, 
			font, 
			x, y, 
			color8, 
			clipRect, 
			align);
	}
}

Size Graphics::MeasureText(const String& text, Font& font/*, Rect* clipRect*/)
{
	if (typeof(m_image) == TYPEOF(GdiImage))
	{
		return MeasureText_GdiImage(
			((GdiImage*)m_image)->Hdc,
			text, 
			font/*,
			clipRect*/);
	}

	return Size(0, 0);
}

int Graphics::MeasureTextFit(const String& text, Font& font, int widthInPixels)
{
	if (typeof(m_image) == TYPEOF(GdiImage))
	{
		return MeasureTextFit_GdiImage(
			((GdiImage*)m_image)->Hdc,
			text, 
			font,
			widthInPixels);
	}

	return 0;
}

Rect Graphics::GetTextRect(int x, int y, const Size& size, const Align& _align)
{
	Align& align = (Align&)_align;

	Rect textRect(x, y, size.Width, size.Height);

	if		(align == Align::Left)		textRect.X = x;
	else if (align == Align::Center)	textRect.X = x - size.Width / 2;
	else if (align == Align::Right)		textRect.X = x - size.Width;

	if		(align == Align::Top)		textRect.Y = y;
	else if (align == Align::Middle)	textRect.Y = y - size.Height / 2; 
	else if (align == Align::Bottom)	textRect.Y = y - size.Height;

	return textRect;
}