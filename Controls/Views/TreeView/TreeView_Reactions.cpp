#include "TreeView.h"

#include <Graphics/Graphics.h>
#include <Types/Graphics/Font.h>
#include <Types/Geometric/FRect.h>
#include <Operators/Operator.h>
#include <Operators/Input.h>
#include <Operators/Output.h>
#include <Operators/Connection.h>
#include <Controls/Views/ImageView/ImageView.h>
#include <Math/Math.h>

#include <Actions/ActionManager.h>
#include <Actions/GroupAction.h>
#include <Operators/Actions/ActMoveOps.h>
#include <Operators/Actions/ActConnect.h>
#include <Operators/Actions/ActDisconnect.h>

#include <Interface\PlayWindow\PlayWindow.h>
#include <limits>

void TreeView::OnPaint(Graphics::Message& msg)
{
	if (!m_tree)
		return;

	for (int i = 0; i < m_tree->Count; i++)
	{
		if (msg.Graphics.ClipRegion.Intersects(GetOpFullPaintRect((*m_tree)[i])))
			PaintOp(msg, (*m_tree)[i]);
	}

	for (int i = 0; i < m_tree->Count; i++)
	{
		// TODO: check for intersection with the clip region
		PaintOpConnections(msg, (*m_tree)[i]);
	}

	/*	Paint hot (pottential) connections. */

	if (m_hotInput && !m_hotOutput)
	{
		FPoint p0, p1, p2, p3;
		p0 = m_hotPoint;

		GetConnectionPoints(null, m_hotInput, &p0, &p1, &p2, &p3);
		
		PaintConnection(
			msg.Graphics, 
			p0, 
			p1, 
			p2, 
			p3, 
			false);
	}
	else if (m_hotOutput)
	{
		FPoint p0, p1, p2, p3;
		p3 = m_hotPoint;

		GetConnectionPoints(m_hotOutput, null, &p0, &p1, &p2, &p3);
		
		PaintConnection(
			msg.Graphics, 
			p0, 
			p1, 
			p2, 
			p3, 
			true); 
	}
}

void TreeView::OnLButtonDown(Mouse::Message& msg)
{
	m_moved = false;
	m_offsetStart.Clear();

	Region updateRgn;

	Object* obj = HitTestOp(msg.Position);

	if (obj && typeof(obj) == TYPEOF(Input))
	{
		if (((Input*)obj)->Connection)
		{
			m_hotInput = (Input*)obj;
			m_hotOutput = ((Input*)obj)->Connection->Out;
		}
		else
		{
			m_hotInput = (Input*)obj;
			m_hotOutput = null;
		}

		m_option = Connecting;

		updateRgn.Add(GetOpFullPaintRect(m_hotInput->GetOperator()));
		Update(updateRgn);
	}
	else if (obj && typeof(obj) == TYPEOF(Output))
	{
		/*	NOTE: ignore that thought about moving multiple connected inputs
			at once like hot air balloons. This is impossible because a new
			potential connection should always be created from an output. */

		/*	TODO: on the other hand, moving hot air balloons could be possible
			by holding down the mouse button a bit, all of the corresponding
			inputs becoming live. */

		m_hotInput = null;
		m_hotOutput = (Output*)obj;

		m_option = Connecting;

		updateRgn.Add(GetOpPaintRect(m_hotOutput->GetOperator()));
		Update(updateRgn);
	}
	else if (obj && typeof(obj) == TYPEOF(Operator))
	{
		Operator* op = (Operator*)obj;

		// Prepare events
		Tree::Message te;
		te.Operators = &m_tree->Selected;

		// Fire pre event
		m_tree->PreUpdateEvent.Send(te);

		// Update selection

		if (!op->IsSelected
			&& !Key::Shift.IsDown 
			&& !Key::Control.IsDown)
		{
			m_tree->Selected.Clear();
		}

		if (op->IsSelected)
		{
			if (Key::Control.IsDown)
				op->IsSelected = false;
		}
		else
		{
			op->IsSelected = true;
			m_tree->SetIndexOf(op, m_tree->Count - 1);
		}

		// Fire post event
		m_tree->UpdateEvent.Send(te);

		m_moveStart = msg.Position;
		m_option = Moving;
	}

	/*	Nothing was clicked. */

	else if (!Key::Shift.IsDown && !Key::Control.IsDown)
	{
		// Prepare events
		Tree::Message te;
		te.Operators = &m_tree->Selected;

		// Fire pre event
		m_tree->PreUpdateEvent.Send(te);

		// Clear selection
		m_tree->Selected.Clear();

		// Fire post event
		m_tree->UpdateEvent.Send(te);
	}
}

