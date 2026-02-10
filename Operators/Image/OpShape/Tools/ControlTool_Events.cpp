#include "ControlTool.h"

#include <Tools\ToolArray.h>
#include <Controls\Views\ImageView\ImageView.h>
#include <Operators\Tree\Tree.h>
#include <Types/Geometric\FRect.h>

void OpShape::ControlTool::button_OnPress(Control::Message& msg)
{
	Container.SetActiveTool(this);
}

void OpShape::ControlTool::OnPaint(Graphics::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	Rect paintRect = view->PaintRect;

	if (m_selecting && m_selectRect)
	{
		Rect selectRect = m_selectRect;
		selectRect.Offset(paintRect.Position);
		selectRect.Normalize();

		Rect shadowRect = selectRect;
		
		shadowRect.X -= 1;	shadowRect.Width  += 2;
		shadowRect.Y -= 1;	shadowRect.Height += 2;

		msg.Graphics.DrawRect(
			shadowRect,
			Color(Color::Black, 0.15));

		msg.Graphics.FillRect(
			selectRect,
			Color(Color::PlayYellow, 0.25));

		msg.Graphics.DrawRect(
			selectRect,
			Color(Color::PlayYellow, 0.5));
	}
}

void OpShape::ControlTool::OnLButtonDown(Mouse::Message& msg)
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

	m_opShape->m_moving = hit;

	if (!hit)
	{
		m_selecting = true;
		m_selectRect.Position = msg.Position;
		m_selectRect.Size = Size(0, 0);
	}
	else if (hit.PointId > -1 && hit.Handle)
	{
		Curve::Point& cp = m_opShape->_Shape.Curves[hit.CurveId].Points[hit.PointId];

		if (hit.Handle > 0)
			m_opShape->m_handleLength = Math::GetDistance(cp, cp.In); // opposite of >0
		else if (hit.Handle < 0)
			m_opShape->m_handleLength = Math::GetDistance(cp, cp.Out); // opposite of <0
	}

	m_opShape->MarkDirty();
	m_opShape->Tree->Update(m_opShape);
}

void OpShape::ControlTool::OnLButtonUp(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;
	HitResult hit = m_opShape->HitTest(view, msg.Position);

	if (m_selecting)
	{
		double hitSize = m_opShape->m_hitSize / 2 / view->Zoom;

		if (!Key::Shift.IsDown)
			m_opShape->m_selected.Clear();

		FRect selectRect = m_selectRect;
		view->ViewToImage(&selectRect);
		selectRect.Normalize();

		for (int i = 0; i < m_opShape->_Shape.Curves.Count; i++)
		{
			Curve& curve = m_opShape->_Shape.Curves[i];

			for (int j = 0; j < curve.Points.Count; j++)
			{
				Curve::Point& p = curve.Points[j];

				FRect pRect(
					p.X - hitSize, p.Y - hitSize,
					hitSize * 2, hitSize * 2);

				if (selectRect.Intersects(pRect))
					m_opShape->m_selected.Add(HitResult(i, j));
			}
		}

		UpdateSelectionControls();
		view->Update();
	}
	else if (!m_opShape->m_moved)
	{
		if (!Key::Shift.IsDown)
			m_opShape->m_selected.Clear();

		if (hit && !m_opShape->m_selected.Contains(hit))
			m_opShape->m_selected.Add(hit);

		UpdateSelectionControls();
		view->Update();
	}

	m_opShape->m_moving = null;
	m_selecting = false;

	m_opShape->m_down  = false;
	m_opShape->m_moved = false;
}

