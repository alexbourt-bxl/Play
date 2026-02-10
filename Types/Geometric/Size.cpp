#include "Size.h"

DEFINE_TYPE(Size, Object);

Size::Size()
{
	//width = 0;
	//height = 0;
}

Size::Size(const Size& size)
{
	Width = size.Width;
	Height = size.Height;
}

Size::Size(int width, int height)
{
	Width = width;
	Height = height;
}

Object* Size::Copy()
{
	return new Size(*this);
}

Size& Size::operator = (const Size& size)
{
	Width = size.Width;
	Height = size.Height;

	return *this;
}

Size& Size::operator *= (int i)
{
	Width  *= i;
	Height *= i;

	return *this;
}

Size& Size::operator * (int i)
{
	return Size(*this) *= i;
}

Size& Size::operator /= (int i)
{
	Width  /= i;
	Height /= i;

	return *this;
}

Size& Size::operator / (int i)
{
	return Size(*this) /= i;
}

bool Size::operator == (const Size& size)
{
	return 
		   Width == size.Width 
		&& Height == size.Height;
}

bool Size::operator != (const Size& size)
{
	return !(*this == size);
}