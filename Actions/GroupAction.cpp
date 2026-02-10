#include "GroupAction.h"

DEFINE_TYPE(GroupAction, Action);

GroupAction::GroupAction()
{

}

GroupAction::~GroupAction()
{
	for (int i = 0; i < Count; i++)
		delete GetItem(i);
}

bool GroupAction::Perform()
{
	assert(m_success.Count == 0);

	for (int i = 0; i < Count; i++)
		m_success.Add(GetItem(i)->Perform());

	bool success = false;

	for (int i = 0; i < m_success.Count; i++)
	{
		assert(m_success[i]);
		success |= m_success[i];
	}

	return success;
}

void GroupAction::Restore()
{
	assert(m_success.Count == Count);

	for (int i = Count - 1; i >= 0; i--)
	{
		if (m_success[i])
			GetItem(i)->Restore();
	}

	m_success.Clear();
}