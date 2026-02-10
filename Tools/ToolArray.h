#ifndef TOOL_ARRAY_H
#define TOOL_ARRAY_H

#include <Types/Object.h>
#include <Containers/Array.h>
#include <Tools\Tool.h>
#include <Controls\Layout\LineLayout\LineLayout.h>
#include <Controls\Layout\Spacer\Spacer.h>

class ToolArray
	: public Object,
	  public Array<Tool*>
{
	DECLARE_TYPE;

public:
	ToolArray();
	ToolArray(const ToolArray& toolArray);
	~ToolArray();

	Object* Copy();

	void Init();

	__declspec(property(get = GetLayout))
	LineLayout& Layout;
	LineLayout& GetLayout();

	__declspec(property(get = GetActiveTool))
	Tool* ActiveTool;
	Tool* GetActiveTool() const;
	void  SetActiveTool(Tool* activeTool);
	void  SetActiveTool(int activeIndex);

	Event<::Message&> PreChangeActiveToolEvent;
	Event<::Message&> ChangeActiveToolEvent;

private:
	LineLayout	m_layout;

	LineLayout	m_toolsLayout;
	Spacer		m_spacer;
	LineLayout	m_paramsLayout;

	int			m_activeIndex;

	void		OnAddTool(Container::Message& msg);
	void		OnRemoveTool(Container::Message& msg);
};

#endif