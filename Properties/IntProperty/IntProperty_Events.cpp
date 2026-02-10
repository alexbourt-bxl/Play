#include "IntProperty.h"

#include <Controls\Valuebox\Valuebox.h>

void IntProperty::OnChange(Property::Message& msg)
{
	if (m_valuebox)
	{
		m_valuebox->ValueChangeEvent.Enabled = false;
		m_valuebox->CommitChangeEvent.Enabled = false;

		m_valuebox->Value = (double)*((int*)msg.Value);

		m_valuebox->ValueChangeEvent.Enabled = true;
		m_valuebox->CommitChangeEvent.Enabled = true;
	}
}

void IntProperty::valuebox_OnValueChange(ValueMessage& msg)
{
	SuppressAction();
	*this = (int)*msg.Value;
}

void IntProperty::valuebox_OnCommitChange(ValueMessage& /*msg*/)
{
	if (EnableAction)
		PerformAction();

	m_oldValue = (int)m_valuebox->Value;
}