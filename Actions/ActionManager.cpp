#include "ActionManager.h"
#include "Action.h"

#include <assert.h>

DEFINE_TYPE(ActionManager, Object);

ActionManager::ActionManager()
{

}

ActionManager::~ActionManager()
{
	m_lock.Lock();

	while (m_actions.First)
	{
		delete m_actions.First->Data;
		m_actions.Remove(m_actions.First);
	}

	while (m_history.First)
	{
		delete m_history.First->Data;
		m_history.Remove(m_history.First);
	}

	while (m_undone.First)
	{
		delete m_undone.First->Data;
		m_undone.Remove(m_undone.First);
	}

	m_lock.Unlock();
}

Object*	ActionManager::Copy()
{
	return null;
}

void ActionManager::Perform(Action* action)
{
	assert(action);

	m_lock.Lock();

	m_actions.InsertLast(new List::Node(action));

	/*	First clear the redo queue. */

	while (m_undone.First)
	{
		delete m_undone.First->Data;
		m_undone.Remove(m_undone.First);
	}
	
	/* Now insert the action into the action queue. */

	while (m_actions.First)
	{
		Action* action = (Action*)m_actions.First->Data;
		assert(action);

		m_actions.Remove(m_actions.First);

		m_lock.Unlock();

		/*	Now all the housekeeping is done,
			perform the action. */

		if (action->Perform())
			m_history.InsertLast(new List::Node(action));
		else
			delete action;

		m_lock.Lock();
	}

	m_lock.Unlock();
}

bool ActionManager::Undo()
{
	m_lock.Lock();

	if (!m_history.Last)
	{
		m_lock.Unlock();
		return false;
	}

	Action* undo = (Action*)m_history.Last->Data;
	assert(undo);

	undo->Restore();

	m_history.Remove(m_history.Last);
	m_undone.InsertFirst(new List::Node(undo));

	m_lock.Unlock();
	return true;
}

bool ActionManager::Redo()
{
	m_lock.Lock();

	if (!m_undone.First)
	{
		m_lock.Unlock();
		return false;
	}

	Action* redo = (Action*)m_undone.First->Data;
	assert(redo);

	redo->Perform();

	m_undone.Remove(m_undone.First);
	m_history.InsertLast(new List::Node(redo));

	m_lock.Unlock();
	return true;
}