#include "AnimationCurve.h"

#include <stdarg.h>

AnimationCurve::AnimationCurve(int nPoints, ...)
{
	va_list vl;
	va_start(vl, nPoints);

	for (int i = 0; i < nPoints; i++)
	{
		Point p = va_arg(vl, Point);
		Points.Add(p);
	}

	va_end(vl);
}

//

AnimationCurve AnimationCurve::Step(2,
	ParamCurve::Point(0.0, 0.0, ParamCurve::Point::Step),
	ParamCurve::Point(1.0, 1.0, ParamCurve::Point::Step));

AnimationCurve AnimationCurve::Linear(2,
	ParamCurve::Point(0.0, 0.0, ParamCurve::Point::Linear),
	ParamCurve::Point(1.0, 1.0, ParamCurve::Point::Linear));

AnimationCurve AnimationCurve::Relaxed(2,
	ParamCurve::Point(
		FPoint(0.0, 0.0),
		FPoint(0.0, 0.0),
		FPoint(0.5, 0.0),
		ParamCurve::Point::Bezier),
	ParamCurve::Point(
		FPoint(1.0, 1.0),
		FPoint(-0.5, 0.0),
		FPoint(1.0, 1.0),
		ParamCurve::Point::Bezier));

AnimationCurve AnimationCurve::Smooth(2,
	ParamCurve::Point(
		FPoint(0.0, 0.0),
		FPoint(0.0, 0.0),
		FPoint(0.25, 0.0),
		ParamCurve::Point::Bezier),
	ParamCurve::Point(
		FPoint(1.0, 1.0),
		FPoint(-0.75, 0.0),
		FPoint(1.0, 1.0),
		ParamCurve::Point::Bezier));

AnimationCurve AnimationCurve::EaseOut(2,
	ParamCurve::Point(
		FPoint(0.0, 0.0),
		FPoint(0.0, 0.0),
		FPoint(0.5, 0.0),
		ParamCurve::Point::Bezier),
	ParamCurve::Point(
		FPoint(1.0, 1.0),
		FPoint(-0.25, -0.5),
		FPoint(1.0, 1.0),
		ParamCurve::Point::Bezier));

AnimationCurve AnimationCurve::EaseIn(2,
	ParamCurve::Point(
		FPoint(0.0, 0.0),
		FPoint(0.0, 0.0),
		FPoint(0.25, 0.5),
		ParamCurve::Point::Bezier),
	ParamCurve::Point(
		FPoint(1.0, 1.0),
		FPoint(-0.5, 0.0),
		FPoint(1.0, 1.0),
		ParamCurve::Point::Bezier));