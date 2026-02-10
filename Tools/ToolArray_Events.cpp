#include "ToolArray.h"

void ToolArray::OnAddTool(Container::Message& msg)
{
	Tool* tool = *((Tool**)msg.Item);
	tool->m_container = this;
	m_toolsLayout.Controls.Add(&tool->ControlLayout);
}

void ToolArray::OnRemoveTool(Container::Message& msg)
{
	Tool* tool = *((Tool**)msg.Item);
	tool->m_container = null;
	m_toolsLayout.Controls.Remove(&tool->ControlLayout);
}