#include "Key.h"

#include <windows.h>

Key::Key()
{
	Code = null;
}

Key::Key(const Key& key)
{
	Code = key.Code;
}

Key::Key(int code)
{
	Code = code;
}

Key Key::FromCode(int code)
{
	switch (code)
	{
		case VK_LBUTTON:				return Key::LMB;
		case VK_RBUTTON:				return Key::RMB;
		case VK_CANCEL:					return Key::Cancel;
		case VK_MBUTTON:				return Key::MMB;
		case VK_XBUTTON1:				return Key::ListButton1;
		case VK_XBUTTON2:				return Key::ListButton2;
		case VK_BACK:					return Key::Backspace;
		case VK_TAB:					return Key::Tab;
		case VK_CLEAR:					return Key::Clear;
		case VK_RETURN:					return Key::Enter;
		case VK_SHIFT:					return Key::Shift;
		case VK_CONTROL:				return Key::Control;
		case VK_MENU:					return Key::Alt;
		case VK_PAUSE:					return Key::Pause;
		case VK_CAPITAL:				return Key::CapsLock;
		case VK_KANA:					return Key::Kana;
	//	case VK_HANGUL:					return Key::Hangul;
		case VK_JUNJA:					return Key::Junja;
		case VK_FINAL:					return Key::Final;
	//	case VK_HANJA:					return Key::Hanja;
		case VK_KANJI:					return Key::Kanji;
		case VK_ESCAPE:					return Key::Escape;
		case VK_CONVERT:				return Key::Convert;
		case VK_NONCONVERT:				return Key::NonConvert;
		case VK_ACCEPT:					return Key::Accept;
		case VK_MODECHANGE:				return Key::ModeChange;
		case VK_SPACE:					return Key::Space;
		case VK_PRIOR:					return Key::PageUp;
		case VK_NEXT:					return Key::PageDown;
		case VK_END:					return Key::End;
		case VK_HOME:					return Key::Home;
		case VK_LEFT:					return Key::Left;
		case VK_UP:						return Key::Up;
		case VK_RIGHT:					return Key::Right;
		case VK_DOWN:					return Key::Down;
		case VK_SELECT:					return Key::Select;
		case VK_PRINT:					return Key::Print;
		case VK_EXECUTE:				return Key::Execute;
		case VK_SNAPSHOT:				return Key::PrintScreen;
		case VK_INSERT:					return Key::Insert;
		case VK_DELETE:					return Key::Delete;
		case VK_HELP:					return Key::Help;

		case 0x30:						return Key::Key0;
		case 0x31:						return Key::Key1;
		case 0x32:						return Key::Key2; 
		case 0x33:						return Key::Key3; 
		case 0x34:						return Key::Key4; 
		case 0x35:						return Key::Key5; 
		case 0x36:						return Key::Key6; 
		case 0x37:						return Key::Key7; 
		case 0x38:						return Key::Key8; 
		case 0x39:						return Key::Key9; 
															
		case 0x41:						return Key::A; 
		case 0x42:						return Key::B; 
		case 0x43:						return Key::C; 
		case 0x44:						return Key::D; 
		case 0x45:						return Key::E; 
		case 0x46:						return Key::F; 
		case 0x47:						return Key::G; 
		case 0x48:						return Key::H; 
		case 0x49:						return Key::I; 
		case 0x4A:						return Key::J; 
		case 0x4B:						return Key::K; 
		case 0x4C:						return Key::L; 
		case 0x4D:						return Key::M; 
		case 0x4E:						return Key::N; 
		case 0x4F:						return Key::O; 
		case 0x50:						return Key::P; 
		case 0x51:						return Key::Q; 
		case 0x52:						return Key::R; 
		case 0x53:						return Key::S; 
		case 0x54:						return Key::T; 
		case 0x55:						return Key::U; 
		case 0x56:						return Key::V;
		case 0x57:						return Key::W;
		case 0x58:						return Key::X;
		case 0x59:						return Key::Y;
		case 0x5A:						return Key::Z;

		case VK_LWIN:					return Key::LeftWindows;
		case VK_RWIN:					return Key::RightWindows;
		case VK_APPS:					return Key::Applications;
		case VK_SLEEP:					return Key::Sleep;

		case VK_NUMPAD0:				return Key::Num0;
		case VK_NUMPAD1:				return Key::Num1;
		case VK_NUMPAD2:				return Key::Num2;
		case VK_NUMPAD3:				return Key::Num3;
		case VK_NUMPAD4:				return Key::Num4;
		case VK_NUMPAD5:				return Key::Num5;
		case VK_NUMPAD6:				return Key::Num6;
		case VK_NUMPAD7:				return Key::Num7;
		case VK_NUMPAD8:				return Key::Num8;
		case VK_NUMPAD9:				return Key::Num9;

		case VK_MULTIPLY:				return Key::Multiply;
		case VK_ADD:					return Key::Add;
		case VK_SEPARATOR:				return Key::Separator;
		case VK_SUBTRACT:				return Key::Subtract;
		case VK_DECIMAL:				return Key::Decimal;
		case VK_DIVIDE:					return Key::Divide;

		case VK_F1:						return Key::F1;
		case VK_F2:						return Key::F2;
		case VK_F3:						return Key::F3;
		case VK_F4:						return Key::F4;
		case VK_F5:						return Key::F5;
		case VK_F6:						return Key::F6;
		case VK_F7:						return Key::F7;
		case VK_F8:						return Key::F8;
		case VK_F9:						return Key::F9;
		case VK_F10:					return Key::F10;
		case VK_F11:					return Key::F11;
		case VK_F12:					return Key::F12;
		case VK_F13:					return Key::F13;
		case VK_F14:					return Key::F14;
		case VK_F15:					return Key::F15;
		case VK_F16:					return Key::F16;
		case VK_F17:					return Key::F17;
		case VK_F18:					return Key::F18;
		case VK_F19:					return Key::F19;
		case VK_F20:					return Key::F20;
		case VK_F21:					return Key::F21;
		case VK_F22:					return Key::F22;
		case VK_F23:					return Key::F23;
		case VK_F24:					return Key::F24;

		case VK_NUMLOCK:				return Key::NumLock;
		case VK_SCROLL:					return Key::ScrollLock;

		case 0x92: 						return Key::KeyOem1;
		case 0x93: 						return Key::KeyOem2;		
		case 0x94: 						return Key::KeyOem3;		
		case 0x95: 						return Key::KeyOem4;		
		case 0x96: 						return Key::KeyOem5;		
		 
 		case VK_LSHIFT:					return Key::LeftShift;
 		case VK_RSHIFT:					return Key::RightShift;
 		case VK_LCONTROL:				return Key::LeftControl;
 		case VK_RCONTROL:				return Key::RightControl;
 		case VK_LMENU:					return Key::LeftAlt;
 		case VK_RMENU:					return Key::RightAlt;
 
		case VK_BROWSER_BACK:			return Key::BrowserBack;
		case VK_BROWSER_FORWARD:		return Key::BrowserForward;
		case VK_BROWSER_REFRESH:		return Key::BrowserRefresh;
		case VK_BROWSER_STOP:			return Key::BrowserStop;
		case VK_BROWSER_SEARCH:			return Key::BrowserSearch;
		case VK_BROWSER_FAVORITES:		return Key::BrowserFavorites;
		case VK_BROWSER_HOME:			return Key::BrowserHome;

		case VK_VOLUME_MUTE:			return Key::VolumeMute;
		case VK_VOLUME_DOWN:			return Key::VolumeDown;
		case VK_VOLUME_UP:				return Key::VolumeUp;

		case VK_MEDIA_NEXT_TRACK:		return Key::MediaNextTrack;
		case VK_MEDIA_PREV_TRACK:		return Key::MediaPreviousTrack;
		case VK_MEDIA_STOP:				return Key::MediaStop;
		case VK_MEDIA_PLAY_PAUSE:		return Key::MediaPlayPause;

		case VK_LAUNCH_MAIL:			return Key::StartMail;
		case VK_LAUNCH_MEDIA_SELECT:	return Key::SelectMedia;
		case VK_LAUNCH_APP1:			return Key::StartApplication1;
		case VK_LAUNCH_APP2:			return Key::StartApplication2;

		case VK_OEM_PLUS:				return Key::Plus;
		case VK_OEM_COMMA:				return Key::Comma;
		case VK_OEM_MINUS:				return Key::Minus;
		case VK_OEM_PERIOD:				return Key::Period;

		case VK_OEM_1:					return Key::Oem1;
		case VK_OEM_2:	 				return Key::Oem2;
		case VK_OEM_3:	 				return Key::Oem3;
		case VK_OEM_4:	 				return Key::Oem4;
		case VK_OEM_5:	 				return Key::Oem5;
		case VK_OEM_6:	 				return Key::Oem6;
		case VK_OEM_7:	 				return Key::Oem7;
		case VK_OEM_8:	 				return Key::Oem8;
												
		case 0xE6:						return Key::OemE6;
		case VK_PACKET:					return Key::Packet;
									
		case 0xE9:						return Key::OemE9;
		case 0xEA:						return Key::OemEA;
		case 0xEB:						return Key::OemEB;
		case 0xEC:						return Key::OemEC;
		case 0xED:						return Key::OemED;
		case 0xEE:						return Key::OemEE;
		case 0xEF:						return Key::OemEF;
		case 0xF0:						return Key::OemF0;
		case 0xF1:						return Key::OemF1;
		case 0xF2:						return Key::OemF2;
		case 0xF3:						return Key::OemF3;
		case 0xF4:						return Key::OemF4;
		case 0xF5:						return Key::OemF5;

		case VK_ATTN:					return Key::Attn;
		case VK_CRSEL:					return Key::CrSel;
		case VK_EXSEL:					return Key::ExSel;
		case VK_EREOF:					return Key::EraseEOF;
		case VK_PLAY:					return Key::Play;
		case VK_ZOOM:					return Key::Zoom;
		case VK_PA1:					return Key::PA1;
		case VK_OEM_CLEAR:				return Key::OemClear;
	}

	return Key::Unknown;
}

