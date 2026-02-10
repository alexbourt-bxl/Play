#include "ColorFan.h"

void ColorFan::OnPaint(Graphics::Message& msg)
{
	msg.Graphics.Antialias = Graphics::Low;

	// Draw the background

	Region rgnOld = msg.Graphics.ClipRegion;
	msg.Graphics.ClipRegion = Region(PaintRect);

	msg.Graphics.FillEllipse(
		FPoint(
			PaintRect.X + PaintRect.Width / 2,
			PaintRect.Bottom),
		(double)m_paddedSize,
		(double)m_paddedSize,
		0.0,
		Color::Panel);

	msg.Graphics.ClipRegion = rgnOld;

	// Draw the hue fan

	msg.Graphics.DrawImage(
		PaintRect.X + m_paddedSize - (int)m_hueMaxRadius,
		PaintRect.Y + m_paddedSize - (int)m_hueMaxRadius,
		&m_imgHue);

	// Draw the saturation & lightness fan

	msg.Graphics.DrawImage(
		PaintRect.X + m_paddedSize - (int)m_satLightRadius,
		PaintRect.Y + m_paddedSize - (int)m_satLightRadius,
		m_imgSatLight);

	// Draw the hue indicator

	Graphics::_Antialias oldAntialias = msg.Graphics.Antialias;
	msg.Graphics.Antialias = Graphics::High;

	bool oldSubpixel = msg.Graphics.Subpixel;
	msg.Graphics.Subpixel = false;
	msg.Graphics.SubpixelHorizontal = true;

	double hueAngle = Hue * PI * 361 / 360 / 359.9 + m_hueAngleOffset;

	if (hueAngle > PI)
		hueAngle -= PI;

	double sinHue = sin(hueAngle);
	double cosHue = cos(hueAngle);

	msg.Graphics.DrawLine(
		PaintRect.X + m_paddedSize - m_hueMaxRadius * cosHue,
		PaintRect.Y + m_paddedSize - m_hueMaxRadius * sinHue,
		PaintRect.X + m_paddedSize - (m_hueMinRadius + 1) * cosHue,
		PaintRect.Y + m_paddedSize - (m_hueMinRadius + 1) * sinHue,
		Color::Black,
		2);

	msg.Graphics.DrawLine(
		PaintRect.X + m_paddedSize - (m_hueMaxRadius - 10) * cosHue,
		PaintRect.Y + m_paddedSize - (m_hueMaxRadius - 10) * sinHue,
		PaintRect.X + m_paddedSize - (m_hueMinRadius + 11) * cosHue,
		PaintRect.Y + m_paddedSize - (m_hueMinRadius + 11) * sinHue,
		Color::White,
		2.5);

	// Draw the saturation & lightness indicator

	FPoint pc = GetSatLightCoords(Saturation, Lightness);

	msg.Graphics.FillEllipse(pc, 4, 4, 0, Color::Black);
	msg.Graphics.FillEllipse(pc, 2, 2, 0, Color::White);

	msg.Graphics.Antialias = oldAntialias;
	msg.Graphics.Subpixel = oldSubpixel;
}

void ColorFan::OnLButtonDown(Mouse::Message& msg)
{
	double hue = Hue;
	double sat = Saturation;
	double light = Lightness;

	if (PickSatLight((double)msg.Position.X, (double)msg.Position.Y, &sat, &light))
	{
		SetColor(hue, sat, light);
		m_changingSatLight = true;
	}
	else if (PickHue((double)msg.Position.X, (double)msg.Position.Y, &hue))
	{
		SetColor(hue, sat, light);
		m_changingHue = true;
	}
}

void ColorFan::OnMouseMove(Mouse::Message& msg)
{
	double hue = Hue;
	double sat = Saturation;
	double light = Lightness;

	if (m_changingSatLight)
	{
		PickSatLight((double)msg.Position.X, (double)msg.Position.Y, &sat, &light, true);
		SetColor(hue, sat, light);
	}
	else if (m_changingHue)
	{
		PickHue((double)msg.Position.X, (double)msg.Position.Y, &hue, true);
		SetColor(hue, sat, light);
	}
}

void ColorFan::OnLButtonUp(Mouse::Message& /*msg*/)
{
	m_changingHue = false;
	m_changingSatLight = false;
}

void ColorFan::OnLDoubleClick(Mouse::Message& msg)
{
	double hue = Hue;

	if (PickHue((double)msg.Position.X, (double)msg.Position.Y, &hue))
		SetColor(hue, 1.0, 0.5);
}

void ColorFan::OnMouseWheel(Mouse::Message& msg)
{
	double hue = Hue;
	double sat = Saturation;
	double light = Lightness;

	if (PickSatLight((double)msg.Position.X, (double)msg.Position.Y, &sat, &light))
	{
		sat = min(max(0.0, (double)Saturation + msg.WheelDelta * 0.01), 1.0);

		SetColor(
			Hue, 
			sat, 
			Lightness);
	}
	else if (PickHue((double)msg.Position.X, (double)msg.Position.Y, &hue))
	{
		hue = (double)Hue + msg.WheelDelta * 1.0;

		if (hue >= 360.0)
			hue -= (double)(((int)hue / 360) * 360);
		else if (hue < 0.0)
			hue += (double)((((int)fabs(hue) / 360) + 1) * 360);

		SetColor(
			hue, 
			Saturation, 
			Lightness);
	}
}

void ColorFan::OnBackColorChange(Control::Message& /*msg*/)
{
	DrawBack();
}

void ColorFan::PreHueChange(Property::Message& msg)
{
	double& hue = *(double*)msg.Value;

	if (hue >= 360.0)
		hue -= (double)(((int)hue / 360) * 360);
	else if (hue < 0.0)
		hue += (double)((((int)fabs(hue) / 360) + 1) * 360);

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

	Trail.Add(rect);
}

void ColorFan::OnHueChange(Property::Message& /*msg*/)
{
	DrawSatLight();

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

	Region updateRgn(rect);
	Update(updateRgn);

	::Message ce(this);
	ColorChangeEvent.Send(ce);
}

void ColorFan::OnSaturationChange(Property::Message& /*msg*/)
{
	::Message ce(this);
	ColorChangeEvent.Send(ce);
}

void ColorFan::OnLightnessChange(Property::Message& /*msg*/)
{
	::Message ce(this);
	ColorChangeEvent.Send(ce);
}