#include "ListProperty.h"

DEFINE_TYPE(ListProperty, Property);

ListProperty::ListProperty()
{
	//m_selectedIndex = -1;

	Init();
}

ListProperty::ListProperty(const ListProperty& )
{
	//m_selectedIndex = -1;

	Init();
}

ListProperty::~ListProperty()
{

}

void ListProperty::Init()
{
	//m_button.Text = Name;
	m_button.Height = 31;
	m_button.SelectItemEvent += EventHandler(this, &ListProperty::button_OnSelectItem);

	//*m_button.Strip.Controls.RemoveEvent += EventHandler(this, &ListProperty::OnRemoveItem);
}

Object* ListProperty::Copy()
{
	return new ListProperty(*this);
}

Control* ListProperty::GetControl() const
{
	return (ListButton*)&m_button;
}

ListButton& ListProperty::GetButton()
{
	return m_button;
}

void ListProperty::PerformAction()
{
	
}