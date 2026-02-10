#include "OpImage.h"

#include <Controls\Views\ImageView\ImageView.h>

void OpImage::OnLButtonDown(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	Point pTL(0, 0);
	Point pBR(m_width, m_height);

	view->ImageToView(&pTL);
	view->ImageToView(&pBR);

	m_grab = GetGrabAspect(view, msg.Position);

	m_dragOffsetTL = Point(0, 0);
	m_dragOffsetBR = Point(0, 0);

	if (m_grab & GrabLeft)
		m_dragOffsetTL.X = msg.Position.X - pTL.X;
	if (m_grab & GrabRight)
		m_dragOffsetBR.X = msg.Position.X - pBR.X;

	if (m_grab & GrabTop)
		m_dragOffsetTL.Y = msg.Position.Y - pTL.Y;
	if (m_grab & GrabBottom)
		m_dragOffsetBR.Y = msg.Position.Y - pBR.Y;
}

void OpImage::OnLButtonUp(Mouse::Message& )
{
	m_grab = GrabNone;
}

void OpImage::OnMouseMove(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;
	
	// Something is being dragged

	if (m_grab)
	{
		Lock();
		view->Surface->Lock();

		Point pTL(0, 0);
		Point pBR(m_width, m_height);

		view->ImageToView(&pTL);
		view->ImageToView(&pBR);

		if (m_grab & GrabLeft)
			pTL.X = msg.Position.X - m_dragOffsetTL.X;
		if (m_grab & GrabRight)
			pBR.X = msg.Position.X - m_dragOffsetBR.X;

		if (m_grab & GrabTop)
			pTL.Y = msg.Position.Y - m_dragOffsetTL.Y;
		if (m_grab & GrabBottom)
			pBR.Y = msg.Position.Y - m_dragOffsetBR.Y;
	
		view->ViewToImage(&pTL);
		view->ViewToImage(&pBR);

		/*	BUG: the resizes are delayed by one update because the conversion
			uses the current surface size, with that size changing at the end,
			so the result becomes "valid for the previous surface size". */

		//Size newSize(
		//	max(1, pBR.X - pTL.X),
		//	max(1, pBR.Y - pTL.Y));

		//pTL = Point(0, 0);
		//pBR = Point(newSize.Width, newSize.Height);

		//view->ImageToView(&pTL, &newSize);
		//view->ImageToView(&pBR, &newSize);

		//if (m_grab & GrabLeft)
		//	pTL.X = msg.Position.X - m_dragOffsetTL.X;
		//if (m_grab & GrabRight)
		//	pBR.X = msg.Position.X - m_dragOffsetBR.X;

		//if (m_grab & GrabTop)
		//	pTL.Y = msg.Position.Y - m_dragOffsetTL.Y;
		//if (m_grab & GrabBottom)
		//	pBR.Y = msg.Position.Y - m_dragOffsetBR.Y;
	
		//view->ViewToImage(&pTL, &newSize);
		//view->ViewToImage(&pBR, &newSize);

		view->Surface->Unlock();
		Unlock();

		/*	TODO: right now the coordinate conversion depends on the dimensions of ImageView::m_surface,
			because everything is centered. This centering needs to be removed and done manually, so that
			more straightforward coordinate conversion can be done. */

		// TODO: all these should go under one action

		Width  = max(1, pBR.X - pTL.X);
		Height = max(1, pBR.Y - pTL.Y);
	}

	// Nothing is being dragged

	else
	{
		int grab = GetGrabAspect(view, msg.Position);

		if ((grab & GrabTop && grab & GrabLeft)
			|| (grab & GrabBottom && grab & GrabRight))
			view->Cursor = Cursor::ScaleTLBR;	
		else if ((grab & GrabBottom && grab & GrabLeft)
			|| (grab & GrabTop && grab & GrabRight))
			view->Cursor = Cursor::ScaleBLTR;
		else if (grab & GrabLeft || grab & GrabRight)
			view->Cursor = Cursor::ScaleLR;
		else if (grab & GrabTop || grab & GrabBottom)
			view->Cursor = Cursor::ScaleTB;
		else
			view->Cursor = Cursor::Arrow;
	}
}

int OpImage::GetGrabAspect(ImageView* view, const Point& p)
{
	Point pTL(0, 0);
	Point pBR(m_width, m_height);

	view->ImageToView(&pTL);
	view->ImageToView(&pBR);

	int grab = GrabNone;

	if (p.Y >= pTL.Y - m_grabSize && p.Y <= pBR.Y + m_grabSize)
	{
		if (abs(p.X - pTL.X) < m_grabSize)
			grab |= GrabLeft;
		else if (abs(p.X - pBR.X) < m_grabSize)
			grab |= GrabRight;
	}

	if (p.X >= pTL.X - m_grabSize && p.X <= pBR.X + m_grabSize)
	{
		if (abs(p.Y - pTL.Y) < m_grabSize)
			grab |= GrabTop;
		else if (abs(p.Y - pBR.Y) < m_grabSize)
			grab |= GrabBottom;
	}

	return grab;
}