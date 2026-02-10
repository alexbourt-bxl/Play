#include "HoverButton.h"

#include <Animation\Animation.h>

void HoverButton::OnPaint(Graphics::Message& msg)
{
	_Rect paintRect = PaintRect;

	msg.Graphics.DrawSkin(
		paintRect,
		s_skinButtonBack,
		BackColor.A);

	if (m_pressed && m_over) // pressed
	{
		msg.Graphics.DrawSkin(
			paintRect, 
			s_skinButtonHighlight, 
			min(max(0.0, m_highlight), 1.0));

		msg.Graphics.DrawSkin(
			paintRect, 
			s_skinButtonPressed, 
			0.75);
		
		if (Icon)
		{
			msg.Graphics.DrawImage(
				paintRect.X + (paintRect.Width - Icon->Width) / 2,
				paintRect.Y + (paintRect.Height - Icon->Height) / 2, // + 1,
				Icon,
				0.65);
		}

		/*	Draw the text. */

		msg.Graphics.DrawText(
			Text, Font::Button, 
			paintRect.X + paintRect.Width / 2, 
			paintRect.Y + paintRect.Height / 2 - 1, 
			Color(Color::Black, 0.15),
			&paintRect, 
			TextAlign);

		msg.Graphics.DrawText(
			Text, Font::Button, 
			paintRect.X + paintRect.Width / 2, 
			paintRect.Y + paintRect.Height / 2, 
			Color(TextColor, 0.65), 
			&paintRect, 
			TextAlign);

		/*	Draw the hover skin. */

		msg.Graphics.DrawSkin(
			paintRect, 
			s_skinButtonHover, 
			min(max(0.0, m_hoverOpacity / 2.0), 1.0));
	}
	else // not pressed
	{
		msg.Graphics.DrawSkin(
			paintRect, 
			s_skinButtonHighlight, 
			min(max(0.0, m_highlight), 1.0));

		if (Icon)
		{
			msg.Graphics.DrawImage(
				paintRect.X + (paintRect.Width - Icon->Width) / 2,
				paintRect.Y + (paintRect.Height - Icon->Height) / 2,
				Icon);
		}

		/*	Draw the text. */

		msg.Graphics.DrawText(
			Text, Font::Button, 
			paintRect.X + paintRect.Width / 2, 
			paintRect.Y + paintRect.Height / 2 - 1, 
			Color(Color::Black, 0.35),
			&paintRect, 
			TextAlign);

		msg.Graphics.DrawText(
			Text, Font::Button, 
			paintRect.X + paintRect.Width / 2, 
			paintRect.Y + paintRect.Height / 2, 
			TextColor,
			&paintRect, 
			TextAlign);

		/*	Draw the hover skin. */

		msg.Graphics.DrawSkin(
			paintRect, 
			s_skinButtonHover, 
			min(max(0.0, m_hoverOpacity), 1.0));
	}
}

void HoverButton::OnMouseEnter(Mouse::Message& /*msg*/)
{
	Animation::Suspend();

	FloatAnimation* anim = new FloatAnimation();
		anim->Property = m_hoverOpacity;
		anim->To       = 1.0;
		anim->Speed    = 5.0;

	Animation::Add(anim);
	Animation::Resume();
}

void HoverButton::OnMouseLeave(Mouse::Message& /*msg*/)
{
	Animation::Suspend();

	FloatAnimation* anim = new FloatAnimation();
		anim->Property = m_hoverOpacity;
		//anim->From   = 1.0;
		anim->To       = 0.0;
		anim->Speed    = 5.0;
		//anim->ConflictAction = Animation::ReplaceCurrent;

	Animation::Add(anim);
	Animation::Resume();
}