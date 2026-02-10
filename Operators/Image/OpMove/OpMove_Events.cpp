#include "OpMove.h"

#include <Controls\Views\ImageView\ImageView.h>

void OpMove::OnLButtonDown(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;
	assert(view);

	Image* in = m_in.Data.Get<Image>(Tag::Image);

	if (!in)
		return; 

	Rect rect(
		m_x, m_y, 
		in->Width, in->Height);
	
	Rect pickRect = rect;

	/*	Make sure any previous offset is taken into account
		when performing the hit test. Note: previous offsets
		are not taken into account when the actual offset is
		changed. */
	
	/*	Trace the path to the viewed op and sum all the offsets.
		If it's being viewed downstream then all the ops between 
		the active one and viewed one will have already been updated, 
		and so their outputs can be relied on. */

	//if (view->ViewedOperator->IsAfter(this))
	//{
	//	Point offsetAfter(0, 0);

	//	Operator* op = this;

	//	while (op)
	//	{
	//		/*	TODO: for simplicity's sake, I only consider the
	//			first output connection (first made, not visibly
	//			topmost). */
	//		
	//		if (op->Out.Count > 0 
	//			&& op->Out[0]->Connections.Count > 0
	//			&& op->Out[0]->Connections[0]->In->Operator->IsAfter(op))
	//		{
	//			Operator* _op = op->Out[0]->Connections[0]->In->Operator;
	//			
	//			if (_op->Out.Count > 0)
	//			{
	//				Point* offsetNext = _op->Out[0]->Data.Get<Point>(Tag::ImageOffset);

	//				if (offsetNext)
	//					offsetAfter += *offsetNext;
	//			}

	//			op = _op;
	//		}
	//		else
	//		{
	//			op = null;
	//		}
	//	}

	//	pickRect.Offset(offsetAfter);
	//}
	//else
	//{
		Point* offset = m_in.Data.Get<Point>(Tag::ImageOffset);
		
		if (offset) 
			pickRect.Offset(*offset);
	//}

	//

	view->ImageToView(&rect);
	view->ImageToView(&pickRect);

	if (pickRect.Contains(msg.Position))
	{
		m_moving = true;
		m_moveOffset = msg.Position - rect.Position;
	}
}

void OpMove::OnLButtonUp(Mouse::Message& )
{
	m_moving = false;
}

void OpMove::OnMouseMove(Mouse::Message& msg)
{
	if (m_moving)
	{
		ImageView* view = (ImageView*)msg.Object;

		Point p = msg.Position - m_moveOffset;

		view->ViewToImage(&p);

		// TODO: single action
		X = p.X;
		Y = p.Y;
	}
}

void OpMove::OnKeyDown(Key::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	int offset = max(1, (int)(1 / view->Zoom));

	if (msg.Key == Key::Left)
		X = X - offset;
	else if (msg.Key == Key::Right)
		X = X + offset;
	else if (msg.Key == Key::Up)
		Y = Y - offset;
	else if (msg.Key == Key::Down)
		Y = Y + offset;
}