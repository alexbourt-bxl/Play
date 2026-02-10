#include "BoolProperty.h"

void BoolProperty::OnChange(Property::Message& /*msg*/)
{
	if (m_button)
		m_button->Highlight = m_value;
}

void BoolProperty::button_OnPress(Control::Message& /*msg*/)
{
	/*	The control events must be disabled here to 
		avoid an endless loop of updates between the 
		control and the property. */

	m_button->DisableEvents();
	*this = !m_button->Highlight;
	m_button->EnableEvents();
}