#include "ControlTool.h"

#include <Controls\Views\View.h>

DEFINE_TYPE(OpShape::ControlTool, Tool);

OpShape::ControlTool::ControlTool()
{
	m_button.Icon = &s_imgControl;
	m_button.PressEvent += EventHandler(this, &OpShape::ControlTool::button_OnPress);
	
	m_btnVertex.Width = 100;
	m_btnVertex.IconOffset.X = 6;
	m_btnVertex.AddItem(L"Broken", &s_imgVertexBroken);
	m_btnVertex.AddItem(L"Sliding", &s_imgVertexSliding);
	m_btnVertex.AddItem(L"Locked", &s_imgVertexFixed);
	m_btnVertex.SelectItemEvent += EventHandler(this, &OpShape::ControlTool::btnVertex_OnSelectItem);

	ControlLayout.Controls.Add(&m_button);
	ParamLayout.Controls.Add(&m_btnVertex);

	m_selecting = false;
}

void OpShape::ControlTool::Select()
{
	m_button.Selected = true;
}

void OpShape::ControlTool::Deselect()
{
	m_button.Selected = false;
}

void OpShape::ControlTool::ConnectViewProtected(View* view)
{
	store_allowKeyRepeat = view->AllowKeyRepeat;
	view->AllowKeyRepeat = true;

	view->PaintEvent       += EventHandler(this, &OpShape::ControlTool::OnPaint);
	view->LButtonDownEvent += EventHandler(this, &OpShape::ControlTool::OnLButtonDown);
	view->LButtonUpEvent   += EventHandler(this, &OpShape::ControlTool::OnLButtonUp);
	view->MouseMoveEvent   += EventHandler(this, &OpShape::ControlTool::OnMouseMove);
	view->KeyDownEvent     += EventHandler(this, &OpShape::ControlTool::OnKeyDown);
	//view->KeyUpEvent     += EventHandler(this, &OpShape::ControlTool::OnKeyUp);
}

void OpShape::ControlTool::DisconnectViewProtected(View* view)
{
	view->PaintEvent       -= EventHandler(this, &OpShape::ControlTool::OnPaint);
	view->LButtonDownEvent -= EventHandler(this, &OpShape::ControlTool::OnLButtonDown);
	view->LButtonUpEvent   -= EventHandler(this, &OpShape::ControlTool::OnLButtonUp);
	view->MouseMoveEvent   -= EventHandler(this, &OpShape::ControlTool::OnMouseMove);
	view->KeyDownEvent     -= EventHandler(this, &OpShape::ControlTool::OnKeyDown);
	//view->KeyUpEvent     -= EventHandler(this, &OpShape::ControlTool::OnKeyUp);

	view->AllowKeyRepeat = store_allowKeyRepeat;
}

void OpShape::ControlTool::UpdateSelectionControls()
{
	bool started = false;
	bool mixed = false;

	Curve::Point::_Joint joint = Curve::Point::Broken;

	for (int i = 0; i < m_opShape->m_selected.Count; i++)
	{
		if (m_opShape->m_selected[i].PointId > -1)
		{
			if (!started)
			{
				joint = m_opShape->_Shape.Curves[m_opShape->m_selected[i].CurveId].Points[m_opShape->m_selected[i].PointId].Joint;
				started = true;
			}
			else if (m_opShape->_Shape.Curves[m_opShape->m_selected[i].CurveId].Points[m_opShape->m_selected[i].PointId].Joint != joint)
			{
				mixed = true;
				break;
			}
		}
	}

	m_btnVertex.DisableEvents();

	if (mixed)
	{
		m_btnVertex.Icon = &s_imgVertexMixed;
		m_btnVertex.Text = L"Mixed";
	}
	else
	{
		switch (joint)
		{
		case Curve::Point::Broken:
			m_btnVertex.SelectedIndex = 0;
			break;

		case Curve::Point::Sliding:		
			m_btnVertex.SelectedIndex = 1;
			break;

		case Curve::Point::Locked:
			m_btnVertex.SelectedIndex = 2;
			break;
		}
	}

	m_btnVertex.EnableEvents();
}