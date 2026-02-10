#ifndef LIST_BUTTON_H
#define LIST_BUTTON_H

#include <Controls\Button\Button.h>
#include <Controls\ListButton\Strip\Strip.h>
#include <Controls\Layout\Spacer\Spacer.h>
#include <Graphics\Graphics.h>
#include <Types/Graphics\Skin.h>

class ListButton
	: public Button
{
	DECLARE_TYPE;

	typedef Strip _Strip;

public:
	ListButton();
	~ListButton();

	_Rect	GetFullPaintRect();

	__declspec(property(get = GetHintHeight, put = SetHintHeight))
	int		HintHeight;
	int		GetHintHeight();
	void	SetHintHeight(int height);

	__declspec(property(get = GetDefaultToLast, put = SetDefaultToLast))
	bool	DefaultToLast;
	bool	GetDefaultToLast();
	void	SetDefaultToLast(bool defaultToLast);

	__declspec(property(get = GetSelectedIndex, put = SetSelectedIndex))
	int		SelectedIndex;
	int		GetSelectedIndex();
	void	SetSelectedIndex(int index);

	void	AddItem(const String& item, GdiImage* icon = null);

	void	RemoveItem(int index);
	void	RemoveItem(const String& item);

	Event<::Message&> SelectItemEvent;

	__declspec(property(get = GetStrip))
	_Strip&	Strip;
	_Strip&	GetStrip();

private:
	int		m_selectedIndex;

	static Skin s_skinHint;
	static void skinHint_Init(Skin* skin);
	
	static Skin s_skinStripNeck;
	static void skinStripNeck_Init(Skin* skin);

	int		m_hintHeight;

	_Strip	m_strip;

	bool	m_defaultToLast; // initially defaults to first added
	Button*	m_btnLast;

	Spacer	m_spacer; // a buffer to prop up the strip buttons

	int		m_stripTimerId;
	static void strip_OnTimerTick(Control* control);

	void	ShowStrip();
	void	HideStrip();

	void	OnPaint(Graphics::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);
	void	OnLButtonDown(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	OnPress(Control::Message& msg);
	void	OnSize(Control::Message& msg);

	//void	OnChangeIcon(ImageMessage& msg);

	void	strip_OnAddControl(Container::Message& msg);
	void	strip_OnRemoveControl(Container::Message& msg);
	void	strip_OnPaint(Graphics::Message& msg);
	void	strip_OnMouseMove(Mouse::Message& msg);
	void	strip_OnLButtonUp(Mouse::Message& msg);

	void	button_OnPress(Control::Message& msg);
};

#endif