#include "FPoint.h"

#include <Types/Geometric/Point.h>

DEFINE_TYPE(FPoint, Object);

FPoint::FPoint()
{
	//X = 0;
	//Y = 0;
}

FPoint::FPoint(const FPoint& p)
{
	X = p.X;
	Y = p.Y;
}

FPoint::FPoint(double x, double y)
{
	X = x;
	Y = y;
}

FPoint::FPoint(const Point& p)
{
	X = p.X;
	Y = p.Y;
}

Object* FPoint::Copy()
{
	return new FPoint(*this);
}

FPoint FPoint::operator - ()
{
	return FPoint(-X, -Y);
}

FPoint& FPoint::operator = (const FPoint& p)
{
	X = p.X;
	Y = p.Y;

	return *this;
}

FPoint& FPoint::operator = (const Point& p)
{
	X = p.X;
	Y = p.Y;

	return *this;
}

FPoint& FPoint::operator += (const FPoint& p)
{
	X += p.X;
	Y += p.Y;

	return *this;
}

FPoint FPoint::operator + (const FPoint& p)
{
	return FPoint(
		X + p.X,
		Y + p.Y);
}

FPoint& FPoint::operator -= (const FPoint& p)
{
	X -= p.X;
	Y -= p.Y;

	return *this;
}

FPoint FPoint::operator - (const FPoint& p)
{
	return FPoint(
		X - p.X,
		Y - p.Y);
}

FPoint&	FPoint::operator *= (double f)
{
	X *= f;
	Y *= f;

	return *this;
}

FPoint FPoint::operator * (double f)
{
	return FPoint(*this) *= f;
}

FPoint&	FPoint::operator /= (double f)
{
	X /= f;
	Y /= f;

	return *this;
}

FPoint FPoint::operator / (double f)
{
	return FPoint(*this) /= f;
}

bool FPoint::operator == (const FPoint& p)
{
	return X == p.X && Y == p.Y;
}

bool FPoint::operator != (const FPoint& p)
{
	return X != p.X || Y != p.Y;
}