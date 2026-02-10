#ifndef SHAPE_VIEW_H
#define SHAPE_VIEW_H

#include <Controls\Views\OpView\OpView.h>
#include <Properties\FloatProperty\FloatProperty.h>
#include <Types/Graphics\GdiImage8.h>
#include <Devices\Keyboard\KeyCombo.h>
#include <Controls\Views\NavControl\NavControl.h>
#include <Controls\Layout\GridLayout\GridLayout.h>
#include <Types/Geometric\FRect.h>
#include <Types/Geometric\Shape.h>

class FPoint;

class ShapeView 
	: public OpView
{
	DECLARE_TYPE;

public:
	FloatProperty PanX;
	FloatProperty PanY;
	FloatProperty Zoom;

	enum Option
	{
		None    = 0,
		Panning = 1,
		Zooming = 2
	};

public:
	ShapeView();
	~ShapeView();

	//__declspec(property(get = GetImage))
	//Image* Image_;
	//Image* GetImage() const;
	//void SetImage(Image* image);

	//__declspec(property(get = GetSurface))
	//GdiImage8& Surface;
	//GdiImage8& GetSurface();

	void UpdateView();
	//void UpdateSurface();

	void ViewToShape(double* x, double* y);
	void ShapeToView(double* x, double* y);

	void ViewToShape(double* x, double* y, double* width, double* height);
	void ShapeToView(double* x, double* y, double* width, double* height);

	void ViewToShape(Point* p);
	void ShapeToView(Point* p);

	void ViewToShape(FPoint* p);
	void ShapeToView(FPoint* p);

	void ViewToShape(_Rect* rect);
	void ShapeToView(_Rect* rect);

	void ViewToShape(FRect* rect);
	void ShapeToView(FRect* rect);

	void ViewToShape(Shape* shape);
	void ShapeToView(Shape* shape);

	void ZoomToRect(const FRect& rect, bool force100, bool max100, bool animate);
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

	//void navZoom_OnPaint(Graphics::Message& msg);
	//void navZoom_OnLButtonDown(Mouse::Message& msg);
	//void navZoomDown2_OnLButtonDown(Mouse::Message& msg);
	//void navZoomUp2_OnLButtonDown(Mouse::Message& msg);
	//void navZoom100_OnLButtonDown(Mouse::Message& msg);
	//void navZoomFit_OnLButtonDown(Mouse::Message& msg);

	void shape_OnUpdate(::Message& msg);
	void viewProperty_OnChange(Property::Message& msg);

	void keysNavigate_OnPress(::Message& msg);
	void keysNavigate_OnRelease(::Message& msg);

private:
	//GdiImage8 m_surface;
	FRect	m_shapeBounds;

	KeyCombo m_keysNavigate;
	bool	m_navigating;

	Option	m_option;

	double	m_xpanStart;
	double	m_ypanStart;
	double	m_zoomStart;

	Point	m_moveStart;

	_Rect	m_storedFitViewRect;

	//GridLayout m_navLayout;
	//GridLayout m_navLayoutLeft;
	//GridLayout m_navLayoutRight;

	//NavControl m_navZoom;
	//NavControl m_navZoomUp2;
	//NavControl m_navZoomDown2;
	//NavControl m_navZoom100;
	//NavControl m_navZoomFit;

	//static GdiImage8 s_imgZoom;
	//static GdiImage8 s_imgZoomHover;
	//static GdiImage8 s_imgZoomUp2;
	//static GdiImage8 s_imgZoomDown2;
	//static GdiImage8 s_imgZoom100;
	//static GdiImage8 s_imgZoomFit;

	void OnSetViewedOperator(Operator::Message& msg);
};

#endif