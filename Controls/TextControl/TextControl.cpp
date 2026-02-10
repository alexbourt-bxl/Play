#include "TextControl.h"

DEFINE_ABSTRACT_TYPE(TextControl, Control);

TextControl::TextControl()
{
	TextChangeEvent.Sender = this;

	m_text	= L"";
	m_font  = Font::Default;
	m_align = Align::Left | Align::Middle;
}

String TextControl::GetText()
{
	return m_text;
}

void TextControl::SetText(const String& text)
{
	m_text = text;
	Update();
	
	Control::Message msg(this);
	TextChangeEvent.Send(msg);
}

Color TextControl::GetTextColor()
{
	return m_textColor;
}

void TextControl::SetTextColor(const Color& color)
{
	m_textColor = color;
	Update();
}

Font& TextControl::GetTextFont()
{
	return m_font;
}

void TextControl::SetTextFont(const Font& font)
{
	m_font = font;
	Update();
}

Align TextControl::GetTextAlign()
{
	return m_align;
}

void TextControl::SetTextAlign(const Align& align)
{
	m_align = align;
	Update();
}