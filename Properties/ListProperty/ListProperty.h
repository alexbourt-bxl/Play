#ifndef LIST_PROPERTY_H
#define LIST_PROPERTY_H

#include <Properties\Property.h>
#include <Controls\ListButton\ListButton.h>

class ListProperty
	: public Property
{
	DECLARE_TYPE;

	typedef Button _Button;

public:
	ListProperty();
	explicit ListProperty(const ListProperty& prop);
	~ListProperty();

	void	Init();
	Object*	Copy();

	_Control* GetControl() const;

	__declspec(property(get = GetButton))
	ListButton& Button;
	ListButton& GetButton();

protected:
	void PerformAction();

private:
	ListButton	m_button;

	void	button_OnSelectItem(::Message& msg);
	//void	btn_OnPress(_Control::Message& msg);
	//void	OnRemoveItem(Container::Message& msg);
};

#endif