#ifndef OP_LIST_H
#define OP_LIST_H

#include <Controls\Layout\LineLayout\LineLayout.h>

class Operator;

class OpList
	: public LineLayout
{
	DECLARE_TYPE;

public:
	OpList();
	~OpList();

	void UpdateList();

private:
	void OnRemoveControl(Container::Message& msg);
};

#endif