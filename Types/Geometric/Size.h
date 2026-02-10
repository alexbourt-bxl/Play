#ifndef SIZE_H
#define SIZE_H

#include <Types/Object.h>

class Size
	: public Object
{
	DECLARE_TYPE;

public:
	int Width;
	int Height;

public:
	Size();
	Size(const Size& size);
	Size(int width, int height);

	Object* Copy();

	Size&	operator = (const Size& size);

	Size&	operator *= (int i);
	Size&	operator *  (int i);
	Size&	operator /= (int i);
	Size&	operator /  (int i);

	bool	operator == (const Size& size);
	bool	operator != (const Size& size);
};

#endif