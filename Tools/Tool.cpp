#include "Tool.h"

#include <Tools\ToolArray.h>
#include <Controls\Views\View.h>

DEFINE_TYPE(Tool, Object);

Tool::Tool()
{
	Init();
}

Tool::Tool(const Tool& /*tool*/)
{
	Init();
}

Tool::~Tool()
{
	DisconnectViews();
}

Object* Tool::Copy()
{
	return new Tool(*this);
}

void Tool::Init()
{
	m_container = null;

	m_controlLayout.Orientation = LineLayout::Horizontal;
	m_controlLayout.AutoSize = true;
	m_controlLayout.ClipChildren = false;

	m_paramLayout.Orientation = LineLayout::Horizontal;
	m_paramLayout.AutoSize = true;
	m_paramLayout.ClipChildren = false;
}

LineLayout& Tool::GetControlLayout()
{
	return m_controlLayout;
}

LineLayout& Tool::GetParamLayout()
{
	return m_paramLayout;
}

Array<View*>& Tool::GetConnectedViews()
{
	return m_connectedViews;
}

void Tool::Select()
{
	//
}

void Tool::Deselect()
{
	//
}

ToolArray& Tool::GetContainer()
{
	assert(m_container);
	return *m_container;
}

void Tool::ConnectView(View* view)
{
	m_connectedViews.Add(view);
	ConnectViewProtected(view);
}

void Tool::DisconnectViews()
{
	for (int i = m_connectedViews.Count - 1; i >= 0; i--)
		DisconnectViewProtected(m_connectedViews[i]);

	m_connectedViews.Clear();
}

void Tool::ConnectViewProtected(View* /*view*/)
{
	//
}

void Tool::DisconnectViewProtected(View* /*view*/)
{
	//
}