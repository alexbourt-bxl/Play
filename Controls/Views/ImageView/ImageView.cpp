#include "ImageView.h"

#include <Math\Math.h>
#include <Types/Geometric\Vector3.h>
#include <Types/Geometric\Matrix33.h>
#include <Types/Geometric\FRect.h>
#include <Types/Geometric\Poly.h>
#include <Types/Geometric\Curve.h>
#include <Types/Geometric\Shape.h>
#include <Graphics\Graphics.h>

DEFINE_TYPE(ImageView, OpView);

ImageView::ImageView()
{
	AllowDoubleClick = true;

	m_surface = null;

	SetViewedOperatorEvent += EventHandler(this, &ImageView::OnSetViewedOperator);

	m_keysNavigate.Add(Key::Space);
	m_keysNavigate.PressEvent   += EventHandler(this, &ImageView::keysNavigate_OnPress);
	m_keysNavigate.ReleaseEvent += EventHandler(this, &ImageView::keysNavigate_OnRelease);
	Keys.Add(&m_keysNavigate);

	PanX = 0;
	PanY = 0;
	Zoom = 1;
	AspectRatio = 1;

	PanX.ChangeEvent += EventHandler(this, &ImageView::viewProperty_OnChange);
	PanY.ChangeEvent += EventHandler(this, &ImageView::viewProperty_OnChange);
	Zoom.ChangeEvent += EventHandler(this, &ImageView::viewProperty_OnChange);
	AspectRatio.ChangeEvent += EventHandler(this, &ImageView::viewProperty_OnChange);

	PaintEvent		 += EventHandler(this, &ImageView::OnPaint);
	SizeEvent		 += EventHandler(this, &ImageView::OnSize);
	LButtonDownEvent += EventHandler(this, &ImageView::OnLButtonDown);
	LButtonUpEvent   += EventHandler(this, &ImageView::OnLButtonUp);
	MouseMoveEvent   += EventHandler(this, &ImageView::OnMouseMove);

	m_option = None;
	m_navigating = false;

	SuspendUpdates();

	m_navZoom.NormalImage		= &s_imgZoom;
	m_navZoom.HoverImage		= &s_imgZoomHover;
	m_navZoom.Size				= _Size(180, 180);
	m_navZoom.PaintEvent		+= EventHandler(this, &ImageView::navZoom_OnPaint);
	m_navZoom.LButtonDownEvent	+= EventHandler(this, &ImageView::navZoom_OnLButtonDown);
	m_navZoom.LButtonUpEvent	+= EventHandler(this, &ImageView::OnLButtonUp);
	m_navZoom.MouseMoveEvent	+= EventHandler(this, &ImageView::OnMouseMove);

	m_navZoomDown2.NormalImage	= &s_imgZoomDown2;
	m_navZoomDown2.Size			= _Size(68, 68);
	m_navZoomDown2.LButtonDownEvent += EventHandler(this, &ImageView::navZoomDown2_OnLButtonDown);

	m_navZoomUp2.NormalImage	= &s_imgZoomUp2;
	m_navZoomUp2.Size			= _Size(68, 68);
	m_navZoomUp2.LButtonDownEvent += EventHandler(this, &ImageView::navZoomUp2_OnLButtonDown);

	m_navZoom100.NormalImage	= &s_imgZoom100;
	m_navZoom100.Size			= _Size(68, 68);
	m_navZoom100.LButtonDownEvent += EventHandler(this, &ImageView::navZoom100_OnLButtonDown);

	m_navZoomFit.NormalImage	= &s_imgZoomFit;
	m_navZoomFit.Size			= _Size(68, 68);
	m_navZoomFit.LButtonDownEvent += EventHandler(this, &ImageView::navZoomFit_OnLButtonDown);
	
	m_navLayoutLeft.SetRowCount(2);
	m_navLayoutLeft.Columns[0]->AutoSize = false;
	m_navLayoutLeft.Columns[0]->Width = 0;
	m_navLayoutLeft.Rows[0]->AutoSize = false;
	m_navLayoutLeft.Rows[0]->Height = 80;
	m_navLayoutLeft.Rows[0]->Align = Align::Top;
	m_navLayoutLeft.Rows[1]->AutoSize = false;
	m_navLayoutLeft.Rows[1]->Height = 80;
	m_navLayoutLeft.Rows[1]->Align = Align::Bottom;
	m_navLayoutLeft[0][0].Controls.Add(&m_navZoom100);
	m_navLayoutLeft[0][1].Controls.Add(&m_navZoomDown2);

	m_navLayoutRight.SetRowCount(2);
	m_navLayoutRight.Columns[0]->AutoSize = false;
	m_navLayoutRight.Columns[0]->Width = 0;
	m_navLayoutRight.Rows[0]->AutoSize = false;
	m_navLayoutRight.Rows[0]->Height = 80;
	m_navLayoutRight.Rows[0]->Align = Align::Top;
	m_navLayoutRight.Rows[1]->AutoSize = false;
	m_navLayoutRight.Rows[1]->Height = 80;
	m_navLayoutRight.Rows[1]->Align = Align::Bottom;
	m_navLayoutRight[0][0].Controls.Add(&m_navZoomFit);
	m_navLayoutRight[0][1].Controls.Add(&m_navZoomUp2);

	m_navLayout.SetColumnCount(3);
	m_navLayout.Rows[0]->Align = Align::Middle;
	m_navLayout.Columns[0]->AutoSize = false;
	m_navLayout.Columns[0]->Width = 90;
	m_navLayout.Columns[0]->Align = Align::Left;
	m_navLayout.Columns[2]->AutoSize = false;
	m_navLayout.Columns[2]->Width = 90;
	m_navLayout.Columns[2]->Align = Align::Right;
	m_navLayout[0][0].Controls.Add(&m_navLayoutLeft);
	m_navLayout[1][0].Controls.Add(&m_navZoom);
	m_navLayout[2][0].Controls.Add(&m_navLayoutRight);
	m_navLayout.Hide();

	Controls.Add(&m_navLayout);

	ResumeUpdates();
}

