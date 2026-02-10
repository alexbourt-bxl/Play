#ifndef ACTION_H
#define ACTION_H

/*
	Any significant action is encapsulated in an object and goes into an action queue 
	to be handled by an action manager. This means that all actions can execute in a 
	separate thread from the GUI, and also no two actions can happen simultaneously, 
	because they are in a queue. This should minimize conflicts between actions.

	Live actions do not commit anything, and do not save any current state. They are used 
	for live updates. Dead actions finalize live updates, or simply perform some modification.

	An action can be dead, but if SaveState() returns false, then it cannot be undone. 
	Such actions can be used for trivial adjustments, for example brush settings.
*/

#include <Types/Object.h>

#include <Actions\ActionManager.h>

class Action
	: public Object
{
	DECLARE_ABSTRACT_TYPE;

	friend class ActionManager;
	friend class GroupAction;

public:
	Action();

	Object* Copy();

	//

	static ActionManager& GetManager();
	static void Perform(Action* action);

protected:
	virtual bool Perform() = 0;
	virtual void Restore() = 0;
};

#endif