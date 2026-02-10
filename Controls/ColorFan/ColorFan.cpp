#include "ColorFan.h"

#include <Controls\Window\Window.h>
#include <Math\Math.h>

#include <stdio.h>

//#pragma warning(disable:4355)

DEFINE_TYPE(ColorFan, Control);

ColorFan::ColorFan()
{
	ColorChangeEvent.Sender = this;

	Properties.Add(&Hue);
	Properties.Add(&Saturation);
	Properties.Add(&Lightness);

	Hue					= 0.0;
	Saturation			= 0.0;
	Lightness			= 0.0;
	m_satLightRadius	= 80.0;
	m_hueMinRadius		= m_satLightRadius + 2.0;
	m_hueMaxRadius		= m_hueMinRadius + 30.0;
	m_padding			= 10.0;
	m_changingHue		= false;
	m_changingSatLight	= false;

	m_hueAngleOffset	= PI / 6.0;

	m_paddedSize		= 0;
	
	m_imgSatLight		= new GdiImage8(
							Math::Round(m_satLightRadius * 2.0), 
							Math::Round(m_satLightRadius));

	PaintEvent			+= EventHandler(this, &ColorFan::OnPaint);

	LButtonDownEvent	+= EventHandler(this, &ColorFan::OnLButtonDown);
	MouseMoveEvent		+= EventHandler(this, &ColorFan::OnMouseMove);
	LButtonUpEvent		+= EventHandler(this, &ColorFan::OnLButtonUp);
	LDoubleClickEvent	+= EventHandler(this, &ColorFan::OnLDoubleClick);
	MouseWheelEvent		+= EventHandler(this, &ColorFan::OnMouseWheel);
	BackColorChangeEvent += EventHandler(this, &ColorFan::OnBackColorChange);

	Hue.PreChangeEvent	+= EventHandler(this, &ColorFan::PreHueChange);
	Hue.ChangeEvent		+= EventHandler(this, &ColorFan::OnHueChange);
	Saturation.ChangeEvent += EventHandler(this, &ColorFan::OnSaturationChange);
	Lightness.ChangeEvent += EventHandler(this, &ColorFan::OnLightnessChange);

	m_flagUpdate		= false;

	SuspendUpdates();

	BackColor = Color::Panel;
	AllowDoubleClick = true;
	UpdateControl();

	ResumeUpdates();
}

ColorFan::~ColorFan()
{
	Lock();

	if (m_imgSatLight)
	{
		delete m_imgSatLight;
		m_imgSatLight = null;
	}

	Unlock();
}

Rect ColorFan::GetFullPaintRect()
{
	_Rect fullPaintRect = PaintRect;

	fullPaintRect.Height += 10;

	return fullPaintRect;
}

Color ColorFan::GetSelectedColor()
{
	return Color::FromHsl(Hue, Saturation, Lightness);
}

void ColorFan::SetSelectedColor(const Color& color)
{
	double hue = Hue, sat, light;

	((Color&)color).ToHsl(&hue, &sat, &light);
	SetColor(hue, sat, light);
}

double ColorFan::GetInnerRadius()
{
	return m_satLightRadius;
}

void ColorFan::SetInnerRadius(double radius)
{
	m_satLightRadius = radius;

	UpdateControl();
	Update();
}

double ColorFan::GetHueInnerRadius()
{
	return m_hueMinRadius;
}

void ColorFan::SetHueInnerRadius(double radius)
{
	m_hueMinRadius = radius;

	UpdateControl();
	Update();
}

double ColorFan::GetHueOuterRadius()
{
	return m_hueMaxRadius;
}

void ColorFan::SetHueOuterRadius(double radius)
{
	m_hueMaxRadius = radius;

	UpdateControl();
	Update();
}

double ColorFan::GetPadding()
{
	return m_padding;
}

void ColorFan::SetPadding(double padding)
{
	m_padding = padding;

	UpdateControl();
	Update();
}