bool Key::GetIsModifier()
{
	return
		   *this == Key::Control
		|| *this == Key::Alt
		|| *this == Key::Shift
		|| *this == Key::LeftControl  
		|| *this == Key::RightControl 
		|| *this == Key::LeftShift    
		|| *this == Key::RightShift   
		|| *this == Key::LeftAlt      
		|| *this == Key::RightAlt;
}

bool Key::GetIsDown()
{
	return (::GetKeyState(Code) & 0x8000) != 0;
}

bool Key::GetIsToggled()
{
	return (::GetKeyState(Code) & 0x0001) != 0;
}

bool Key::operator == (const Key& key)
{
	if (Code == Key::Control.Code)
		return key.Code == Key::LeftControl.Code || key.Code == Key::RightControl.Code;
	else if (Code == Key::Alt.Code)
		return key.Code == Key::LeftAlt.Code || key.Code == Key::RightAlt.Code;
	else if (Code == Key::Shift.Code)
		return key.Code == Key::LeftShift.Code || key.Code == Key::RightShift.Code;

	return Code == key.Code;
}

bool Key::operator != (const Key& key)
{
	if (Code == Key::Control.Code)
		return key.Code != Key::LeftControl.Code && key.Code != Key::RightControl.Code;
	else if (Code == Key::Alt.Code)
		return key.Code != Key::LeftAlt.Code && key.Code != Key::RightAlt.Code;
	else if (Code == Key::Shift.Code)
		return key.Code != Key::LeftShift.Code && key.Code != Key::RightShift.Code;

	return Code != key.Code;
}

