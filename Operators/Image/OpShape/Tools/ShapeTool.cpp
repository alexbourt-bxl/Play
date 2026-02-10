#include "ShapeTool.h"

#include <Controls\Views\View.h>
#include <Controls\Views\ImageView\ImageView.h>
#include <Operators\Tree\Tree.h>

DEFINE_TYPE(OpShape::ShapeTool, Tool);

OpShape::ShapeTool::ShapeTool()
{
	m_button.DefaultToLast = true;
	m_button.AddItem(L"", &s_imgEllipse);
	m_button.AddItem(L"", &s_imgRect);
	m_button.AddItem(L"", &s_imgPolygon);
	m_button.SelectItemEvent += EventHandler(this, &OpShape::ShapeTool::button_OnSelectItem);

	m_valEdges.Width = 80;
	m_valEdges.ValueName = L"Edges";
	m_valEdges.Value = 3;
	m_valEdges.MinValue = 3; m_valEdges.MinVisible = 0;
	m_valEdges.MaxVisible = 16;
	m_valEdges.Relative = true;
	m_valEdges.Granularity = 1;

	ControlLayout.Controls.Add(&m_button);

	m_tool = 0;

	m_proportional = false;
	m_centerPivot  = false;
}

void OpShape::ShapeTool::Select()
{
	m_button.Selected = true;

	switch (m_button.SelectedIndex)
	{
	case 0:
		m_tool = EllipseTool;
		break;

	case 1: 
		m_tool = RectTool;
		break;

	case 2:
		m_tool = PolygonTool;
		ParamLayout.Controls.Add(&m_valEdges);
		break;
	}
}

void OpShape::ShapeTool::Deselect()
{
	m_button.Selected = false;

	ParamLayout.Controls.Remove(&m_valEdges);
}

void OpShape::ShapeTool::ConnectViewProtected(View* view)
{
	view->LButtonDownEvent += EventHandler(this, &OpShape::ShapeTool::OnLButtonDown);
	view->LButtonUpEvent   += EventHandler(this, &OpShape::ShapeTool::OnLButtonUp);
	view->MouseMoveEvent   += EventHandler(this, &OpShape::ShapeTool::OnMouseMove);
	view->KeyDownEvent     += EventHandler(this, &OpShape::ShapeTool::OnKeyDown);
	view->KeyUpEvent       += EventHandler(this, &OpShape::ShapeTool::OnKeyUp);
}

void OpShape::ShapeTool::DisconnectViewProtected(View* view)
{
	view->LButtonDownEvent -= EventHandler(this, &OpShape::ShapeTool::OnLButtonDown);
	view->LButtonUpEvent   -= EventHandler(this, &OpShape::ShapeTool::OnLButtonUp);
	view->MouseMoveEvent   -= EventHandler(this, &OpShape::ShapeTool::OnMouseMove);
	view->KeyDownEvent     -= EventHandler(this, &OpShape::ShapeTool::OnKeyDown);
	view->KeyUpEvent       -= EventHandler(this, &OpShape::ShapeTool::OnKeyUp);
}

void OpShape::ShapeTool::UpdateCreatedCurve(ImageView* view, FPoint& p)
{
	if (!m_opShape->m_moved)
	{
		m_opShape->_Shape.Curves.Add(Curve());
		m_opShape->m_moved = true;
	}

	Curve curve = m_unitCurve;

	FPoint po = m_dragStart;
	view->ViewToImage(&po);

	FPoint ps = p - po;
	FPoint pc(0, 0);

	if (m_proportional)
	{
		ps = FPoint(
			max(p.X - po.X, p.Y - po.Y),
			max(p.X - po.X, p.Y - po.Y));
	}

	if (m_centerPivot)
	{
		pc = FPoint(0.5, 0.5);
		ps *= 2;
	}

	// TODO: placement problem with centerPivot at large zooms (jumps to pixel borders)

	Math::Transform(curve, po, 0, ps, &pc);

	m_opShape->LockProps();

	m_opShape->_Shape.Curves.RemoveAt(m_opShape->_Shape.Curves.Count - 1);
	m_opShape->_Shape.Curves.Add(curve);

	m_opShape->m_selected.Clear();
	m_opShape->m_selected.Add(HitResult(m_opShape->_Shape.Curves.Count - 1, -1));

	m_opShape->UnlockProps();

	view->Update();

	m_opShape->MarkDirty();
	m_opShape->Tree->Update(m_opShape);
}