#include "OpGradient.h"

#include <Controls\Views\ImageView\ImageView.h>
#include <Operators\Tree\Tree.h>

void OpGradient::OnPaint(Graphics::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	// Get display coordinates

	if (!view->Surface)
		return;

	FPoint p0((double)X0 * (double)view->Surface->Width, (double)Y0 * (double)view->Surface->Height);
	FPoint p1((double)X1 * (double)view->Surface->Width, (double)Y1 * (double)view->Surface->Height);

	view->ImageToView(&p0);
	view->ImageToView(&p1);

	// Offset coordinates to match control

	Point _p(0, 0);
	view->PointToWindow(&_p);

	p0.X += (double)_p.X;
	p0.Y += (double)_p.Y;

	p1.X += (double)_p.X;
	p1.Y += (double)_p.Y;

	// Draw the colors

	msg.Graphics.Antialias = Graphics::Medium;

	_Color shadowColor(_Color::Black, 0.15);

	switch (Type.Button.SelectedIndex)
	{
	case 0: // Linear
	case 1: // Radial
		{
			msg.Graphics.DrawLine(p0 + FPoint(-1, 0.5), p1 + FPoint(-1, 0.5), shadowColor);
			msg.Graphics.DrawLine(p0 + FPoint(1, 0.5),  p1 + FPoint(1, 0.5),  shadowColor);
			msg.Graphics.DrawLine(p0 + FPoint(0, -0.5), p1 + FPoint(0, -0.5), shadowColor);
			msg.Graphics.DrawLine(p0 + FPoint(0, 1.5),  p1 + FPoint(0, 1.5),  shadowColor);

			msg.Graphics.DrawLine(p0, p1, _Color(_Color::White, 0.5));

			for (int i = 0; i < m_colors.Count; i++)
			{
				_Color borderColor =
					_Color::GetLuminance(m_colors[i]) > 0.5
					? _Color(_Color::Black, 0.5)
					: _Color(_Color::White, 0.75);

				FPoint p(
					p0.X + (p1.X - p0.X) * m_positions[i] + 0.5,
					p0.Y + (p1.Y - p0.Y) * m_positions[i] + 0.5);

				// Shadow

				double srad = m_colorRadius + m_colorBorder;

				msg.Graphics.FillEllipse(p + FPoint(-1.5, 1), srad, srad, 0, shadowColor);
				msg.Graphics.FillEllipse(p + FPoint(1.5, 1),  srad, srad, 0, shadowColor);
				msg.Graphics.FillEllipse(p + FPoint(0, -0.5), srad, srad, 0, shadowColor);
				msg.Graphics.FillEllipse(p + FPoint(0, 2.5),  srad, srad, 0, shadowColor);

				// Border
				
				msg.Graphics.FillEllipse(
					p, 
					srad, 
					srad, 
					0,
					borderColor);

				// Color
				
				msg.Graphics.FillEllipse(
					p, 
					m_colorRadius, 
					m_colorRadius, 
					0,
					m_colors[i]);

				// Selected

				if (i == m_selected)
				{
					msg.Graphics.DrawEllipse(
						p,
						m_colorRadius + m_selectedBorder * 2,
						m_colorRadius + m_selectedBorder * 2,
						0,
						borderColor,
						m_selectedBorder);
				}
			}

			break;
		}
	case 2: // Angle
		{
			double gradientRadius = Math::GetDistance(p0, p1);

			double breakAngle = Math::GetAngle(p0, p1);
			double radiusOffset = 1.3 * m_colorRadius;
			double _gradientRadius = gradientRadius - radiusOffset;

			FPoint p0_(
				p0.X - radiusOffset * cos(breakAngle),
				p0.Y - radiusOffset * sin(breakAngle));

			// Draw the center circle

			msg.Graphics.DrawEllipse(
				p0,
				m_colorRadius + m_colorBorder / 2,
				m_colorRadius + m_colorBorder / 2,
				0,
				_Color(_Color::Black, 0.5),
				m_colorBorder);

			// Draw the arc

			msg.Graphics.DrawArc(p0_ + FPoint(-1, 0.5), _gradientRadius, breakAngle, breakAngle + PI, shadowColor);
			msg.Graphics.DrawArc(p0_ + FPoint(1, 0.5),  _gradientRadius, breakAngle, breakAngle + PI, shadowColor);
			msg.Graphics.DrawArc(p0_ + FPoint(0, -0.5), _gradientRadius, breakAngle, breakAngle + PI, shadowColor);
			msg.Graphics.DrawArc(p0_ + FPoint(0, 1.5),  _gradientRadius, breakAngle, breakAngle + PI, shadowColor);

			msg.Graphics.DrawArc(p0_,  _gradientRadius, breakAngle, breakAngle + PI, _Color(_Color::White, 0.5));

			// BUG: incomplete arc (in the gap between the two arcs)

			msg.Graphics.DrawArc(p0 + FPoint(-1, 0.5), gradientRadius, breakAngle + PI, breakAngle + 2 * PI, shadowColor);
			msg.Graphics.DrawArc(p0 + FPoint(1, 0.5),  gradientRadius, breakAngle + PI, breakAngle + 2 * PI, shadowColor);
			msg.Graphics.DrawArc(p0 + FPoint(0, -0.5), gradientRadius, breakAngle + PI, breakAngle + 2 * PI, shadowColor);
			msg.Graphics.DrawArc(p0 + FPoint(0, 1.5),  gradientRadius, breakAngle + PI, breakAngle + 2 * PI, shadowColor);

			msg.Graphics.DrawArc(p0,  gradientRadius, breakAngle + PI, breakAngle + 2 * PI, _Color(_Color::White, 0.5));

			for (int i = 0; i < m_colors.Count; i++)
			{
				_Color borderColor =
					_Color::GetLuminance(m_colors[i]) > 0.5
					? _Color(_Color::Black, 0.5)
					: _Color(_Color::White, 0.75);

				FPoint p;
				
				double colorAngle = breakAngle + m_positions[i] * 2 * PI;

				if (m_positions[i] < 0.5)
				{
					p.X = p0_.X + _gradientRadius * cos(colorAngle) + 0.5;
					p.Y = p0_.Y + _gradientRadius * sin(colorAngle) + 0.5;
				}
				else
				{
					p.X = p0.X + gradientRadius * cos(colorAngle) + 0.5;
					p.Y = p0.Y + gradientRadius * sin(colorAngle) + 0.5;
				}

				// Radius line

				if (i < m_colors.Count - 1)
				{
					msg.Graphics.DrawLine(p0 + FPoint(-1.5, 0.5), p + FPoint(-1.5, 1), shadowColor);
					msg.Graphics.DrawLine(p0 + FPoint(1.5, 0.5),  p + FPoint(1.5, 1),  shadowColor);
					msg.Graphics.DrawLine(p0 + FPoint(0, -0.5), p + FPoint(0, -0.5), shadowColor);
					msg.Graphics.DrawLine(p0 + FPoint(0, 1.5),  p + FPoint(0, 2.5),  shadowColor);

					msg.Graphics.DrawLine(p0, p, borderColor);
				}

				// Shadow

				double srad = m_colorRadius + m_colorBorder;

				msg.Graphics.FillEllipse(p + FPoint(-1.5, 1), srad, srad, 0, shadowColor);
				msg.Graphics.FillEllipse(p + FPoint(1.5, 1),  srad, srad, 0, shadowColor);
				msg.Graphics.FillEllipse(p + FPoint(0, -0.5), srad, srad, 0, shadowColor);
				msg.Graphics.FillEllipse(p + FPoint(0, 2.5),  srad, srad, 0, shadowColor);

				// Border
				
				msg.Graphics.FillEllipse(
					p, 
					srad, 
					srad, 
					0,
					borderColor);

				// Color
				
				msg.Graphics.FillEllipse(
					p, 
					m_colorRadius, 
					m_colorRadius, 
					0,
					m_colors[i]);

				// Selected

				if (i == m_selected)
				{
					msg.Graphics.DrawEllipse(
						p,
						m_colorRadius + m_selectedBorder * 2,
						m_colorRadius + m_selectedBorder * 2,
						0,
						borderColor,
						m_selectedBorder);
				}
			}

			break;
		}
	}
}

