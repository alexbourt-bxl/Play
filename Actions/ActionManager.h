#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include <Types/Object.h>

#include <Containers/List.h>
#include <Threads\ThreadLock.h>

class Action;

class ActionManager
	: public Object
{
	DECLARE_TYPE;

public:
	ActionManager();
	~ActionManager();

	Object*	Copy();

	void Perform(Action* action);

	void Flush();
	
	bool Undo();
	bool Redo();

private:
	List m_actions;
	
	List m_history;
	List m_undone;

	ThreadLock m_lock;
};

#endif