// Keys

Key Key::Unknown;

Key	Key::LMB(VK_LBUTTON);
Key Key::RMB(VK_RBUTTON);
Key Key::Cancel(VK_CANCEL);
Key Key::MMB(VK_MBUTTON);
Key Key::ListButton1(VK_XBUTTON1);
Key Key::ListButton2(VK_XBUTTON2);
Key Key::Backspace(VK_BACK);
Key Key::Tab(VK_TAB);
Key Key::Clear(VK_CLEAR);
Key Key::Enter(VK_RETURN);
Key Key::Shift(VK_SHIFT);
Key Key::Control(VK_CONTROL);
Key Key::Alt(VK_MENU);
Key Key::Pause(VK_PAUSE);
Key Key::CapsLock(VK_CAPITAL);
Key Key::Kana(VK_KANA);
Key Key::Hangul(VK_HANGUL);
Key Key::Junja(VK_JUNJA);
Key Key::Final(VK_FINAL);
Key Key::Hanja(VK_HANJA);
Key Key::Kanji(VK_KANJI);
Key Key::Escape(VK_ESCAPE);
Key Key::Convert(VK_CONVERT);
Key Key::NonConvert(VK_NONCONVERT);
Key Key::Accept(VK_ACCEPT);
Key Key::ModeChange(VK_MODECHANGE);
Key Key::Space(VK_SPACE);
Key Key::PageUp(VK_PRIOR);
Key Key::PageDown(VK_NEXT);
Key Key::End(VK_END);
Key Key::Home(VK_HOME);
Key Key::Left(VK_LEFT);
Key Key::Up(VK_UP);
Key Key::Right(VK_RIGHT);
Key Key::Down(VK_DOWN);
Key Key::Select(VK_SELECT);
Key Key::Print(VK_PRINT);
Key Key::Execute(VK_EXECUTE);
Key Key::PrintScreen(VK_SNAPSHOT);
Key Key::Insert(VK_INSERT);
Key Key::Delete(VK_DELETE);
Key Key::Help(VK_HELP);

