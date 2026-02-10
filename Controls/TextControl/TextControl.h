#ifndef TEXT_CONTROL_H
#define TEXT_CONTROL_H

#include <Controls\Control.h>
#include <Types/Basic\String.h>
#include <Types/Graphics\Font.h>
#include <Types/Geometric\Align.h>

class TextControl 
	: public Control
{
	DECLARE_ABSTRACT_TYPE;

public:
	TextControl();

	__declspec(property(get = GetText, put = SetText))
	String	Text;
	String	GetText();
	void	SetText(const String& text);

	__declspec(property(get = GetTextColor, put = SetTextColor))
	Color	TextColor;
	Color	GetTextColor();
	void	SetTextColor(const Color& color);

	__declspec(property(get = GetTextFont, put = SetTextFont))
	Font&	TextFont;
	Font&	GetTextFont();
	void	SetTextFont(const Font& font);

	__declspec(property(get = GetTextAlign, put = SetTextAlign))
	Align	TextAlign;
	Align	GetTextAlign();
	void	SetTextAlign(const Align& align);

	Event<Control::Message&> TextChangeEvent;

protected:
	String	m_text;
	Color	m_textColor;
	Font	m_font;
	Align	m_align;
};

#endif