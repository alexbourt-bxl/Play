#include "KeyCombo.h"

#include <windows.h>

KeyCombo* KeyCombo::s_pressed = null;

KeyCombo::KeyCombo()
{
	PressEvent.Sender = this;
	ReleaseEvent.Sender = this;
	
	m_allowRepeat = false;
}

KeyCombo::~KeyCombo()
{

}

bool KeyCombo::GetAllowRepeat()
{
	return m_allowRepeat;
}

void KeyCombo::SetAllowRepeat(bool allowRepeat)
{
	m_allowRepeat = allowRepeat;
}

bool KeyCombo::GetAllModifiers()
{
	for (int i = 0; i < Count; i++)
	{
		if (!GetItem(i).IsModifier)
			return false;
	}

	return true;
}

bool KeyCombo::GetIsPressed()
{
	for (int i = 0; i < Count; i++)
	{
		if (!GetItem(i).IsDown)
			return false;
	}

	return true;
}