void ColorFan::UpdateControl()
{
	m_paddedSize = Math::Round(m_hueMaxRadius + m_padding);

	m_imgBack.SetSize(_Size(m_paddedSize * 2, m_paddedSize));
	m_imgHue.SetSize(_Size(Math::Round(m_hueMaxRadius * 2.0), Math::Round(m_hueMaxRadius)));

	DrawBack();
	DrawHue();
	DrawSatLight();
}

void ColorFan::DrawBack()
{
	int width  = m_paddedSize * 2;
	int height = m_paddedSize;

	int imgWidth = m_imgBack.Width;

	double floatPaddedSize = (double)m_paddedSize;

	double dx, dy, radius;
	int color, _opacity;
	int backColor = Color8(BackColor);

	int* buffer = (int*)m_imgBack.Buffer;

	for (int y = 0; y < height; y++)
	{
		dy = (double)(y - (height - 1));

		for (int x = 0; x < width; x++)
		{
			dx = (double)(x - (width / 2 - 1));
			
			radius = sqrt(dx*dx + dy*dy);

			_opacity = floatPaddedSize - radius < 1.0
				? Math::ToInt8(floatPaddedSize - radius)
				: 0xFF;

			color = Color8::MultiplyScalar(backColor, _opacity);

			buffer[y * imgWidth + x] = 
				radius <= floatPaddedSize
				? color
				: 0x00000000;
		}
	}
}

void ColorFan::DrawHue()
{
	int width  = Math::Round(m_hueMaxRadius * 2.0);
	int height = Math::Round(m_hueMaxRadius);

	int imgWidth = m_imgHue.Width;

	double dx, dy, radius, angle;
	Color color;

	int* buffer = (int*)m_imgHue.Buffer;

	for (int y = 0; y < height; y++)
	{
		dy = (double)(y - (height - 1));

		for (int x = 0; x < width; x++)
		{
			dx = (double)(x - (width / 2 - 1)) - 0.5;
			
			radius = sqrt(dx*dx + dy*dy);

			if (radius == 0.0)
				radius = ZERO;

			angle = acos(dx / radius) + m_hueAngleOffset;

			if (m_hueMaxRadius - radius >= 0.0 && m_hueMaxRadius - radius <= 1.0)
			{
				color = Color(
					Color::FromHsl((1.0 - angle / PI) * 359.9, 1.0, 0.5),
					m_hueMaxRadius - radius);
				color = Color::Premult(color);
			}
			else if (radius - m_hueMinRadius >= 0.0 && radius - m_hueMinRadius <= 1.0)
			{
				color = Color(
					Color::FromHsl((1.0 - angle / PI) * 359.9, 1.0, 0.5),
					radius - m_hueMinRadius);
				color = Color::Premult(color);
			}
			else if (radius < m_hueMaxRadius && radius > m_hueMinRadius)
			{
				color = Color::FromHsl((1.0 - angle / PI) * 359.9, 1.0, 0.5);
			}
			else
			{
				color = Color::Transparent;
			}

			buffer[y * imgWidth + x] = Color8(color);
		}
	}
}

void ColorFan::DrawSatLight()
{
	m_flagUpdate = true;
	Start();
}

