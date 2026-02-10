#ifndef BOOL_PROPERTY_H
#define BOOL_PROPERTY_H

#include <Properties\Property.h>
#include <Controls\Button\Button.h>

class BoolProperty
	: public Property
{
	DECLARE_TYPE;

	typedef Button _Button;

public:
	BoolProperty();
	explicit BoolProperty(const BoolProperty& prop);
	~BoolProperty();

	Object* Copy();

	operator bool();
	
	BoolProperty& operator = (bool value);

	bool operator == (bool value);
	bool operator != (bool value);

	_Control* GetControl() const;

	__declspec(property(get = GetButton))
	_Button* Button;
	_Button* GetButton() const;

	void CheckCreateButton();

protected:
	void PerformAction();

private:
	bool m_value;
	void OnChange(Property::Message& msg);

	_Button* m_button;
	void button_OnPress(Control::Message& msg);
};

#endif