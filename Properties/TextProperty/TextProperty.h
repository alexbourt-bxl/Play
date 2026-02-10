#ifndef TEXT_PROPERTY_H
#define TEXT_PROPERTY_H

#include <Properties\Property.h>
#include <Types/Basic\String.h>
#include <Controls\Textbox\Textbox.h>

class Textbox;

class TextProperty
	: public Property
{
	DECLARE_TYPE;

	typedef Textbox _Textbox;

public:
	TextProperty();
	explicit TextProperty(const TextProperty& prop);
	~TextProperty();

	Object* Copy();

	operator String();
	
	TextProperty& operator = (const String& s);

	bool operator == (const String& s);
	bool operator != (const String& s);

	_Control* GetControl() const;

	__declspec(property(get = GetTextbox))
	_Textbox* Textbox;
	_Textbox* GetTextbox() const;

	void CheckCreateTextbox();

protected:
	void PerformAction();

private:
	String m_text;
	void OnChange(Property::Message& msg);

	_Textbox* m_textbox;
	void textbox_OnFinishEditing(_Textbox::Message& msg);
};

#endif