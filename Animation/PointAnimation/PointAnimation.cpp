#include "PointAnimation.h"

#include <Properties\IntProperty\IntProperty.h>
#include <Math\Math.h>

#include <limits>
#include <assert.h>

DEFINE_TYPE(PointAnimation, Animation);

PointAnimation::PointAnimation()
{
	m_propertyX = null;
	m_propertyY = null;

	m_from = FPoint(
		std::numeric_limits<double>::quiet_NaN(), 
		std::numeric_limits<double>::quiet_NaN());
	
	m_to = FPoint(
		std::numeric_limits<double>::quiet_NaN(), 
		std::numeric_limits<double>::quiet_NaN());
}

PointAnimation::PointAnimation(const PointAnimation& anim) : Animation(anim)
{
	m_propertyX = anim.m_propertyX;
	m_propertyY = anim.m_propertyY;

	m_from = anim.m_from;
	m_to   = anim.m_to;
}

Object* PointAnimation::Copy()
{
	return new PointAnimation(*this);
}

IntProperty* PointAnimation::GetPropertyX() const
{
	return m_propertyX;
}

void PointAnimation::SetPropertyX(IntProperty* prop)
{
	m_propertyX = prop;
}

void PointAnimation::SetPropertyX(IntProperty& prop)
{
	SetPropertyX(&prop);
}

IntProperty* PointAnimation::GetPropertyY() const
{
	return m_propertyY;
}

void PointAnimation::SetPropertyY(IntProperty* prop)
{
	m_propertyY = prop;
}

void PointAnimation::SetPropertyY(IntProperty& prop)
{
	SetPropertyY(&prop);
}

Point PointAnimation::GetFrom()
{
	return Point(
		(int)m_from.X,
		(int)m_from.Y);
}

void PointAnimation::SetFrom(Point from)
{
	m_from = FPoint(
		(double)from.X,
		(double)from.Y);
}

Point PointAnimation::GetTo()
{
	return Point(
		(int)m_to.X,
		(int)m_to.Y);
}

void PointAnimation::SetTo(Point to)
{
	m_to = FPoint(
		(double)to.X,
		(double)to.Y);
}

void PointAnimation::UpdateValueCurve()
{
	if (m_valueCurveX.Points.Count > 0 || m_valueCurveY.Points.Count > 0)
		return; // curves already initialized

	double duration = GetActualDuration();

	for (int i = 0; i < Curve.Points.Count; i++)
	{
		ParamCurve::Point& p = Curve.Points[i];

		ParamCurve::Point pX;
		ParamCurve::Point pY;

		// Time
		
		pX.X      = p.X      * duration;
		pX.rIn.X  = p.rIn.X  * duration;
		pX.rOut.X = p.rOut.X * duration;
		
		pY.X      = p.X      * duration;
		pY.rIn.X  = p.rIn.X  * duration;
		pY.rOut.X = p.rOut.X * duration;

		// Value
		
		pX.Y      = m_from.X + (m_to.X - m_from.X) * p.Y;
		pX.rIn.Y  = (m_to.X - m_from.X) * p.rIn.Y;
		pX.rOut.Y = (m_to.X - m_from.X) * p.rOut.Y;

		pY.Y      = m_from.Y + (m_to.Y - m_from.Y) * p.Y;
		pY.rIn.Y  = (m_to.Y - m_from.Y) * p.rIn.Y;
		pY.rOut.Y = (m_to.Y - m_from.Y) * p.rOut.Y;
		
		// Interpolation

		pX.Interpolation = p.Interpolation;
		pY.Interpolation = p.Interpolation;

		m_valueCurveX.Points.Add(pX);
		m_valueCurveY.Points.Add(pY);
	}
}

void PointAnimation::CheckFrom()
{
	if ((_isnan(m_from.X) || _isnan(m_from.Y))
		&& m_propertyX && m_propertyY)
	{
		From = Point(
			*m_propertyX, 
			*m_propertyY);
	}
}

double PointAnimation::GetDistance()
{
	return Math::GetDistance(m_from, m_to);
}

void PointAnimation::PerformUpdate()
{
	assert(m_propertyX && m_propertyY);
	assert(!_isnan(m_to.X) && !_isnan(m_to.Y));

	double time = ElapsedTime;

	*m_propertyX = Math::Round(m_valueCurveX.GetValue(time));
	*m_propertyY = Math::Round(m_valueCurveY.GetValue(time));
}

void PointAnimation::MatchFromToEnd(Animation* anim)
{
	assert(typeof(anim) == TYPEOF(PointAnimation));
	m_from = ((PointAnimation*)anim)->m_to;
}

bool PointAnimation::Match(Animation* anim)
{
	if (typeof(anim) != TYPEOF(PointAnimation))
		return false;

	return 
		m_propertyX == ((PointAnimation*)anim)->m_propertyX
		&& m_propertyY == ((PointAnimation*)anim)->m_propertyY;
}

void PointAnimation::Inherit(Animation* _anim)
{
	assert(typeof(_anim) == TYPEOF(PointAnimation));
	PointAnimation* anim = (PointAnimation*)_anim;

	/*	NOTE: Here I am gluing animation curves by determining the current curve
		tangent line at the point of inheritance, and extending the Out tangent
		control point along that tangent line. */

	double time = anim->ElapsedTime;

	FPoint pCurrentX(time, anim->m_valueCurveX.GetValue(time));
	FPoint pCurrentY(time, anim->m_valueCurveY.GetValue(time));

	m_from = FPoint(
		pCurrentX.Y,
		pCurrentY.Y);

	UpdateValueCurve();

	double dTime = GetAnimator().TimeDelta;
	double prevTime = time - dTime;

	double inertialFactor = ScaleInertia / dTime * INERTIA_MASS_BASE;

	// Adjust X value curve

	FPoint pPrevX(prevTime, anim->m_valueCurveX.GetValue(prevTime));
	FPoint dxOut = (pCurrentX - pPrevX) * inertialFactor;

	m_valueCurveX.Points[0].Out = m_valueCurveX.Points[0] + dxOut;

	m_valueCurveX.Points[0].Interpolation = ParamCurve::Point::Bezier;
	m_valueCurveX.Points[1].Interpolation = ParamCurve::Point::Bezier;

	// Adjust Y value curve

	FPoint pPrevY(prevTime, anim->m_valueCurveY.GetValue(prevTime));
	FPoint dyOut = (pCurrentY - pPrevY) * inertialFactor;

	m_valueCurveY.Points[0].Out = m_valueCurveY.Points[0] + dyOut;

	m_valueCurveY.Points[0].Interpolation = ParamCurve::Point::Bezier;
	m_valueCurveY.Points[1].Interpolation = ParamCurve::Point::Bezier;

	// TODO: Adjust input handle of p1 so that the inherited animation
	// doesn't do the S thing.

	/*	Extend animation duration by moving up the 
		time of all but the first curve points. */

	double addedTime = max(dxOut.X, dyOut.X);

	for (int i = 1; i < m_valueCurveX.Points.Count; i++)
		m_valueCurveX.Points[i].X += addedTime;

	for (int i = 1; i < m_valueCurveY.Points.Count; i++)
		m_valueCurveY.Points[i].X += addedTime;
}

double PointAnimation::GetEndTime()
{
	double endTime = 0.0;

	if (m_valueCurveX.Points.Count > 0)
		endTime = max(endTime, m_valueCurveX.Points[m_valueCurveX.Points.Count - 1].X);

	if (m_valueCurveY.Points.Count > 0)
		endTime = max(endTime, m_valueCurveY.Points[m_valueCurveY.Points.Count - 1].X);

	return endTime;
}