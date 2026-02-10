#ifndef FRECT_H
#define FRECT_H

#include <Types/Object.h>
#include "FPoint.h"
#include "FSize.h"

class Rect;

class FRect
	: public Object
{
	DECLARE_TYPE;

public:
	double X;
	double Y;
	double Width;
	double Height;

	// A width or hight of 0 indicates a null rect

public:
	FRect();
	FRect(const FRect& rect);
	FRect(double x, double y, double width, double height);
	FRect(const Rect& rect);

	Object* Copy();

	__declspec(property(get = GetIsNull))
	bool	IsNull;
	bool	GetIsNull();

	__declspec(property(get = GetPosition, put = SetPosition))
	FPoint	Position;
	FPoint	GetPosition();
	void	SetPosition(const FPoint& p);

	__declspec(property(get = GetSize, put = SetSize))
	FSize	Size; // ugly, but must be so
	FSize	GetSize();
	void	SetSize(const FSize& size);

	__declspec(property(get = GetLeft, put = SetLeft))
	double	Left;
	double	GetLeft() const;
	void	SetLeft(double left); // undefined for a null rect

	__declspec(property(get = GetTop, put = SetTop))
	double	Top;
	double	GetTop() const;
	void	SetTop(double top); // undefined for a null rect

	__declspec(property(get = GetRight, put = SetRight))
	double	Right;
	double	GetRight() const;
	void	SetRight(double right); // undefined for a null rect

	__declspec(property(get = GetBottom, put = SetBottom))
	double	Bottom;
	double	GetBottom() const;
	void	SetBottom(double bottom); // undefined for a null rect

	void	Normalize();

	void	Offset(const FPoint& p);
	void	Expand(const FPoint& p);
	void	Expand(const FRect& rect);
	void	Clip(const FRect& rect);

	bool	Contains(const FPoint& p);
	bool	Intersects(const FRect& rect);
	static bool	IntersectTest(const FRect& rect1, const FRect& rect2);

	operator bool();

	FRect&	operator =  (const FRect& rect);
	FRect&	operator =  (const Rect& rect);

	FRect&	operator += (const FPoint& p);
	FRect&	operator -= (const FPoint& p);

	bool	operator == (const FRect& rect);
	bool	operator != (const FRect& rect);

public:
	static FRect Null;
};

#endif