uint ColorFan::ThreadProc()
{
	while (!WasKilled && m_flagUpdate)
	{
		m_flagUpdate = false;

		GdiImage8* imgSatLight = new GdiImage8(
			Math::Round(m_satLightRadius * 2.0), 
			Math::Round(m_satLightRadius));

		int width  = Math::Round(m_satLightRadius * 2.0);
		int height = Math::Round(m_satLightRadius);

		int imgWidth = imgSatLight->Width;

		double dx, dy, radius, a;
		Color color;

		double hue = Hue;

		Color cl = Color::FromHsl(hue, 1.0, 0.0);
		Color ct = Color::FromHsl(hue, 1.0, 0.5);
		Color cr = Color::FromHsl(hue, 1.0, 1.0);
		Color cg = Color::FromHsl(hue, 0.0, 0.5);

		int* buffer = (int*)imgSatLight->Buffer;

		for (int y = 0; y < height; y++)
		{
			dy = (double)(y - height);

			for (int x = 0; x < width; x++)
			{
				dx = (double)(x - (width / 2 - 1)) - 0.5;
				
				radius = sqrt(dx*dx + dy*dy);

				if (radius == 0.0)
					radius = ZERO;

				a = 1.0 - acos(dx / radius) / PI; // normalized angle [0-1]

				color = a < 0.5
					? Color::Lerp(cl, ct, a * 2.0)
					: Color::Lerp(cr, ct, (1.0 - a) * 2.0);

				color = Color::Lerp(cg, color, radius / m_satLightRadius);

				if (m_satLightRadius - radius >= 0.0 && m_satLightRadius - radius < 1.0)
				{
					color.A = m_satLightRadius - radius;
					color = Color::Premult(color);
				}
				else if (radius > m_satLightRadius)
				{
					color = Color::Transparent;
				}

				buffer[y * imgWidth + x] = Color8(color);
			}
		}

		Lock();

		if (m_imgSatLight)
			delete m_imgSatLight;
			
		m_imgSatLight = imgSatLight;

		Region updateRgn(_Rect(
			PaintRect.X + m_paddedSize - (int)m_satLightRadius,
			PaintRect.Y + m_paddedSize - (int)m_satLightRadius,
			m_imgSatLight->Width,
			m_imgSatLight->Height));

		if (m_flagUpdate)
			Trail.Add(updateRgn);

		Unlock();

		if (!m_flagUpdate)
			Update(updateRgn);
	}

	//       FAN
	return 0xFA2;
}

bool ColorFan::AdjustSize(_Size& size)
{
	size = m_imgBack.Size;
	return true;
}

bool ColorFan::PickHue(double x, double y, double* hue, bool forcePick)
{
	double dx = x - (double)m_paddedSize;
	double dy = min(y - (double)m_paddedSize, 0);
			
	double radius = sqrt(dx*dx + dy*dy);

	if ((radius < m_hueMaxRadius && radius > m_hueMinRadius) || forcePick)
	{
		if (radius == 0.0)
			radius = ZERO;

		double angle = acos(dx / radius) + m_hueAngleOffset;

		*hue = (PI - angle) * 359.9 / PI;

		return true;
	}

	return false;
}

bool ColorFan::PickSatLight(double x, double y, double* sat, double* light, bool forcePick)
{
	double dx = x - (double)m_paddedSize;
	double dy = min(y - (double)m_paddedSize, 0);
	
	/*	Make sure the indicator stays "inside" the fan. */

	double satLightRadius = m_satLightRadius - 2.0;
	double radius = sqrt(dx*dx + dy*dy);
	
	if (radius > satLightRadius)
	{
		if (!forcePick)
			return false;

		dx /= radius / satLightRadius;
		dy /= radius / satLightRadius;
		radius = satLightRadius;
	}

	if (radius <= m_satLightRadius + 3.0 || forcePick)
	{
		if (radius == 0.0)
			radius = ZERO;

		double a = 1.0 - acos(dx / radius) / PI; // normalized angle [0-1]

		double hue = Hue;

		Color cl = Color::FromHsl(hue, 1.0, 0.0);
		Color ct = Color::FromHsl(hue, 1.0, 0.5);
		Color cr = Color::FromHsl(hue, 1.0, 1.0);
		Color cg = Color::FromHsl(hue, 0.0, 0.5);

		Color color = a < 0.5
			? Color::Lerp(cl, ct, a * 2.0)
			: Color::Lerp(cr, ct, (1.0 - a) * 2.0);

		color = Color::Lerp(cg, color, radius / m_satLightRadius);

		Color(color).ToHsl(&hue, sat, light);

		/*	The bottom edge is 
			a special case. */
		if (y > m_paddedSize)
			*sat = 0.0;

		return true;
	}

	return false;
}

