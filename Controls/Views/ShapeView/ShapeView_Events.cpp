#include "ShapeView.h"

#include <Graphics\Graphics.h>
#include <Types/Graphics\Font.h>

void ShapeView::OnSetViewedOperator(Operator::Message& msg)
{
	assert(typeof(ViewedOperator->Out[0]->Object) == TYPEOF(Shape));
	UpdateView();
}

void ShapeView::OnPaint(Graphics::Message& msg)
{
	//m_surface.Lock();

	//double fox = 0.0;
	//double foy = 0.0;

	//ShapeToView(&fox, &foy);

	//int ox = (int)fox;
	//int oy = (int)foy;
	//
	//double tlx = 0.0;
	//double tly = 0.0;
	//double brx = (double)PaintRect.Width;
	//double bry = (double)PaintRect.Height;

	//ViewToShape(&tlx, &tly);
	//ViewToShape(&brx, &bry);

	//if (tlx < 0.0) tlx = 0.0;
	//if (tly < 0.0) tly = 0.0;
	//if (brx > m_shapeBounds.Width) brx = m_shapeBounds.Width;
	//if (bry > m_shapeBounds.Height) bry = m_shapeBounds.Height;
	//
	//double stlx = tlx = (double)(int)tlx;
	//double stly = tly = (double)(int)tly;
	//double sbrx = brx = min((double)(int)brx + 1.0, m_shapeBounds.Width);
	//double sbry = bry = min((double)(int)bry + 1.0, m_shapeBounds.Height);

	//ShapeToView(&tlx, &tly);
	//ShapeToView(&brx, &bry);

	////msg.graphics.SetClipRegion(msg.ClipRegion);
	//msg.Graphics.DrawImage(
	//	PaintRect.X + (int)tlx,
	//	PaintRect.Y + (int)tly,
	//	(int)(brx - tlx),
	//	(int)(bry - tly),
	//	&m_surface,
	//	(int)stlx,
	//	(int)stly,
	//	(int)(sbrx - stlx),
	//	(int)(sbry - stly));

	//m_surface.Unlock();
}

void ShapeView::OnSize(Control::Message& /*msg*/)
{
	SuspendUpdates();

	//m_navLayout.X = ClientRect.X + (ClientRect.Width - m_navLayout.Width) / 2;
	//m_navLayout.Y = ClientRect.Y + (ClientRect.Height - m_navLayout.Height) / 2;

	ResumeUpdates();
}

void ShapeView::OnLButtonDown(Mouse::Message& /*msg*/)
{
	if (m_navigating)
	{
		if (Key::LeftControl.IsDown || Key::RightControl.IsDown)
			StartZooming();
		else
			StartPanning();
	}
}

void ShapeView::OnLButtonUp(Mouse::Message& msg)
{
	if ((m_option & (Panning | Zooming)) != 0)
	{
		m_option = None;
		msg.Result = true;

		if (!m_navigating)
			StopNavigation();

		//Mouse::SetCursor(GetCursor());
	}
}

void ShapeView::OnMouseMove(Mouse::Message& /*msg*/)
{
	/*	Because zooming can be caused by more than one control
		absolute mouse coordinates are used. */

	Point p = Mouse::GetPosition();
	PointScreenToClient(&p);

	// Zooming
	
	if ((m_option & Zooming) != 0)
	{

		//double _zoom = ((ImageView*)msg.control)->Zoom;
		double zoom = LimitZoom(pow(2.0, log(m_zoomStart) / log(2.0) - (double)(m_moveStart.X - p.X) / (PHI * 100.0)));

		//if (m_tool)
		//	m_tool->PassData(1, (void*)&m_zoom);

		// Calculate the pan offset based on the zoom center.

		int xo = PaintRect.Width / 2;
		int yo = PaintRect.Height / 2;

		double dx = (double)(m_moveStart.X - xo) - m_xpanStart;
		double dy = (double)(m_moveStart.Y - yo) - m_ypanStart;

		// Set zoom and pan

		SuspendUpdates();

		Zoom = zoom;
		PanX = (double)(m_moveStart.X - xo) - dx * zoom / m_zoomStart;
		PanY = (double)(m_moveStart.Y - yo) - dy * zoom / m_zoomStart;

		//UpdateLabel(Zoom);

		ResumeUpdates(true);
	}

	// Panning
	
	else if ((m_option & Panning) != 0)
	{
		SuspendUpdates();

		PanX = m_xpanStart + (double)(p.X - m_moveStart.X);
		PanY = m_ypanStart + (double)(p.Y - m_moveStart.Y);

		ResumeUpdates(true);
	}
}

