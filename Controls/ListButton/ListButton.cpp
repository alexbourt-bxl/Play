#include "ListButton.h"

#include <Types/Graphics\GdiImage8.h>
#include <Controls\Window\Window.h>

DEFINE_TYPE(ListButton, Button);

ListButton::ListButton()
{
	SelectItemEvent.Sender = this;

	m_selectedIndex = -1;

	m_hintHeight = 3;

	PaintEvent.Clear();
	PaintEvent.AllowDuplicates = true;
	PaintEvent += EventHandler(this, &ListButton::OnPaint);
	PaintEvent += EventHandler(this, &Button::OnPaint);
	PaintEvent.AllowDuplicates = false;

	/*	^ TODO: I am really not happy with this 
		workaround, and ideally the event should 
		be able to distinguish these two handlers 
		correctly, which right now it does not. */

	MouseMoveEvent   += EventHandler(this, &ListButton::OnMouseMove);
	LButtonDownEvent += EventHandler(this, &ListButton::OnLButtonDown);
	LButtonUpEvent   += EventHandler(this, &ListButton::OnLButtonUp);
	PressEvent       += EventHandler(this, &ListButton::OnPress);
	SizeEvent		 += EventHandler(this, &ListButton::OnSize);

	m_defaultToLast = false;
	m_btnLast = null;

	m_strip.AutoSize = true;
	m_strip.Gap = 1;
	m_strip.Reverse = true;
	m_strip.NeckSkin = &s_skinStripNeck;
	*m_strip.Controls.AddEvent += EventHandler(this, &ListButton::strip_OnAddControl);
	*m_strip.Controls.RemoveEvent += EventHandler(this, &ListButton::strip_OnRemoveControl);
	m_strip.PaintEvent += EventHandler(this, &ListButton::strip_OnPaint);
	m_strip.MouseMoveEvent += EventHandler(this, &ListButton::strip_OnMouseMove);
	m_strip.LButtonUpEvent += EventHandler(this, &ListButton::strip_OnLButtonUp);
	
	m_stripTimerId = Window::CreateTimerId();

	m_spacer.Size = Size;
	m_spacer.CanShrink = false;
	m_strip.Controls.Add(&m_spacer);
}

ListButton::~ListButton()
{

}

Rect ListButton::GetFullPaintRect()
{
	_Rect fullPaintRect = Button::GetFullPaintRect();

	fullPaintRect.Top -= m_hintHeight;

	return fullPaintRect;
}

int ListButton::GetHintHeight()
{
	return m_hintHeight;
}

void ListButton::SetHintHeight(int height)
{
	Trail.Add(FullPaintRect);
	m_hintHeight = height;
	Update();
}

bool ListButton::GetDefaultToLast()
{
	return m_defaultToLast;
}

void ListButton::SetDefaultToLast(bool defaultToLast)
{
	m_defaultToLast = defaultToLast;
}

int ListButton::GetSelectedIndex()
{
	return m_selectedIndex;
}

void ListButton::SetSelectedIndex(int index)
{
	assert(index >= 0 && index < m_strip.Controls.Count - 1);

	//Property::Message pe(&index);
	//PreChangeEvent.Send(pe);

	m_selectedIndex = index;

	Control* control = m_strip.Controls[m_selectedIndex + 1];

	if (typeof(control) == TYPEOF(Button))
	{
		Button* btn = (Button*)control;
		Icon = btn->Icon;
		IconOffset = btn->IconOffset;
		Text = btn->Text;
	}

	::Message msg(this);
	SelectItemEvent.Send(msg);
}

void ListButton::AddItem(const String& item, GdiImage* icon)
{
	Button* btn = new Button();

	btn->Text = item;
	btn->Icon = icon;
	btn->IconOffset = IconOffset;
	btn->Width = Width;
	btn->Height = 31;
	btn->PressEvent += EventHandler(this, &ListButton::button_OnPress);

	m_strip.Controls.Add(btn);

	if (m_selectedIndex < 0)
		SelectedIndex = 0; // initialize the main button
}

void ListButton::RemoveItem(int index)
{
	Control* control = m_strip.Controls[index];

	if (typeof(control) == TYPEOF(Button))
	{
		Button* btn = (Button*)control;
		btn->PressEvent -= EventHandler(this, &ListButton::button_OnPress);
	}

	m_strip.Controls.RemoveAt(index);
}

void ListButton::RemoveItem(const String& item)
{
	for (int i = 1; i < m_strip.Controls.Count; i++)
	{
		Control* control = m_strip.Controls[i];

		if (typeof(control) != TYPEOF(Button))
			continue;

		Button* btn = (Button*)control;

		if (btn->Text == item)
		{
			btn->PressEvent -= EventHandler(this, &ListButton::button_OnPress);
			m_strip.Controls.Remove(btn);
		}
	}
}

Strip& ListButton::GetStrip()
{
	return m_strip;
}

void ListButton::ShowStrip()
{
	m_strip.UpdateLayout();

	m_strip.SetPosition(
		PaintRect.X + (Width - m_strip.Width) / 2,
		PaintRect.Y + (int)Height - m_strip.Height + m_strip.Padding);

	for (int i = 1; i < m_strip.Controls.Count; i++) // exclude the first icon image
	{
		if (typeof(m_strip.Controls[i]) != TYPEOF(Button))
			continue;

		Button* btn = (Button*)m_strip.Controls[i];
		btn->m_pressed = true;
	}

	m_strip.Capture();
	Window->ShowFloating(&m_strip);
}

void ListButton::HideStrip()
{
	Window->HideFloating(&m_strip);
	m_strip.Uncapture();

	for (int i = 1; i < m_strip.Controls.Count; i++) // exclude the first icon image
	{
		if (typeof(m_strip.Controls[i]) != TYPEOF(Button))
			continue;

		Button* btn = (Button*)m_strip.Controls[i];
		btn->m_pressed = false;
	}
}