void OpShape::ControlTool::OnMouseMove(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	FPoint p = msg.Position;
	view->ViewToImage(&p);

	FPoint pp = msg.PrevPosition;
	view->ViewToImage(&pp);

	HitResult hit = m_opShape->HitTest(view, msg.Position);

	view->Cursor = Cursor::Move;

	if (m_selecting)
	{
		Region updateRgn;
		updateRgn.Add(m_opShape->GetViewRect(view, m_selectRect, 2));

		m_selectRect.Width  = msg.Position.X - m_selectRect.X;
		m_selectRect.Height = msg.Position.Y - m_selectRect.Y;

		updateRgn.Add(m_opShape->GetViewRect(view, m_selectRect, 2));
		view->Update(updateRgn);
	}
	else if (m_opShape->m_down)
		m_opShape->MoveSelection(view, m_opShape->m_moving, p, pp);
}

void OpShape::ControlTool::OnKeyDown(Key::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	FPoint offset(0, 0);

	if (msg.Key == Key::Left)
		offset.X = -1 / view->Zoom;
	else if (msg.Key == Key::Right)
		offset.X = 1 / view->Zoom;
	else if (msg.Key == Key::Up)
		offset.Y = -1 / view->Zoom;
	else if (msg.Key == Key::Down)
		offset.Y = 1 / view->Zoom;

	for (int i = 0; i < m_opShape->m_selected.Count; i++)
	{
		HitResult& hit = m_opShape->m_selected[i];

		if (hit.PointId > -1) // point
		{
			m_opShape->_Shape.Curves[hit.CurveId].Points[hit.PointId] += offset;
		}
		else // curve
		{
			Curve& curve = m_opShape->_Shape.Curves[hit.CurveId];

			for (int j = 0; j < curve.Points.Count; j++)
				curve.Points[j] += offset;
		}
	}

	view->Update();

	m_opShape->MarkDirty();
	m_opShape->Tree->Update(m_opShape);
}

//void OpShape::ControlTool::OnKeyUp(Key::Message& msg)
//{
//
//}

void OpShape::ControlTool::btnVertex_OnSelectItem(::Message& msg)
{
	Curve::Point::_Joint newJoint = Curve::Point::Broken;

	switch (m_btnVertex.SelectedIndex)
	{
	case 0:
		newJoint = Curve::Point::Broken;
		break;
	case 1:
		newJoint = Curve::Point::Sliding;
		break;
	case 2:
	default:
		newJoint = Curve::Point::Locked;
		break;
	}

	for (int i = 0; i < m_opShape->m_selected.Count; i++)
	{
		if (m_opShape->m_selected[i].PointId > -1)
		{
			Curve::Point& p = m_opShape->_Shape.Curves[m_opShape->m_selected[i].CurveId].Points[m_opShape->m_selected[i].PointId];
			
			p.Joint = newJoint;

			//

			double inLength  = Math::GetDistance(p, p.In);
			double outLength = Math::GetDistance(p, p.Out);

			double inAngle   = Math::GetAngle(p.In, p);
			double outAngle  = Math::GetAngle(p, p.Out);

			double diffAngle = Math::GetAngleDifference(inAngle, outAngle);

			double angle = 0;

			if (inLength > 0 && outLength == 0)  
				angle = inAngle;
			else if (inLength == 0 && outLength > 0) 
				angle = outAngle;
			else
				angle = inAngle + diffAngle / 2;

			if (angle > 2 * PI)	
				angle -= 2 * PI; // limit to one revolution

			//

			if (newJoint == Curve::Point::Locked)
			{
				double avgLength = (inLength + outLength) / 2;

				// Average the handle lengths and angles

				p.In = FPoint(
					p.X - avgLength * cos(angle),
					p.Y - avgLength * sin(angle));
				
				p.Out = FPoint(
					p.X + avgLength * cos(angle),
					p.Y + avgLength * sin(angle));

			}
			else if (newJoint == Curve::Point::Sliding)
			{
				// Average the angles only

				p.In = FPoint(
					p.X - inLength * cos(angle),
					p.Y - inLength * sin(angle));
				
				p.Out = FPoint(
					p.X + outLength * cos(angle),
					p.Y + outLength * sin(angle));
			}
		}
	}

	//ImageView* view = (ImageView*)msg.Object;
	//view->Update();
}