#include "ParamCurve.h"

ParamCurve::Point::Point()
{
	rIn  = FPoint(0.0, 0.0);
	rOut = FPoint(0.0, 0.0);

	Interpolation = Linear;
}

ParamCurve::Point::Point(const Point& p)
{
	X = p.X;
	Y = p.Y;

	rIn  = p.rIn;
	rOut = p.rOut;

	Interpolation = p.Interpolation;
}

ParamCurve::Point::Point(const FPoint& p)
{
	X = p.X;
	Y = p.Y;

	rIn  = FPoint(0.0, 0.0);
	rOut = FPoint(0.0, 0.0);

	Interpolation = Linear;
}

ParamCurve::Point::Point(double x, double y, _Interpolation interpolation)
{
	X = x;
	Y = y;

	rIn  = FPoint(0.0, 0.0);
	rOut = FPoint(0.0, 0.0);

	Interpolation = interpolation;
}

ParamCurve::Point::Point(const FPoint& p, const FPoint& in, const FPoint& out, _Interpolation interpolation)
{
	X = p.X;
	Y = p.Y;

	rIn  = in;
	rOut = out;

	Interpolation = interpolation;
}

ParamCurve::Point& ParamCurve::Point::operator = (const Point& p)
{
	X = p.X;
	Y = p.Y;

	rIn  = p.rIn;
	rOut = p.rOut;

	Interpolation = p.Interpolation;

	return *this;
}

FPoint ParamCurve::Point::GetIn()
{
	return *this + rIn;
}

void ParamCurve::Point::SetIn(const FPoint& in)
{
	rIn = (FPoint&)in - *this;
}

FPoint ParamCurve::Point::GetOut()
{
	return *this + rOut;
}

void ParamCurve::Point::SetOut(const FPoint& out)
{
	rOut = (FPoint&)out - *this;
}