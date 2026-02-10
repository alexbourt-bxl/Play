#ifndef SCROLL_LAYOUT_H
#define SCROLL_LAYOUT_H

/*	ScrollLayout can only handle one child control which it moves around 
	based on what the user is doing. This is the only way that makes sense
	to me so far. */

#include <Controls\Layout\Layout.h>
#include <Types/Geometric\Point.h>

class ScrollLayout 
	: public Layout
{
	DECLARE_TYPE;

public:
	ScrollLayout();

	void OnMouseMove(Mouse::Message& msg);
	void controls_OnAdd(Container::Message& msg);

private:
	void ScrollTo(int x, int y, bool animate);

	IntProperty m_xOffset;
	IntProperty m_yOffset;

	void OnOffsetChange(Property::Message& msg);
};

#endif