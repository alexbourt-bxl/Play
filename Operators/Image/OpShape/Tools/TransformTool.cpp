#include "TransformTool.h"

#include <Controls\Views\View.h>

DEFINE_TYPE(OpShape::TransformTool, Tool);

OpShape::TransformTool::TransformTool()
{
	m_button.Icon = &s_imgTransform;
	m_button.PressEvent += EventHandler(this, &OpShape::TransformTool::button_OnPress);

	ControlLayout.Controls.Add(&m_button);
}

void OpShape::TransformTool::Select()
{
	m_button.Selected = true;
}

void OpShape::TransformTool::Deselect()
{
	m_button.Selected = false;
}

void OpShape::TransformTool::ConnectViewProtected(View* view)
{
	view->LButtonDownEvent += EventHandler(this, &OpShape::TransformTool::OnLButtonDown);
	view->LButtonUpEvent   += EventHandler(this, &OpShape::TransformTool::OnLButtonUp);
	view->MouseMoveEvent   += EventHandler(this, &OpShape::TransformTool::OnMouseMove);
	view->KeyDownEvent     += EventHandler(this, &OpShape::TransformTool::OnKeyDown);
	view->KeyUpEvent       += EventHandler(this, &OpShape::TransformTool::OnKeyUp);
}

void OpShape::TransformTool::DisconnectViewProtected(View* view)
{
	view->LButtonDownEvent -= EventHandler(this, &OpShape::TransformTool::OnLButtonDown);
	view->LButtonUpEvent   -= EventHandler(this, &OpShape::TransformTool::OnLButtonUp);
	view->MouseMoveEvent   -= EventHandler(this, &OpShape::TransformTool::OnMouseMove);
	view->KeyDownEvent     -= EventHandler(this, &OpShape::TransformTool::OnKeyDown);
	view->KeyUpEvent       -= EventHandler(this, &OpShape::TransformTool::OnKeyUp);
}