/*	TODO:

	for touch screens, if two objects overlap each other, putting two fingers on
	overlapping objects and then pinch-stretching could just move the two objects
	apart. However if the intention was to scale then that has to be taken care of
	somehow. */

void TreeView::OnLButtonUp(Mouse::Message& msg)
{
	Object* obj = HitTestOp(Point(
		(int)m_hotPoint.X - PaintRect.X,
		(int)m_hotPoint.Y - PaintRect.Y));

	Region updateRgn;

	if (m_hotOutput)
	{
		//if (m_hotInput)
		//{
		//	Operator* op = m_hotInput->GetOperator();
		//	_Rect paintRectOld = GetOpPaintRect(op);
		//	updateRgn.Add(GetOpRegion(op));

		//	//m_hotInput->Disconnect();
		//	Action::Perform(new ActDisconnect(m_hotInput));

		//	m_hotInput = null;

		//	_Rect paintRectNew = GetOpPaintRect(op);

		//	op->Position = op->Position + FPoint(
		//		(double)(paintRectOld.Width - paintRectNew.Width) / 2.0,
		//		(double)(paintRectOld.Height - paintRectNew.Height) / 2.0);

		//	updateRgn.Add(GetOpRegion(op));
		//}

		if (obj && typeof(obj) == TYPEOF(Input))
		{
			Input* in = (Input*)obj;

			Operator* op = in->GetOperator();
			_Rect paintRectOld = GetOpPaintRect(op);
			updateRgn.Add(GetOpRegion(op));

			if (in != m_hotInput) // not reconnecting to same input
			{
				GroupAction* action = new GroupAction();

				if (m_hotInput) // moving connection
					action->Add(new ActDisconnect(m_hotInput));

				action->Add(new ActConnect(m_hotOutput, in));
				Action::Perform(action);
			}

			_Rect paintRectNew = GetOpPaintRect(op);

			op->Position = op->Position + FPoint(
				(double)(paintRectOld.Width - paintRectNew.Width) / 2.0,
				(double)(paintRectOld.Height - paintRectNew.Height) / 2.0);

			updateRgn.Add(GetOpRegion(op));
		}

		updateRgn.Add(GetOpPaintRect(m_hotOutput->GetOperator()));
		updateRgn.Add(GetHotOutputRect(m_hotOutput, FPoint((double)msg.Position.X, (double)msg.Position.Y)));
		Update(updateRgn);
	}
	else if (m_hotInput)
	{
		if (obj && typeof(obj) == TYPEOF(Output))
		{
			Output* out = (Output*)obj;

			Operator* opIn = m_hotInput->GetOperator();
			_Rect paintRectOld = GetOpPaintRect(opIn);
			updateRgn.Add(GetOpRegion(opIn));

			out->Connect(m_hotInput);
			_Rect paintRectNew = GetOpPaintRect(opIn);
			
			//opIn->m_positionY += (double)(paintRectOld.Height - paintRectNew.Height) / 2.0; // don't think this should move
			
			updateRgn.Add(GetOpRegion(opIn));
		}

		updateRgn.Add(GetOpFullPaintRect(m_hotInput->GetOperator()));
		updateRgn.Add(GetHotInputRect(FPoint((double)msg.Position.X, (double)msg.Position.Y), m_hotInput));
		Update(updateRgn);
	}
	else if (obj && typeof(obj) == TYPEOF(Operator))
	{
		Operator* op = (Operator*)obj;

		//if (!m_moved 
		//	&& !Key::Shift.IsDown 
		//	&& !Key::Control.IsDown)
		//{
		//	//for (int i = 0; i < m_tree->Selected.Count; i++)
		//	//	updateRgn.Add(GetOpFullPaintRect(m_tree->Selected[i]));

		//	//m_tree->Selected.Clear();
		//	//m_offsetStart.Clear();

		//	//m_tree->Selected.Add(op);
		//	//m_offsetStart.Add(op->Position);

		//	//updateRgn.Add(GetOpFullPaintRect(op));
		//	//Update(updateRgn);
		//}
		//else // finished moving

		if (m_moved)
		{
			ActMoveOps* act = new ActMoveOps();

			FPoint offset(
				(double)(msg.Position.X - m_moveStart.X),
				(double)(msg.Position.Y - m_moveStart.Y));

			for (int i = 0; i < m_tree->Selected.Count; i++)
			{
				act->Operators.Add(m_tree->Selected[i]);
				act->Offsets.Add(offset);

				/*	Return the operator to its initial place
					before the action moves it again. */
				m_tree->Selected[i]->Position -= offset;
			}

			Action::Perform(act);
		}
	}
	else if (!Key::Shift.IsDown
		&& !Key::Control.IsDown)
	{
		for (int i = 0; i < m_tree->Selected.Count; i++)
			updateRgn.Add(GetOpFullPaintRect(m_tree->Selected[i]));
		
		m_tree->Selected.Clear();
		m_offsetStart.Clear();

		Update(updateRgn);
	}

	m_hotInput = null;
	m_hotOutput = null;

	m_option = None;
}

