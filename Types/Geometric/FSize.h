#ifndef FSIZE_H
#define FSIZE_H

#include <Types/Object.h>

class FSize
	: public Object
{
	DECLARE_TYPE;

public:
	double	Width;
	double	Height;

public:
	FSize();
	FSize(const FSize& size);
	FSize(double width, double height);

	Object* Copy();

	FSize&	operator = (const FSize& size);

	FSize&	operator *= (double i);
	FSize&	operator *  (double i);
	FSize&	operator /= (double i);
	FSize&	operator /  (double i);

	bool	operator == (const FSize& size);
	bool	operator != (const FSize& size);
};

#endif