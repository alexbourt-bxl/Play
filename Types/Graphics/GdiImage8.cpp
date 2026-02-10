#include "GdiImage8.h"

#include <Native\Gdi.h>

DEFINE_TYPE(GdiImage8, GdiImage, Serializable);

GdiImage8::GdiImage8()
{
	Init(1, 1);
}

GdiImage8::GdiImage8(const GdiImage8& _image)
{
	GdiImage8& image = (GdiImage8&)_image;

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

GdiImage8::GdiImage8(uint width, uint height)
{
	Init(width, height);
}

GdiImage8::GdiImage8(const _Size& size)
{
	Init(size.Width, size.Height);
}

GdiImage8::GdiImage8(HBITMAP hbitmap)
{
	BITMAP b;
	::GetObject(hbitmap, sizeof(b), &b);

	Init(b.bmWidth, b.bmHeight);

	HDC hdc = ::GetDC(null);

	BITMAPINFO bi;
	Gdi::InitBitmapHeader(
		&bi.bmiHeader, 
		b.bmWidth, 
		b.bmHeight);

	::GetDIBits(
		hdc,
		hbitmap,
		0,
		b.bmHeight,
		m_buffer,
		&bi,
		DIB_RGB_COLORS);

	::ReleaseDC(null, hdc);
}

GdiImage8::~GdiImage8()
{

}

void GdiImage8::Init(int width, int height)
{
	HDC hdc = ::GetDC(null);

	m_hdc = ::CreateCompatibleDC(hdc);

	BITMAPINFO bi;
	Gdi::InitBitmapHeader(
		&bi.bmiHeader, 
		width, 
		height);

	m_hbitmap = ::CreateDIBSection(
		hdc, 
		&bi, 
		DIB_RGB_COLORS,
		(void**)&m_buffer,
		null,
		0);
	m_hbitmapOld = (HBITMAP)::SelectObject(m_hdc, m_hbitmap);

	::ReleaseDC(null, hdc);
}

Object* GdiImage8::Copy()
{
	return new GdiImage8(*this);
}

void GdiImage8::SetSize(const _Size& size)
{
	/*	If the current size and the desired 
		size are the same, do nothing. */

	BITMAP b;
	::GetObject(m_hbitmap, sizeof(b), &b);

	if (size.Width == b.bmWidth
		&& size.Height == b.bmHeight)
		return;

#ifdef _DEBUG
	Gdi::LeakTest gdiLeakTest(L"GdiImage8::SetSize(const Size& size)");
#endif

	/*	Resize the bitmap. */

	//GdiImage8 copy(*this);

	::SelectObject(m_hdc, m_hbitmapOld);
	::DeleteObject(m_hbitmap);

	HDC hdc = ::GetDC(null);

	BITMAPINFO bi;
	Gdi::InitBitmapHeader(
		&bi.bmiHeader, 
		size.Width, 
		size.Height);

	m_hbitmap = ::CreateDIBSection(
		hdc, 
		&bi, 
		DIB_RGB_COLORS,
		(void**)&m_buffer,
		null,
		0);
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

void GdiImage8::Clear()
{
	BITMAP b;
	::GetObject(m_hbitmap, sizeof(b), &b);
	int bufferSize = b.bmWidth * b.bmHeight * 4;
	memset(m_buffer, 0, bufferSize);
}

byte* GdiImage8::GetBuffer()
{
	return m_buffer;
}