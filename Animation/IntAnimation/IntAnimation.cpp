#include "IntAnimation.h"

#include <Properties\IntProperty\IntProperty.h>
#include <Math\Math.h>

#include <limits>
#include <assert.h>

DEFINE_TYPE(IntAnimation, Animation);

IntAnimation::IntAnimation()
{
	m_property = null;

	m_from = std::numeric_limits<double>::quiet_NaN();
	m_to   = std::numeric_limits<double>::quiet_NaN();
}

IntAnimation::IntAnimation(const IntAnimation& anim) : Animation(anim)
{
	m_property = anim.m_property;

	m_from = anim.m_from;
	m_to   = anim.m_to;
}

Object* IntAnimation::Copy()
{
	return new IntAnimation(*this);
}

IntProperty* IntAnimation::GetProperty() const
{
	return m_property;
}

void IntAnimation::SetProperty(IntProperty* prop)
{
	m_property = prop;
}

void IntAnimation::SetProperty(IntProperty& prop)
{
	SetProperty(&prop);
}

int IntAnimation::GetFrom()
{
	return (int)m_from;
}

void IntAnimation::SetFrom(int from)
{
	m_from = (double)from;
}

int IntAnimation::GetTo()
{
	return (int)m_to;
}

void IntAnimation::SetTo(int to)
{
	m_to = (double)to;
}

void IntAnimation::UpdateValueCurve()
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

void IntAnimation::CheckFrom()
{
	if (_isnan(m_from) && m_property)
		From = *m_property;
}

double IntAnimation::GetDistance()
{
	return fabs(m_to - m_from);
}

void IntAnimation::PerformUpdate()
{
	assert(m_property);
	assert(m_to != INT_NAN);

	*m_property = Math::Round(m_valueCurve.GetValue(ElapsedTime));
}

void IntAnimation::MatchFromToEnd(Animation* anim)
{
	assert(typeof(anim) == TYPEOF(IntAnimation));
	m_from = ((IntAnimation*)anim)->m_to;
}

bool IntAnimation::Match(Animation* anim)
{
	if (typeof(anim) != TYPEOF(IntAnimation))
		return false;

	return m_property == ((IntAnimation*)anim)->m_property;
}

void IntAnimation::Inherit(Animation* _anim)
{
	assert(typeof(_anim) == TYPEOF(IntAnimation));
	IntAnimation* anim = (IntAnimation*)_anim;

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

double IntAnimation::GetEndTime()
{
	double endTime = 0.0;

	if (m_valueCurve.Points.Count > 0)
		endTime = max(endTime, m_valueCurve.Points[m_valueCurve.Points.Count - 1].X);

	return endTime;
}