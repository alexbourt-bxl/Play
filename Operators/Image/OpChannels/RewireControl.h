#ifndef REWIRE_CONTROL_H
#define REWIRE_CONTROL_H

#include <Controls\Control.h>
#include <Types/Color\Color.h>
#include <Types/Geometric\FSize.h>

class RewireControl
	: public Control
{
	DECLARE_TYPE;

	friend class Channel;

public:
	class Channel;

public:
	RewireControl();

	__declspec(property(get = GetChannels))
	Array<Channel>& Channels;
	Array<Channel>& GetChannels();

	Event<Control::Message&> ChangeEvent;

private:
	Array<Channel> m_channels;

	void	channels_OnAdd(Container::Message& msg);

	int		m_chanSize;
	int		m_chanGap;
	int		m_chasmSize;
	FSize	m_arrowSize;

	FPoint	GetChannelPoint(int channel, bool right);
	int		HitTestChannel(const Point& p);

	// current connection
	int		m_dragStart;
	int		m_dragEnd;

	//

	void	OnPaint(Graphics::Message& msg);
	void	DrawConnection(Graphics& g, FPoint& pIn, FPoint& pOut, const Color& color);

	void	OnLButtonDown(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);
};

//

class RewireControl::Channel
{
	friend class RewireControl;

	typedef Color _Color;

public:
	Channel();
	Channel(const Channel& channel);
	Channel(int channel, const String& name, const _Color color);

	operator int();
	Channel& operator = (int channel);

	__declspec(property(get = GetName))
	String& Name;
	String& GetName();

	__declspec(property(get = GetColor, put = SetColor))
	_Color Color;
	_Color GetColor();
	void   SetColor(const _Color& color);

private:
	int    m_channel;
	String m_name;
	_Color m_color;

	RewireControl* m_control;
};

#endif