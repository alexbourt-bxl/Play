#include "Action.h"

DEFINE_ABSTRACT_TYPE(Action, Object);

Action::Action()
{

}

Object* Action::Copy()
{
	return null;
}

ActionManager& Action::GetManager()
{
	static ActionManager manager;
	return manager;
}

void Action::Perform(Action* action)
{
	GetManager().Perform(action);
}