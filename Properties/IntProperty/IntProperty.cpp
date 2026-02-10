#include "IntProperty.h"
#include "ActChangeIntProperty.h"

#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(IntProperty, Property);

IntProperty::IntProperty()
{
	//m_value = 0; // don't initialize the value for debugging
	ChangeEvent += EventHandler(this, &IntProperty::OnChange);
	
	m_valuebox = null;
}

IntProperty::IntProperty(const IntProperty& prop)
{
	m_value = prop.m_value;
	m_oldValue = m_value;
	ChangeEvent += EventHandler(this, &IntProperty::OnChange);

	m_valuebox = null;

	/*	TODO: copy valuebox configuration from prop. */
}

IntProperty::~IntProperty()
{
	if (m_valuebox)
		delete m_valuebox;
}

Object* IntProperty::Copy()
{
	return new IntProperty(*this);
}

IntProperty::operator int()
{
	return m_value;
}

IntProperty& IntProperty::operator = (int i)
{
	int newVal = i;
	Property::Message msg(&newVal);

	PreChangeEvent.Send(msg);
	m_value = newVal;
	ChangeEvent.Send(msg);

	return *this;
}

IntProperty& IntProperty::operator += (int i)
{
	*this = m_value + i;
	return *this;
}

IntProperty& IntProperty::operator -= (int i)
{
	*this = m_value - i;
	return *this;
}

IntProperty& IntProperty::operator *= (int i)
{
	*this = m_value * i;
	return *this;
}

IntProperty& IntProperty::operator /= (int i)
{
	*this = m_value / i;
	return *this;
}

int IntProperty::operator - () { return -m_value; }

int IntProperty::operator + (int i)	{ return m_value + i; }
int IntProperty::operator - (int i) { return m_value - i; }
int IntProperty::operator * (int i) { return m_value * i; }
int IntProperty::operator / (int i) { return m_value / i; }

bool IntProperty::operator == (int i) { return m_value == i; }
bool IntProperty::operator != (int i) { return m_value != i; }
bool IntProperty::operator >  (int i) { return m_value >  i; }
bool IntProperty::operator >= (int i) { return m_value >= i; }
bool IntProperty::operator <  (int i) { return m_value <  i; }
bool IntProperty::operator <= (int i) { return m_value <= i; }

Control* IntProperty::GetControl() const
{
	return Valuebox;
}

Valuebox* IntProperty::GetValuebox() const
{
	((IntProperty*)this)->CheckCreateValuebox();
	return m_valuebox;
}

void IntProperty::CheckCreateValuebox()
{
	if (!m_valuebox)
	{
		m_valuebox = new _Valuebox();
		m_valuebox->ValueName = Name;
		m_valuebox->DecimalPlaces = 0;
		m_valuebox->Granularity = 1;
		m_valuebox->Value = m_value;
		m_valuebox->ValueChangeEvent += EventHandler(this, &IntProperty::valuebox_OnValueChange);
		m_valuebox->CommitChangeEvent += EventHandler(this, &IntProperty::valuebox_OnCommitChange);
	}
}

void IntProperty::PerformAction()
{
	Action::Perform(new ActChangeIntProperty(this, m_value, m_oldValue));
}