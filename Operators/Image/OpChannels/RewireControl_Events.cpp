#include "RewireControl.h"

#include <Types/Graphics\Font.h>
#include <Types/Geometric\Poly.h>

void RewireControl::channels_OnAdd(Container::Message& msg)
{
	Channel* channel = (Channel*)msg.Item;
	channel->m_control = this;
}

void RewireControl::OnPaint(Graphics::Message& msg)
{
	_Rect paintRect = PaintRect;
	FPoint paintRectPos(paintRect.Position);

	msg.Graphics.Antialias = Graphics::High;
	
	/*	Draw the channels. */

	Font font(Font::Default.Name, 21);
	Color shadowColor(Color::Black, 0.35);

	for (int i = 0; i < m_channels.Count; i++)
	{
		FPoint pIn  = GetChannelPoint(i, false);
		FPoint pOut = GetChannelPoint(i, true);

		// In connector

		msg.Graphics.FillEllipse(
			paintRectPos + pIn + FPoint(0, 0.5),
			(double)m_chanSize / 2 + 0.5,
			(double)m_chanSize / 2 + 0.5,
			0,
			shadowColor);

		msg.Graphics.FillEllipse(
			paintRectPos + pIn,
			(double)m_chanSize / 2,
			(double)m_chanSize / 2,
			0,
			m_channels[i].Color);

		// Out connector

	
		if (m_channels[i] > 0 
			&& !(i == m_dragEnd && m_channels[m_dragEnd] == m_dragStart + 1))
		{
			msg.Graphics.FillEllipse(
				paintRectPos + pOut + FPoint(0, 0.5),
				(double)m_chanSize / 2 + 0.5,
				(double)m_chanSize / 2 + 0.5,
				0,
				shadowColor);

			msg.Graphics.FillEllipse(
				paintRectPos + pOut,
				(double)m_chanSize / 2,
				(double)m_chanSize / 2,
				0,
				m_channels[i].Color);

			msg.Graphics.FillEllipse(
				paintRectPos + pOut,
				(double)m_chanSize / 2 - 2,
				(double)m_chanSize / 2 - 2,
				0,
				m_channels[m_channels[i] - 1].Color);
		}
		else
		{
			msg.Graphics.DrawEllipse(
				paintRectPos + pOut + FPoint(0, 0.5),
				(double)m_chanSize / 2 - 1,
				(double)m_chanSize / 2 - 1,
				0,
				shadowColor,
				3);

			msg.Graphics.DrawEllipse(
				paintRectPos + pOut,
				(double)m_chanSize / 2 - 1,
				(double)m_chanSize / 2 - 1,
				0,
				m_channels[i].Color,
				2);
		}

		// Draw labels

		msg.Graphics.DrawText(
			m_channels[i].Name, 
			font, 
			paintRect.X + (int)(pIn.X - (double)m_chanSize / 2 - 6), 
			paintRect.Y + (int)(pIn.Y - 1), 
			m_channels[i].Color, 
			null,
			Align::Right | Align::Middle);

		msg.Graphics.DrawText(
			m_channels[i].Name, 
			font, 
			paintRect.X + (int)(pOut.X + (double)m_chanSize / 2 + 6), 
			paintRect.Y + (int)(pOut.Y - 1), 
			m_channels[i].Color, 
			null, 
			Align::Left | Align::Middle);
	}

	/*	Draw the connections. */

	for (int i = 0; i < m_channels.Count; i++)
	{
		if (m_channels[i] > 0 
			&& !(i == m_dragEnd && m_channels[m_dragEnd] == m_dragStart + 1))
		{
			int c = m_channels[i] - 1;

			FPoint pIn = paintRectPos + GetChannelPoint(c, false);
			FPoint pOut   = paintRectPos + GetChannelPoint(i, true);

			DrawConnection(msg.Graphics, pIn, pOut, m_channels[c].Color);
		}
	}

	/*	Draw the loose connection. */

	if (m_dragStart > -1)
	{
		Point p = Mouse::GetPosition();
		PointScreenToClient(&p);

		FPoint pIn = paintRectPos + GetChannelPoint(m_dragStart, false);
		FPoint pOut  = paintRectPos + p + FPoint((double)m_chanSize / 2, 0);

		DrawConnection(msg.Graphics, pIn, pOut, m_channels[m_dragStart].Color);
	}
}

void RewireControl::DrawConnection(Graphics& g, FPoint& pIn, FPoint& pOut, const Color& color)
{
	FPoint p0 = pIn + FPoint((double)m_chanSize / 2, 0);
	FPoint p3 = pOut   + FPoint(-(double)m_chanSize / 2 - m_arrowSize.Width + 1, 0);

	double opacity = 1;
	
	double dx = fabs(p3.X + (double)m_chanSize - p0.X);
	double tolerance = m_arrowSize.Width * 1.5;

	if (dx < tolerance)
		opacity = dx / tolerance;

	FPoint p1(p0.X + (p3.X - p0.X) / 3, p0.Y);
	FPoint p2(p3.X - (p3.X - p0.X) / 3, p3.Y);

	FPoint so(0, 1); // shadow offset
	Color shadowColor(Color::Black, 0.35 * opacity);

	g.DrawBezier(p0 + so, p1 + so, p2 + so, p3 + so, shadowColor);
	g.DrawBezier(p0, p1, p2, p3, Color(color, opacity));

	Poly arrow;
	arrow.Points.Add(p3 + FPoint(0, -m_arrowSize.Height / 2));
	arrow.Points.Add(p3 + FPoint(0,  m_arrowSize.Height / 2));
	arrow.Points.Add(p3 + FPoint(m_arrowSize.Width, 0));

	Poly arrowShadow = arrow;
	arrowShadow.Offset(so);

	g.FillPoly(&arrowShadow, 1, shadowColor);
	g.FillPoly(&arrow, 1, Color(color, opacity));
}

void RewireControl::OnLButtonDown(Mouse::Message& msg)
{
	int channel = HitTestChannel(msg.Position);

	if (channel == 0)
		return;

	if ((channel & 0xFFFF0000) > 0)
	{
		m_dragEnd   = (channel & 0x0000FFFF) - 1;
		m_dragStart = m_channels[m_dragEnd] - 1;
	}
	else
	{
		m_dragStart = (channel & 0x0000FFFF) - 1;
		m_dragEnd   = -1;
	}

	Update();
}

void RewireControl::OnLButtonUp(Mouse::Message& msg)
{
	for (int i = 0; i < m_channels.Count; i++)
	{
		if (i == m_dragEnd && m_channels[m_dragEnd] == m_dragStart + 1)
			m_channels[m_dragEnd] = 0;
	}

	int channel = HitTestChannel(msg.Position);

	if ((channel & 0xFFFF0000) > 0)
	{
		m_channels[(channel & 0x0000FFFF) - 1] = m_dragStart + 1;
	}

	m_dragStart = -1;
	m_dragEnd   = -1;

	Update();
}

void RewireControl::OnMouseMove(Mouse::Message& msg)
{
	if (m_dragStart > -1)
		Update();
}