ImageView::~ImageView()
{

}

//Image* ImageView::GetImage() const
//{
//	return m_image;
//}
//
//void ImageView::SetImage(Image* image)
//{
//	assert(image);
//	m_image = image;
//
//	m_surface.SetSize(m_image->Size);
//
//	UpdateSurface();
//	Update();
//}

GdiImage8* ImageView::GetSurface() const
{
	return m_surface;
}

void ImageView::UpdateView()
{
	UpdateSurface();
	Update();
}

void ImageView::UpdateSurface()
{
	/*	TODO:

		this method should take an update region. */

	//ViewedOperator->Lock();
	if (m_surface)
	{
		if (!ViewedOperator)
		{
			delete m_surface;
			m_surface = null;
			return;
		}
		else
		{
			m_surface->Lock();
		}
	}

	Output* out = ViewedOperator->Out[0];

	out->Lock();

	Image* pre = out->Data.Get<Image>(Tag::ImagePreview);
	Image* img = pre ? pre : out->Data.Get<Image>(Tag::Image);

	if (!m_surface)
	{
		m_surface = new GdiImage8(img->Size);
		m_surface->Lock();
	}
	else if (m_surface->Size != img->Size)
	{
		m_surface->SetSize(img->Size);
	}

	Graphics g(m_surface);
	g.CopyImage(
		0, 0,
		img,
		0, 0,
		img->Width, img->Height);

	out->Unlock();

	if (m_surface)
		m_surface->Unlock();
	
	//ViewedOperator->Unlock();
}

void ImageView::ViewToImage(double* x, double* y)
{
	Vector3 p(
		-(double)(PaintRect.Width / 2) + *x, 
		-(double)(PaintRect.Height / 2) + *y, 
		1.0);

	Matrix33 m(
		1.0 / Zoom,	0.0,		-PanX / Zoom,
		0.0,		1.0 / Zoom,	-PanY / Zoom,
		0.0,		0.0,		1.0);

	p *= m;

	int surfaceWidth = m_surface ? m_surface->Width : 0;
	int surfaceHeight = m_surface ? m_surface->Height : 0;

	*x = p.X + (double)(surfaceWidth / 2);
	*y = p.Y + (double)(surfaceHeight / 2);
}

