#ifndef POINT_H
#define POINT_H

#include <Types/Object.h>

class Point
	: public Object
{
	DECLARE_TYPE;

public:
	int X;
	int Y;

public:
	Point();
	Point(const Point& p);
	Point(int x, int y);

	Object* Copy();

	Point&	operator =  (const Point& p);

	Point	operator -  ();

	Point&	operator += (const Point& p);
	Point	operator +  (const Point& p);
	Point&	operator -= (const Point& p);
	Point	operator -  (const Point& p);

	bool	operator == (const Point& p);
	bool	operator != (const Point& p);

private:
	//static type	s_type;
};

#endif