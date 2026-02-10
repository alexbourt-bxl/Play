#include "FloatProperty.h"

#include <Controls\Valuebox\Valuebox.h>

void FloatProperty::OnChange(Property::Message& msg)
{
	if (m_valuebox)
		m_valuebox->Value = *((double*)msg.Value);
}

void FloatProperty::valuebox_OnValueChange(ValueMessage& msg)
{
	/*	The control events must be disabled here
		to avoid an endless loop of updates between
		the control and the property. */

	m_valuebox->DisableEvents();
	*this = *msg.Value;
	m_valuebox->EnableEvents();
}