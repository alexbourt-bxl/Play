#include "PropertyContainer.h"

#include <Properties\Property.h>

void PropertyContainer::properties_OnAdd(Container::Message& msg)
{
	Property* prop = *((Property**)msg.Item);
	prop->m_container = this;
}