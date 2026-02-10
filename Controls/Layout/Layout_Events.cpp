#include "Layout.h"

#include <Graphics\Graphics.h>

void Layout::OnPaint(Graphics::Message& msg)
{
	msg.Graphics.FillRect(
		PaintRect,
		BackColor);

	//msg.Graphics.DrawRect(
	//	PaintRect.X,
	//	PaintRect.Y,
	//	PaintRect.Width - 1,
	//	PaintRect.Height - 1,
	//	Color::Red);
}

void Layout::OnAddControl(Container::Message& msg)
{
	//MarkForLayout();
	//Update();
}

void Layout::OnRemoveControl(Container::Message& msg)
{
	//MarkForLayout();
	//Update();
}