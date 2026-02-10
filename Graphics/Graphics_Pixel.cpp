#include "Graphics.h"

#include <Types/Graphics/Image8.h>
#include <Types/Graphics/GdiImage8.h>
#include <Types/Graphics/GdiImage.h>

Color Graphics::GetPixel(int x, int y)
{
	/*	TODO: Even though using this method for any kind of image processing
		is inherently slow, the parameters should be cached so that the type
		checks are not repeated every time, and after the first time all that
		is needed is to access the buffer. (Maybe with function pointes.) */

	if (typeof(m_image) == TYPEOF(Image8))
	{
		assert(x >= 0);
		assert(x < m_image->Width);
		assert(y >= 0);
		assert(y < m_image->Height);

		Image8* image8 = (Image8*)m_image;
		int* buffer = (int*)image8->Buffer;

		return Color8(buffer[y * image8->Width + x]);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage8))
	{
		assert(x >= 0);
		assert(x < m_image->Width);
		assert(y >= 0);
		assert(y < m_image->Height);

		GdiImage8* gdiImage8 = (GdiImage8*)m_image;
		int* buffer = (int*)gdiImage8->Buffer;

		return Color8(buffer[y * gdiImage8->Width + x]);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage))
	{
		assert(x >= 0);
		assert(x < m_image->Width);
		assert(y >= 0);
		assert(y < m_image->Height);

		GdiImage* gdiImage = (GdiImage*)m_image;

		return Color8::FromGdi(::GetPixel(gdiImage->Hdc, x, y));
	}

	DebugBreak(); // the image is of unknown type; should never happen
	return Color::Transparent;
}

void Graphics::SetPixel(int x, int y, const Color& color)
{
	/*	TODO: Even though using this method for any kind of image processing
		is inherently slow, the parameters should be cached so that the type
		checks are not repeated every time, and after the first time all that
		is needed is to access the buffer. (Maybe with function pointes.) */

	if (typeof(m_image) == TYPEOF(Image8))
	{
		assert(x >= 0);
		assert(x < m_image->Width);
		assert(y >= 0);
		assert(y < m_image->Height);

		Image8* image8 = (Image8*)m_image;
		int* buffer = (int*)image8->Buffer;
		Color8 color8 = (Color&)color;

		buffer[y * image8->Width + x] = color8;
	}
	else if (typeof(m_image) == TYPEOF(GdiImage8))
	{
		assert(x >= 0);
		assert(x < m_image->Width);
		assert(y >= 0);
		assert(y < m_image->Height);

		GdiImage8* gdiImage8 = (GdiImage8*)m_image;
		int* buffer = (int*)gdiImage8->Buffer;
		Color8 color8 = (Color&)color;

		buffer[y * gdiImage8->Width + x] = color8;
	}
	else if (typeof(m_image) == TYPEOF(GdiImage))
	{
		assert(x >= 0);
		assert(x < m_image->Width);
		assert(y >= 0);
		assert(y < m_image->Height);

		GdiImage* gdiImage = (GdiImage*)m_image;
		Color8 color8 = (Color&)color;

		::SetPixel(gdiImage->Hdc, x, y, color8.ToGdi());
	}

	DebugBreak(); // the image is of unknown type; should never happen
}