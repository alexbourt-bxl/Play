#include "TextProperty.h"

DEFINE_TYPE(TextProperty, Property);

TextProperty::TextProperty()
{
	m_text = String::Empty;
	ChangeEvent += EventHandler(this, &TextProperty::OnChange);
	m_textbox = null;
}

TextProperty::TextProperty(const TextProperty& prop)
{
	m_text = prop.m_text;
	ChangeEvent += EventHandler(this, &TextProperty::OnChange);
	m_textbox = null;

	/*	TODO: copy valuebox configuration from prop. */
}

TextProperty::~TextProperty()
{
	if (m_textbox)
		delete m_textbox;
}

Object* TextProperty::Copy()
{
	return new TextProperty(*this);
}

TextProperty::operator String()
{
	return m_text;
}

TextProperty& TextProperty::operator = (const String& s)
{
	String newVal = s;
	Property::Message msg(&newVal);

	PreChangeEvent.Send(msg);
	m_text = newVal;
	ChangeEvent.Send(msg);

	return *this;
}

bool TextProperty::operator == (const String& s) { return m_text == s; }
bool TextProperty::operator != (const String& s) { return m_text != s; }

Control* TextProperty::GetControl() const
{
	return Textbox;
}

Textbox* TextProperty::GetTextbox() const
{
	((TextProperty*)this)->CheckCreateTextbox();
	return m_textbox;
}

void TextProperty::CheckCreateTextbox()
{
	if (!m_textbox)
	{
		m_textbox = new _Textbox();
		m_textbox->Text = m_text;
		m_textbox->Width = 400;
		m_textbox->FinishEditingEvent += EventHandler(this, &TextProperty::textbox_OnFinishEditing);
	}
}

void TextProperty::PerformAction()
{
	
}