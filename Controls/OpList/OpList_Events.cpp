#include "OpList.h"

void OpList::OnRemoveControl(Container::Message& msg)
{
	Control* control = *((Control**)msg.Item);
	delete control;
}