Key Key::Key0(0x30);
Key Key::Key1(0x31);
Key Key::Key2(0x32);
Key Key::Key3(0x33);
Key Key::Key4(0x34);
Key Key::Key5(0x35);
Key Key::Key6(0x36);
Key Key::Key7(0x37);
Key Key::Key8(0x38);
Key Key::Key9(0x39);

Key Key::A(0x41);
Key Key::B(0x42);
Key Key::C(0x43);
Key Key::D(0x44);
Key Key::E(0x45);
Key Key::F(0x46);
Key Key::G(0x47);
Key Key::H(0x48);
Key Key::I(0x49);
Key Key::J(0x4A);
Key Key::K(0x4B);
Key Key::L(0x4C);
Key Key::M(0x4D);
Key Key::N(0x4E);
Key Key::O(0x4F);
Key Key::P(0x50);
Key Key::Q(0x51);
Key Key::R(0x52);
Key Key::S(0x53);
Key Key::T(0x54);
Key Key::U(0x55);
Key Key::V(0x56);
Key Key::W(0x57);
Key Key::X(0x58);
Key Key::Y(0x59);
Key Key::Z(0x5A);

Key Key::LeftWindows(VK_LWIN);
Key Key::RightWindows(VK_RWIN);
Key Key::Applications(VK_APPS);
Key Key::Sleep(VK_SLEEP);

Key Key::Num0(VK_NUMPAD0);
Key Key::Num1(VK_NUMPAD1);
Key Key::Num2(VK_NUMPAD2);
Key Key::Num3(VK_NUMPAD3);
Key Key::Num4(VK_NUMPAD4);
Key Key::Num5(VK_NUMPAD5);
Key Key::Num6(VK_NUMPAD6);
Key Key::Num7(VK_NUMPAD7);
Key Key::Num8(VK_NUMPAD8);
Key Key::Num9(VK_NUMPAD9);

Key Key::Multiply(VK_MULTIPLY);
Key Key::Add(VK_ADD);
Key Key::Separator(VK_SEPARATOR);
Key Key::Subtract(VK_SUBTRACT);
Key Key::Decimal(VK_DECIMAL);
Key Key::Divide(VK_DIVIDE);

