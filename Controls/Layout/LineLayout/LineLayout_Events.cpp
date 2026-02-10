#include "LineLayout.h"

#include <Controls\Layout\Spacer\Spacer.h>

void LineLayout::OnPaint(Graphics::Message& msg)
{
	//msg.Graphics.DrawRect(
	//	PaintRect.X,
	//	PaintRect.Y,
	//	PaintRect.Width - 1,
	//	PaintRect.Height - 1,
	//	Color::Red);
}

void LineLayout::OnPreUpdateLayout(::Message& /*msg*/)
{
	m_cachedContentSize = _Size(0, 0);
}

void LineLayout::OnPreVisibleChange(Control::Message& /*msg*/)
{
	m_forceNoAnimate = true;
}

void LineLayout::OnAddControl(Container::Message& msg)
{
	m_cachedContentSize = _Size(0, 0);

	Control* control = *((Control**)msg.Item);
	control->UpdateLayout();
}

void LineLayout::OnRemoveControl(Container::Message& /*msg*/)
{
	m_cachedContentSize = _Size(0, 0);
}