#include "Property.h"

#include <Properties\PropertyContainer.h>

DEFINE_ABSTRACT_TYPE(Property, Object);

Property::Property()
{
	PreChangeEvent.Sender = this;
	ChangeEvent.Sender = this;

	ChangeEvent += EventHandler(this, &Property::OnChange);

	m_container = false;

	m_enableAction = false;
	m_suppressAction = false;
}

String Property::GetName()
{
	return m_name;
}

void Property::SetName(const String& name)
{
	m_name = name;
}

PropertyContainer* Property::GetContainer()
{
	return m_container;
}

bool Property::GetEnableAction()
{
	return m_enableAction;
}

void Property::SetEnableAction(bool enableAction)
{
	m_enableAction = enableAction;
}

void Property::SuppressAction()
{
	m_suppressAction = true;
}