void TreeView::OnMouseMove(Mouse::Message& msg)
{
	Region updateRgn;
	
	if (m_option == Moving)
	{
		m_prevPoint = m_hotPoint;
		m_hotPoint = GetHotPoint(msg.Position);

		if (!m_moved)
		{
			for (int i = 0; i < m_tree->Selected.Count; i++)
				m_offsetStart.Add(m_tree->Selected[i]->Position);
		
			m_moved = true;
		}

		// Prepare events
		Tree::Message te;
		te.Operators = &m_tree->Selected;

		// Fire pre event
		m_tree->PreUpdateEvent.Send(te);

		/*	Move the ops. */

		assert(m_tree->Selected.Count == m_offsetStart.Count);

		for (int i = 0; i < m_tree->Selected.Count; i++)
		{
			m_tree->Selected[i]->Position = FPoint(
				m_offsetStart[i].X + (double)(msg.Position.X - m_moveStart.X), 
				m_offsetStart[i].Y + (double)(msg.Position.Y - m_moveStart.Y));
		}

		// Fire post event
		m_tree->UpdateEvent.Send(te);
	}
	else if (m_option == Connecting)
	{
		if (m_hotInput && !m_hotOutput)
		{
			m_prevPoint = m_hotPoint;
			m_hotPoint = GetHotPoint(msg.Position);

			updateRgn.Add(GetHotInputRect(m_prevPoint, m_hotInput));
			updateRgn.Add(GetHotInputRect(m_hotPoint, m_hotInput));
			Update(updateRgn);
		}
		else if (m_hotOutput)
		{
			m_prevPoint = m_hotPoint;
			m_hotPoint = GetHotPoint(msg.Position);

			updateRgn.Add(GetHotOutputRect(m_hotOutput, m_prevPoint));
			updateRgn.Add(GetHotOutputRect(m_hotOutput, m_hotPoint));
			Update(updateRgn);
		}
	}
	else
	{
		m_prevPoint = m_hotPoint;
		m_hotPoint = GetHotPoint(msg.Position);
	}
}

void TreeView::OnLDoubleClick(Mouse::Message& msg)
{
	Object* obj = HitTestOp(msg.Position);

	if (obj && typeof(obj) == TYPEOF(Operator))
	{
		PlayWindow* wnd = (PlayWindow*)Window;
		Operator* op = (Operator*)obj;
		wnd->ShowOp(op);
	}
}

