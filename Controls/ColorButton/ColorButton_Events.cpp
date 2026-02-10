#include "ColorButton.h"

void ColorButton::OnPaint(Graphics::Message& msg)
{
	msg.Graphics.DrawSkin(
		PaintRect.X,
		PaintRect.Y - HintHeight,
		PaintRect.Width,
		s_skinHint.Image->Height,
		s_skinHint);

	if (m_pressed) // pressed
	{
		msg.Graphics.DrawSkin(
			PaintRect, 
			m_over
				? s_skinButtonPressed 
				: s_skinButton);

		msg.Graphics.DrawImage(
			PaintRect.X + (PaintRect.Width - m_imgColor.Width) / 2,
			PaintRect.Y + (PaintRect.Height - m_imgColor.Height) / 2,
			&m_imgColor,
			0.85);
	}
	else // not pressed
	{
		msg.Graphics.DrawSkin(
			PaintRect, 
			s_skinButton);

		msg.Graphics.DrawImage(
			PaintRect.X + (PaintRect.Width - m_imgColor.Width) / 2,
			PaintRect.Y + (PaintRect.Height - m_imgColor.Height) / 2,
			&m_imgColor);
	}
}