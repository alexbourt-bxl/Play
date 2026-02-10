#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <Controls\Views\OpView\OpView.h>
#include <Properties\FloatProperty\FloatProperty.h>
#include <Types/Graphics\GdiImage8.h>
#include <Devices\Keyboard\KeyCombo.h>
#include <Controls\Views\NavControl\NavControl.h>
#include <Controls\Layout\GridLayout\GridLayout.h>

class Image;
class FRect;
class FPoint;

class ImageView 
	: public OpView
{
	DECLARE_TYPE;

public:
	FloatProperty PanX;
	FloatProperty PanY;
	FloatProperty Zoom;
	FloatProperty AspectRatio;

	enum Option
	{
		None    = 0,
		Panning = 1,
		Zooming = 2
	};

public:
	ImageView();
	~ImageView();

	//__declspec(property(get = GetImage))
	//Image* Image_;
	//Image* GetImage() const;
	//void SetImage(Image* image);

	__declspec(property(get = GetSurface))
	GdiImage8* Surface;
	GdiImage8* GetSurface() const;

	void UpdateView();
	void UpdateSurface();

	void ViewToImage(double* x, double* y);
	void ImageToView(double* x, double* y);

	void ViewToImage(double* x, double* y, double* width, double* height);
	void ImageToView(double* x, double* y, double* width, double* height);

	void ViewToImage(Point* p);
	void ImageToView(Point* p);

	void ViewToImage(FPoint* p);
	void ImageToView(FPoint* p);

	void ViewToImage(_Rect* rect);
	void ImageToView(_Rect* rect);

	void ViewToImage(FRect* rect);
	void ImageToView(FRect* rect);

	void ViewToImage(Poly* poly);
	void ImageToView(Poly* poly);

	void ViewToImage(Curve* curve);
	void ImageToView(Curve* curve);

	void ViewToImage(Shape* shape);
	void ImageToView(Shape* shape);

	void ZoomToRect(const _Rect& rect, bool force100, bool max100, bool animate);
	void ZoomTo(double zoom, bool animate);

	double LimitZoom(double zoom);

	void StartPanning();
	void StartZooming();

	void StartNavigation();
	void StopNavigation();

	void OnPaint(Graphics::Message& msg);
	void OnSize(Control::Message& msg);
	void OnLButtonDown(Mouse::Message& msg);
	void OnLButtonUp(Mouse::Message& msg);
	void OnMouseMove(Mouse::Message& msg);

	void navZoom_OnPaint(Graphics::Message& msg);
	void navZoom_OnLButtonDown(Mouse::Message& msg);
	void navZoomDown2_OnLButtonDown(Mouse::Message& msg);
	void navZoomUp2_OnLButtonDown(Mouse::Message& msg);
	void navZoom100_OnLButtonDown(Mouse::Message& msg);
	void navZoomFit_OnLButtonDown(Mouse::Message& msg);

	void image_OnUpdate(::Message& msg);
	void viewProperty_OnChange(Property::Message& msg);

	void keysNavigate_OnPress(::Message& msg);
	void keysNavigate_OnRelease(::Message& msg);

private:
	//Image*	m_image;
	GdiImage8* m_surface;

	KeyCombo m_keysNavigate;
	bool	m_navigating;

	Option	m_option;

	double	m_xpanStart;
	double	m_ypanStart;
	double	m_zoomStart;

	Point	m_moveStart;

	_Rect	m_storedFitViewRect;

	GridLayout m_navLayout;
	GridLayout m_navLayoutLeft;
	GridLayout m_navLayoutRight;

	NavControl m_navZoom;
	NavControl m_navZoomUp2;
	NavControl m_navZoomDown2;
	NavControl m_navZoom100;
	NavControl m_navZoomFit;

	static GdiImage8 s_imgZoom;
	static GdiImage8 s_imgZoomHover;
	static GdiImage8 s_imgZoomUp2;
	static GdiImage8 s_imgZoomDown2;
	static GdiImage8 s_imgZoom100;
	static GdiImage8 s_imgZoomFit;

	void OnSetViewedOperator(Operator::Message& msg);
};

#endif