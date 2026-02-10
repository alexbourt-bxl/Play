#include "ParamCurve.h"

#include <Math\Math.h>
#include <assert.h>

ParamCurve::ParamCurve()
{

}

ParamCurve::ParamCurve(const ParamCurve& curve)
{
	for (int i = 0; i < ((ParamCurve&)curve).m_points.Count; i++)
		m_points.Add(((ParamCurve&)curve).m_points[i]);
}

ParamCurve& ParamCurve::operator = (const ParamCurve& curve)
{
	m_points.Clear();

	for (int i = 0; i < ((ParamCurve&)curve).m_points.Count; i++)
		m_points.Add(((ParamCurve&)curve).Points[i]);

	return *this;
}

Array<ParamCurve::Point>& ParamCurve::GetPoints()
{
	return m_points;
}

void ParamCurve::InsertPoint(const Point& p, Point& pPrev)
{
	int prevIndex = m_points.GetIndexOf(pPrev);

	m_points.Add(p);
	m_points.SetIndexAt(m_points.Count - 1, prevIndex + 1);
}

double ParamCurve::GetValue(double time)
{
	assert(m_points.Count > 0);

	if (time < m_points[0].X)
		return m_points[0].Y;

	if (time > m_points[m_points.Count - 1].X)
		return m_points[m_points.Count - 1].Y;

	Point* p0 = null;
	Point* p1 = null;

	// For cubic interpolation
	Point* pp = null;
	Point* pn = null;

	for (int i = 0; i < m_points.Count - 1; i++)
	{
		if (time >= m_points[i].X && time <= m_points[i + 1].X)
		{
			p0 = &m_points[i];
			p1 = &m_points[i + 1];

			pp = 
				i > 0
				? &m_points[i - 1]
				: p0;
			
			pn = 
				i < m_points.Count - 2 
				? &m_points[i + 2] 
				: p1;

			break;
		}
	}

	assert(p0 && p1);

	double dX = p1->X - p0->X;
	if (dX == 0.0) dX = ZERO;

	double _1_dX = 1.0 / dX;
	double t = (time - p0->X) * _1_dX;

	switch (p1->Interpolation)
	{
	case Point::Step:
		{
			return
				time < p1->X
				? p0->Y
				: p1->Y;
		}
	case Point::Linear:
	default:
		{
			return p0->Y + t * (p1->Y - p0->Y);		
		}
	case Point::Sine:
		{
			t = (1.0 - cos(t * PI)) * 0.5;
			return p0->Y + t * (p1->Y - p0->Y);		
		}
	//case Point::Cubic:
	//	{
	//		double tb = (p0->Out.X - p0->X) * _1_dx;
	//		double tc = (p1->In.X - p0->X) * _1_dx;

	//		double ax = 3.0*tb - 3.0*tc + 1.0;
	//		double bx = -6.0*tb + 3.0*tc;
	//		double cx = 3.0*tb;

	//		//t = Math::SolveCubicNR(ax, bx, cx, -t, 0.0, 1.0);

	//		assert(pp && pn);

	//		//double a = -p0->Y + 3.0*p0->Out.Y - 3.0*p1->In.Y + p1->Y;
	//		//double b = 3.0*p0->Y - 6.0*p0->Out.Y + 3.0*p1->In.Y;
	//		//double c = -3.0*p0->Y + 3.0*p0->Out.Y;
	//		//double d = p0->Y;

	//		double p = (pn->Y - p1->Y) - (pp->Y - p0->Y);
	//		double q = (pp->Y - p0->Y) - p;
	//		double r =  p1->Y - pp->Y;
	//		double s =  p0->Y;

	//		return p*t*t*t + q*t*t + r*t + s;
	//	}
	case Point::Bezier:
		{
			double tb = (p0->Out.X - p0->X) * _1_dX;
			double tc = (p1->In.X - p0->X) * _1_dX;

			double ax = 3.0*tb - 3.0*tc + 1.0;
			double bx = -6.0*tb + 3.0*tc;
			double cx = 3.0*tb;

			t = Math::SolveCubicNR(ax, bx, cx, -t, 0.0, 1.0);

			double a = -p0->Y + 3.0*p0->Out.Y - 3.0*p1->In.Y + p1->Y;
			double b = 3.0*p0->Y - 6.0*p0->Out.Y + 3.0*p1->In.Y;
			double c = -3.0*p0->Y + 3.0*p0->Out.Y;
			double d = p0->Y;

			return a*t*t*t + b*t*t + c*t + d;
		}
	}
}