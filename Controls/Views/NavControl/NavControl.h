#ifndef NAV_CONTROL_H
#define NAV_CONTROL_H

#include <Controls\Control.h>

class Image;

class NavControl
	: public Control
{
	DECLARE_TYPE;

	friend class ImageView;
	friend class ShapeView;
	friend class TreeView;

public:
	NavControl();
	~NavControl();

	__declspec(property(get = GetNormalImage, put = SetNormalImage))
	Image* NormalImage;
	Image* GetNormalImage() const;
	void   SetNormalImage(Image* image);

	__declspec(property(get = GetHoverImage, put = SetHoverImage))
	Image* HoverImage;
	Image* GetHoverImage() const;
	void   SetHoverImage(Image* image);

	__declspec(property(get = GetNormalOpacity, put = SetNormalOpacity))
	double Normalopacity;
	double GetNormalOpacity();
	void  SetNormalOpacity(double opacity);

	__declspec(property(get = GetHoverOpacity, put = SetHoverOpacity))
	double Hoveropacity;
	double GetHoverOpacity();
	void  SetHoverOpacity(double opacity);

	void OnPaint(Graphics::Message& msg);
	void OnMouseMove(Mouse::Message& msg);
	void OnMouseLeave(Mouse::Message& msg);
	void OnLButtonDown(Mouse::Message& msg);
	void OnLButtonUp(Mouse::Message& msg);

private:
	Image* m_normalImage;
	Image* m_hoverImage;

	double m_normalOpacity;
	double m_hoverOpacity;

	bool m_over;
	bool m_pressed;
};

#endif