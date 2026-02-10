#include "Rect.h"

#include <Math\Math.h>

DEFINE_TYPE(Rect, Object);

Rect::Rect()
{
	//X = 0;
	//Y = 0;
	Width = 0;
	Height = 0;
}

Rect::Rect(const Rect& rect)
{
	X = rect.X;
	Y = rect.Y;
	Width = rect.Width;
	Height = rect.Height;
}

Rect::Rect(int x, int y, int width, int height)
{
	X = x;
	Y = y;
	Width = width;
	Height = height;
}

Object* Rect::Copy()
{
	return new Rect(*this);
}

bool Rect::GetIsNull()
{
	return Width == 0 || Height == 0;
}

Point Rect::GetPosition()
{
	return Point(X, Y);
}

void Rect::SetPosition(const Point& p)
{
	X = p.X;
	Y = p.Y;
}

Size Rect::GetSize()
{
	return _Size(Width, Height);
}

void Rect::SetSize(const _Size& size)
{
	Width = size.Width;
	Height = size.Height;
}

int Rect::GetLeft() const
{
	return X;
}

void Rect::SetLeft(int left)
{
	Width -= left - X;
	X = left;
}

int Rect::GetTop() const
{
	return Y;
}

void Rect::SetTop(int top)
{
	Height -= top - Y;
	Y = top;
}

int Rect::GetRight() const
{
	return X + Width;
}

void Rect::SetRight(int right)
{
	Width = right - X;
}

int Rect::GetBottom() const
{
	return Y + Height;
}

void Rect::SetBottom(int bottom)
{
	Height = bottom - Y;
}

void Rect::Normalize()
{
	if (Width < 0)
	{
		X += Width;
		Width = -Width;
	}

	if (Height < 0)
	{
		Y += Height;
		Height = -Height;
	}
}

void Rect::Offset(const Point& p)
{
	X += p.X;
	Y += p.Y;
}

void Rect::Expand(const Point& p)
{
	if (IsNull)
	{
		// A null rectangle expands "around nothing"
		// and essentially "becomes the point".

		X = p.X;
		Y = p.Y;
		Width = 1;
		Height = 1;
	}
	else
	{
		if (Left   > p.X) { Left   = p.X; }
		if (Top    > p.Y) { Top    = p.Y; }
		if (Right  < p.X) { Right  = p.X; }
		if (Bottom < p.Y) { Bottom = p.Y; }
	}
}

void Rect::Expand(const Rect& rect)
{
	if (((Rect&)rect).IsNull)
		return;

	if (IsNull)
	{
		// As this is a null rectangle, it expands "around nothing";
		// basically, it becomes the other rectangle.

		X = rect.X;
		Y = rect.Y;
		Width = rect.Width;
		Height = rect.Height;
	}
	else
	{
		if (Left   > rect.Left  ) { Left   = rect.Left;   }
		if (Top    > rect.Top   ) { Top    = rect.Top;    }
		if (Right  < rect.Right ) { Right  = rect.Right;  }
		if (Bottom < rect.Bottom) { Bottom = rect.Bottom; }
	}
}

void Rect::Clip(const Rect& rect)
{
	if (((Rect&)rect).IsNull)
		return;

	Left   = max(Left, rect.Left);
	Top    = max(Top, rect.Top);
	Right  = min(Right, rect.Right);
	Bottom = min(Bottom, rect.Bottom);
}

bool Rect::Contains(const Point& p)
{
	if (IsNull)
		return false;

	return
		   p.X >= Left 
		&& p.Y >= Top  
		&& p.X < Right 
		&& p.Y < Bottom;
}

bool Rect::Intersects(const Rect& rect)
{
	if (((Rect&)rect).IsNull)
		return false;

	return !(
		   Left > rect.Right
		|| Right < rect.Left
		|| Top > rect.Bottom
		|| Bottom < rect.Top); 
}

bool Rect::IntersectTest(const Rect& rect1, const Rect& rect2)
{
	if (((Rect&)rect1).IsNull 
		&& ((Rect&)rect2).IsNull)
		return false;

	return !(
		   rect1.Left > rect2.Right
		|| rect1.Right < rect2.Left
		|| rect1.Top > rect2.Bottom
		|| rect1.Bottom < rect2.Top); 
}

Rect::operator bool()
{
	return !IsNull;
}

Rect& Rect::operator = (const Rect& rect)
{
	X = rect.X;
	Y = rect.Y;
	Width = rect.Width;
	Height = rect.Height;

	return *this;
}

Rect& Rect::operator += (const Point& p)
{
	X += p.X;
	Y += p.Y;

	return *this;
}

Rect& Rect::operator -= (const Point& p)
{
	X -= p.X;
	Y -= p.Y;

	return *this;
}

bool Rect::operator == (const Rect& rect)
{
	return
		   X == rect.X 
		&& Y == rect.Y 
		&& Width == rect.Width 
		&& Height == rect.Height;
}

bool Rect::operator != (const Rect& rect)
{
	return !(*this == rect);
}

//

Rect Rect::Null;