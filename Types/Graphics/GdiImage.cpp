#include "GdiImage.h"

#include <Native\Gdi.h>

DEFINE_TYPE(GdiImage, Image);

GdiImage::GdiImage()
{
	Init(1, 1);
}

GdiImage::GdiImage(const GdiImage& _image)
{
	GdiImage& image = (GdiImage&)_image;

	Init(image.Width, image.Height);

	::BitBlt(
		m_hdc, 
		0, 
		0, 
		image.Width, 
		image.Height, 
		image.Hdc, 
		0, 
		0, 
		SRCCOPY);
}

GdiImage::GdiImage(uint width, uint height)
{
	Init(width, height);
}

GdiImage::GdiImage(const _Size& size)
{
	Init(size.Width, size.Height);
}

GdiImage::GdiImage(HBITMAP hbitmap)
{
	BITMAP b;
	::GetObject(hbitmap, sizeof(b), &b);

	Init(b.bmWidth, b.bmHeight);

	HDC hdc = ::GetDC(null);
	HDC hdcSrc = ::CreateCompatibleDC(hdc);
	HBITMAP hbitmapOld = (HBITMAP)::SelectObject(hdcSrc, hbitmap);

	::BitBlt(
		m_hdc, 
		0, 
		0, 
		b.bmWidth, 
		b.bmHeight, 
		hdcSrc, 
		0, 
		0, 
		SRCCOPY);

	::SelectObject(hdcSrc, hbitmapOld);
	::DeleteDC(hdcSrc);
	::ReleaseDC(null, hdc);
}

GdiImage::~GdiImage()
{
	if (m_hbitmap)
	{
		::SelectObject(m_hdc, m_hbitmapOld);
		::DeleteObject(m_hbitmap);
	}

	if (m_hdc)
		::DeleteDC(m_hdc);
}

Object* GdiImage::Copy()
{
	return new GdiImage(*this);
}

void GdiImage::Init(int width, int height)
{
	HDC hdc = ::GetDC(null);

	m_hdc = ::CreateCompatibleDC(hdc);
	m_hbitmap = ::CreateCompatibleBitmap(
		hdc, 
		width, 
		height);
	m_hbitmapOld = (HBITMAP)::SelectObject(m_hdc, m_hbitmap);

	::ReleaseDC(null, hdc);
}

Size GdiImage::GetSize()
{
	BITMAP b;
	::GetObject(m_hbitmap, sizeof(b), &b);
	return _Size(b.bmWidth, b.bmHeight);
}

void GdiImage::SetSize(const _Size& size)
{
	/*	If the current size and the desired 
		size are the same, do nothing. */

	BITMAP b;
	::GetObject(m_hbitmap, sizeof(b), &b);

	if (size.Width == b.bmWidth
		&& size.Height == b.bmHeight)
		return;

#ifdef _DEBUG
	Gdi::LeakTest gdiLeakTest(L"GdiImage::SetSize(const Size& size)");
#endif

	/*	Resize the bitmap. */

	//GdiImage copy(*this);

	::SelectObject(m_hdc, m_hbitmapOld);
	::DeleteObject(m_hbitmap);

	HDC hdc = ::GetDC(null);

	m_hbitmap = ::CreateCompatibleBitmap(
		hdc, 
		size.Width, 
		size.Height);
	m_hbitmapOld = (HBITMAP)::SelectObject(m_hdc, m_hbitmap);
	
	::ReleaseDC(null, hdc);

	//int minWidth = min(width, copy.width);
	//int minHeight = min(height, copy.height);

	//::BitBlt(
	//	m_hdc, 
	//	0, 
	//	0, 
	//	minWidth, 
	//	minHeight, 
	//	copy.m_hdc, 
	//	0, 
	//	0, 
	//	SRCCOPY);
}

void GdiImage::Clear()
{
	BITMAP b;
	::GetObject(m_hbitmap, sizeof(b), &b);

	RECT r = { 0, 0, b.bmWidth, b.bmHeight };

	::SelectClipRgn(m_hdc, null); // any clip regions have to be removed for this

	HBRUSH hb = ::CreateSolidBrush(0);
	::FillRect(m_hdc, &r, hb);
	::DeleteObject(hb);
}

HDC GdiImage::GetHdc()
{
	return m_hdc;
}

HBITMAP GdiImage::GetHbitmap()
{
	return m_hbitmap;
}