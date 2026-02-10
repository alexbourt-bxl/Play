#include "Property.h"

void Property::OnChange(Property::Message& /*msg*/)
{
	if (m_suppressAction)
	{
		m_suppressAction = false;
		return;
	}

	if (m_enableAction)
		PerformAction();
}