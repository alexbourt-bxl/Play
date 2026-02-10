#include "ShapeView.h"

#include <Math\Math.h>
#include <Types/Geometric\Vector3.h>
#include <Types/Geometric\Matrix33.h>
#include <Types/Geometric\FRect.h>
#include <Graphics\Graphics.h>

DEFINE_TYPE(ShapeView, OpView);

ShapeView::ShapeView()
{
	AllowDoubleClick = true;

	SetViewedOperatorEvent += EventHandler(this, &ShapeView::OnSetViewedOperator);

	m_keysNavigate.Add(Key::Space);
	m_keysNavigate.PressEvent   += EventHandler(this, &ShapeView::keysNavigate_OnPress);
	m_keysNavigate.ReleaseEvent += EventHandler(this, &ShapeView::keysNavigate_OnRelease);
	Keys.Add(&m_keysNavigate);

	PanX = 0.0;
	PanY = 0.0;
	Zoom = 1.0;

	PanX.ChangeEvent += EventHandler(this, &ShapeView::viewProperty_OnChange);
	PanY.ChangeEvent += EventHandler(this, &ShapeView::viewProperty_OnChange);
	Zoom.ChangeEvent += EventHandler(this, &ShapeView::viewProperty_OnChange);

	PaintEvent		 += EventHandler(this, &ShapeView::OnPaint);
	SizeEvent		 += EventHandler(this, &ShapeView::OnSize);
	LButtonDownEvent += EventHandler(this, &ShapeView::OnLButtonDown);
	LButtonUpEvent   += EventHandler(this, &ShapeView::OnLButtonUp);
	MouseMoveEvent   += EventHandler(this, &ShapeView::OnMouseMove);

	m_option = None;
	m_navigating = false;

	SuspendUpdates();

	//m_navZoom.NormalImage		= &s_imgZoom;
	//m_navZoom.HoverImage		= &s_imgZoomHover;
	//m_navZoom.Size				= _Size(180, 180);
	//m_navZoom.PaintEvent		+= EventHandler(this, &ShapeView::navZoom_OnPaint);
	//m_navZoom.LButtonDownEvent	+= EventHandler(this, &ShapeView::navZoom_OnLButtonDown);
	//m_navZoom.LButtonUpEvent	+= EventHandler(this, &ShapeView::OnLButtonUp);
	//m_navZoom.MouseMoveEvent	+= EventHandler(this, &ShapeView::OnMouseMove);

	//m_navZoomDown2.NormalImage	= &s_imgZoomDown2;
	//m_navZoomDown2.Size			= _Size(68, 68);
	//m_navZoomDown2.LButtonDownEvent += EventHandler(this, &ShapeView::navZoomDown2_OnLButtonDown);

	//m_navZoomUp2.NormalImage	= &s_imgZoomUp2;
	//m_navZoomUp2.Size			= _Size(68, 68);
	//m_navZoomUp2.LButtonDownEvent += EventHandler(this, &ShapeView::navZoomUp2_OnLButtonDown);

	//m_navZoom100.NormalImage	= &s_imgZoom100;
	//m_navZoom100.Size			= _Size(68, 68);
	//m_navZoom100.LButtonDownEvent += EventHandler(this, &ShapeView::navZoom100_OnLButtonDown);

	//m_navZoomFit.NormalImage	= &s_imgZoomFit;
	//m_navZoomFit.Size			= _Size(68, 68);
	//m_navZoomFit.LButtonDownEvent += EventHandler(this, &ShapeView::navZoomFit_OnLButtonDown);
	//
	//m_navLayoutLeft.SetRowCount(2);
	//m_navLayoutLeft.Columns[0]->AutoSize = false;
	//m_navLayoutLeft.Columns[0]->Width = 0;
	//m_navLayoutLeft.Rows[0]->AutoSize = false;
	//m_navLayoutLeft.Rows[0]->Height = 80;
	//m_navLayoutLeft.Rows[0]->Align = Align::Top;
	//m_navLayoutLeft.Rows[1]->AutoSize = false;
	//m_navLayoutLeft.Rows[1]->Height = 80;
	//m_navLayoutLeft.Rows[1]->Align = Align::Bottom;
	//m_navLayoutLeft[0][0].Controls.Add(&m_navZoom100);
	//m_navLayoutLeft[0][1].Controls.Add(&m_navZoomDown2);

	//m_navLayoutRight.SetRowCount(2);
	//m_navLayoutRight.Columns[0]->AutoSize = false;
	//m_navLayoutRight.Columns[0]->Width = 0;
	//m_navLayoutRight.Rows[0]->AutoSize = false;
	//m_navLayoutRight.Rows[0]->Height = 80;
	//m_navLayoutRight.Rows[0]->Align = Align::Top;
	//m_navLayoutRight.Rows[1]->AutoSize = false;
	//m_navLayoutRight.Rows[1]->Height = 80;
	//m_navLayoutRight.Rows[1]->Align = Align::Bottom;
	//m_navLayoutRight[0][0].Controls.Add(&m_navZoomFit);
	//m_navLayoutRight[0][1].Controls.Add(&m_navZoomUp2);

	//m_navLayout.SetColumnCount(3);
	//m_navLayout.Rows[0]->Align = Align::Middle;
	//m_navLayout.Columns[0]->AutoSize = false;
	//m_navLayout.Columns[0]->Width = 90;
	//m_navLayout.Columns[0]->Align = Align::Left;
	//m_navLayout.Columns[2]->AutoSize = false;
	//m_navLayout.Columns[2]->Width = 90;
	//m_navLayout.Columns[2]->Align = Align::Right;
	//m_navLayout[0][0].Controls.Add(&m_navLayoutLeft);
	//m_navLayout[1][0].Controls.Add(&m_navZoom);
	//m_navLayout[2][0].Controls.Add(&m_navLayoutRight);
	//m_navLayout.Hide();

	//Controls.Add(&m_navLayout);

	ResumeUpdates();
}

