#ifndef COLOR_BUTTON_H
#define COLOR_BUTTON_H

#include <Controls\ListButton\ListButton.h>
#include <Types/Graphics\GdiImage8.h>

class GdiImage8;

class ColorButton
	: public ListButton
{
	DECLARE_TYPE;

	typedef Color _Color;

public:
	ColorButton();
	~ColorButton();

	__declspec(property(get = GetColor))
	_Color	Color;
	_Color	GetColor();
	void	SetColor(const _Color& color, bool updatePrevColor = true);

	__declspec(property(get = GetPreviousColor))
	_Color	PreviousColor;
	_Color	GetPreviousColor();
	
	Event<::Message&> ColorChangeEvent;

private:
	_Color	m_color;
	_Color	m_prevColor;

	GdiImage8 m_imgColor;
	void UpdateColor();

	static Skin s_skinButton;
	static Skin s_skinButtonPressed;
	static void skinButton_Init(Skin* skin);

	static Skin s_skinHint;
	static void skinHint_Init(Skin* skin);

	static Skin s_skinStripNeck;
	static void skinStripNeck_Init(Skin* skin);

	void	OnPaint(Graphics::Message& msg);
};

#endif