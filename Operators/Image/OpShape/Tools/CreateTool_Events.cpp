#include "CreateTool.h"

#include <Tools\ToolArray.h>
#include <Controls\Views\ImageView\ImageView.h>
#include <Operators\Tree\Tree.h>

void OpShape::CreateTool::button_OnPress(Control::Message& msg)
{
	Container.SetActiveTool(this);
}

void OpShape::CreateTool::OnLButtonDown(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	FPoint p(
		msg.Position.X,
		msg.Position.Y);

	view->ViewToImage(&p);

	//LockProps();

	m_opShape->m_down = true;
	m_opShape->m_moved = false;

	HitResult hit = m_opShape->HitTest(view, msg.Position);

	int curveId = -1;

	/*	If only one last point of an open curve is 
		selected, continue creation of that curve. */
		
	if (m_opShape->m_selected.Count == 1 
		&& !m_opShape->_Shape.Curves[m_opShape->m_selected[0].CurveId].Closed)
		//&& m_selected[0].PointId == _Shape.Curves[m_selected[0].CurveId].Points.Count - 1)
	{
		curveId = m_opShape->m_selected[0].CurveId;
	}

	/*	Otherwise create a new open curve. */
	
	else
	{
		// Create a new curve
		Curve newCurve;
		newCurve.Closed = false;

		m_opShape->_Shape.Curves.Add(newCurve);
		curveId = m_opShape->_Shape.Curves.Count - 1;
	}

	assert(curveId > -1);
	Curve& curve = m_opShape->_Shape.Curves[curveId];

	int pointId = -1;

	/*	If the cursor is over an existing point on the 
		curve, adjust the handles of that point. */

	if (hit.CurveId == curveId)
	{
		if (hit.PointId == 0) // close the curve if first point
			curve.Closed = true;

		pointId = hit.PointId;
		m_opShape->m_collapseHandles = true;
	}

	/*	Otherwise create a new point. */

	else
	{
		Curve::Point cp = p;
		cp.Joint = Curve::Point::Locked;

		curve.Points.Add(cp);
		pointId = curve.Points.Count - 1;
	}

	assert(pointId > -1);

	// Select the created point
	m_opShape->m_selected.Clear();
	m_opShape->m_selected.Add(HitResult(curveId, pointId));

	// Set the out handle as moving
	m_opShape->m_moving = HitResult(curveId, pointId, 1);

	view->Update();

	m_opShape->MarkDirty();
	m_opShape->Tree->Update(m_opShape);
}

void OpShape::CreateTool::OnLButtonUp(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;
	HitResult hit = m_opShape->HitTest(view, msg.Position);

	m_opShape->m_down  = false;
	m_opShape->m_moved = false;
	m_opShape->m_moving = null;
}

void OpShape::CreateTool::OnMouseMove(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	FPoint p = msg.Position;
	view->ViewToImage(&p);

	FPoint pp = msg.PrevPosition;
	view->ViewToImage(&pp);

	HitResult hit = m_opShape->HitTest(view, msg.Position);

	if (m_opShape->m_down)
	{
		view->Cursor = Cursor::Move;
		m_opShape->MoveSelection(view, m_opShape->m_moving, p, pp);
	}
	else if (hit.CurveId > -1 
		&& !m_opShape->_Shape.Curves[hit.CurveId].Closed)
	{
		if (hit.PointId == 0)
			view->Cursor = Cursor::Complete;
		else
			view->Cursor = Cursor::Adjust;
	}
	else
		view->Cursor = Cursor::Ghost;
}

//void OpShape::CreateTool::OnKeyDown(Key::Message& msg)
//{
//
//}
//
//void OpShape::CreateTool::OnKeyUp(Key::Message& msg)
//{
//
//}