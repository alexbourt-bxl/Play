#include "TextProperty.h"

void TextProperty::OnChange(Property::Message& msg)
{
	if (m_textbox)
		m_textbox->Text = (String)*((String*)msg.Value);
}

void TextProperty::textbox_OnFinishEditing(_Textbox::Message& /*msg*/)
{
	/*	The control events must be disabled here
		to avoid an endless loop of updates between
		the control and the property. */

	m_textbox->DisableEvents();
	*this = m_textbox->Text;
	m_textbox->EnableEvents();
}