void ImageView::ImageToView(double* x, double* y)
{
	int surfaceWidth = m_surface ? m_surface->Width : 0;
	int surfaceHeight = m_surface ? m_surface->Height : 0;

	Vector3 p(
		-(double)(surfaceWidth / 2) + *x, 
		-(double)(surfaceHeight / 2) + *y, 
		1.0);

	Matrix33 m(
		Zoom,	0.0,	PanX,
		0.0,	Zoom,	PanY,
		0.0,	0.0,	1.0);

	p *= m;

	*x = p.X + (double)(PaintRect.Width / 2);
	*y = p.Y + (double)(PaintRect.Height / 2);
}

void ImageView::ViewToImage(double* x, double* y, double* width, double* height)
{
	double r = *x + *width;
	double b = *y + *height;
	
	ViewToImage(x, y);
	ViewToImage(&r, &b);

	*width = r - *x;
	*height = b - *y;
}

void ImageView::ImageToView(double* x, double* y, double* width, double* height)
{
	double r = *x + *width;
	double b = *y + *height;
	
	ImageToView(x, y);
	ImageToView(&r, &b);

	*width = r - *x;
	*height = b - *y;
}

void ImageView::ViewToImage(Point* p)
{
	double x = (double)p->X;
	double y = (double)p->Y;
	
	ViewToImage(&x, &y);

	p->X = (int)x;
	p->Y = (int)y;
}

void ImageView::ImageToView(Point* p)
{
	double x = (double)p->X;
	double y = (double)p->Y;
	
	ImageToView(&x, &y);

	p->X = (int)x;
	p->Y = (int)y;
}

void ImageView::ViewToImage(FPoint* p)
{
	ViewToImage(&p->X, &p->Y);
}

void ImageView::ImageToView(FPoint* p)
{
	ImageToView(&p->X, &p->Y);
}

void ImageView::ViewToImage(_Rect* rect)
{
	double l = (double)rect->X;
	double t = (double)rect->Y;
	double r = (double)(rect->X + rect->Width);
	double b = (double)(rect->Y + rect->Height);
	
	ViewToImage(&l, &t);
	ViewToImage(&r, &b);

	double dx = l - (double)(int)l;
	double dy = t - (double)(int)t;

	rect->X      = (int)l;
	rect->Y      = (int)t;
	rect->Width  = (int)(r - l + dx);
	rect->Height = (int)(b - t + dy);
}

void ImageView::ImageToView(_Rect* rect)
{
	double l = (double)rect->X;
	double t = (double)rect->Y;
	double r = (double)(rect->X + rect->Width);
	double b = (double)(rect->Y + rect->Height);
	
	ImageToView(&l, &t);
	ImageToView(&r, &b);

	double dx = l - (double)(int)l;
	double dy = t - (double)(int)t;

	rect->X      = (int)l;
	rect->Y      = (int)t;
	rect->Width  = (int)(r - l + dx);
	rect->Height = (int)(b - t + dy);
}

void ImageView::ViewToImage(FRect* rect)
{
	double r = rect->X + rect->Width;
	double b = rect->Y + rect->Height;
	
	ViewToImage(&rect->X, &rect->Y);
	ViewToImage(&r, &b);

	rect->Width  = r - rect->X;
	rect->Height = b - rect->Y;
}

void ImageView::ImageToView(FRect* rect)
{
	double r = rect->X + rect->Width;
	double b = rect->Y + rect->Height;
	
	ImageToView(&rect->X, &rect->Y);
	ImageToView(&r, &b);

	rect->Width  = r - rect->X;
	rect->Height = b - rect->Y;
}

void ImageView::ViewToImage(Poly* poly)
{
	for (int i = 0; i < poly->Points.Count; i++)
		ViewToImage(&poly->Points[i]);
}

void ImageView::ImageToView(Poly* poly)
{
	for (int i = 0; i < poly->Points.Count; i++)
		ImageToView(&poly->Points[i]);
}

void ImageView::ViewToImage(Curve* curve)
{
	for (int i = 0; i < curve->Points.Count; i++)
	{
		Curve::Point p = curve->Points[i];

		FPoint pIn = p.In;
		FPoint pOut = p.Out;

		ViewToImage(&p);
		ViewToImage(&pIn);
		ViewToImage(&pOut);

		p.In = pIn;
		p.Out = pOut;

		curve->Points[i] = p;
	}
}

