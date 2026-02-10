#include "FloatAnimation.h"

#include <Properties\FloatProperty\FloatProperty.h>
#include <Math\Math.h>

#include <limits>
#include <assert.h>

DEFINE_TYPE(FloatAnimation, Animation);

FloatAnimation::FloatAnimation()
{
	m_property = null;

	m_from = std::numeric_limits<double>::quiet_NaN();
	m_to   = std::numeric_limits<double>::quiet_NaN();
}

FloatAnimation::FloatAnimation(const FloatAnimation& anim) : Animation(anim)
{
	m_property = anim.m_property;

	m_from = anim.m_from;
	m_to   = anim.m_to;
}

Object* FloatAnimation::Copy()
{
	return new FloatAnimation(*this);
}

FloatProperty* FloatAnimation::GetProperty() const
{
	return m_property;
}

void FloatAnimation::SetProperty(FloatProperty* prop)
{
	m_property = prop;
}

void FloatAnimation::SetProperty(FloatProperty& prop)
{
	SetProperty(&prop);
}

double FloatAnimation::GetFrom()
{
	return m_from;
}

void FloatAnimation::SetFrom(double from)
{
	m_from = from;
}

double FloatAnimation::GetTo()
{
	return m_to;
}

void FloatAnimation::SetTo(double to)
{
	m_to = to;
}

void FloatAnimation::UpdateValueCurve()
{
	if (m_valueCurve.Points.Count > 0)
		return; // curve already initialized

	double duration = GetActualDuration();

	for (int i = 0; i < Curve.Points.Count; i++)
	{
		ParamCurve::Point& p = Curve.Points[i];
		ParamCurve::Point pC;

		// Time
		
		pC.X      = p.X      * duration;
		pC.rIn.X  = p.rIn.X  * duration;
		pC.rOut.X = p.rOut.X * duration;
		
		// Value
		
		pC.Y      = m_from + (m_to - m_from) * p.Y;
		pC.rIn.Y  = (m_to - m_from) * p.rIn.Y;
		pC.rOut.Y = (m_to - m_from) * p.rOut.Y;

		// Interpolation

		pC.Interpolation = p.Interpolation;

		m_valueCurve.Points.Add(pC);
	}
}

void FloatAnimation::CheckFrom()
{
	if (_isnan(m_from) && m_property)
		From = *m_property;
}

double FloatAnimation::GetDistance()
{
	return fabs(m_to - m_from);
}

void FloatAnimation::PerformUpdate()
{
	assert(m_property);
	assert(!_isnan(m_to));

	*m_property = m_valueCurve.GetValue(ElapsedTime);
}

void FloatAnimation::MatchFromToEnd(Animation* anim)
{
	assert(typeof(anim) == TYPEOF(FloatAnimation));
	m_from = ((FloatAnimation*)anim)->m_to;
}

bool FloatAnimation::Match(Animation* anim)
{
	if (typeof(anim) != TYPEOF(FloatAnimation))
		return false;

	return m_property == ((FloatAnimation*)anim)->m_property;
}

void FloatAnimation::Inherit(Animation* _anim)
{
	assert(typeof(_anim) == TYPEOF(FloatAnimation));
	FloatAnimation* anim = (FloatAnimation*)_anim;

	/*	NOTE: Here I am gluing animation curves by determining the current curve
		tangent line at the point of inheritance, and extending the Out tangent
		control point along that tangent line. */

	double time = anim->ElapsedTime;
	FPoint pCurrent(time, anim->m_valueCurve.GetValue(time));

	m_from = pCurrent.Y;
	UpdateValueCurve();

	double dTime = GetAnimator().TimeDelta;
	double prevTime = time - dTime;

	double inertialFactor = ScaleInertia / dTime * INERTIA_MASS_BASE;

	// Adjust value curve

	FPoint pPrev(prevTime, anim->m_valueCurve.GetValue(prevTime));
	FPoint dOut = (pCurrent - pPrev) * inertialFactor;

	m_valueCurve.Points[0].Out = m_valueCurve.Points[0] + dOut;
	
	m_valueCurve.Points[0].Interpolation = ParamCurve::Point::Bezier;
	m_valueCurve.Points[1].Interpolation = ParamCurve::Point::Bezier;

	/*	Extend animation duration by moving up the 
		time of all but the first curve points. */

	for (int i = 1; i < m_valueCurve.Points.Count; i++)
		m_valueCurve.Points[i].X += dOut.X;
}

double FloatAnimation::GetEndTime()
{
	double endTime = 0.0;

	if (m_valueCurve.Points.Count > 0)
		endTime = max(endTime, m_valueCurve.Points[m_valueCurve.Points.Count - 1].X);

	return endTime;
}