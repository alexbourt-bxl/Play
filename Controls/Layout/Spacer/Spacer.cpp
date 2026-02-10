#include "Spacer.h"

DEFINE_TYPE(Spacer, Control);

Spacer::Spacer()
{
	IgnoreHitTest = true;

	m_weight = 1.0;
	m_canShrink = false;

	PaintEvent += EventHandler(this, &Spacer::OnPaint);
	Width.ChangeEvent += EventHandler(this, &Spacer::Width_OnChange);
	Height.ChangeEvent += EventHandler(this, &Spacer::Height_OnChange);

	m_setWidth = 0;
	m_setHeight = 0;

	SetSize(10, 10);
}

void Spacer::SetSize(int width, int height)
{
	m_setWidth = width;
	m_setHeight = height;

	Control::SetSize(width, height);
}

bool Spacer::GetCanShrink()
{
	return m_canShrink;
}

void Spacer::SetCanShrink(bool canShrink)
{
	m_canShrink = canShrink;

	if (Parent)
		Parent->UpdateLayout();
}