//void ShapeView::navZoom_OnPaint(Graphics::Message& msg)
//{
//	String strZoom = String::FormatValue(Zoom * 100.0, 0) + L"%";
//	Font font(L"Segoe UI", 28.0, Font::Bold);
//
//	Point offset(3, -50);
//	_Rect paintRect = m_navZoom.PaintRect;
//
//	msg.Graphics.DrawText(
//		strZoom, 
//		font, 
//		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X - 1, 
//		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y, 
//		Color(Color::White, 0.5), 
//		&paintRect, 
//		Align::Center | Align::Middle);
//
//	msg.Graphics.DrawText(
//		strZoom, 
//		font, 
//		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X + 1, 
//		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y, 
//		Color(Color::White, 0.5), 
//		&paintRect, 
//		Align::Center | Align::Middle);
//
//	msg.Graphics.DrawText(
//		strZoom, 
//		font, 
//		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X, 
//		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y - 1, 
//		Color(Color::White, 0.5), 
//		&paintRect, 
//		Align::Center | Align::Middle);
//
//	msg.Graphics.DrawText(
//		strZoom, 
//		font, 
//		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X , 
//		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y + 1, 
//		Color(Color::White, 0.5), 
//		&paintRect, 
//		Align::Center | Align::Middle);
//
//	msg.Graphics.DrawText(
//		strZoom, 
//		font, 
//		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X, 
//		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y, 
//		Color(Color::Black, 0.5), 
//		&paintRect, 
//		Align::Center | Align::Middle);
//}
//
//void ShapeView::navZoom_OnLButtonDown(Mouse::Message& /*msg*/)
//{
//	StartZooming();
//}
//
//void ShapeView::navZoomDown2_OnLButtonDown(Mouse::Message& /*msg*/)
//{
//	double zoom = pow(2.0, floor(log(Zoom) / log(2.0)));
//	ZoomTo(zoom / (zoom < Zoom ? 1.0 : 2.0), true);
//}
//
//void ShapeView::navZoomUp2_OnLButtonDown(Mouse::Message& /*msg*/)
//{
//	double zoom = pow(2.0, ceil(log(Zoom) / log(2.0)));
//	ZoomTo(zoom * (zoom > Zoom ? 1.0 : 2.0), true);
//}
//
//void ShapeView::navZoom100_OnLButtonDown(Mouse::Message& /*msg*/)
//{
//	//Shape* img = (Image*)Operator->Out[0]->Result;
//
//	//_Rect bounds(0, 0, img->Width, img->Height);
//	//ZoomToRect(bounds, true, false, true);
//}
//
//void ShapeView::navZoomFit_OnLButtonDown(Mouse::Message& /*msg*/)
//{
//	//Image* img = (Image*)Operator->Out[0]->Result;
//
//	//if (!img)
//	//	return;
//
//	//if (m_storedFitViewRect)
//	//{
//	//	ZoomToRect(m_storedFitViewRect, false, false, true);
//	//	m_storedFitViewRect = Rect::Null;
//	//}
//	//else
//	//{
//	//	m_storedFitViewRect = ClientRect;
//	//	ViewToImage(&m_storedFitViewRect);
//
//	//	_Rect bounds(0, 0, img->Width, img->Height);
//	//	ZoomToRect(bounds, false, false, true);
//	//}
//}

void ShapeView::shape_OnUpdate(::Message& /*msg*/)
{
	Shape* shape = (Shape*)ViewedOperator->Out[0]->Object;

	m_shapeBounds = shape->Bounds;

	//m_surface.SetSize(img->Size);
	//UpdateSurface();
	Update();
}

void ShapeView::viewProperty_OnChange(Property::Message& /*msg*/)
{
	Update();
}

void ShapeView::keysNavigate_OnPress(::Message& /*msg*/)
{
	m_navigating = true;
	StartNavigation();
}

void ShapeView::keysNavigate_OnRelease(::Message& /*msg*/)
{
	StopNavigation();
	m_navigating = false;
}