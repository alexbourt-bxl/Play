#include "OpChannels.h"

#include <Operators\Tree\Tree.h>

void OpChannels::control_OnChange(Control::Message& msg)
{
	MarkDirty();
	Tree->Update(this);
}