#ifndef GDI_IMAGE_H
#define GDI_IMAGE_H

#include <Types/Graphics\Image.h>

#include <windows.h>

class GdiImage
	: public Image
{
	DECLARE_TYPE;

public:
	GdiImage();
	GdiImage(const GdiImage& image);
	GdiImage(uint width, uint height);
	GdiImage(const _Size& size);
	GdiImage(HBITMAP hbitmap);
	~GdiImage();

	void		Init(int width, int height);
	Object*		Copy();

	Image*		ConvertTo(Type type);

	_Size		GetSize();
	void		SetSize(const _Size& size);

	void		Clear();

	__declspec(property(get = GetHdc))
	HDC			Hdc;
	HDC			GetHdc();

	__declspec(property(get = GetHbitmap))
	HBITMAP		Hbitmap;
	HBITMAP		GetHbitmap();

protected:
	HDC			m_hdc;
	HBITMAP 	m_hbitmap;
	HBITMAP 	m_hbitmapOld;
};

#endif