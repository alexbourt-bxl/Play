#ifndef KEY_COMBO_H
#define KEY_COMBO_H

#include <Devices\Keyboard\Key.h>
#include <Events/Event.h>
#include <Containers/Array.h>

class KeyCombo 
	: public Array<Key>
{
	friend class Control;
	friend class Window;

public:
	KeyCombo();
	~KeyCombo();

	__declspec(property(get = GetAllowRepeat, put = SetAllowRepeat))
	bool AllowRepeat;
	bool GetAllowRepeat();
	void SetAllowRepeat(bool allowRepeat);

	__declspec(property(get = GetAllModifiers))
	bool AllModifiers;
	bool GetAllModifiers();
	
	__declspec(property(get = GetIsPressed))
	bool IsPressed;
	bool GetIsPressed();

	Event<::Message&> PressEvent;
	Event<::Message&> ReleaseEvent;

private:
	bool m_allowRepeat;

	static KeyCombo* s_pressed;
};

#endif