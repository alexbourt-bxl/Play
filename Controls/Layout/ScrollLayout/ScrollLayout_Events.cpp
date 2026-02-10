#include "ScrollLayout.h"

void ScrollLayout::OnMouseMove(Mouse::Message& msg)
{
	if (Controls.Count < 1)
		return;

	_Size refSize = ClientRect.Size;
	_Size controlSize = Controls[0]->Size;

	double xFactor = (double)msg.Position.X / (double)refSize.Width;
	double yFactor = (double)msg.Position.Y / (double)refSize.Height;

	//xFactor = Math::LerpBezier(0.0, 0.0, 1.0, 1.0, xFactor);
	//yFactor = Math::LerpBezier(0.0, 0.0, 1.0, 1.0, yFactor);

	ScrollTo(
		min((int)(xFactor * (double)(refSize.Width - controlSize.Width)), 0),
		min((int)(yFactor * (double)(refSize.Height - controlSize.Height)), 0),
		true);
}

void ScrollLayout::controls_OnAdd(Container::Message& msg)
{
	/*	ScrollLayout can only handle one child control,
		so any time a child control is added, the previous
		control is first erased. */
	Controls.Clear();
}

void ScrollLayout::OnOffsetChange(Property::Message& /*msg*/)
{
	Controls[0]->Position = Point(
		m_xOffset,
		m_yOffset);
}