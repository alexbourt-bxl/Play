#include "Sidebar.h"

void Sidebar::OnPaint(Graphics::Message& msg)
{
	msg.Graphics.DrawImage(
		PaintRect.X,
		PaintRect.Y,
		PaintRect.Width,
		PaintRect.Height,
		&s_imgShadow,
		0,
		0,
		s_imgShadow.Width,
		s_imgShadow.Height);
}