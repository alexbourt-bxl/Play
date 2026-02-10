#include "ColorButton.h"

#include <Types/Graphics\GdiImage8.h>

DEFINE_TYPE(ColorButton, ListButton);

ColorButton::ColorButton()
{
	ColorChangeEvent.Sender = this;

	PaintEvent.Clear();
	PaintEvent += EventHandler(this, &ColorButton::OnPaint);

	Strip.NeckSkin = &s_skinStripNeck;

	m_imgColor.SetSize(_Size(31, 31));
	UpdateColor();

	Icon = &m_imgColor;
}

ColorButton::~ColorButton()
{
}

Color ColorButton::GetColor()
{
	return m_color;
}

void ColorButton::SetColor(const _Color& color, bool updatePrevColor)
{
	m_color = _Color(color, 1.0);

	if (updatePrevColor)
		m_prevColor = m_color;

	UpdateColor();
	Update();

	Control::Message msg(this);
	ColorChangeEvent.Send(msg);
}

Color ColorButton::GetPreviousColor()
{
	return m_prevColor;
}

void ColorButton::UpdateColor()
{
	int width = m_imgColor.Width;
	int height = m_imgColor.Height;

	double imgRadius = (double)min(width, height) / 2.0;
	int prevBreak = height * 3 / 5;

	m_imgColor.Clear();

	Graphics g(&m_imgColor);
	g.Antialias = Graphics::Medium;

	g.ClipRegion = Region(_Rect(0, 0, width, prevBreak));
	g.FillEllipse(
		FPoint(
			(double)width / 2.0,
			(double)height / 2.0),
		imgRadius, imgRadius,
		0.0,
		m_color);

	g.ClipRegion = Region(_Rect(0, prevBreak, width, height - prevBreak));
	g.FillEllipse(
		FPoint(
			(double)width / 2.0,
			(double)height / 2.0),
		imgRadius, imgRadius,
		0.0,
		m_prevColor);

	g.ClipRegion = Region::Null;
	g.DrawEllipse(
		FPoint(
			(double)width / 2.0,
			(double)height / 2.0),
		imgRadius - 0.5, imgRadius - 0.5,
		0.0,
		_Color(_Color::Black, 0.14));
}