Key Key::F1(VK_F1);
Key Key::F2(VK_F2);
Key Key::F3(VK_F3);
Key Key::F4(VK_F4);
Key Key::F5(VK_F5);
Key Key::F6(VK_F6);
Key Key::F7(VK_F7);
Key Key::F8(VK_F8);
Key Key::F9(VK_F9);
Key Key::F10(VK_F10);
Key Key::F11(VK_F11);
Key Key::F12(VK_F12);
Key Key::F13(VK_F13);
Key Key::F14(VK_F14);
Key Key::F15(VK_F15);
Key Key::F16(VK_F16);
Key Key::F17(VK_F17);
Key Key::F18(VK_F18);
Key Key::F19(VK_F19);
Key Key::F20(VK_F20);
Key Key::F21(VK_F21);
Key Key::F22(VK_F22);
Key Key::F23(VK_F23);
Key Key::F24(VK_F24);

Key Key::NumLock(VK_NUMLOCK);
Key Key::ScrollLock(VK_SCROLL);

Key Key::KeyOem1(0x92);
Key Key::KeyOem2(0x93);
Key Key::KeyOem3(0x94);
Key Key::KeyOem4(0x95);
Key Key::KeyOem5(0x96);

Key Key::LeftShift(VK_LSHIFT);
Key Key::RightShift(VK_RSHIFT);
Key Key::LeftControl(VK_LCONTROL);
Key Key::RightControl(VK_RCONTROL);
Key Key::LeftAlt(VK_LMENU);
Key Key::RightAlt(VK_RMENU);

Key Key::BrowserBack(VK_BROWSER_BACK);
Key Key::BrowserForward(VK_BROWSER_FORWARD);
Key Key::BrowserRefresh(VK_BROWSER_REFRESH);
Key Key::BrowserStop(VK_BROWSER_STOP);
Key Key::BrowserSearch(VK_BROWSER_SEARCH);
Key Key::BrowserFavorites(VK_BROWSER_FAVORITES);
Key Key::BrowserHome(VK_BROWSER_HOME);

Key Key::VolumeMute(VK_VOLUME_MUTE);
Key Key::VolumeDown(VK_VOLUME_DOWN);
Key Key::VolumeUp(VK_VOLUME_UP);

Key Key::MediaNextTrack(VK_MEDIA_NEXT_TRACK);
Key Key::MediaPreviousTrack(VK_MEDIA_PREV_TRACK);
Key Key::MediaStop(VK_MEDIA_STOP);
Key Key::MediaPlayPause(VK_MEDIA_PLAY_PAUSE);

Key Key::StartMail(VK_LAUNCH_MAIL);
Key Key::SelectMedia(VK_LAUNCH_MEDIA_SELECT);
Key Key::StartApplication1(VK_LAUNCH_APP1);
Key Key::StartApplication2(VK_LAUNCH_APP2);

Key Key::Plus(VK_OEM_PLUS);
Key Key::Comma(VK_OEM_COMMA);
Key Key::Minus(VK_OEM_MINUS);
Key Key::Period(VK_OEM_PERIOD);

Key Key::Oem1(VK_OEM_1);
Key Key::Oem2(VK_OEM_2);
Key Key::Oem3(VK_OEM_3);
Key Key::Oem4(VK_OEM_4);
Key Key::Oem5(VK_OEM_5);
Key Key::Oem6(VK_OEM_6);
Key Key::Oem7(VK_OEM_7);
Key Key::Oem8(VK_OEM_8);

Key Key::OemE6(0xE6);

Key Key::Packet(VK_PACKET);

Key Key::OemE9(0xE9);
Key Key::OemEA(0xEA);
Key Key::OemEB(0xEB);
Key Key::OemEC(0xEC);
Key Key::OemED(0xED);
Key Key::OemEE(0xEE);
Key Key::OemEF(0xEF);
Key Key::OemF0(0xF0);
Key Key::OemF1(0xF1);
Key Key::OemF2(0xF2);
Key Key::OemF3(0xF3);
Key Key::OemF4(0xF4);
Key Key::OemF5(0xF5);

Key Key::Attn(VK_ATTN);
Key Key::CrSel(VK_CRSEL);
Key Key::ExSel(VK_EXSEL);
Key Key::EraseEOF(VK_EREOF);
Key Key::Play(VK_PLAY);
Key Key::Zoom(VK_ZOOM);
Key Key::PA1(VK_PA1);
Key Key::OemClear(VK_OEM_CLEAR);
