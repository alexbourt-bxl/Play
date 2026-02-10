#include "ActChangeIntProperty.h"

#include <Properties\IntProperty\IntProperty.h>

#include <assert.h>

DEFINE_TYPE(ActChangeIntProperty, Action);

ActChangeIntProperty::ActChangeIntProperty()
{
	m_prop = null;
}

ActChangeIntProperty::ActChangeIntProperty(IntProperty* prop, int newValue, int oldValue)
{
	assert(prop);
	m_prop = prop;

	m_newValue = newValue;
	m_oldValue = oldValue;
}

IntProperty* ActChangeIntProperty::GetProperty() const
{
	return m_prop;
}

void ActChangeIntProperty::SetProperty(IntProperty* prop)
{
	m_prop = prop;
}

int ActChangeIntProperty::GetNewValue()
{
	return m_newValue;
}

void ActChangeIntProperty::SetNewValue(int value)
{
	m_newValue = value;
}

int ActChangeIntProperty::GetOldValue()
{
	return m_oldValue;
}

void ActChangeIntProperty::SetOldValue(int value)
{
	m_oldValue = value;
}

bool ActChangeIntProperty::Perform()
{
	m_prop->EnableAction = false;
	*m_prop = m_newValue;
	m_prop->EnableAction = true;

	return true;
}

void ActChangeIntProperty::Restore()
{
	m_prop->EnableAction = false;
	*m_prop = m_oldValue;
	m_prop->EnableAction = true;
}