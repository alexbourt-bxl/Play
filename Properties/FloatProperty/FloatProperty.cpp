#include "FloatProperty.h"

#include <Controls\Valuebox\Valuebox.h>
#include <limits>

DEFINE_TYPE(FloatProperty, Property);

FloatProperty::FloatProperty()
{
	m_value = std::numeric_limits<double>::quiet_NaN();
	ChangeEvent += EventHandler(this, &FloatProperty::OnChange);
	m_valuebox = null;
}

FloatProperty::FloatProperty(const FloatProperty& prop)
{
	m_value = prop.m_value;
	ChangeEvent += EventHandler(this, &FloatProperty::OnChange);
	m_valuebox = null;

	/*	TODO: copy valuebox configuration from prop. */
}

FloatProperty::~FloatProperty()
{
	if (m_valuebox)
		delete m_valuebox;
}

Object* FloatProperty::Copy()
{
	return new FloatProperty(*this);
}

FloatProperty::operator double()
{
	return m_value;
}

FloatProperty& FloatProperty::operator = (double f)
{
	double newVal = f;
	Property::Message msg(&newVal);

	PreChangeEvent.Send(msg);
	m_value = newVal;
	ChangeEvent.Send(msg);

	return *this;
}

FloatProperty& FloatProperty::operator += (double f)
{
	double newVal = m_value + f;
	Property::Message msg(&newVal);

	PreChangeEvent.Send(msg);
	m_value = newVal;
	ChangeEvent.Send(msg);

	return *this;
}

FloatProperty& FloatProperty::operator -= (double f)
{
	double newVal = m_value - f;
	Property::Message msg(&newVal);

	PreChangeEvent.Send(msg);
	m_value = newVal;
	ChangeEvent.Send(msg);

	return *this;
}

FloatProperty& FloatProperty::operator *= (double f)
{
	double newVal = m_value * f;
	Property::Message msg(&newVal);

	PreChangeEvent.Send(msg);
	m_value = newVal;
	ChangeEvent.Send(msg);

	return *this;
}

FloatProperty& FloatProperty::operator /= (double f)
{
	double newVal = m_value / f;
	Property::Message msg(&newVal);

	PreChangeEvent.Send(msg);
	m_value = newVal;
	ChangeEvent.Send(msg);

	return *this;
}

double FloatProperty::operator - () { return -m_value; }

double FloatProperty::operator + (double f) { return m_value + f; }
double FloatProperty::operator - (double f) { return m_value - f; }
double FloatProperty::operator * (double f) { return m_value * f; }
double FloatProperty::operator / (double f) { return m_value / f; }

bool FloatProperty::operator == (double f) { return m_value == f; }
bool FloatProperty::operator == (int i)    { return m_value == i; }
bool FloatProperty::operator != (double f) { return m_value != f; }
bool FloatProperty::operator != (int i)    { return m_value != i; }
bool FloatProperty::operator >  (double f) { return m_value >  f; }
bool FloatProperty::operator >  (int i)    { return m_value >  i; }
bool FloatProperty::operator >= (double f) { return m_value >= f; }
bool FloatProperty::operator >= (int i)    { return m_value >= i; }
bool FloatProperty::operator <  (double f) { return m_value <  f; }
bool FloatProperty::operator <  (int i)    { return m_value <  i; }
bool FloatProperty::operator <= (double f) { return m_value <= f; }
bool FloatProperty::operator <= (int i)    { return m_value <= i; }

Control* FloatProperty::GetControl() const
{
	return Valuebox;
}

Valuebox* FloatProperty::GetValuebox() const
{
	((FloatProperty*)this)->CheckCreateValuebox();
	return m_valuebox;
}

void FloatProperty::CheckCreateValuebox()
{
	if (!m_valuebox)
	{	
		m_valuebox = new _Valuebox();
		m_valuebox->ValueName = Name;
		m_valuebox->Value = m_value;
		m_valuebox->ValueChangeEvent += EventHandler(this, &FloatProperty::valuebox_OnValueChange);
	}
}

void FloatProperty::PerformAction()
{
	
}