#include "ImageControl.h"

#include <Types/Graphics\Image.h>

void ImageControl::OnPaint(Graphics::Message& msg)
{
	if (!m_image)
		return;

	int x = 0;
	int y = 0;

	if (!m_autoSize)
	{
		if		(m_align & Align::Left)   x = 0;
		else if (m_align & Align::Center) x = (PaintRect.Width - m_image->Width) / 2;
		else if (m_align & Align::Right)  x = PaintRect.Width - m_image->Width;

		if		(m_align & Align::Top)    y = 0;
		else if (m_align & Align::Middle) y = (PaintRect.Height - m_image->Height) / 2;
		else if (m_align & Align::Bottom) y = PaintRect.Height - m_image->Height;
	}

	if (m_fitImage)
	{
		msg.Graphics.DrawImage(
			PaintRect.X, 
			PaintRect.Y,
			PaintRect.Width,
			PaintRect.Height,
			m_image,
			0, 0,
			m_image->Width,
			m_image->Height);
	}
	else
	{
		msg.Graphics.DrawImage(
			PaintRect.X + x, 
			PaintRect.Y + y,
			m_image);
	}
}