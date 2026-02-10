#include "ImageControl.h"

#include <Types/Graphics\Image.h>

DEFINE_TYPE(ImageControl, Control);

ImageControl::ImageControl()
{
	PaintEvent += EventHandler(this, &ImageControl::OnPaint);

	m_image = null;
	m_fitImage = false;

	m_align = Align::Center | Align::Middle;
	m_autoSize = false;
}

ImageControl::ImageControl(_Image* image, bool own)
{
	PaintEvent += EventHandler(this, &ImageControl::OnPaint);

	m_image = null;

	m_align = Align::Center | Align::Middle;
	m_autoSize = false;

	SetImage(image, own);
}

ImageControl::~ImageControl()
{
	if (m_image && m_own)
		delete m_image;
}

Image* ImageControl::GetImage() const
{
	return m_image;
}

void ImageControl::SetImage(_Image* image, bool own)
{
	if (m_image && m_own)
	{
		delete m_image;
		m_image = null;
	}

	m_image = image;
	m_own = own;

	if (m_image && m_autoSize)
		Size = m_image->Size;
}

bool ImageControl::GetFitImage()
{
	return m_fitImage;
}

void ImageControl::SetFitImage(bool fitImage)
{
	m_fitImage = fitImage;
	Update();
}

Align ImageControl::GetAlign()
{
	return m_align;
}

void ImageControl::SetAlign(_Align align)
{
	m_align = align;

	MarkForLayout();
	Update();
}

bool ImageControl::GetAutoSize()
{
	return m_autoSize;
}

void ImageControl::SetAutoSize(bool autoSize)
{
	m_autoSize = autoSize;

	if (m_image)
		Size = m_image->Size;

	MarkForLayout();
	Update();
}