void ImageView::ImageToView(Curve* curve)
{
	for (int i = 0; i < curve->Points.Count; i++)
	{
		Curve::Point p = curve->Points[i];

		FPoint pIn = p.In;
		FPoint pOut = p.Out;

		ImageToView(&p);
		ImageToView(&pIn);
		ImageToView(&pOut);

		p.In = pIn;
		p.Out = pOut;

		curve->Points[i] = p;
	}
}

void ImageView::ViewToImage(Shape* shape)
{
	for (int i = 0; i < shape->Curves.Count; i++)
		ViewToImage(&shape->Curves[i]);
}

void ImageView::ImageToView(Shape* shape)
{
	for (int i = 0; i < shape->Curves.Count; i++)
		ImageToView(&shape->Curves[i]);
}

void ImageView::ZoomToRect(const _Rect& rect, bool force100, bool max100, bool animate)
{
	// Get source image rect

	FRect frect(
		(double)rect.X,
		(double)rect.Y,
		(double)rect.Width,
		(double)rect.Height);

	// Determine new zoom

	double zoom = frect.Width / frect.Height > (double)Width / (double)Height
		? (double)Width / frect.Width / AspectRatio
		: (double)Height / frect.Height;

	zoom = LimitZoom(zoom);

	// Limit zoom if necessary

	if (force100 || (zoom > 1.0 && max100))
		zoom = 1.0;

	// Convert target destination view rect to destination image rect using new zoom and current pan

	// Store current zoom
	double oldZoom = Zoom;
	Zoom = zoom;

	FRect viewRect = frect;
	ImageToView(&viewRect);

	viewRect.X = ((double)Width - viewRect.Width) / 2.0;
	viewRect.Y = ((double)Height - viewRect.Height) / 2.0;

	ViewToImage(&viewRect);

	// Restore current zoom
	Zoom = oldZoom;

	// Get pixel difference between source and destination image rects

	double dx = viewRect.X - frect.X;
	double dy = viewRect.Y - frect.Y;

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

void ImageView::ZoomTo(double zoom, bool animate)
{
	zoom = LimitZoom(zoom);

	FRect imgRect(
		(double)ClientRect.X,
		(double)ClientRect.Y,
		(double)ClientRect.Width,
		(double)ClientRect.Height);

	ViewToImage(&imgRect);

	FPoint pc(
		imgRect.X + imgRect.Width / 2.0,
		imgRect.Y + imgRect.Height / 2.0);

	double zoomFactor = Zoom / zoom;

	_Rect zoomRect(
		Math::Round(pc.X - imgRect.Width * zoomFactor / 2.0),
		Math::Round(pc.Y - imgRect.Height * zoomFactor / 2.0),
		Math::Round(imgRect.Width * zoomFactor),
		Math::Round(imgRect.Height * zoomFactor));

	ZoomToRect(zoomRect, zoom == 1.0, false, animate);
}

double ImageView::LimitZoom(double zoom)
{
	/*	The smallest zoom shows the smaller dimension of the image as a single pixel. The 1.5 is for 
		correct rounding. The largest zoom shows fills the view with a single pixel. 
	
		NOTE: With images this is not an issue, but with spaces it is, so for spaces the zoom out
		extents might have to be locked for the content never to become too small. */

	double minZoom = 0.05; //1.5 / (double)min(GetSurfaceWidth(), GetSurfaceHeight()); 
	double maxZoom = (double)min(PaintRect.Width, PaintRect.Height);

	return min(max(minZoom, zoom), maxZoom);
}

void ImageView::StartPanning()
{
	m_option = Panning;

	m_xpanStart = PanX;
	m_ypanStart = PanY;

	Point p = Mouse::GetPosition();
	PointScreenToClient(&p);

	m_moveStart = p;
}

void ImageView::StartZooming()
{
	m_option = Zooming;

	m_zoomStart = Zoom;
	m_xpanStart = PanX;
	m_ypanStart = PanY;

	Point p = Mouse::GetPosition();
	PointScreenToClient(&p);

	m_moveStart = p;
}

void ImageView::StartNavigation()
{
	m_navLayout.Show();
}

void ImageView::StopNavigation()
{
	if (m_navZoom.m_pressed)
		return;

	m_navLayout.Hide();
}