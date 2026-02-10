#include "Point.h"

DEFINE_TYPE(Point, Object);

Point::Point()
{
	//X = 0;
	//Y = 0;
}

Point::Point(const Point& p)
{
	X = p.X;
	Y = p.Y;
}

Point::Point(int x, int y)
{
	X = x;
	Y = y;
}

Object* Point::Copy()
{
	return new Point(*this);
}

Point Point::operator - ()
{
	return Point(-X, -Y);
}

Point& Point::operator = (const Point& p)
{
	X = p.X;
	Y = p.Y;

	return *this;
}

Point& Point::operator += (const Point& p)
{
	X += p.X;
	Y += p.Y;

	return *this;
}

Point Point::operator + (const Point& p)
{
	return Point(
		X + p.X,
		Y + p.Y);
}

Point& Point::operator -= (const Point& p)
{
	X -= p.X;
	Y -= p.Y;

	return *this;
}

Point Point::operator - (const Point& p)
{
	return Point(
		X - p.X,
		Y - p.Y);
}

bool Point::operator == (const Point& p)
{
	return X == p.X && Y == p.Y;
}

bool Point::operator != (const Point& p)
{
	return X != p.X || Y != p.Y;
}