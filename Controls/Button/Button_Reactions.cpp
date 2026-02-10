#include "Button.h"

#include <Controls\Window\Window.h>

void Button::OnPaint(Graphics::Message& msg)
{
	_Rect paintRect = PaintRect;

	if (m_selected)
		msg.Graphics.DrawSkin(paintRect, s_skinSelected);

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

	// Draw the button

	if (m_pressed && m_over) // pressed
	{
		msg.Graphics.DrawSkin(
			paintRect, 
			s_skinButtonPressed);

		if (m_highlight)
		{
			msg.Graphics.DrawSkin(
				paintRect, 
				s_skinButtonHighlight, 
				0.35);
		}

		if (m_icon)
		{
			msg.Graphics.DrawImage(
				iconOffset.X,
				iconOffset.Y,
				m_icon,
				0.85);
		}

		/*	Draw the text. */

		msg.Graphics.DrawText(
			Text, Font::Button, 
			textOffset.X, 
			textOffset.Y + 1, 
			Color(0.77, 0.7, 0.69, 0.19),
			&paintRect, 
			TextAlign);

		msg.Graphics.DrawText(
			Text, Font::Button, 
			textOffset.X, 
			textOffset.Y, 
			Color(TextColor, 0.85), 
			&paintRect, 
			TextAlign);
	}
	else // not pressed
	{
		msg.Graphics.DrawSkin(
			paintRect, 
			s_skinButton);

		if (m_highlight)
		{
			msg.Graphics.DrawSkin(
				paintRect, 
				s_skinButtonHighlight);
		}

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
			m_highlight
				? Color(0.96, 0.89, 0.0, 0.82)
				: Color(0.96, 0.89, 0.88, 0.82),
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
}

void Button::OnMouseMove(Mouse::Message& msg)
{
	bool over = ClientRect.Contains(msg.Position);

	if (over != m_over)
	{
		m_over = over;
		Update();
	}
}

void Button::OnMouseEnter(Mouse::Message& /*msg*/)
{
	m_over = true;
	Update();
}

void Button::OnMouseLeave(Mouse::Message& /*msg*/)
{
	m_over = false;
	Update();
}

void Button::OnLButtonDown(Mouse::Message& /*msg*/)
{
	m_pressed = true;
	Update();
}

void Button::OnLButtonUp(Mouse::Message& /*msg*/) 
{
	m_pressed = false;
	Update();
}

void Button::OnLClick(Mouse::Message& /*msg*/)
{
	Control::Message msg(this);
	PressEvent.Send(msg);
}