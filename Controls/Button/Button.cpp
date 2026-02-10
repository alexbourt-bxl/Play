#include "Button.h"

#include <Controls\Window\Window.h>
#include <assert.h>

DEFINE_TYPE(Button, TextControl);

Button::Button()
{
	PressEvent.Sender = this;
	PreChangeIconEvent.Sender = this;
	ChangeIconEvent.Sender = this;

	m_over			= false;
	m_pressed		= false;

	m_selected		= false;
	m_highlight		= false;

	m_icon			= null;
	m_iconOffset	= Point(0, 0);

	PaintEvent		+= EventHandler(this, &Button::OnPaint);
	MouseMoveEvent	+= EventHandler(this, &Button::OnMouseMove);
	MouseEnterEvent	+= EventHandler(this, &Button::OnMouseEnter);
	MouseLeaveEvent	+= EventHandler(this, &Button::OnMouseLeave);
	LButtonDownEvent+= EventHandler(this, &Button::OnLButtonDown);
	LButtonUpEvent	+= EventHandler(this, &Button::OnLButtonUp);
	LClickEvent		+= EventHandler(this, &Button::OnLClick);

	SuspendUpdates();

	SetSize(35, 35); // 31
	TextColor = Color(0.19, 0.19, 0.19);
	TextAlign = Align::Center | Align::Middle;

	ResumeUpdates();
}

Button::~Button()
{

}

Rect Button::GetFullPaintRect()
{
	_Rect fullPaintRect = PaintRect;

	if (m_selected)
	{
		fullPaintRect.X 	 -= 3;
		fullPaintRect.Y 	 -= 5;
		fullPaintRect.Width	 += 6;
		fullPaintRect.Height += 9;
	}
	else
	{
		fullPaintRect.X 	 -= 2;
		fullPaintRect.Y 	 -= 3;
		fullPaintRect.Width	 += 4;
		fullPaintRect.Height += 7;
	}

	return fullPaintRect;
}

bool Button::GetSelected()
{
	return m_selected;
}

void Button::SetSelected(bool selected)
{
	Trail.Add(FullPaintRect);
	m_selected = selected;
	Update();
}

bool Button::GetHighlight()
{
	return m_highlight;
}

void Button::SetHighlight(bool highlight)
{
	m_highlight = highlight;
	Update();
}

GdiImage* Button::GetIcon() const
{
	return m_icon;
}

void Button::SetIcon(GdiImage* icon)
{
	ImageMessage msg(icon);
	PreChangeIconEvent.Send(msg);

	m_icon = icon;
	Update();

	ChangeIconEvent.Send(msg);
}

Point& Button::GetIconOffset()
{
	return m_iconOffset;
}

void Button::SetIconOffset(const Point& offset)
{
	m_iconOffset = offset;
	Update();
}