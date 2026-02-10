#include "ListProperty.h"

//void ListProperty::btn_OnPress(_Control::Message& msg)
//{
//	//SelectedIndex = m_button.Strip.Controls.GetIndexOf(msg.Control) - 1;
//}
//
//void ListProperty::OnRemoveItem(Container::Message& msg)
//{
//	//_Control* control = *((_Control**)msg.Item);
//	//delete control;
//}

void ListProperty::button_OnSelectItem(::Message& )
{
	Property::Message pe(this);
	ChangeEvent.Send(pe);
}