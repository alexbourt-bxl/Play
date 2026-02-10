#ifndef GROUP_ACTION_H
#define GROUP_ACTION_H

#include <Actions/Action.h>
#include <Containers/Array.h>

class GroupAction
	: public Action,
	  public Array<Action*>
{
	DECLARE_TYPE;

public:
	GroupAction();
	~GroupAction();

	bool Perform();
	void Restore();

private:
	Array<bool> m_success;
};

#endif