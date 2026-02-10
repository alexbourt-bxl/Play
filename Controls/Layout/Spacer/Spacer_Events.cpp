#include "Spacer.h"

void Spacer::OnPaint(Graphics::Message& /*msg*/)
{
	//msg.Graphics.DrawRect(
	//	PaintRect,
	//	Color::Red);
}

void Spacer::Width_OnChange(Property::Message& msg)
{
	m_setWidth = *((int*)msg.Value);
}

void Spacer::Height_OnChange(Property::Message& msg)
{
	m_setHeight = *((int*)msg.Value);
}