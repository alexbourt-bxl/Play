#include "NavControl.h"

#include <Types/Graphics\Image.h>
#include <Graphics\Graphics.h>
#include <Math\Math.h>

void NavControl::OnPaint(Graphics::Message& msg)
{
	//int opacity = Math::ToInt8(m_normalOpacity);

	if (m_over || m_pressed)
	{
		msg.Graphics.FillEllipse(
			FPoint(
				(double)(PaintRect.X + PaintRect.Width / 2),
				(double)(PaintRect.Y + PaintRect.Height / 2)),
			(double)(PaintRect.Width / 2 - 2),
			(double)(PaintRect.Height / 2 - 2),
			0.0,
			Color::Gray);

		msg.Graphics.DrawEllipse(
			FPoint(
				(double)(PaintRect.X + PaintRect.Width / 2),
				(double)(PaintRect.Y + PaintRect.Height / 2)),
			(double)(PaintRect.Width / 2 - 1),
			(double)(PaintRect.Height / 2 - 1),
			0.0,
			Color(0.19, 0.19, 0.19));

		msg.Graphics.DrawEllipse(
			FPoint(
				(double)(PaintRect.X + PaintRect.Width / 2),
				(double)(PaintRect.Y + PaintRect.Height / 2)),
			(double)(PaintRect.Width / 2 - 2),
			(double)(PaintRect.Height / 2 - 2),
			0.0,
			Color(0.19, 0.19, 0.19));

		msg.Graphics.DrawEllipse(
			FPoint(
				(double)(PaintRect.X + PaintRect.Width / 2),
				(double)(PaintRect.Y + PaintRect.Height / 2)),
			(double)(PaintRect.Width / 2 - 3),
			(double)(PaintRect.Height / 2 - 3),
			0.0,
			Color(Color::White, 0.69));

		Image* hoverImage = m_hoverImage
			? m_hoverImage
			: m_normalImage;

		if (hoverImage)
		{
			msg.Graphics.DrawImage(
				PaintRect.X + (PaintRect.Width - hoverImage->Width) / 2,
				PaintRect.Y + (PaintRect.Height - hoverImage->Height) / 2,
				hoverImage);
		}
	}
	else
	{
		msg.Graphics.FillEllipse(
			FPoint(
				(double)(PaintRect.X + PaintRect.Width / 2),
				(double)(PaintRect.Y + PaintRect.Height / 2)),
			(double)(PaintRect.Width / 2 - 2),
			(double)(PaintRect.Height / 2 - 2),
			0.0,
			Color(Color::Gray, 0.5));

		msg.Graphics.DrawEllipse(
			FPoint(
				(double)(PaintRect.X + PaintRect.Width / 2),
				(double)(PaintRect.Y + PaintRect.Height / 2)),
			(double)(PaintRect.Width / 2 - 1),
			(double)(PaintRect.Height / 2 - 1),
			0.0,
			Color::Black);

		msg.Graphics.DrawEllipse(
			FPoint(
				(double)(PaintRect.X + PaintRect.Width / 2),
				(double)(PaintRect.Y + PaintRect.Height / 2)),
			(double)(PaintRect.Width / 2 - 2),
			(double)(PaintRect.Height / 2 - 2),
			0.0,
			Color(Color::White, 0.69));

		if (m_normalImage)
		{
			msg.Graphics.DrawImage(
				PaintRect.X + (PaintRect.Width - m_normalImage->Width) / 2,
				PaintRect.Y + (PaintRect.Height - m_normalImage->Height) / 2,
				m_normalImage);
		}
	}
}

void NavControl::OnMouseMove(Mouse::Message& msg)
{
	double distFromCenter = sqrt((double)(SQR(msg.Position.X - ClientRect.Width / 2) + SQR(msg.Position.Y - ClientRect.Height / 2)));

	bool over = distFromCenter < (double)ClientRect.Width / 2.0;

	if (over != m_over && !m_pressed)
	{
		m_over = over;

		Opacity = m_over
			? m_hoverOpacity
			: m_normalOpacity;
	}
}

void NavControl::OnMouseLeave(Mouse::Message& /*msg*/)
{
	if (m_over)
	{
		m_over = false;
		Opacity = m_normalOpacity;
	}
}

void NavControl::OnLButtonDown(Mouse::Message& /*msg*/)
{
	if (m_over)
		m_pressed = true;
}

void NavControl::OnLButtonUp(Mouse::Message& /*msg*/)
{
	m_pressed = false;
}