#include "ToolArray.h"

#include <Controls\Window\Window.h>

DEFINE_TYPE(ToolArray, Object);

ToolArray::ToolArray()
{
	Init();
}

ToolArray::ToolArray(const ToolArray& toolArray)
{
	for (int i = 0; i < ((ToolArray&)toolArray).Count; i++)
		Add(toolArray[i]);

	Init();
}

ToolArray::~ToolArray()
{

}

Object* ToolArray::Copy()
{
	return new ToolArray(*this);
}

void ToolArray::Init()
{
	PreChangeActiveToolEvent.Sender = this;
	ChangeActiveToolEvent.Sender = this;

	*AddEvent    += EventHandler(this, &ToolArray::OnAddTool);
	*RemoveEvent += EventHandler(this, &ToolArray::OnRemoveTool);

	m_toolsLayout.Orientation = LineLayout::Horizontal;
	m_toolsLayout.AutoSize = true;
	m_toolsLayout.Gap = 5;
	m_toolsLayout.ClipChildren = false;

	m_spacer.Width = 30;

	m_paramsLayout.Orientation = LineLayout::Horizontal;
	m_paramsLayout.AutoSize = true;
	m_paramsLayout.ClipChildren = false;

	m_layout.Orientation = LineLayout::Horizontal;
	m_layout.AutoSize = true;
	m_layout.ClipChildren = false;
	m_layout.Controls.Add(&m_toolsLayout);
	m_layout.Controls.Add(&m_spacer);
	m_layout.Controls.Add(&m_paramsLayout);

	m_activeIndex = -1;
}

LineLayout& ToolArray::GetLayout()
{
	return m_layout;
}

Tool* ToolArray::GetActiveTool() const
{
	return 
		m_activeIndex > -1
		? GetItem(m_activeIndex)
		: null;
}

void ToolArray::SetActiveTool(Tool* activeTool)
{
	SetActiveTool(GetIndexOf(activeTool));
}

void ToolArray::SetActiveTool(int activeIndex)
{
	if (m_activeIndex > -1)
	{
		::Message pe;
		pe.Object = GetItem(m_activeIndex);
		PreChangeActiveToolEvent.Send(pe);
	}

	m_layout.SuspendUpdates();

	m_layout.Trail.Add(m_layout.FullPaintRect);

	if (m_activeIndex > -1)
	{
		GetItem(m_activeIndex)->Deselect();
		m_paramsLayout.Controls.Clear();
	}

	m_activeIndex = activeIndex;

	if (m_activeIndex > -1)
	{
		GetItem(m_activeIndex)->Select();
		m_paramsLayout.Controls.Add(&GetItem(m_activeIndex)->ParamLayout);
	}

	//m_layout.MarkForLayout();

	m_layout.ResumeUpdates(true);

	//

	if (m_activeIndex > -1)
	{
		::Message msg;
		msg.Object = GetItem(m_activeIndex);
		ChangeActiveToolEvent.Send(msg);
	}
}