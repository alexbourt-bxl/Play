#include "OpCrop.h"

#include <Types/Graphics\Image.h>
#include <Types/Geometric\Poly.h>
#include <Controls\Valuebox\Valuebox.h>
#include <Controls\Views\ImageView\ImageView.h>

void OpCrop::in_OnConnect(Input::Message& msg)
{
	if (m_init)
		return;

	Image* image = msg.Input->Data.Get<Image>(Tag::Image);

	Left.DisableEvents();
	Top.DisableEvents();
	Right.DisableEvents();
	Bottom.DisableEvents();

	Left.Valuebox->MinVisible = 0;
	Left.Valuebox->MaxVisible = image->Width;

	Right.Valuebox->MinVisible = 0;
	Right.Valuebox->MaxVisible = image->Width;

	Top.Valuebox->MinVisible = 0;
	Top.Valuebox->MaxVisible = image->Height;

	Bottom.Valuebox->MinVisible = 0;
	Bottom.Valuebox->MaxVisible = image->Height;

	Left = 0;
	Left.Valuebox->Value = 0;

	Top = 0;
	Top.Valuebox->Value = 0;

	Right = image->Width;
	Right.Valuebox->Value = image->Width;

	Bottom = image->Height;
	Bottom.Valuebox->Value = image->Height;

	Left.EnableEvents();
	Top.EnableEvents();
	Right.EnableEvents();
	Bottom.EnableEvents();

	m_inSize = image->Size;

	m_init = true;
}

void OpCrop::Left_PreChange(Property::Message& msg)
{
	int& left = *(int*)msg.Value;
	left = min(left, Right - 1);
}

void OpCrop::Top_PreChange(Property::Message& msg)
{
	int& top = *(int*)msg.Value;
	top = min(top, Bottom - 1);
}

void OpCrop::Right_PreChange(Property::Message& msg)
{
	int& right = *(int*)msg.Value;
	right = max(Left + 1, right);
}

void OpCrop::Bottom_PreChange(Property::Message& msg)
{
	int& bottom = *(int*)msg.Value;
	bottom = max(Top + 1, bottom);
}

void OpCrop::btnTrim_OnPress(Control::Message& /*msg*/)
{
	Image* in = m_in.Data.Get<Image>(Tag::Image);

	if (!in)
		return; 

	Rect rect = Graphics::GetContentRect(in);

	Left   = rect.Left;
	Right  = rect.Right;
	Top    = rect.Top;
	Bottom = rect.Bottom;
}

void OpCrop::OnPaint(Graphics::Message& msg)
{
	Lock();

	ImageView* view = (ImageView*)msg.Object;
	Rect paintRect = view->PaintRect;

	Point pTL(m_left, m_top);
	Point pBR(m_right, m_bottom);

	view->ImageToView(&pTL);
	view->ImageToView(&pBR);

	/*	Draw the crop cover. */

	Color coverColor(Color::Back, m_hide);

	msg.Graphics.FillRect( // left
		paintRect.X, 
		paintRect.Y,
		pTL.X,
		view->Height,
		coverColor);

	msg.Graphics.FillRect( // right
		paintRect.X + pBR.X,
		paintRect.Y,
		view->Width - pBR.X,
		view->Height,
		coverColor);

	msg.Graphics.FillRect( // top
		paintRect.X + pTL.X,
		paintRect.Y,
		pBR.X - pTL.X,
		pTL.Y,
		coverColor);

	msg.Graphics.FillRect( // bottom
		paintRect.X + pTL.X,
		paintRect.Y + pBR.Y,
		pBR.X - pTL.X,
		view->Height - pBR.Y,
		coverColor);

	/*	Draw marching ants. */

	Poly poly;

	poly.Points.Add(FPoint(paintRect.X + pTL.X - 1, paintRect.Y + pTL.Y - 1));
	poly.Points.Add(FPoint(paintRect.X + pTL.X - 1, paintRect.Y + pBR.Y));
	poly.Points.Add(FPoint(paintRect.X + pBR.X, paintRect.Y + pBR.Y));
	poly.Points.Add(FPoint(paintRect.X + pBR.X, paintRect.Y + pTL.Y - 1));

	msg.Graphics.DrawMAPoly(
		poly, 
		Color(Color::White, 0.5), 
		Color(Color::Black, 0.5));

	/*	Draw corners. */

	msg.Graphics.DrawImage(
		paintRect.X + pTL.X - s_imgTL.Width + 2, 
		paintRect.Y + pTL.Y - s_imgTL.Height + 2,
		&s_imgTL);

	msg.Graphics.DrawImage(
		paintRect.X + pBR.X - 2, 
		paintRect.Y + pTL.Y - s_imgTR.Height + 2,
		&s_imgTR);

	msg.Graphics.DrawImage(
		paintRect.X + pTL.X - s_imgBL.Width + 2, 
		paintRect.Y + pBR.Y - 2,
		&s_imgBL);

	msg.Graphics.DrawImage(
		paintRect.X + pBR.X - 2, 
		paintRect.Y + pBR.Y - 2,
		&s_imgBR);

	Unlock();
}

void OpCrop::OnLButtonDown(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	Point pTL(m_left, m_top);
	Point pBR(m_right, m_bottom);

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

void OpCrop::OnLButtonUp(Mouse::Message& /*msg*/)
{
	m_grab = GrabNone;
}

void OpCrop::OnMouseMove(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	// Something is being dragged

	if (m_grab)
	{
		Point pTL(m_left, m_top);
		Point pBR(m_right, m_bottom);

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

		// TODO: all these should go under one action

		Left   = pTL.X;
		Top    = pTL.Y;
		Right  = pBR.X;
		Bottom = pBR.Y;
	}

	// Nothing is being dragged

	else
	{
		int grab = GetGrabAspect(view, msg.Position);

		if (grab & GrabLeft && grab & GrabRight
			&& grab & GrabTop && grab & GrabBottom)
			view->Cursor = Cursor::Move;
		else if ((grab & GrabTop && grab & GrabLeft)
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
			view->Cursor = Cursor::Aim;
	}
}

int OpCrop::GetGrabAspect(ImageView* view, const Point& p)
{
	Point pTL(m_left, m_top);
	Point pBR(m_right, m_bottom);

	view->ImageToView(&pTL);
	view->ImageToView(&pBR);

	int grab = GrabNone;

	if (p.X > pTL.X && p.X < pBR.X
		&& p.Y > pTL.Y && p.Y < pBR.Y)
	{
		grab = GrabLeft | GrabRight | GrabTop | GrabBottom;
	}
	else
	{
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
	}

	return grab;
}