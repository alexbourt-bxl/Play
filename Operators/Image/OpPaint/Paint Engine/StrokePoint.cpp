#include "StrokePoint.h"
#include "Math.h"
#include <math.h>

StrokePoint::StrokePoint()
{
	X         = 0;
	Y         = 0;
	Pressure  = 1;
	Speed     = 1;
	Azimuth   = 0;
	Elevation = 0;
	Twist     = 0;
	dTime     = 0;

	_flowing   = false;
}

StrokePoint::StrokePoint(const StrokePoint& p)
{
	X         = p.X;
	Y         = p.Y;
	Pressure  = p.Pressure;
	Speed     = p.Speed;
	Azimuth   = p.Azimuth;
	Elevation = p.Elevation;
	Twist     = p.Twist;
	dTime     = p.dTime;

	_flowing  = p._flowing;
}

StrokePoint::StrokePoint(double x, double y, double pressure, double azimuth, double elevation, double twist, double dt)
{
	X         = x;
	Y         = y;
	Pressure  = pressure;
	Speed     = 1;
	Azimuth   = azimuth;
	Elevation = elevation;
	Twist     = twist;
	dTime     = dt;

	_flowing  = false;
}

StrokePoint::~StrokePoint()
{

}

StrokePoint& StrokePoint::operator += (const StrokePoint& p)
{
	X         += p.X;
	Y         += p.Y;
	Pressure  += p.Pressure;
	Speed     += p.Speed;
	Azimuth   += p.Azimuth;
	Elevation += p.Elevation;
	Twist     += p.Twist;
	dTime     += p.dTime;

	return *this;
}

StrokePoint StrokePoint::operator + (const StrokePoint& p) const
{
	return StrokePoint(*this) += p;
}

StrokePoint& StrokePoint::operator -= (const StrokePoint& p)
{
	X         -= p.X;
	Y         -= p.Y;
	Pressure  -= p.Pressure;
	Speed     -= p.Speed;
	Azimuth   -= p.Azimuth;
	Elevation -= p.Elevation;
	Twist     -= p.Twist;
	dTime     -= p.dTime;

	return *this;
}

StrokePoint StrokePoint::operator - (const StrokePoint& p) const
{
	return StrokePoint(*this) -= p;
}

StrokePoint& StrokePoint::operator *= (double val)
{
	X         *= val;
	Y         *= val;
	Pressure  *= val;
	Speed     *= val;
	Azimuth   *= val;
	Elevation *= val;
	Twist     *= val;
	dTime     *= val;

	return *this;
}

StrokePoint StrokePoint::operator * (double val)
{
	return StrokePoint(*this) *= val;
}

StrokePoint& StrokePoint::operator /= (double val)
{
	X         /= val;
	Y         /= val;
	Pressure  /= val;
	Speed     /= val;
	Azimuth   /= val;
	Elevation /= val;
	Twist     /= val;
	dTime     /= val;

	return *this;
}

StrokePoint StrokePoint::operator / (double val)
{
	return StrokePoint(*this) /= val;
}