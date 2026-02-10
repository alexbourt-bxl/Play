#include "BoolProperty.h"

DEFINE_TYPE(BoolProperty, Property);

BoolProperty::BoolProperty()
{
	m_value = false;
	ChangeEvent += EventHandler(this, &BoolProperty::OnChange);
	m_button = null;
}

BoolProperty::BoolProperty(const BoolProperty& prop)
{
	m_value = prop.m_value;
	ChangeEvent += EventHandler(this, &BoolProperty::OnChange);
	m_button = null;

	/*	TODO: copy valuebox configuration from prop. */
}

BoolProperty::~BoolProperty()
{
	if (m_button)
		delete m_button;
}

Object* BoolProperty::Copy()
{
	return new BoolProperty(*this);
}

BoolProperty::operator bool()
{
	return m_value;
}

BoolProperty& BoolProperty::operator = (bool value)
{
	bool newVal = value;
	Property::Message msg(&newVal);

	PreChangeEvent.Send(msg);
	m_value = newVal;
	ChangeEvent.Send(msg);

	return *this;
}

bool BoolProperty::operator == (bool value) { return m_value == value; }
bool BoolProperty::operator != (bool value) { return m_value != value; }

Control* BoolProperty::GetControl() const
{
	return Button;
}

Button* BoolProperty::GetButton() const
{
	((BoolProperty*)this)->CheckCreateButton();
	return m_button;
}

void BoolProperty::CheckCreateButton()
{
	if (!m_button)
	{	
		m_button = new _Button();
		//m_button->Text = Name;
		m_button->Highlight = m_value;
		m_button->PressEvent += EventHandler(this, &BoolProperty::button_OnPress);
	}
}

void BoolProperty::PerformAction()
{
	
}