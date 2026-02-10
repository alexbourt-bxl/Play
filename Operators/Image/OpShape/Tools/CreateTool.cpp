#include "CreateTool.h"

#include <Controls\Views\View.h>

DEFINE_TYPE(OpShape::CreateTool, Tool);

OpShape::CreateTool::CreateTool()
{
	m_button.Icon = &s_imgCreate;
	m_button.PressEvent += EventHandler(this, &OpShape::CreateTool::button_OnPress);

	ControlLayout.Controls.Add(&m_button);
}

void OpShape::CreateTool::Select()
{
	m_button.Selected = true;
}

void OpShape::CreateTool::Deselect()
{
	m_button.Selected = false;
}

void OpShape::CreateTool::ConnectViewProtected(View* view)
{
	view->LButtonDownEvent += EventHandler(this, &OpShape::CreateTool::OnLButtonDown);
	view->LButtonUpEvent   += EventHandler(this, &OpShape::CreateTool::OnLButtonUp);
	view->MouseMoveEvent   += EventHandler(this, &OpShape::CreateTool::OnMouseMove);
	//view->KeyDownEvent   += EventHandler(this, &OpShape::CreateTool::OnKeyDown);
	//view->KeyUpEvent     += EventHandler(this, &OpShape::CreateTool::OnKeyUp);
}

void OpShape::CreateTool::DisconnectViewProtected(View* view)
{
	view->LButtonDownEvent -= EventHandler(this, &OpShape::CreateTool::OnLButtonDown);
	view->LButtonUpEvent   -= EventHandler(this, &OpShape::CreateTool::OnLButtonUp);
	view->MouseMoveEvent   -= EventHandler(this, &OpShape::CreateTool::OnMouseMove);
	//view->KeyDownEvent   -= EventHandler(this, &OpShape::CreateTool::OnKeyDown);
	//view->KeyUpEvent     -= EventHandler(this, &OpShape::CreateTool::OnKeyUp);
}