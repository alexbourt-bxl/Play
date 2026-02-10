#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <Controls\TextControl\TextControl.h>
#include <Types/Basic\String.h>
#include <System\System.h>
#include <Events/Event.h>
#include <Types/Graphics\Skin.h>

class Textbox 
	: public TextControl
{
	DECLARE_TYPE;

public:
	class Message;

public:
	Textbox();
	~Textbox();

	_Rect	GetFullPaintRect();
	::Cursor& GetCursor();

	_Rect	GetTextRect(); // no property for this

	__declspec(property(get = GetAllowEdit, put = SetAllowEdit))
	bool	AllowEdit;
	bool	GetAllowEdit();
	void	SetAllowEdit(bool allowEdit);

	__declspec(property(get = GetCaretPos, put = SetCaretPos))
	int		CaretPos;
	int		GetCaretPos();
	void	SetCaretPos(int pos);

	int		GetCaretPos(const Point& p);

	void	Select(int selStart, int selLength = 0);

	int		Insert(const String& s, int pos = -1);
	void	DeleteSelection();

	void	CutSelection();
	bool	CopySelection();
	void	PasteSelection();

	void	StartEditing();
	void	FinishEditing(bool success);

	static void SetCaretBlinkInterval(double interval);

	void	OnPaint(Graphics::Message& msg);

	void	OnKeyDown(Key::Message& msg);
	void	OnKeyUp(Key::Message& msg);
	void	OnChar(Key::Message& msg);

	/************************************************/
	
	bool	Key_Enter();
	bool	Key_Escape();
	bool	Key_Insert();
	bool	Key_Left();
	bool	Key_Right();
	bool	Key_End();
	bool	Key_Home();
	bool	Key_Backspace();
	bool	Key_Delete();
	bool	Key_Tab();
	bool	Key_A();
	bool	Key_D();
	bool	Key_X();
	bool	Key_C();
	bool	Key_V();

	/************************************************/

	void	OnLButtonDown(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);

	void	OnGetFocus(Control::Message& msg);
	void	OnLoseFocus(Control::Message& msg);

	Event<Message&> StartEditingEvent;
	Event<Message&> FinishEditingEvent; // msg.result > 0 indicates successful (not cancelled) editing.

private:
	String	m_oldText;

	/*	The selection length can be negative. 
		This is important for text cursor placement. */
	int		m_selStart;
	int		m_selLength;

	int		m_xOffset;

	bool	m_allowEdit;

	bool	m_editing;
	bool	m_selecting;
	bool	m_insert;

	void	UpdateOffset();

	static bool	 s_showCaret;
	static int	 s_blinkTimerId;
	static double s_blinkInterval;
	static void	 CaretBlink(Control* control);

	static Skin	 s_skinBorder;
	static void	 skinBorder_Init(Skin* skin);

	static Skin	 s_skinSelShadow;
	static void	 skinSelShadow_Init(Skin* skin);

public:
	static ::Cursor s_curText;
};

//

class Textbox::Message
	: public ::Message
{
public:
	bool Success;

	Message(bool success)
		: Success(success) {}
};

#endif