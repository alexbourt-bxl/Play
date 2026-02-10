#include "NavControl.h"

#include <Types/Graphics\Image.h>

DEFINE_TYPE(NavControl, Control);

NavControl::NavControl()
{
	m_normalImage = null;
	m_hoverImage  = null;

	m_normalOpacity = 0.1;
	m_hoverOpacity  = 0.4;

	m_over    = false;
	m_pressed = false;

	PaintEvent		+= EventHandler(this, &NavControl::OnPaint);
	MouseMoveEvent	+= EventHandler(this, &NavControl::OnMouseMove);
	MouseLeaveEvent += EventHandler(this, &NavControl::OnMouseLeave);
	LButtonDownEvent += EventHandler(this, &NavControl::OnLButtonDown);
	LButtonUpEvent	+= EventHandler(this, &NavControl::OnLButtonUp);

	SuspendUpdates();

	AllowFocus = false;
	Width   = 100;
	Height  = 100;
	Opacity = m_normalOpacity;

	ResumeUpdates();
}

NavControl::~NavControl()
{

}

Image* NavControl::GetNormalImage() const
{
	return m_normalImage;
}

void NavControl::SetNormalImage(Image* image)
{
	m_normalImage = image;
	Update();
}

Image* NavControl::GetHoverImage() const
{
	return m_hoverImage;
}

void NavControl::SetHoverImage(Image* image)
{
	m_hoverImage = image;
	Update();
}

double NavControl::GetNormalOpacity()
{
	return m_normalOpacity;
}

void NavControl::SetNormalOpacity(double opacity)
{
	m_normalOpacity = opacity;
	Update();
}

double NavControl::GetHoverOpacity()
{
	return m_hoverOpacity;
}

void NavControl::SetHoverOpacity(double opacity)
{
	m_hoverOpacity = opacity;
	Update();
}