void TreeView::OnLButtonHold(Mouse::Message& msg)
{
	Object* obj = HitTestOp(msg.Position);

	if (obj && typeof(obj) == TYPEOF(Operator))
	{
		PlayWindow* wnd = (PlayWindow*)Window;
		Operator* op = (Operator*)obj;

		View* view = wnd->RequestView(op);

		if (!view || typeof(view) != TYPEOF(OpView))
			return;

		Region updateRgn;

		/*	Assign the view to the operator. */

		OpView* opView = (OpView*)view;

		updateRgn.Add(GetOpFullPaintRect(opView->ActiveOperator));

		opView->ActiveOperator = op;

		//wnd->m_propView.SetPropertyContainer(op, typeof(op).DisplayName);

		updateRgn.Add(GetOpFullPaintRect(op));
		Update(updateRgn);
	}
}

void TreeView::OnKeyDown(Key::Message& msg)
{
	if (msg.Key == Key::Delete || msg.Key == Key::Backspace)
		DeleteSelected();
	else if (msg.Key == Key::A && Key::Control.IsDown)
		SelectAll();
	else if (msg.Key == Key::D && Key::Control.IsDown)
		DeselectAll();
}

void TreeView::tree_OnAddOperator(Container::Message& msg)
{
	Operator* op = *((Operator**)msg.Item);

	/*	Determine operator placement in the tree. If the op
		has been placed, leave it alone. */
	
	if (!_isnan(op->Position.X) && !_isnan(op->Position.Y))
		return;
	
	/*	If there are no ops, place the new one at 1/5 from the left
		and 1/3 from the top of the active view. */

	if (m_tree->Count == 0)
	{
		_Rect rect = ClientRect;

		op->Position = FPoint(
			rect.Width / 5, 
			rect.Height / 3);
	}
	else
	{
		/*	If one op is selected, place the new one to the right. */

		if (m_tree->Selected.Count == 1)
		{
			Operator* selectedOp = m_tree->Selected[0];

			FSize selSize = GetOpSize(selectedOp);
			FSize newSize = GetOpSize(op);

			FRect newRect(
				selectedOp->Position.X + selSize.Width + m_placementGap.X,
				selectedOp->Position.Y + (selSize.Height - newSize.Height) / 2,
				newSize.Width,
				newSize.Height);

			op->Position = newRect.Position;

			/*	TODO: if the new op overlaps an existing op, move it lower. Alternatively,
				add it as the lowest outgoing connection of the input op. */
		}

		/*	Otherwise left-align it below the lowest op. */

		else
		{
			FRect bounds = GetOpBounds(*m_tree);

			op->Position = FPoint(
				bounds.Left,
				bounds.Bottom + m_placementGap.Y);
		}
	}
}

void TreeView::tree_PreUpdate(Tree::Message& msg)
{
	if (msg.Operators)
	{
		Array<Operator*>& ops = *msg.Operators;

		for (int i = 0; i < ops.Count; i++)
			m_rgnUpdate.Add(GetOpFullPaintRect(ops[i]));
	}

	if (msg.Connections)
	{
		Array<Connection*>& connections = *msg.Connections;

		for (int i = 0; i < connections.Count; i++)
			m_rgnUpdate.Add(GetConnectionRegion(connections[i]));
	}
}

void TreeView::tree_OnUpdate(Tree::Message& msg)
{
	if (msg.Operators)
	{
		Array<Operator*>& ops = *msg.Operators;

		for (int i = 0; i < ops.Count; i++)
			m_rgnUpdate.Add(GetOpFullPaintRect(ops[i]));
	}

	if (msg.Connections)
	{
		Array<Connection*>& connections = *msg.Connections;

		for (int i = 0; i < connections.Count; i++)
			m_rgnUpdate.Add(GetConnectionRegion(connections[i]));
	}

	Update(m_rgnUpdate);
	m_rgnUpdate.Clear();
}