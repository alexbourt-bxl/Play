#ifndef COLOR_FAN_H
#define COLOR_FAN_H

#include <Controls\Control.h>
#include <Types/Graphics\GdiImage8.h>
#include <Properties\FloatProperty\FloatProperty.h>
#include <Types/Color\Color.h>
#include <Threads\Thread.h>
#include <Threads\ThreadLock.h>

class ColorFan 
	: public Control, 
	  public Thread
{
	DECLARE_TYPE;

public:
	ColorFan();
	~ColorFan();

	_Rect GetFullPaintRect();

	FloatProperty Hue;
	FloatProperty Saturation;
	FloatProperty Lightness;

	__declspec(property(get = GetSelectedColor, put = SetSelectedColor))
	Color	SelectedColor;
	Color	GetSelectedColor();
	void	SetSelectedColor(const Color& color);

	__declspec(property(get = GetInnerRadius, put = SetInnerRadius))
	double	InnerRadius;
	double	GetInnerRadius();
	void	SetInnerRadius(double radius);

	__declspec(property(get = GetHueInnerRadius, put = SetHueInnerRadius))
	double	HueInnerRadius;
	double	GetHueInnerRadius();
	void	SetHueInnerRadius(double radius);

	__declspec(property(get = GetHueOuterRadius, put = SetHueOuterRadius))
	double	HueOuterRadius;
	double	GetHueOuterRadius();
	void	SetHueOuterRadius(double radius);

	__declspec(property(get = GetPadding, put = SetPadding))
	double	Padding;
	double	GetPadding();
	void	SetPadding(double padding);

	uint	ThreadProc();

	bool	AdjustSize(_Size& size);

	Event<::Message&> ColorChangeEvent;

private:
	double	m_satLightRadius;
	double	m_hueMinRadius;
	double	m_hueMaxRadius;
	double	m_padding;
	int		m_paddedSize; // the total "radius" of the control

	double	m_hueAngleOffset;

	GdiImage8 m_imgBack;
	GdiImage8 m_imgHue;
	GdiImage8* m_imgSatLight;

	void	UpdateControl();

	void	DrawBack();
	void	DrawHue();
	void	DrawSatLight();

	bool	PickHue(double x, double y, double* hue, bool forcePick = false);
	bool	PickSatLight(double x, double y, double* sat, double* light, bool forcePick = false);

	void	SetColor(double hue, double saturation, double lightness);

	FPoint	GetSatLightCoords(double saturation, double lightness);
	double	GetMinComponent(const Color& color);
	double	GetMaxComponent(const Color& color);
	double	GetFarthest(const Color& color);
	
	bool	m_changingHue;
	bool	m_changingSatLight;

	bool	m_flagUpdate;

	void	OnPaint(Graphics::Message& msg);

	void	OnLButtonDown(Mouse::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	OnLDoubleClick(Mouse::Message& msg);
	void	OnMouseWheel(Mouse::Message& msg);
	void	OnBackColorChange(Control::Message& msg);

	void	PreHueChange(Property::Message& msg);
	void	OnHueChange(Property::Message& msg);
	void	OnSaturationChange(Property::Message& msg);
	void	OnLightnessChange(Property::Message& msg);
};

#endif