#include "Curve.h"

#include <Types/Geometric\FPoint.h>

Curve::Point::Point()
{
	rIn  = FPoint(0.0, 0.0);
	rOut = FPoint(0.0, 0.0);

	Interpolation = Linear;
	Joint = Broken;
}

Curve::Point::Point(const Point& p)
{
	X = p.X;
	Y = p.Y;

	rIn  = p.rIn;
	rOut = p.rOut;

	Interpolation = p.Interpolation;
	Joint = p.Joint;
}

Curve::Point::Point(const FPoint& p)
{
	X = p.X;
	Y = p.Y;

	rIn  = FPoint(0.0, 0.0);
	rOut = FPoint(0.0, 0.0);

	Interpolation = Linear;
	Joint = Broken;
}

Curve::Point::Point(double x, double y, _Interpolation interpolation)
{
	X = x;
	Y = y;

	rIn  = FPoint(0.0, 0.0);
	rOut = FPoint(0.0, 0.0);

	Interpolation = interpolation;
	Joint = Broken;
}

Curve::Point::Point(const FPoint& p, const FPoint& in, const FPoint& out, _Interpolation interpolation)
{
	X = p.X;
	Y = p.Y;

	rIn  = in;
	rOut = out;

	Interpolation = interpolation;
	Joint = Broken;
}

Curve::Point& Curve::Point::operator = (const Point& p)
{
	X = p.X;
	Y = p.Y;

	rIn  = p.rIn;
	rOut = p.rOut;

	Interpolation = p.Interpolation;
	Joint = p.Joint;

	return *this;
}

Curve::Point& Curve::Point::operator = (const FPoint& p)
{
	X = p.X;
	Y = p.Y;

	return *this;
}

FPoint Curve::Point::GetIn()
{
	return *this + rIn;
}

void Curve::Point::SetIn(const FPoint& in)
{
	rIn = (FPoint&)in - *this;
}

FPoint Curve::Point::GetOut()
{
	return *this + rOut;
}

void Curve::Point::SetOut(const FPoint& out)
{
	rOut = (FPoint&)out - *this;
}