ShapeView::~ShapeView()
{

}

//Image* ShapeView::GetImage() const
//{
//	return m_image;
//}
//
//void ShapeView::SetImage(Image* image)
//{
//	assert(image);
//	m_image = image;
//
//	m_surface.SetSize(m_image->Size);
//
//	UpdateSurface();
//	Update();
//}

//GdiImage8& ShapeView::GetSurface()
//{
//	return m_surface;
//}

void ShapeView::UpdateView()
{
	//UpdateSurface();
	Update();
}

//void ShapeView::UpdateSurface()
//{
//	/*	TODO:
//
//		this method should take an update region. */
//
//	Operator->Lock();
//	m_surface.Lock();
//
//	Image* img = (Image*)Operator->Out[0]->Result;
//
//	if (m_surface.Size != img->Size)
//		m_surface.SetSize(img->Size);
//
//	Graphics g(&m_surface);
//	g.CopyImage(
//		0, 0,
//		img,
//		0, 0,
//		img->Width, img->Height);
//
//	m_surface.Unlock();
//	Operator->Unlock();
//}

void ShapeView::ViewToShape(double* x, double* y)
{
	Vector3 p(
		-(double)PaintRect.Width / 2.0 + *x, 
		-(double)PaintRect.Height / 2.0 + *y, 
		1.0);

	Matrix33 m(
		1.0 / Zoom,	0.0,		-PanX / Zoom,
		0.0,		1.0 / Zoom,	-PanY / Zoom,
		0.0,		0.0,		1.0);

	p *= m;

	*x = p.X + (int)(m_shapeBounds.Width / 2.0);
	*y = p.Y + (int)(m_shapeBounds.Height / 2.0);
}

