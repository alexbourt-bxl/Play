#include "Align.h"

Align::Align()
{
	m_align = 0;
}

Align::Align(const Align& align)
{
	m_align = align.m_align;
}

Align::Align(int _align)
{
	m_align = _align;
}

Align::operator bool()
{
	return m_align > 0;
}

bool Align::operator == (const Align& align)
{
	return (m_align & align.m_align) != 0;
}

bool Align::operator != (const Align& align)
{
	return !(*this == align);
}

Align& Align::operator = (const Align& align)
{
	m_align = align.m_align;
	return *this;
}

Align& Align::operator &= (const Align& align)
{
	m_align &= align.m_align;
	return *this;
}

Align Align::operator & (const Align& align)
{
	return Align(*this) &= align;
}

Align& Align::operator |= (const Align& align)
{
	m_align |= align.m_align;
	return *this;
}

Align Align::operator | (const Align& align)
{
	return Align(*this) |= align;
}

//

Align Align::None	  = Align(0x00);

Align Align::Left	  = Align(0x01);
Align Align::Center	  = Align(0x02);
Align Align::Right	  = Align(0x04);

Align Align::Top	  = Align(0x10);
Align Align::Middle	  = Align(0x20);
Align Align::Bottom	  = Align(0x40);
Align Align::Baseline = Align(0x80);

Align Align::Default  = Align(Align::Left | Align::Top);