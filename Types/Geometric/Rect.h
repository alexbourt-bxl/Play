#ifndef RECT_H
#define RECT_H

#include <Types/Object.h>
#include "Point.h"
#include "Size.h"

class Rect
	: public Object
{
	DECLARE_TYPE;

	typedef Size _Size;

public:
	int X;
	int Y;
	int Width;
	int Height;

	// A width or hight of 0 indicates a null rect

public:
	Rect();
	Rect(const Rect& rect);
	Rect(int x, int y, int width, int height);

	Object* Copy();

	__declspec(property(get = GetIsNull))
	bool	IsNull;
	bool	GetIsNull();

	__declspec(property(get = GetPosition, put = SetPosition))
	Point	Position;
	Point	GetPosition();
	void	SetPosition(const Point& p);

	__declspec(property(get = GetSize, put = SetSize))
	_Size	Size;
	_Size	GetSize();
	void	SetSize(const _Size& size);
	
	__declspec(property(get = GetLeft, put = SetLeft))
	int		Left;
	int		GetLeft() const;
	void	SetLeft(int left); // undefined for a null rect

	__declspec(property(get = GetTop, put = SetTop))
	int		Top;
	int		GetTop() const;
	void	SetTop(int top); // undefined for a null rect

	__declspec(property(get = GetRight, put = SetRight))
	int		Right;
	int		GetRight() const;
	void	SetRight(int right); // undefined for a null rect

	__declspec(property(get = GetBottom, put = SetBottom))
	int		Bottom;
	int		GetBottom() const;
	void	SetBottom(int bottom); // undefined for a null rect

	void	Normalize();

	void	Offset(const Point& p);
	void	Expand(const Point& p);
	void	Expand(const Rect& rect);
	void	Clip(const Rect& rect);

	bool	Contains(const Point& p);
	bool	Intersects(const Rect& rect);
	static bool	IntersectTest(const Rect& rect1, const Rect& rect2);

	operator bool();

	Rect&	operator =  (const Rect& rect);

	Rect&	operator += (const Point& p);
	Rect&	operator -= (const Point& p);

	bool	operator == (const Rect& rect);
	bool	operator != (const Rect& rect);

public:
	static Rect Null;
};

typedef Rect Rect;

#endif