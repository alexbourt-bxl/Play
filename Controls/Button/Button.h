#ifndef BUTTON_H
#define BUTTON_H

#include <Controls\TextControl\TextControl.h>
#include <Graphics\Graphics.h>
#include <Containers/Array.h>
#include <Types/Graphics\Skin.h>

class ImageMessage;

class Button 
	: public TextControl
{
	DECLARE_TYPE;

	friend class ListButton;

	typedef Skin _Skin;

public:
	Button();
	~Button();
	
	_Rect	GetFullPaintRect();

	__declspec(property(get = GetSelected, put = SetSelected))
	bool	Selected;
	bool	GetSelected();
	virtual void SetSelected(bool selected);

	__declspec(property(get = GetHighlight, put = SetHighlight))
	bool	Highlight;
	bool	GetHighlight();
	virtual void SetHighlight(bool highlight);

	__declspec(property(get = GetIcon, put = SetIcon))
	GdiImage* Icon;
	GdiImage* GetIcon() const;
	void	SetIcon(GdiImage* icon);

	__declspec(property(get = GetIconOffset, put = SetIconOffset))
	Point&	IconOffset;
	Point&	GetIconOffset();
	void	SetIconOffset(const Point& offset);

	void 	OnPaint(Graphics::Message& msg);
	void 	OnMouseMove(Mouse::Message& msg);
	void 	OnMouseEnter(Mouse::Message& msg);
	void 	OnMouseLeave(Mouse::Message& msg);
	void 	OnLButtonDown(Mouse::Message& msg);
	void 	OnLButtonUp(Mouse::Message& msg);
	void	OnLClick(Mouse::Message& msg);

	Event<Control::Message&> PressEvent;
	Event<ImageMessage&> PreChangeIconEvent;
	Event<ImageMessage&> ChangeIconEvent;

protected:
	bool	m_over;
	bool	m_pressed;

	bool	m_selected;
	bool	m_highlight;

	Point	m_iconOffset;

private:
	GdiImage* m_icon;

	static Skin s_skinButton;
	static Skin s_skinButtonPressed;
	static Skin s_skinButtonHighlight;
	static void skinButton_Init(Skin* skin);

	static Skin s_skinSelected;
	static void skinSelected_Init(Skin* skin);
};

//

class ImageMessage
	: public ::Message
{
	typedef Image _Image;

public:
	_Image* Image;

	ImageMessage(_Image* image)
		: Image(image) {}
};

#endif