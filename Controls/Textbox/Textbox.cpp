#include "Textbox.h"

#include <Controls\Window\Window.h>

//#pragma warning(disable:4355)

DEFINE_TYPE(Textbox, TextControl);

bool   Textbox::s_showCaret = false;
int	   Textbox::s_blinkTimerId = Window::CreateTimerId();
double Textbox::s_blinkInterval = 0.5;

Textbox::Textbox()
{
	StartEditingEvent.Sender = this;
	FinishEditingEvent.Sender = this;

	AllowKeyRepeat = true;

	TextColor = Color(0.06, 0.06, 0.06);

	m_selStart	= 0;
	m_selLength	= 0;
	m_xOffset	= 0;
	m_allowEdit	= true;
	m_editing	= true;
	m_selecting	= false;
	m_insert	= false;

	PaintEvent		+= EventHandler(this, &Textbox::OnPaint);
	KeyDownEvent	+= EventHandler(this, &Textbox::OnKeyDown);
	KeyUpEvent		+= EventHandler(this, &Textbox::OnKeyUp);
	CharEvent		+= EventHandler(this, &Textbox::OnChar);
	LButtonDownEvent += EventHandler(this, &Textbox::OnLButtonDown);
	LButtonUpEvent	+= EventHandler(this, &Textbox::OnLButtonUp);
	MouseMoveEvent	+= EventHandler(this, &Textbox::OnMouseMove);
	GetFocusEvent	+= EventHandler(this, &Textbox::OnGetFocus);
	LoseFocusEvent	+= EventHandler(this, &Textbox::OnLoseFocus);

	SuspendUpdates();

	SetSize(175, 27);
	TextColor = Color(0.13, 0.13, 0.13);
	BackColor = Color(0.79, 0.79, 0.79);
	AllowFocus = true;

	ResumeUpdates();
}

Textbox::~Textbox()
{

}

Rect Textbox::GetFullPaintRect()
{
	_Rect fullPaintRect = PaintRect;

	fullPaintRect.X      -= 1;
	fullPaintRect.Y      -= 1;
	fullPaintRect.Width  += 2;
	fullPaintRect.Height += 2;

	return fullPaintRect;
}

Cursor& Textbox::GetCursor()
{
	return m_editing
		? s_curText
		: Cursor::Dot;
}

Rect Textbox::GetTextRect()
{
	_Rect textRect = ClientRect;

	textRect.X      += 3;
	textRect.Y      += 2;
	textRect.Width  -= 6;
	textRect.Height -= 4;

	return textRect;
}

bool Textbox::GetAllowEdit()
{
	return m_allowEdit;
}

void Textbox::SetAllowEdit(bool allowEdit)
{
	m_allowEdit = allowEdit;
}

int Textbox::GetCaretPos(const Point& p)
{
	_Rect textRect = GetTextRect();

	Graphics g(this);
	_Size size = g.MeasureText(m_text, m_font);
	int alignOffset = 0;

	if		(TextAlign == Align::Left)	 alignOffset = 0;
	else if (TextAlign == Align::Center) alignOffset = (textRect.Width - size.Width) / 2;
	else if (TextAlign == Align::Right)  alignOffset = textRect.Width - size.Width;

	int pos = g.MeasureTextFit(
		m_text, 
		m_font, 
		p.X - textRect.X - alignOffset - m_xOffset);

	return pos;
}

int Textbox::GetCaretPos()
{
	return m_selStart;
}

void Textbox::SetCaretPos(int pos)
{
	Select(pos);
}

void Textbox::Select(int selStart, int selLength)
{
	m_selStart = min(max(0, selStart), m_text.Length);
	
	if (selLength > 0)
		m_selLength = min(selLength, m_text.Length - m_selStart);
	else if (selLength < 0)
		m_selLength = max(selLength, -m_selStart);
	else
		m_selLength = selLength;

	UpdateOffset();
	Update();
}

int Textbox::Insert(const String& _string, int pos)
{
	String& string = (String&)_string;

	if (string.Length < 1)
		return 0;

	if (m_selLength != 0)
	{
		if (pos > -1)
			m_selLength = 0;
		else
			DeleteSelection();
	}

	int _pos = pos > -1 
		? pos 
		: m_selStart;

	String first = m_text.SubString(0, _pos);
	String second = m_text.SubString(_pos, m_text.Length - _pos);

	m_text = first + string + second;

	if (pos < 0)
		CaretPos = _pos + string.Length;

	Control::Message msg(this);
	TextChangeEvent.Send(msg);

	return string.Length; // Return how many characters were inserted
}

void Textbox::DeleteSelection()
{
	if (m_selLength == 0)
		return;

	int selStart = min(m_selStart, m_selStart + m_selLength);
	int selEnd = max(m_selStart, m_selStart + m_selLength);
	
	String first = m_text.SubString(0, selStart);
	String second = m_text.SubString(selEnd, m_text.Length - selEnd);

	m_text = first + second;

	CaretPos = selStart;

	Control::Message msg(this);
	TextChangeEvent.Send(msg);
}

void Textbox::StartEditing()
{
	m_editing = true;
	Control::s_keyboardOwner = this;

	if (!HasFocus)
		SetFocus();

	m_oldText = m_text;

	Message msg(false);
	StartEditingEvent.Send(msg);
}

void Textbox::FinishEditing(bool success)
{
	m_editing = false;
	Control::s_keyboardOwner = null;

	if (HasFocus)
		LoseFocus();

	// Clean up after editing

	if (!success)
		m_text = m_oldText;

	m_oldText = String::Empty;

	m_selStart = m_text.Length;
	m_selLength = 0;

	Update();

	Message msg(success);
	FinishEditingEvent.Send(msg);
}

void Textbox::SetCaretBlinkInterval(double interval)
{
	s_blinkInterval = interval;
}

void Textbox::UpdateOffset()
{
	_Rect textRect = GetTextRect();

	Graphics g(this);
	_Size size = g.MeasureText(m_text, m_font);
	int alignOffset = 0;

	if		(TextAlign == Align::Left)	  alignOffset = 0;
	else if (TextAlign == Align::Center) alignOffset = (textRect.Width - size.Width) / 2;
	else if (TextAlign == Align::Right)  alignOffset = textRect.Width - size.Width;

	_Size szCaret = g.MeasureText(
		m_text.SubString(0, m_selStart + m_selLength),
		m_font);

	if (size.Width <= textRect.Width)
		m_xOffset = 0;
	else if (szCaret.Width > textRect.Width - alignOffset - m_xOffset)
		m_xOffset = textRect.Width - szCaret.Width - alignOffset;
	else if (szCaret.Width < ClientRect.X - alignOffset - m_xOffset)
		m_xOffset = -szCaret.Width - alignOffset;

	/*	BUG:
	
		If the edge of the text is inside the textRect
		but the size of the text is greater than textRect,
		adjust the offset. (This is an issue when deleting
		text.) */

	//int textStart = -alignOffset - m_xOffset;
	//int textEnd = size.Width - alignOffset - m_xOffset;
}