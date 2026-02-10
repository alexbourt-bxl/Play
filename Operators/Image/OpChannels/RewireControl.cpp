#include "RewireControl.h"

DEFINE_TYPE(RewireControl, Control);

RewireControl::RewireControl()
{
	ChangeEvent.Sender = this;

	m_chanSize  = 12;
	m_chanGap   = 9;
	m_chasmSize = 65;
	m_arrowSize = FSize(10, 10);

	*m_channels.AddEvent += EventHandler(this, &RewireControl::channels_OnAdd);
	
	PaintEvent     += EventHandler(this, &RewireControl::OnPaint);
	LButtonDownEvent += EventHandler(this, &RewireControl::OnLButtonDown);
	LButtonUpEvent += EventHandler(this, &RewireControl::OnLButtonUp);
	MouseMoveEvent += EventHandler(this, &RewireControl::OnMouseMove);

	m_dragStart = -1;
	m_dragEnd   = -1;
	
	Size = _Size(150, 90);
}

Array<RewireControl::Channel>& RewireControl::GetChannels()
{
	return m_channels;
}

FPoint RewireControl::GetChannelPoint(int channel, bool right)
{
	_Rect paintRect = PaintRect;

	int width  = m_chanSize * 2 + m_chasmSize;
	int height = m_channels.Count * m_chanSize + (m_channels.Count - 1) * m_chanGap;

	int yStart = (paintRect.Height - height) / 2;

	double x =
		right
		? (double)(paintRect.Width + width) / 2
		: (double)(paintRect.Width - width) / 2;

	double y = yStart + channel * m_chanSize + channel * m_chanGap + (double)m_chanSize / 2;

	return FPoint(x, y);
}

int RewireControl::HitTestChannel(const Point& _p)
{
	FPoint hp = _p;
	FPoint p;

	for (int i = 0; i < m_channels.Count; i++)
	{
		p = GetChannelPoint(i, false);

		if (hp.X >= p.X - (double)m_chanSize / 2 && hp.X < p.X + (double)m_chanSize / 2
			&& hp.Y >= p.Y - (double)m_chanSize / 2 && hp.Y < p.Y + (double)m_chanSize / 2)
		{
			return 0x00000000 | (i + 1);
		}

		p = GetChannelPoint(i, true);

		if (hp.X >= p.X - (double)m_chanSize / 2 && hp.X < p.X + (double)m_chanSize / 2
			&& hp.Y >= p.Y - (double)m_chanSize / 2 && hp.Y < p.Y + (double)m_chanSize / 2)
		{
			return 0x00010000 | (i + 1);
		}
	}

	return 0;
}

//

RewireControl::Channel::Channel()
{
	m_channel = 0;
	m_color   = Color::White;

	m_control = null;
}

RewireControl::Channel::Channel(const Channel& channel)
{
	m_channel = channel.m_channel;
	m_name    = channel.m_name;
	m_color   = channel.m_color;

	m_control = null;
}

RewireControl::Channel::Channel(int channel, const String& name, const _Color color)
{
	m_channel = channel;
	m_name    = name;
	m_color   = color;

	m_control = null;
}

RewireControl::Channel::operator int()
{
	return m_channel;
}

RewireControl::Channel& RewireControl::Channel::operator = (int channel)
{
	m_channel = channel;

	assert(m_control);

	if (m_control)
	{
		Control::Message msg;
		msg.Control = m_control;

		m_control->ChangeEvent.Send(msg);
	}

	return *this;
}

String& RewireControl::Channel::GetName()
{
	return m_name;
}

Color RewireControl::Channel::GetColor()
{
	return m_color;
}

void RewireControl::Channel::SetColor(const _Color& color)
{
	m_color = color;

	if (m_control)
		m_control->Update();
}