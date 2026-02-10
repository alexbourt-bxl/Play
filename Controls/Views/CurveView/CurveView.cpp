#include "CurveView.h"

#include <Types/Geometric\ParamCurve.h>
#include <Types/Geometric\Vector3.h>
#include <Types/Geometric\Matrix33.h>

DEFINE_TYPE(CurveView, View);

CurveView::CurveView()
{
	PaintEvent		 += EventHandler(this, &CurveView::OnPaint);
	LButtonDownEvent += EventHandler(this, &CurveView::OnLButtonDown);
	LButtonUpEvent   += EventHandler(this, &CurveView::OnLButtonUp);
	MouseMoveEvent	 += EventHandler(this, &CurveView::OnMouseMove);

	m_curve = null;
	m_movePoint = null;

	PanX = 0.0;
	PanY = 0.0;
	ZoomX = 256.0;
	ZoomY = 256.0;

	PanX.ChangeEvent  += EventHandler(this, &CurveView::viewProperty_OnChange);
	PanY.ChangeEvent  += EventHandler(this, &CurveView::viewProperty_OnChange);
	ZoomX.ChangeEvent += EventHandler(this, &CurveView::viewProperty_OnChange);
	ZoomY.ChangeEvent += EventHandler(this, &CurveView::viewProperty_OnChange);

	m_gridStep = FPoint(0.25, 0.25);

	m_curveColor   = Color::White;
	m_tangentColor = Color(0.65, 0.45, 0.25);
	m_gridColor    = Color(Color::Black, 0.1);

	BackColor = Color(Color::Black, 0.1);
}

ParamCurve* CurveView::GetCurve() const
{
	return m_curve;
}

void CurveView::SetCurve(ParamCurve* curve)
{
	m_curve = curve;
	Update();
}

void CurveView::SetCurve(ParamCurve& curve)
{
	SetCurve(&curve);
}

void CurveView::ViewToGraph(double* x, double* y)
{
	Vector3 p(*x, *y, 1.0);

	Matrix33 m(
		1.0 / ZoomX,	0.0,			-PanX / ZoomX,
		0.0,			1.0 / ZoomY,	-PanY / ZoomY,
		0.0,			0.0,			1.0);

	p *= m;

	*x = p.X;
	*y = p.Y;
}

void CurveView::GraphToView(double* x, double* y)
{
	Vector3 p(*x, *y, 1.0);

	Matrix33 m(
		ZoomX,	0.0,	PanX,
		0.0,	ZoomY,	PanY,
		0.0,	0.0,	1.0);

	p *= m;

	*x = p.X;
	*y = p.Y;
}

FPoint*	CurveView::HitTestPoints(const Point& p)
{
	if (m_curve)
	{
		// First test control points

		for (int i = 0; i < m_curve->Points.Count; i++)
		{
			ParamCurve::Point& point = m_curve->Points[i];
			
			ParamCurve::Point* pp = 
				i > 0
				? &m_curve->Points[i - 1]
				: null;

			ParamCurve::Point* pn = 
				i < m_curve->Points.Count - 1
				? &m_curve->Points[i + 1]
				: null;

			if (point.Interpolation == ParamCurve::Point::Bezier
				&& pp && (pp->Interpolation == ParamCurve::Point::Bezier || pp->Interpolation == ParamCurve::Point::Cubic || pp->Interpolation == ParamCurve::Point::Sine)
				&& p.X >= point.In.X * ZoomX - 5 && p.X <= point.In.X * ZoomX + 5
				&& p.Y >= ClientRect.Bottom - point.In.Y * ZoomY - 5 && p.Y <= ClientRect.Bottom - point.In.Y * ZoomY + 5)
				return &point.rIn;

			if (point.Interpolation == ParamCurve::Point::Bezier
				&& pn && (pn->Interpolation == ParamCurve::Point::Bezier || pp->Interpolation == ParamCurve::Point::Cubic || pn->Interpolation == ParamCurve::Point::Sine)
				&& p.X >= point.Out.X * ZoomX - 5 && p.X <= point.Out.X * ZoomX + 5
				&& p.Y >= ClientRect.Bottom - point.Out.Y * ZoomY - 5 && p.Y <= ClientRect.Bottom - point.Out.Y * ZoomY + 5)
				return &point.rOut;
		}

		// Now test anchor points

		for (int i = 0; i < m_curve->Points.Count; i++)
		{
			FPoint* point = &m_curve->Points[i];

			if (p.X >= point->X * ZoomX - 5 && p.X <= point->X * ZoomX + 5
				&& p.Y >= ClientRect.Bottom - point->Y * ZoomY - 5 && p.Y <= ClientRect.Bottom - point->Y * ZoomY + 5)
				return point;
		}
	}

	return null;
}