void OpGradient::OnLButtonDown(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	// Get display coordinates

	FPoint p0 = m_p0; //((double)X0 * (double)Width, (double)Y0 * (double)Height);
	FPoint p1 = m_p1; //((double)X1 * (double)Width, (double)Y1 * (double)Height);

	view->ImageToView(&p0);
	view->ImageToView(&p1);

	// Offset coordinates to match control

	//Point _p(0, 0);
	//view->PointToWindow(&_p);

	//p0.X += (double)_p.X;
	//p0.Y += (double)_p.Y;

	//p1.X += (double)_p.X;
	//p1.Y += (double)_p.Y;

	switch (Type.Button.SelectedIndex)
	{
	case 0: // Linear
	case 1: // Radial
		{
			for (int i = m_colors.Count - 1; i >= 0; i--)
			{
				FPoint p(
					p0.X + (p1.X - p0.X) * m_positions[i] + 0.5,
					p0.Y + (p1.Y - p0.Y) * m_positions[i] + 0.5);

				if (Math::GetDistance(FPoint(msg.Position.X, msg.Position.Y), p) <= m_colorRadius + m_colorBorder)
				{
					m_selected = i;
					m_held = i;
					view->Update(); // TODO: update region around previous and this colors
					break;
				}
			}

			break;
		}
	case 2: // Angle
		{
			if (Math::GetDistance(FPoint(msg.Position.X, msg.Position.Y), p0) <= m_colorRadius + m_colorBorder)
			{
				m_held = -1;
			}
			else
			{
				double gradientRadius = Math::GetDistance(p0, p1);

				double breakAngle = Math::GetAngle(p0, p1);
				double radiusOffset = 1.3 * m_colorRadius;
				double _gradientRadius = gradientRadius - radiusOffset;

				FPoint p0_(
					p0.X - radiusOffset * cos(breakAngle),
					p0.Y - radiusOffset * sin(breakAngle));

				for (int i = m_colors.Count - 1; i >= 0; i--)
				{
					FPoint p;
					
					double colorAngle = breakAngle + m_positions[i] * 2 * PI;

					if (m_positions[i] < 0.5)
					{
						p.X = p0_.X + _gradientRadius * cos(colorAngle) + 0.5;
						p.Y = p0_.Y + _gradientRadius * sin(colorAngle) + 0.5;
					}
					else
					{
						p.X = p0.X + gradientRadius * cos(colorAngle) + 0.5;
						p.Y = p0.Y + gradientRadius * sin(colorAngle) + 0.5;
					}

					if (Math::GetDistance(FPoint(msg.Position.X, msg.Position.Y), p) <= m_colorRadius + m_colorBorder)
					{
						m_selected = i;
						m_held = i;
						view->Update(); // TODO: update region around previous and this colors
						break;
					}
				}
			}

			break;
		}
	}
}

