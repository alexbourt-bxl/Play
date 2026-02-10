#include "ImageView.h"

#include <Graphics\Graphics.h>
#include <Types/Graphics\Font.h>

void ImageView::OnSetViewedOperator(Operator::Message& msg)
{
	//if (ViewedOperator)
	//{
	//	Image* image = ViewedOperator->Out[0]->Data.Get<Image>(Tag::Image);
	//	assert(image);
	//}

	UpdateView();
}

void ImageView::OnPaint(Graphics::Message& msg)
{
	_Rect paintRect = PaintRect;

	if (!m_surface)
		return;

	m_surface->Lock();

	double fox = 0;
	double foy = 0;

	ImageToView(&fox, &foy);

	int ox = (int)fox;
	int oy = (int)foy;
	
	double tlx = 0;
	double tly = 0;
	double brx = (double)paintRect.Width;
	double bry = (double)paintRect.Height;

	ViewToImage(&tlx, &tly);
	ViewToImage(&brx, &bry);

	if (tlx < 0) tlx = 0;
	if (tly < 0) tly = 0;
	if (brx > (double)m_surface->Width) brx = (double)m_surface->Width;
	if (bry > (double)m_surface->Height) bry = (double)m_surface->Height;
	
	double stlx = tlx = (double)(int)tlx;
	double stly = tly = (double)(int)tly;
	double sbrx = brx = min((double)(int)brx + 1, (double)m_surface->Width);
	double sbry = bry = min((double)(int)bry + 1, (double)m_surface->Height);

	ImageToView(&tlx, &tly);
	ImageToView(&brx, &bry);

	/*	Draw the border. */

	msg.Graphics.DrawRect(
		paintRect.X + (int)tlx - 1,
		paintRect.Y + (int)tly - 1,
		(int)(brx - tlx) + 1,
		(int)(bry - tly) + 1,
		Color(Color::White, 0.15));

	//msg.graphics.SetClipRegion(msg.ClipRegion);
	msg.Graphics.DrawImage(
		paintRect.X + (int)tlx,
		paintRect.Y + (int)tly,
		(int)(brx - tlx),
		(int)(bry - tly),
		m_surface,
		(int)stlx,
		(int)stly,
		(int)(sbrx - stlx),
		(int)(sbry - stly));

	m_surface->Unlock();
}

void ImageView::OnSize(Control::Message& /*msg*/)
{
	SuspendUpdates();

	m_navLayout.X = ClientRect.X + (ClientRect.Width - m_navLayout.Width) / 2;
	m_navLayout.Y = ClientRect.Y + (ClientRect.Height - m_navLayout.Height) / 2;

	ResumeUpdates();
}

void ImageView::OnLButtonDown(Mouse::Message& /*msg*/)
{
	if (m_navigating)
	{
		if (Key::LeftControl.IsDown || Key::RightControl.IsDown)
			StartZooming();
		else
			StartPanning();
	}
}

void ImageView::OnLButtonUp(Mouse::Message& msg)
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

void ImageView::OnMouseMove(Mouse::Message& /*msg*/)
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

void ImageView::navZoom_OnPaint(Graphics::Message& msg)
{
	String strZoom = String::FormatValue(Zoom * 100.0, 0) + L"%";
	Font font(L"Segoe UI", 28.0, Font::Bold);

	Point offset(3, -50);
	_Rect paintRect = m_navZoom.PaintRect;

	msg.Graphics.DrawText(
		strZoom, 
		font, 
		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X - 1, 
		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y, 
		Color(Color::White, 0.5), 
		&paintRect, 
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		strZoom, 
		font, 
		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X + 1, 
		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y, 
		Color(Color::White, 0.5), 
		&paintRect, 
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		strZoom, 
		font, 
		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X, 
		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y - 1, 
		Color(Color::White, 0.5), 
		&paintRect, 
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		strZoom, 
		font, 
		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X , 
		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y + 1, 
		Color(Color::White, 0.5), 
		&paintRect, 
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		strZoom, 
		font, 
		m_navZoom.PaintRect.X + m_navZoom.PaintRect.Width / 2 + offset.X, 
		m_navZoom.PaintRect.Y + m_navZoom.PaintRect.Height / 2 + offset.Y, 
		Color(Color::Black, 0.5), 
		&paintRect, 
		Align::Center | Align::Middle);
}

void ImageView::navZoom_OnLButtonDown(Mouse::Message& /*msg*/)
{
	StartZooming();
}

void ImageView::navZoomDown2_OnLButtonDown(Mouse::Message& /*msg*/)
{
	double zoom = pow(2.0, floor(log(Zoom) / log(2.0)));
	ZoomTo(zoom / (zoom < Zoom ? 1.0 : 2.0), true);
}

void ImageView::navZoomUp2_OnLButtonDown(Mouse::Message& /*msg*/)
{
	double zoom = pow(2.0, ceil(log(Zoom) / log(2.0)));
	ZoomTo(zoom * (zoom > Zoom ? 1.0 : 2.0), true);
}

void ImageView::navZoom100_OnLButtonDown(Mouse::Message& /*msg*/)
{
	Image* img = ViewedOperator->Out[0]->Data.Get<Image>(Tag::Image);

	_Rect bounds(0, 0, img->Width, img->Height);
	ZoomToRect(bounds, true, false, true);
}

void ImageView::navZoomFit_OnLButtonDown(Mouse::Message& /*msg*/)
{
	Image* img = ViewedOperator->Out[0]->Data.Get<Image>(Tag::Image);

	if (!img)
		return;

	if (m_storedFitViewRect)
	{
		ZoomToRect(m_storedFitViewRect, false, false, true);
		m_storedFitViewRect = Rect::Null;
	}
	else
	{
		m_storedFitViewRect = ClientRect;
		ViewToImage(&m_storedFitViewRect);

		_Rect bounds(0, 0, img->Width, img->Height);
		ZoomToRect(bounds, false, false, true);
	}
}

void ImageView::image_OnUpdate(::Message& /*msg*/)
{
	Image* img = ViewedOperator->Out[0]->Data.Get<Image>(Tag::Image);

	m_surface->SetSize(img->Size);
	UpdateSurface();
	Update();
}

void ImageView::viewProperty_OnChange(Property::Message& /*msg*/)
{
	Update();
}

void ImageView::keysNavigate_OnPress(::Message& /*msg*/)
{
	m_navigating = true;
	StartNavigation();
}

void ImageView::keysNavigate_OnRelease(::Message& /*msg*/)
{
	StopNavigation();
	m_navigating = false;
}