#include "Image.h"

#include <windows.h>
#include <assert.h>

#include <Types/Graphics\Image8.h>
#include <Types/Graphics\GdiImage.h>
#include <Types/Graphics\GdiImage8.h>

#include <Native\Gdi.h>

DEFINE_ABSTRACT_TYPE(Image, Object);

Image::Image()
{
	//
}

Image* Image::Create(int width, int height, Type type)
{
	if (type.GetId() == TYPEOF(Image8).GetId())
		return new Image8(width, height);

	/*	GdiImage8 has to go first here, because
		it derives from GdiImage, and ID comparisons
		have to be used for precision. */

	else if (type.GetId() == TYPEOF(GdiImage8).GetId())
		return new GdiImage8(width, height);
	else if (type.GetId() == TYPEOF(GdiImage).GetId())
		return new GdiImage(width, height);

	return null;
}

Image* Image::Create(const _Size& size, Type type)
{
	return Create(size.Width, size.Height, type);
}

Image* Image::Convert(Image* image, Type toType)
{
	assert(image);

	if (toType != TYPEOF(Image) 
		|| toType.IsAbstract)
		return null; // maybe "return image;" is better here?

	/*	If the image doesn't need to be converted
		return it directly. */

	if (typeof(image) == toType)
		return image;

	/*	Otherwise created a converted copy
		and destroy the original. */

	Image* result = image->ConvertTo(toType);
	delete image;

	return result;
}

void Image::SetSize(int width, int height)
{
	SetSize(_Size(width, height));
}

int Image::GetWidth()
{
	return Size.Width;
}

int Image::GetHeight()
{
	return Size.Height;
}

//#ifdef TEST
//#include <stdio.h>
//void Image::UnitTest()
//{
//	// Test image conversions
//
//	// Mem8 ->
//
//	Image8 mem8(800, 600);
//
//	wprintf_s(L"Mem8 -> Gdi");	GdiImage* gdiFromMem8 = (GdiImage*)Image::Convert(&mem8, Image::Gdi);	wprintf_s(L" OK\n");
//	wprintf_s(L"Mem8 -> Gdi8");	GdiImage8* gdi8FromMem8 = (GdiImage8*)Image::Convert(&mem8, Image::Gdi8);	wprintf_s(L" OK\n");
//
//	delete gdiFromMem8;
//	delete gdi8FromMem8;
//
//	// Gdi ->
//
//	GdiImage gdi(800, 600);
//
//	wprintf_s(L"Gdi -> Mem8");	Image8* mem8FromGdi = (Image8*)Image::Convert(&gdi, Image::Mem8);	wprintf_s(L" OK\n");
//	wprintf_s(L"Gdi -> Gdi8");	GdiImage8* gdi8FromGdi = (GdiImage8*)Image::Convert(&gdi, Image::Gdi8);	wprintf_s(L" OK\n");
//
//	delete mem8FromGdi;
//	delete gdi8FromGdi;
//	
//	// Gdi8 ->
//
//	GdiImage8 gdi8(800, 600);
//
//	wprintf_s(L"Gdi8 -> Gdi");	GdiImage* gdiFromGdi8 = (GdiImage*)Image::Convert(&gdi8, Image::Gdi);	wprintf_s(L" OK\n");
//	wprintf_s(L"Gdi8 -> Mem8");	Image8* mem8FromGdi8 = (Image8*)Image::Convert(&gdi8, Image::Mem8);	wprintf_s(L" OK\n");
//
//	delete gdiFromGdi8;
//	delete mem8FromGdi8;
//
//	// Test image copying
//	// Test space copying
//}
//#endif