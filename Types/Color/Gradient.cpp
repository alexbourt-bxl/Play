#include "Gradient.h"

DEFINE_TYPE(Gradient, Object);

Gradient::Gradient()
{
	//
}

Gradient::Gradient(const Gradient& _grad)
{
	Gradient& grad = (Gradient&)_grad;

	for (int i = 0; i < grad.m_colors.Count; i++)
		m_colors.Add(grad.Colors[i]);
}

Object* Gradient::Copy()
{
	return new Gradient(*this);
}

Gradient& Gradient::operator = (const Gradient& _grad)
{
	Gradient& grad = (Gradient&)_grad;

	m_colors.Clear();

	for (int i = 0; i < grad.m_colors.Count; i++)
		m_colors.Add(grad.Colors[i]);

	return *this;
}

Array<Gradient::Color>& Gradient::GetColors()
{
	return m_colors;
}

//////////////////////////////////////////////////////////

Gradient::Color::Color()
{
	m_color = ::Color::Transparent;
	m_position = 0;
}

Gradient::Color::Color(::Color color, double position)
{
	m_color = color;
	m_position = position;
}

Gradient::Color::operator Color()
{
	return m_color;
}

Gradient::Color& Gradient::Color::operator = (const ::Color& color)
{
	m_color = color;
	return *this;
}

double Gradient::Color::GetPosition()
{
	return m_position;
}

void Gradient::Color::SetPosition(double position)
{
	m_position = position;
}