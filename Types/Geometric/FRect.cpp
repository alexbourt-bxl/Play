#include "FRect.h"

#include <Math\Math.h>
#include <limits>

#include <Types/Geometric/Rect.h>

DEFINE_TYPE(FRect, Object);

FRect::FRect()
{
	//X = 0.0;
	//Y = 0.0;
	Width = std::numeric_limits<double>::quiet_NaN();
	Height = std::numeric_limits<double>::quiet_NaN();
}

FRect::FRect(const FRect& rect)
{
	X = rect.X;
	Y = rect.Y;
	Width = rect.Width;
	Height = rect.Height;
}

FRect::FRect(double x, double y, double width, double height)
{
	X = x;
	Y = y;
	Width = width;
	Height = height;
}

FRect::FRect(const Rect& rect)
{
	X = rect.X;
	Y = rect.Y;
	Width = rect.Width;
	Height = rect.Height;
}

Object* FRect::Copy()
{
	return new FRect(*this);
}

bool FRect::GetIsNull()
{
	return _isnan(Width) || _isnan(Height);
}

FPoint FRect::GetPosition()
{
	return FPoint(X, Y);
}

void FRect::SetPosition(const FPoint& p)
{
	X = p.X;
	Y = p.Y;
}

FSize FRect::GetSize()
{
	return FSize(Width, Height);
}

void FRect::SetSize(const FSize& size)
{
	Width = size.Width;
	Height = size.Height;
}

double FRect::GetLeft() const
{
	return X;
}

void FRect::SetLeft(double left)
{
	Width -= left - X;
	X = left;
}

double FRect::GetTop() const
{
	return Y;
}

void FRect::SetTop(double top)
{
	Height -= top - Y;
	Y = top;
}

double FRect::GetRight() const
{
	return X + Width;
}

void FRect::SetRight(double right)
{
	Width = right - X;
}

double FRect::GetBottom() const
{
	return Y + Height;
}

void FRect::SetBottom(double bottom)
{
	Height = bottom - Y;
}

void FRect::Normalize()
{
	if (Width < 0.0)
	{
		X += Width;
		Width = -Width;
	}

	if (Height < 0.0)
	{
		Y += Height;
		Height = -Height;
	}
}

void FRect::Offset(const FPoint& p)
{
	X += p.X;
	Y += p.Y;
}

void FRect::Expand(const FPoint& p)
{
	if (IsNull)
	{
		// A null rectangle expands "around nothing"
		// and essentially "becomes the point".

		X = p.X;
		Y = p.Y;
		Width = 0.0;
		Height = 0.0;
	}
	else
	{
		if (Left   > p.X) { Left   = p.X; }
		if (Right  < p.X) { Right  = p.X; }
		if (Top    > p.Y) { Top    = p.Y; }
		if (Bottom < p.Y) { Bottom = p.Y; }
	}
}

void FRect::Expand(const FRect& rect)
{
	if (((FRect&)rect).IsNull)
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
		if (Right  < rect.Right ) { Right  = rect.Right;  }
		if (Top    > rect.Top   ) { Top    = rect.Top;    }
		if (Bottom < rect.Bottom) { Bottom = rect.Bottom; }
	}
}

void FRect::Clip(const FRect& rect)
{
	if (((FRect&)rect).IsNull)
		return;

	Left   = max(Left, rect.Left);
	Top    = max(Top, rect.Top);
	Right  = min(Right, rect.Right);
	Bottom = min(Bottom, rect.Bottom);
}

bool FRect::Contains(const FPoint& p)
{
	if (IsNull)
		return false;

	return
		   p.X >= Left 
		&& p.Y >= Top  
		&& p.X < Right 
		&& p.Y < Bottom;
}

bool FRect::Intersects(const FRect& rect)
{
	if (((FRect&)rect).IsNull)
		return false;

	return !(
		   Left > rect.Right
		|| Right < rect.Left
		|| Top > rect.Bottom
		|| Bottom < rect.Top); 
}

bool FRect::IntersectTest(const FRect& rect1, const FRect& rect2)
{
	if (((FRect&)rect1).IsNull 
		&& ((FRect&)rect2).IsNull)
		return false;

	return !(
		   rect1.Left > rect2.Right
		|| rect1.Right < rect2.Left
		|| rect1.Top > rect2.Bottom
		|| rect1.Bottom < rect2.Top); 
}

FRect::operator bool()
{
	return !IsNull;
}

FRect& FRect::operator = (const FRect& rect)
{
	X = rect.X;
	Y = rect.Y;
	Width = rect.Width;
	Height = rect.Height;

	return *this;
}

FRect& FRect::operator = (const Rect& rect)
{
	X = rect.X;
	Y = rect.Y;
	Width = rect.Width;
	Height = rect.Height;

	return *this;
}

FRect& FRect::operator += (const FPoint& p)
{
	X += p.X;
	Y += p.Y;

	return *this;
}

FRect& FRect::operator -= (const FPoint& p)
{
	X -= p.X;
	Y -= p.Y;

	return *this;
}

bool FRect::operator == (const FRect& rect)
{
	return
		   X == rect.X 
		&& Y == rect.Y 
		&& Width == rect.Width 
		&& Height == rect.Height;
}

bool FRect::operator != (const FRect& rect)
{
	return !(*this == rect);
}

//

FRect FRect::Null;