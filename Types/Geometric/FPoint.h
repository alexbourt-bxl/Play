#ifndef FPOINT_H
#define FPOINT_H

#include <Types/Object.h>

class Point;

class FPoint
	: public Object
{
	DECLARE_TYPE;

public:
	double X;
	double Y;

public:
	FPoint();
	FPoint(const FPoint& p);
	FPoint(double x, double y);
	FPoint(const Point& p);

	Object* Copy();

	FPoint&	operator =  (const FPoint& p);
	FPoint& operator =  (const Point& p);

	FPoint	operator -  ();

	FPoint&	operator += (const FPoint& p);
	FPoint	operator +  (const FPoint& p);
	FPoint&	operator -= (const FPoint& p);
	FPoint	operator -  (const FPoint& p);

	FPoint&	operator *= (double f);
	FPoint	operator *  (double f);
	FPoint&	operator /= (double f);
	FPoint	operator /  (double f);

	bool	operator == (const FPoint& p);
	bool	operator != (const FPoint& p);
};

#endif