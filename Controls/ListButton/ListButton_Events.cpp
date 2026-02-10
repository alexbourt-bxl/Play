#include "ListButton.h"

#include <Controls\Window\Window.h>

void ListButton::OnPaint(Graphics::Message& msg)
{
	msg.Graphics.DrawSkin(
		PaintRect.X,
		PaintRect.Y - m_hintHeight,
		PaintRect.Width,
		PaintRect.Height,
		s_skinHint);
}

void ListButton::OnMouseMove(Mouse::Message& msg)
{
	if (Window->IsTimerRunning(m_stripTimerId)) 
	{
		/*	If the mouse is quickly moved up
			the strip is shown directly. */
		if (msg.Position.Y < 0 
			&& msg.Position.X >= 0 && msg.Position.X < Width)
		{
			Window->StopTimer(m_stripTimerId);

			Uncapture();
			m_pressed = false;

			ShowStrip();
		}

		/*	If the mouse leaves the button,
			forget about the strip. */
		else if (!ClientRect.Contains(msg.Position))
		{
			Window->StopTimer(m_stripTimerId);
		}
	}

	/*	Otherwise if the mouse is inside the 
		button and the timer isn't running,
		start the timer. */
	else if (m_pressed && ClientRect.Contains(msg.Position))
	{
		Window->StartTimer(
			m_stripTimerId,
			0.25,
			this,
			strip_OnTimerTick);
	}
}

void ListButton::OnLButtonDown(Mouse::Message& /*msg*/)
{
	if (m_defaultToLast)
	{
		Window->StartTimer(
			m_stripTimerId,
			0.25,
			this,
			strip_OnTimerTick);
	}
	else
	{
		Uncapture();
		m_pressed = false;
	
		ShowStrip();
	}
}

void ListButton::OnLButtonUp(Mouse::Message& /*msg*/)
{
	if (Window->IsTimerRunning(m_stripTimerId))
		Window->StopTimer(m_stripTimerId);
}

void ListButton::OnPress(Control::Message& /*msg*/)
{
	if (m_defaultToLast && m_btnLast)
	{
		Control::Message ce(m_btnLast);
		m_btnLast->PressEvent.Send(ce);
	}
}

void ListButton::OnSize(Control::Message& /*msg*/)
{
	m_spacer.Size = Size;

	for (int i = 1; i < m_strip.Controls.Count; i++)
		m_strip.Controls[i]->Size = m_spacer.Size;
}

void ListButton::strip_OnTimerTick(Control* control)
{
	ListButton* btn = (ListButton*)control;

	btn->Window->StopTimer(btn->m_stripTimerId);

	btn->Uncapture();
	btn->m_pressed = false;

	btn->ShowStrip();
}

void ListButton::strip_OnAddControl(Container::Message& msg)
{
	Control* control = *((Control**)msg.Item);

	if (typeof(control) != TYPEOF(Button))
		return;

	Button* btn = (Button*)control;
	//btn->PressEvent += EventHandler(this, &ListButton::button_OnPress);
	btn->Size = m_spacer.Size;

	if (!m_btnLast)
		m_btnLast = btn;
}

void ListButton::strip_OnRemoveControl(Container::Message& msg)
{
	Control* control = *((Control**)msg.Item);

	if (typeof(control) != TYPEOF(Button))
		return;

	//Button* btn = (Button*)control;
	//btn->PressEvent -= EventHandler(this, &ListButton::button_OnPress);

	if (m_strip.Controls.Count < 2) // only imcIcon is left
		m_btnLast = null;
}

void ListButton::strip_OnPaint(Graphics::Message& msg)
{
	_Rect paintRect = PaintRect;

	// Set up offsets

	Point textOffset(
		paintRect.X + paintRect.Width / 2,
		paintRect.Y + paintRect.Height / 2);

	Point iconOffset;
	
	if (m_icon)
	{
		iconOffset.X = 
			Text == L""
			? paintRect.X + m_iconOffset.X + (paintRect.Width - m_icon->Width) / 2
			: paintRect.X + m_iconOffset.X + (paintRect.Height - m_icon->Height) / 2;

		iconOffset.Y = paintRect.Y + m_iconOffset.Y + (paintRect.Height - m_icon->Height) / 2;

		textOffset.X = paintRect.X + m_icon->Width + (paintRect.Width - m_icon->Width) / 2;
	}

	// Draw the button contents

	if (m_icon)
	{
		msg.Graphics.DrawImage(
			iconOffset.X,
			iconOffset.Y,
			m_icon);
	}

	/*	Draw the text. */

	msg.Graphics.DrawText(
		Text, Font::Button, 
		textOffset.X, 
		textOffset.Y + 1, 
		Color(0.96, 0.89, 0.0, 0.82),
		&paintRect, 
		TextAlign);

	msg.Graphics.DrawText(
		Text, Font::Button, 
		textOffset.X, 
		textOffset.Y, 
		TextColor,
		&paintRect, 
		TextAlign);
}

void ListButton::strip_OnMouseMove(Mouse::Message& msg)
{
	/*	TODO: modularize this fallthrough code into
		a reusable function or something. */

	_Rect rect = m_strip.Rect;

	assert(m_strip.Parent);
	m_strip.Parent->RectToScreen(&rect);

	Point p = msg.Position;
	p += rect.Position;

	Control* control = m_strip.HitTest(p);

	if (control && control != this)
	{
		control->CheckMouseEnter(
			msg.Position - control->Position, 
			msg.PrevPosition - control->Position);

		Mouse::Message me(
			msg.Position - control->Position, 
			msg.PrevPosition - control->Position);
		control->MouseMoveEvent.Send(me); 
	}
}

void ListButton::strip_OnLButtonUp(Mouse::Message& msg)
{
	_Rect rect = m_strip.Rect;

	if (!m_strip.Parent)
		return;

	m_strip.Parent->RectToScreen(&rect);

	Point p = msg.Position;
	p += rect.Position;

	Control* control = m_strip.HitTest(p);

	if (control && control != this)
	{
		Mouse::Message me(
			msg.Position - control->Position, 
			msg.PrevPosition - control->Position);
		control->LClickEvent.Send(me); 

		return;
	}

	HideStrip();
}

void ListButton::button_OnPress(Control::Message& msg)
{
	Button* btn = (Button*)msg.Control;
	m_btnLast = btn;

	SelectedIndex = m_strip.Controls.GetIndexOf(msg.Control) - 1;

	HideStrip();
}