void ShapeView::ShapeToView(double* x, double* y)
{
	Vector3 p(
		-m_shapeBounds.Width / 2.0 + *x, 
		-m_shapeBounds.Height / 2.0 + *y, 
		1.0);

	Matrix33 m(
		Zoom,	0.0,	PanX,
		0.0,	Zoom,	PanY,
		0.0,	0.0,	1.0);

	p *= m;

	*x = p.X + (double)PaintRect.Width / 2.0;
	*y = p.Y + (double)PaintRect.Height / 2.0;
}

void ShapeView::ViewToShape(double* x, double* y, double* width, double* height)
{
	double r = *x + *width;
	double b = *y + *height;
	
	ViewToShape(x, y);
	ViewToShape(&r, &b);

	*width = r - *x;
	*height = b - *y;
}

void ShapeView::ShapeToView(double* x, double* y, double* width, double* height)
{
	double r = *x + *width;
	double b = *y + *height;
	
	ShapeToView(x, y);
	ShapeToView(&r, &b);

	*width = r - *x;
	*height = b - *y;
}

void ShapeView::ViewToShape(Point* p)
{
	double x = (double)p->X;
	double y = (double)p->Y;
	
	ViewToShape(&x, &y);

	p->X = (int)x;
	p->Y = (int)y;
}

void ShapeView::ShapeToView(Point* p)
{
	double x = (double)p->X;
	double y = (double)p->Y;
	
	ShapeToView(&x, &y);

	p->X = (int)x;
	p->Y = (int)y;
}

void ShapeView::ViewToShape(FPoint* p)
{
	ViewToShape(&p->X, &p->Y);
}

void ShapeView::ShapeToView(FPoint* p)
{
	ShapeToView(&p->X, &p->Y);
}

void ShapeView::ViewToShape(_Rect* rect)
{
	double l = (double)rect->X;
	double t = (double)rect->Y;
	double r = (double)(rect->X + rect->Width);
	double b = (double)(rect->Y + rect->Height);
	
	ViewToShape(&l, &t);
	ViewToShape(&r, &b);

	double dx = l - (double)(int)l;
	double dy = t - (double)(int)t;

	rect->X = (int)l;
	rect->Y = (int)t;
	rect->Width = (int)(r - l + dx);
	rect->Height = (int)(b - t + dy);
}

void ShapeView::ShapeToView(_Rect* rect)
{
	double l = (double)rect->X;
	double t = (double)rect->Y;
	double r = (double)(rect->X + rect->Width);
	double b = (double)(rect->Y + rect->Height);
	
	ShapeToView(&l, &t);
	ShapeToView(&r, &b);

	double dx = l - (double)(int)l;
	double dy = t - (double)(int)t;

	rect->X = (int)l;
	rect->Y = (int)t;
	rect->Width = (int)(r - l + dx);
	rect->Height = (int)(b - t + dy);
}

void ShapeView::ViewToShape(FRect* rect)
{
	double r = rect->X + rect->Width;
	double b = rect->Y + rect->Height;
	
	ViewToShape(&rect->X, &rect->Y);
	ViewToShape(&r, &b);

	rect->Width = r - rect->X;
	rect->Height = b - rect->Y;
}

void ShapeView::ShapeToView(FRect* rect)
{
	double r = rect->X + rect->Width;
	double b = rect->Y + rect->Height;
	
	ShapeToView(&rect->X, &rect->Y);
	ShapeToView(&r, &b);

	rect->Width = r - rect->X;
	rect->Height = b - rect->Y;
}

