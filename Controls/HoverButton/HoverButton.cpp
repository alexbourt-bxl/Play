#include "HoverButton.h"

#include <Animation\Animation.h>

DEFINE_TYPE(HoverButton, Button);

HoverButton::HoverButton()
{
	m_hoverOpacity = 0.0;
	m_hoverOpacity.ChangeEvent += EventHandler(this, &Control::Opacity_OnChange);

	m_highlight = 0.0;
	m_highlight.ChangeEvent += EventHandler(this, &Control::Opacity_OnChange);

	PaintEvent.Clear();
	PaintEvent += EventHandler(this, &HoverButton::OnPaint);
	
	MouseEnterEvent	+= EventHandler(this, &HoverButton::OnMouseEnter);
	MouseLeaveEvent	+= EventHandler(this, &HoverButton::OnMouseLeave);

	SuspendUpdates();

	SetSize(44, 44);
	TextColor = Color(0.9, 0.9, 0.9);

	ResumeUpdates();
}

Rect HoverButton::GetFullPaintRect()
{
	_Rect fullPaintRect = PaintRect;

	fullPaintRect.X      -= 1;
	fullPaintRect.Y      -= 1;
	fullPaintRect.Width  += 2;
	fullPaintRect.Height += 2;

	return fullPaintRect;
}

void HoverButton::SetHighlight(bool highlight)
{
	Animation::Suspend();

	FloatAnimation* anim = new FloatAnimation();
		anim->Property = m_highlight;
		anim->To       = highlight ? 1.0 : 0.0;
		anim->Speed    = 10.0;

	Animation::Add(anim);
	Animation::Resume();
}