void ColorFan::SetColor(double hue, double saturation, double lightness)
{
	Region updateRgn;

	Hue.DisableEvents();
	Saturation.DisableEvents();
	Lightness.DisableEvents();

	if (Hue != hue)
	{
		double hueAngle = Hue * PI * 361.0 / 360.0 / 359.9 + m_hueAngleOffset;

		double sinHue = sin(hueAngle);
		double cosHue = cos(hueAngle);

		_Rect rect(
			PaintRect.X + m_paddedSize - (int)(m_hueMaxRadius * cosHue),
			PaintRect.Y + m_paddedSize - (int)(m_hueMaxRadius * sinHue),
			(int)((m_hueMaxRadius - m_hueMinRadius) * cosHue),
			(int)((m_hueMaxRadius - m_hueMinRadius) * sinHue));

		rect.Normalize();

		rect.X      -= 8;
		rect.Y      -= 8;
		rect.Width  += 16;
		rect.Height += 16;

		updateRgn.Add(rect);

		Hue = hue;
		DrawSatLight();

		hueAngle = Hue * PI * 361.0 / 360.0 / 359.9 + m_hueAngleOffset;

		sinHue = sin(hueAngle);
		cosHue = cos(hueAngle);

		rect = _Rect(
			PaintRect.X + m_paddedSize - (int)(m_hueMaxRadius * cosHue),
			PaintRect.Y + m_paddedSize - (int)(m_hueMaxRadius * sinHue),
			(int)((m_hueMaxRadius - m_hueMinRadius) * cosHue),
			(int)((m_hueMaxRadius - m_hueMinRadius) * sinHue));

		rect.Normalize();

		rect.X      -= 8;
		rect.Y      -= 8;
		rect.Width  += 16;
		rect.Height += 16;

		updateRgn.Add(rect);
	}

	if (Saturation != saturation
		|| Lightness != lightness)
	{
		FPoint pcPrev = GetSatLightCoords(Saturation, Lightness);
		Trail.Add(_Rect(
			(int)pcPrev.X - 5,
			(int)pcPrev.Y - 5,
			10,
			10));

		Saturation = saturation;
		Lightness = lightness;

		FPoint pc = GetSatLightCoords(Saturation, Lightness);
		updateRgn.Add(_Rect(
			(int)pc.X - 5,
			(int)pc.Y - 5,
			10,
			10));
	}

	Hue.EnableEvents();
	Saturation.EnableEvents();
	Lightness.EnableEvents();

	Update(updateRgn);

	::Message msg(this);
	ColorChangeEvent.Send(msg);
}

FPoint ColorFan::GetSatLightCoords(double saturation, double lightness)
{
	Color color = Color::FromHsl(Hue, saturation, lightness);

	/*	First check for the special case when s = 0 and L = 0.5, which
		means that the indicator is at the center of the bottom edge. */

	if (saturation == 0.0 && lightness == 0.5)
	{
		return FPoint(
			(double)(PaintRect.X + m_paddedSize),
			(double)(PaintRect.Y + m_paddedSize));
	}

	/*	Get the radius based on the most distant (from gray) RGB component. */

	double cFar = GetFarthest(color);
	double radius = fabs(cFar - 0.5) * 2.0;

	/*	Extrapolate from the current color to the border. */

	Color borderColor = Color::Lerp(
		Color::Gray, color, 
		1.0 / radius);

	/*	Determine the angle by finding where the most characteristic 
		component of the border color is in relation to its neighbors. */

	double a = lightness > 0.5
		? 0.5 + GetMinComponent(borderColor) / 2.0
		: GetMaxComponent(borderColor) / 2.0;

	double angle = (1.0 - a) * PI;

	double x = cos(angle) * m_satLightRadius * radius;
	double y = sin(angle) * m_satLightRadius * radius;
	
	return FPoint(
		(double)(PaintRect.X + m_paddedSize) + x,
		(double)(PaintRect.Y + m_paddedSize) - y);
}

double ColorFan::GetMinComponent(const Color& color)
{
	return min(min(color.R, color.G), color.B);
}

double ColorFan::GetMaxComponent(const Color& color)
{
	return max(max(color.R, color.G), color.B);
}

double ColorFan::GetFarthest(const Color& color)
{
	double dR = fabs(color.R - 0.5);
	double dG = fabs(color.G - 0.5);
	double dB = fabs(color.B - 0.5);

	if (dR >= dG && dR >= dB)
		return color.R;
	else if (dG >= dR && dG >= dB)
		return color.G;
	else if (dB >= dR && dB >= dG)
		return color.B;

	DebugBreak();
	return 0.0; // should never happen
}