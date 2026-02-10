#include "FSize.h"

DEFINE_TYPE(FSize, Object);

FSize::FSize()
{
	//width = 0;
	//height = 0;
}

FSize::FSize(const FSize& size)
{
	Width = size.Width;
	Height = size.Height;
}

FSize::FSize(double width, double height)
{
	Width = width;
	Height = height;
}

Object* FSize::Copy()
{
	return new FSize(*this);
}

FSize& FSize::operator = (const FSize& size)
{
	Width = size.Width;
	Height = size.Height;

	return *this;
}

FSize& FSize::operator *= (double i)
{
	Width  *= i;
	Height *= i;

	return *this;
}

FSize& FSize::operator * (double i)
{
	return FSize(*this) *= i;
}

FSize& FSize::operator /= (double i)
{
	Width  /= i;
	Height /= i;

	return *this;
}

FSize& FSize::operator / (double i)
{
	return FSize(*this) /= i;
}

bool FSize::operator == (const FSize& size)
{
	return 
		   Width == size.Width 
		&& Height == size.Height;
}

bool FSize::operator != (const FSize& size)
{
	return !(*this == size);
}