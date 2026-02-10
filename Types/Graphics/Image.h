#ifndef IMAGE_H
#define IMAGE_H

#include <Types/Object.h>
#include <Threads\ThreadLock.h>
#include <Types/Geometric\Point.h>
#include <Types/Geometric\Size.h>
#include <Types/Geometric\Rect.h>
//#include <Types/Geometric\Region.h>
#include <Types/Color\Color.h>

//#define TEST

class Image
	: public Object,
	  public ThreadLock
{
	DECLARE_ABSTRACT_TYPE;

	typedef Size _Size;

public:
	static  Image* Create(int width, int height, Type type);
	static  Image* Create(const _Size& size, Type type);

	static  Image* Convert(Image* image, Type toType);
	virtual Image* ConvertTo(Type type) = 0;

protected:
	Image();

public:
	__declspec(property(get = GetSize))
	_Size Size;
	virtual _Size GetSize() = 0;

	void SetSize(int width, int height);
	virtual void SetSize(const _Size& size) = 0;

	__declspec(property(get = GetWidth))
	int Width;
	int GetWidth();

	__declspec(property(get = GetHeight))
	int Height;
	int GetHeight();

	virtual void Clear() = 0;

#ifdef TEST
	static void UnitTest();
#endif
};

#endif