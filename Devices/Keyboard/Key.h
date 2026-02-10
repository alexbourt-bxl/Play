#ifndef KEY_H
#define KEY_H

#include <Types/BaseTypes.h>
#include <Events/Message.h>

class Key
{
public:
	class Message;

public:
	uint Code;

public:
	Key();
	Key(const Key& key);
	Key(int code);

	static Key FromCode(int code);

public:
	__declspec(property(get = GetIsModifier))
	bool IsModifier;
	bool GetIsModifier();

	__declspec(property(get = GetIsDown))
	bool IsDown;
	bool GetIsDown();

	__declspec(property(get = GetIsToggled))
	bool IsToggled;
	bool GetIsToggled();

	bool operator == (const Key& key);
	bool operator != (const Key& key);

public:

	// Keys

	static Key	Unknown;

	static Key	LMB;
	static Key	RMB;
	static Key	Cancel;
	static Key	MMB;
	static Key	ListButton1;
	static Key	ListButton2;
	static Key	Backspace;
	static Key	Tab;
	static Key	Clear;
	static Key	Enter;
	static Key	Shift;
	static Key	Control;
	static Key	Alt;
	static Key	Pause;
	static Key	CapsLock;
	static Key	Kana;
	static Key	Hangul;
	static Key	Junja;
	static Key	Final;
	static Key	Hanja;
	static Key	Kanji;
	static Key	Escape;
	static Key	Convert;
	static Key	NonConvert;
	static Key	Accept;
	static Key	ModeChange;
	static Key	Space;
	static Key	PageUp;
	static Key	PageDown;
	static Key	End;
	static Key	Home;
	static Key	Left;
	static Key	Up;
	static Key	Right;
	static Key	Down;
	static Key	Select;
	static Key	Print;
	static Key	Execute;
	static Key	PrintScreen;
	static Key	Insert;
	static Key	Delete;
	static Key	Help;

	static Key	Key0;
	static Key	Key1;
	static Key	Key2;
	static Key	Key3;
	static Key	Key4;
	static Key	Key5;
	static Key	Key6;
	static Key	Key7;
	static Key	Key8;
	static Key	Key9;

	static Key	A;
	static Key	B;
	static Key	C;
	static Key  D;
	static Key  E;
	static Key	F;
	static Key	G;
	static Key	H;
	static Key	I;
	static Key	J;
	static Key	K;
	static Key	L;
	static Key	M;
	static Key	N;
	static Key	O;
	static Key	P;
	static Key	Q;
	static Key	R;
	static Key	S;
	static Key	T;
	static Key	U;
	static Key	V;
	static Key	W;
	static Key	X;
	static Key	Y;
	static Key	Z;

	static Key	LeftWindows;
	static Key	RightWindows;
	static Key	Applications;
	static Key	Sleep;

	static Key	Num0;
	static Key	Num1;
	static Key	Num2;
	static Key	Num3;
	static Key	Num4;
	static Key 	Num5;
	static Key 	Num6;
	static Key 	Num7;
	static Key 	Num8;
	static Key 	Num9;

	static Key 	Multiply;
	static Key 	Add;
	static Key 	Separator;
	static Key 	Subtract;
	static Key 	Decimal;
	static Key 	Divide;

	static Key 	F1;
	static Key 	F2;
	static Key 	F3;
	static Key 	F4;
	static Key 	F5;
	static Key 	F6;
	static Key 	F7;
	static Key 	F8;
	static Key 	F9;
	static Key 	F10;
	static Key 	F11;
	static Key 	F12;
	static Key 	F13;
	static Key 	F14;
	static Key 	F15;
	static Key 	F16;
	static Key 	F17;
	static Key 	F18;
	static Key 	F19;
	static Key 	F20;
	static Key 	F21;
	static Key 	F22;
	static Key 	F23;
	static Key 	F24;

	static Key 	NumLock;
	static Key 	ScrollLock;

	static Key 	KeyOem1;
	static Key 	KeyOem2;
	static Key 	KeyOem3;
	static Key 	KeyOem4;
	static Key 	KeyOem5;

	static Key 	LeftShift;
	static Key 	RightShift;
	static Key 	LeftControl;
	static Key 	RightControl;
	static Key 	LeftAlt;
	static Key 	RightAlt;

	static Key 	BrowserBack;
	static Key 	BrowserForward;
	static Key 	BrowserRefresh;
	static Key 	BrowserStop;
	static Key 	BrowserSearch;
	static Key 	BrowserFavorites;
	static Key 	BrowserHome;

	static Key 	VolumeMute;
	static Key 	VolumeDown;
	static Key 	VolumeUp;

	static Key 	MediaNextTrack;
	static Key 	MediaPreviousTrack;
	static Key 	MediaStop;
	static Key 	MediaPlayPause;

	static Key 	StartMail;
	static Key 	SelectMedia;
	static Key 	StartApplication1;
	static Key 	StartApplication2;

	static Key 	Plus;
	static Key 	Comma;
	static Key 	Minus;
	static Key 	Period;

	static Key 	Oem1;
	static Key 	Oem2;
	static Key 	Oem3;
	static Key 	Oem4;
	static Key 	Oem5;
	static Key 	Oem6;
	static Key 	Oem7;
	static Key 	Oem8;

	static Key 	OemE6;

	static Key 	Packet;

	static Key 	OemE9;
	static Key 	OemEA;
	static Key 	OemEB;
	static Key 	OemEC;
	static Key 	OemED;
	static Key 	OemEE;
	static Key 	OemEF;
	static Key 	OemF0;
	static Key 	OemF1;
	static Key 	OemF2;
	static Key 	OemF3;
	static Key 	OemF4;
	static Key 	OemF5;

	static Key 	Attn;
	static Key 	CrSel;
	static Key 	ExSel;
	static Key 	EraseEOF;
	static Key 	Play;
	static Key 	Zoom;
	static Key 	PA1;
	static Key 	OemClear;
};

class Key::Message
	: public ::Message
{
	typedef Key _Key;

public:
	_Key    Key;
	wchar_t Char;
	bool    SendChar;

	Message() 
		: Char(null), SendChar(false) {}

	Message(_Key key, wchar_t wchar, bool sendChar)
		: Key(key), Char(wchar), SendChar(sendChar) {}
};

#endif