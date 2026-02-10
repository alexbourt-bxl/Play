#ifndef PANEL_BUTTON_H
#define PANEL_BUTTON_H

#include <Controls\Button\Button.h>

class HoverButton 
	: public Button
{
	DECLARE_TYPE;

public:
	HoverButton();
	
	_Rect GetFullPaintRect();

	void SetHighlight(bool highlight);

private:
	static Skin	s_skinButtonHover;
	static Skin	s_skinButtonHighlight;
	static void	skinButtonHover_Init(Skin* skin);

	static Skin	s_skinButtonPressed;
	static void	skinButtonPressed_Init(Skin* skin);

	static Skin	s_skinButtonBack;
	static void	skinButtonBack_Init(Skin* skin);

	/*	These private properties are
		not exposed to the outside. */
	FloatProperty m_hoverOpacity;
	FloatProperty m_highlight;

	//

	void OnPaint(Graphics::Message& msg);
	void OnMouseEnter(Mouse::Message& msg);
	void OnMouseLeave(Mouse::Message& msg);
};

#endif