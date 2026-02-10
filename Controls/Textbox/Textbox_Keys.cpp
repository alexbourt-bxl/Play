#include "Textbox.h"

bool Textbox::Key_Enter()
{
	FinishEditing(true);
	return true;
}

bool Textbox::Key_Escape()
{
	FinishEditing(false);
	return true;
}

bool Textbox::Key_Insert()
{
	m_insert = !m_insert;

	s_showCaret = true;
	Update();

	return true;
}

bool Textbox::Key_Left()
{
	Textbox::s_showCaret = true;

	if (Key::Shift.IsDown)
		Select(m_selStart, max(-m_selStart, m_selLength - 1));
	else if (m_selLength != 0)
		Select(min(m_selStart, m_selStart + m_selLength), 0);
	else
		Select(max(0, m_selStart - 1), 0);

	return true;
}

bool Textbox::Key_Right()
{
	Textbox::s_showCaret = true;

	if (Key::Shift.IsDown)
		Select(m_selStart, min(m_selLength + 1, m_text.Length - m_selStart));
	else if (m_selLength != 0)
		Select(max(m_selStart, m_selStart + m_selLength), 0);
	else
		Select(min(m_selStart + 1, m_text.Length), 0);

	return true;
}

bool Textbox::Key_End()
{
	Textbox::s_showCaret = true;

	if (Key::Shift.IsDown)
		Select(m_selStart, m_text.Length - m_selStart);
	else
		Select(m_text.Length, 0);

	return true;
}

bool Textbox::Key_Home()
{
	Textbox::s_showCaret = true;

	if (Key::Shift.IsDown)
		Select(m_selStart, -m_selStart);
	else
		Select(0, 0);

	return true;
}

bool Textbox::Key_Backspace()
{
	Textbox::s_showCaret = true;

	if (m_selLength != 0)
	{
		DeleteSelection();
	}
	else if (m_selStart > 0)
	{
		m_selStart--;
		m_selLength = 1;
		DeleteSelection();
	}

	return true;
}

bool Textbox::Key_Delete()
{
	Textbox::s_showCaret = true;

	if (m_selLength != 0)
	{
		DeleteSelection();
	}
	else if (m_selStart < m_text.Length)
	{
		m_selLength = 1;
		DeleteSelection();
	}

	return true;
}

bool Textbox::Key_Tab()
{
	return false;
}

bool Textbox::Key_A()
{
	if (Key::Control.IsDown)
	{
		Textbox::s_showCaret = true;
		Select(0, m_text.Length);
		return true;
	}

	return false;
}

bool Textbox::Key_D()
{
	if (Key::Control.IsDown)
	{
		Textbox::s_showCaret = true;
		m_selLength = 0;
		Update();
		return true;
	}

	return false;
}

bool Textbox::Key_X()
{
	if (Key::Control.IsDown)
	{
		Textbox::s_showCaret = true;
		CutSelection();
		return true;
	}

	return false;
}

bool Textbox::Key_C()
{
	if (Key::Control.IsDown)
	{
		CopySelection();
		return true;
	}

	return false;
}

bool Textbox::Key_V()
{
	if (Key::Control.IsDown)
	{
		Textbox::s_showCaret = true;
		PasteSelection();
		return true;
	}

	return false;
}