void ShapeView::ZoomToRect(const FRect& rect, bool force100, bool max100, bool animate)
{
	// Get source image rect

	double zoom = rect.Width / rect.Height > (double)Width / (double)Height
		? (double)Width / rect.Width
		: (double)Height / rect.Height;

	zoom = LimitZoom(zoom);

	// Limit zoom if necessary

	if (force100 || (zoom > 1.0 && max100))
		zoom = 1.0;

	// Convert target destination view rect to destination image rect using new zoom and current pan

	// Store current zoom
	double oldZoom = Zoom;
	Zoom = zoom;

	FRect viewRect = rect;
	ShapeToView(&viewRect);

	viewRect.X = ((double)Width - viewRect.Width) / 2.0;
	viewRect.Y = ((double)Height - viewRect.Height) / 2.0;

	ViewToShape(&viewRect);

	// Restore current zoom
	Zoom = oldZoom;

	// Get pixel difference between source and destination image rects

	double dx = viewRect.X - rect.X;
	double dy = viewRect.Y - rect.Y;

	// Add the difference to the current pan

	double panx = PanX + dx * zoom;
	double pany = PanY + dy * zoom;

	// Animate

	//if (animate)
	//{
	//	double zoomDuration = 0.5;

	//	FloatAnimation* animPanX = new FloatAnimation();
	//		animPanX->value = &PanX;
	//		animPanX->to    = panx;
	//		animPanX->duration = zoomDuration;
	//		animPanX->curve = Animation::Sine;

	//	FloatAnimation* animPanY = new FloatAnimation();
	//		animPanY->value = &PanY;
	//		animPanY->to    = pany;
	//		animPanY->duration = zoomDuration;
	//		animPanY->curve = Animation::Sine;

	//	FloatAnimation* animZoom = new FloatAnimation();
	//		animZoom->value = &Zoom;
	//		animZoom->to    = zoom;
	//		animZoom->duration = zoomDuration;
	//		animZoom->curve = Animation::Sine;

	//	GroupAnimation* animZoomTo = new GroupAnimation();
	//		animZoomTo->AddAnimation(animPanX);
	//		animZoomTo->AddAnimation(animPanY);
	//		animZoomTo->AddAnimation(animZoom);
	//	
	//	animZoomTo->Start();
	//}
	//else
	//{
		SuspendUpdates();
		{
			PanX = panx;
			PanY = pany;
			Zoom = zoom;
		}
		ResumeUpdates(true);
	//}
}

void ShapeView::ZoomTo(double zoom, bool animate)
{
	zoom = LimitZoom(zoom);

	FRect imgRect(
		(double)ClientRect.X,
		(double)ClientRect.Y,
		(double)ClientRect.Width,
		(double)ClientRect.Height);

	ViewToShape(&imgRect);

	FPoint pc(
		imgRect.X + imgRect.Width / 2.0,
		imgRect.Y + imgRect.Height / 2.0);

	double zoomFactor = Zoom / zoom;

	FRect zoomRect(
		pc.X - imgRect.Width * zoomFactor / 2.0,
		pc.Y - imgRect.Height * zoomFactor / 2.0,
		imgRect.Width * zoomFactor,
		imgRect.Height * zoomFactor);

	ZoomToRect(zoomRect, zoom == 1.0, false, animate);
}

double ShapeView::LimitZoom(double zoom)
{
	/*	The smallest zoom shows the smaller dimension of the image as a single pixel. The 1.5 is for 
		correct rounding. The largest zoom shows fills the view with a single pixel. 
	
		NOTE: With images this is not an issue, but with spaces it is, so for spaces the zoom out
		extents might have to be locked for the content never to become too small. */

	double minZoom = 0.05; //1.5 / (double)min(GetSurfaceWidth(), GetSurfaceHeight()); 
	double maxZoom = (double)min(PaintRect.Width, PaintRect.Height);

	return min(max(minZoom, zoom), maxZoom);
}

void ShapeView::StartPanning()
{
	m_option = Panning;

	m_xpanStart = PanX;
	m_ypanStart = PanY;

	Point p = Mouse::GetPosition();
	PointScreenToClient(&p);

	m_moveStart = p;
}

void ShapeView::StartZooming()
{
	m_option = Zooming;

	m_zoomStart = Zoom;
	m_xpanStart = PanX;
	m_ypanStart = PanY;

	Point p = Mouse::GetPosition();
	PointScreenToClient(&p);

	m_moveStart = p;
}

void ShapeView::StartNavigation()
{
	//m_navLayout.Show();
}

void ShapeView::StopNavigation()
{
	//if (m_navZoom.m_pressed)
	//	return;

	//m_navLayout.Hide();
}