#include "OpPaint.h"

#include <Controls\Views\ImageView\ImageView.h>
#include <Operators\Tree\Tree.h>

void OpPaint::in_OnConnect(Input::Message& )
{
	Image* in = m_in.Data.Get<Image>(Tag::Image);
	assert(typeof(in) == TYPEOF(Image8));

	if (!m_imgPaint)
	{
		m_imgPaint = new Image8(in->Size);
		m_imgErase = new Image8(in->Size);
	}
	else
	{
		m_imgPaint->SetSize(in->Size);
		m_imgErase->SetSize(in->Size);
	}
}

//void OpPaint::in_OnDisconnect(Input::Message& msg)
//{
//
//}

void OpPaint::OnPaint(Graphics::Message& /*msg*/)
{

}

void OpPaint::OnLButtonDown(Mouse::Message& /*msg*/)
{
	if (!m_currentBrush)
		return;

	m_moved = false;

	Image* in = m_in.Data.Get<Image>(Tag::Image);
	assert(typeof(in) == TYPEOF(Image8));

	m_currentBrush->Down(
		m_imgPaint,
		m_imgErase,
		in,
		BrushColor);
}

void OpPaint::OnLButtonUp(Mouse::Message& /*msg*/)
{
	if (!m_currentBrush)
		return;

	// TODO: make sure the current brush doesn't change while it's down
	
	m_currentBrush->Up();
}

void OpPaint::OnMouseMove(Mouse::Message& msg)
{
	if (!m_currentBrush)
		return;

	if (!m_moved)
	{
		::QueryPerformanceCounter(&m_lastPerfCount);
		m_moved = true;
	}

	ImageView* view = (ImageView*)msg.Object;

	FPoint p = msg.Position;
	view->ViewToImage(&p);

	// Offset brush position by the offset
	Point* offset = m_in.Data.Get<Point>(Tag::ImageOffset);

	if (offset)
		p -= FPoint(offset->X, offset->Y);

	LARGE_INTEGER perfCount;
	::QueryPerformanceCounter(&perfCount);

	double dTime = (double)(perfCount.QuadPart - m_lastPerfCount.QuadPart) / (double)m_perfFrequency.QuadPart;

	m_lastPerfCount = perfCount;

	//

	m_currentBrush->Move(p, 1, 0, 90, 0, dTime);
}

void OpPaint::OnPenMove(Tablet::Message& msg)
{
	if (!m_currentBrush)
		return;

	ImageView* view = (ImageView*)msg.Object;

	Array<StrokePoint> points;

	Point* offset = m_in.Data.Get<Point>(Tag::ImageOffset);

	for (int i = 0; i < msg.Points.Count; i++)
	{
		Tablet::Message::Point& tp = msg.Points[i];

		StrokePoint sp(
			tp.X, tp.Y,
			tp.Pressure,
			tp.Azimuth,
			tp.Elevation,
			tp.Twist,
			(double)tp.dTime / 1000);

		view->ViewToImage(&sp);

		// Offset brush position by the offset
		if (offset)
			sp -= StrokePoint(offset->X, offset->Y);
		
		points.Add(sp);
		//CheckStraight(points[i].x, points[i].y);
	}

	m_currentBrush->Move(points);

	// Indicate that the pen was moved
	msg.Result = true;
}

void OpPaint::OnKeyDown(Key::Message& /*msg*/)
{

}

void OpPaint::OnKeyUp(Key::Message& /*msg*/)
{

}

void OpPaint::brush_OnMoveStroke(StrokeMessage& msg)
{
	assert(msg.UpdateRect);

	if (msg.UpdateRect->IsNull)
		return;

	MarkDirty(/**msg.UpdateRect*/);
	Tree->Update(this);
}

void OpPaint::brush_OnEndStroke(StrokeMessage& msg)
{
	assert(msg.UpdateRect);
	
	if (msg.UpdateRect->IsNull)
		return;

	//MarkDirty();
	//Tree->Update(this);
}