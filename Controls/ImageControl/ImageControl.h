#ifndef IMAGE_CONTROL_H
#define IMAGE_CONTROL_H

#include <Controls\Control.h>
#include <Types/Geometric\Align.h>
#include <Graphics\Graphics.h>

class Image;

class ImageControl 
	: public Control
{
	DECLARE_TYPE;

	typedef Image _Image;
	typedef Align _Align;

public:
	ImageControl();
	ImageControl(Image* image, bool own = true);
	~ImageControl();

	__declspec(property(get = GetImage))
	_Image*	Image;
	_Image*	GetImage() const;
	void	SetImage(_Image* image, bool own = true);

	__declspec(property(get = GetFitImage, put = SetFitImage))
	bool	FitImage;
	bool	GetFitImage();
	void	SetFitImage(bool fitImage);

	__declspec(property(get = GetAlign, put = SetAlign))
	_Align	Align;
	_Align	GetAlign();
	void	SetAlign(_Align align);

	__declspec(property(get = GetAutoSize, put = SetAutoSize))
	bool	AutoSize;
	bool	GetAutoSize();
	void	SetAutoSize(bool autoSize);

private:
	_Image*	m_image;
	bool	m_own;

	bool	m_fitImage;

	_Align	m_align;
	bool	m_autoSize;

public:
	void	OnPaint(Graphics::Message& msg);
};

#endif