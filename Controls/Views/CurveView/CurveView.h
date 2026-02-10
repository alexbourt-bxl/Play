#ifndef CURVE_VIEW_H
#define CURVE_VIEW_H

#include <Controls\Views\View.h>
#include <Properties\FloatProperty\FloatProperty.h>

class ParamCurve;

class CurveView
	: public View
{
	DECLARE_TYPE;

public:
	CurveView();

	__declspec(property(get = GetCurve, put = SetCurve))
	ParamCurve* Curve;
	ParamCurve* GetCurve() const;
	void	SetCurve(ParamCurve* curve);
	void	SetCurve(ParamCurve& curve);

	FloatProperty PanX;
	FloatProperty PanY;
	FloatProperty ZoomX;
	FloatProperty ZoomY;

	void	ViewToGraph(double* x, double* y);
	void	GraphToView(double* x, double* y);

private:
	ParamCurve* m_curve;

	/*	The grid values determine the minimum value 
		steps to indicate in each dimension. */
	FPoint	m_gridStep;

	Point	m_startPos;

	FPoint*	m_movePoint;
	FPoint	m_startPoint;

	Color	m_curveColor;
	Color	m_tangentColor;
	Color	m_gridColor;

	FPoint*	HitTestPoints(const Point& p);

	void	OnPaint(Graphics::Message& msg);
	void	OnLButtonDown(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);

	void	viewProperty_OnChange(Property::Message& msg);
};

#endif