void OpGradient::OnLButtonUp(Mouse::Message& /*msg*/)
{
	m_held = -2;
}

void OpGradient::OnMouseMove(Mouse::Message& msg)
{
	ImageView* view = (ImageView*)msg.Object;

	// Get display coordinates

	FPoint p0 = m_p0; //((double)X0 * (double)Width, (double)Y0 * (double)Height);
	FPoint p1 = m_p1; //((double)X1 * (double)Width, (double)Y1 * (double)Height);

	view->ImageToView(&p0);
	view->ImageToView(&p1);

	switch (Type.Button.SelectedIndex)
	{
	case 0: // Linear
	case 1: // Radial
		{
			if (m_held > -1)
			{
				FPoint p(msg.Position.X, msg.Position.Y);

				if (m_held == 0)
				{
					FPoint _p0 = p;
					view->ViewToImage(&_p0);

					X0 = _p0.X / view->Surface->Width;
					Y0 = _p0.Y / view->Surface->Height;
				}
				else if (m_held == m_colors.Count - 1)
				{
					FPoint _p1 = p;
					view->ViewToImage(&_p1);

					X1 = _p1.X / view->Surface->Width;
					Y1 = _p1.Y / view->Surface->Height;
				}
				else
				{
					m_positions[m_held] = Math::GetDistance(p0, p) / Math::GetDistance(p0, p1);
				}
	
				view->Update();

				MarkDirty();
				Tree->Update(this);
			}

			break;
		}
	case 2: // Angle
		{
			if (m_held > -2)
			{
				FPoint p(msg.Position.X, msg.Position.Y);

				double ra = Math::GetAngle(p0, p1);
				double r = Math::GetAngle(p0, p);

				if (m_held == -1)
				{
					FPoint _p0 = p;
					view->ViewToImage(&_p0);

					X0 = _p0.X / view->Surface->Width;
					Y0 = _p0.Y / view->Surface->Height;
				}
				else if (m_held == 0 || m_held == m_colors.Count - 1)
				{
					FPoint _p1 = p;
					view->ViewToImage(&_p1);

					X1 = _p1.X / view->Surface->Width;
					Y1 = _p1.Y / view->Surface->Height;
				}
				else
				{
					m_positions[m_held] = (r - ra) / (2 * PI);
				}
	
				view->Update();

				MarkDirty();
				Tree->Update(this);
			}

			break;
		}
	}
}