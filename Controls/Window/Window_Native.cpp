#include "Window.h"

#include <Types/Graphics\GdiImage8.h>
#include <Animation\Animation.h>

#include <Devices\Keyboard\KeyCombo.h>
//#include "Tooltip.h"
#include <Devices\Tablet\Tablet.h>
#include <Clipboard\Clipboard.h>
#include <Controls\Grip\Grip.h>
#include <Controls\Textbox\Textbox.h>

#include <locale.h>
#include <stdio.h>

LRESULT CALLBACK Window::WindowProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// This is the static window proc which allows the actual window proc to exist within a class

	if (msg == WM_CREATE)
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)(LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);

	return ((Window*)(LONG_PTR)::GetWindowLongPtr(hwnd, GWLP_USERDATA))->WindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// For keyboard messages, some things have to be done first

	int scanCode = 0;
	int keyCode = 0;
	wchar_t wchar = null;

	byte keyState[256];
	::GetKeyboardState(keyState);

	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		{
			switch (wParam)
			{
			case VK_NUMPAD0:
			case VK_NUMPAD1:
			case VK_NUMPAD2:
			case VK_NUMPAD3:
			case VK_NUMPAD4:
			case VK_NUMPAD5:
			case VK_NUMPAD6:
			case VK_NUMPAD7:
			case VK_NUMPAD8:
			case VK_NUMPAD9:
			case VK_DECIMAL:
			case VK_ADD:
			case VK_SUBTRACT:
			case VK_MULTIPLY:
			case VK_DIVIDE:
				{
					keyCode = (int)wParam;
					scanCode = ::MapVirtualKeyEx(keyCode, MAPVK_VK_TO_VSC, m_hkl);

					wchar_t wchars[2] = { null };

					int result = ::ToAsciiEx(keyCode, scanCode, keyState, (WORD*)wchars, 0, m_hkl);

					if (result == 1)
						wchar = wchars[0];

					break;
				}
			default:
				{
					scanCode = ((UINT)lParam & 0xFF0000) >> 16;
					keyCode = ::MapVirtualKeyEx(((UINT)lParam & 0xFF0000) >> 16, MAPVK_VSC_TO_VK_EX, m_hkl);

					wchar_t wchars[2] = { null };

					int result = ::ToAsciiEx(keyCode, scanCode, keyState, (WORD*)wchars, 0, m_hkl);

					if (result == 1)
						wchar = wchars[0];

					break;
				}
			}

			break;
		}
	case WM_CHAR:
	case WM_SYSCHAR:
		{
			wchar = (wchar_t)wParam;
			//keyCode = MapVirtualKeyEx(wchar, MAPVK_VSC_TO_VK, m_hkl);
			break;
		}
	}

	// Now process the message

	switch (msg) 
	{
	//case WM_WINDOWPOSCHANGING:	return WmWindowPosChanging(wParam, lParam);
	//case WM_WINDOWPOSCHANGED:		return WmWindowPosChanged(wParam, lParam);
	case WM_NCACTIVATE:				return WmNcActivate(wParam, lParam);
	case WM_ACTIVATE:				return WmActivate(wParam, lParam);
	case WM_PAINT:					return WmPaint(wParam, lParam);
	case WM_MOVING:					return WmMoving(wParam, lParam);
	case WM_MOVE:					return WmMove(wParam, lParam);
	case WM_SIZING:					return WmSizing(wParam, lParam);
	case WM_SIZE:					return WmSize(wParam, lParam);
	case WM_SETTINGCHANGE:			return WmSettingChange(wParam, lParam);
	case WM_INPUTLANGCHANGEREQUEST:	return WmInputLangChangeRequest(wParam, lParam);
	case WM_ERASEBKGND:				return WmEraseBkgnd(wParam, lParam);
	case WM_NCHITTEST:				return WmNcHitTest(wParam, lParam);
	case WM_NCMOUSEMOVE:			return WmNcMouseMove(wParam, lParam);
	case WM_MOUSEMOVE:				return WmMouseMove(Mouse::GetPosition());
	case WM_NCMOUSELEAVE:			return WmNcMouseLeave(wParam, lParam);
	case WM_MOUSELEAVE:				return WmMouseLeave(wParam, lParam);
	case WM_LBUTTONDOWN:			return WmLButtonDown(Mouse::GetPosition());
	case WM_LBUTTONUP:				return WmLButtonUp(wParam, lParam);
	case WM_MOUSEWHEEL:				return WmMouseWheel(Mouse::GetPosition(), (double)GET_WHEEL_DELTA_WPARAM(wParam) / (double)WHEEL_DELTA);
	case WM_MOUSEHWHEEL:			return WmMouseHWheel(Mouse::GetPosition(), (double)GET_WHEEL_DELTA_WPARAM(wParam) / (double)WHEEL_DELTA);
	case WT_PACKET:					return WtPacket(wParam, lParam);
	case WT_PROXIMITY:				return WtProximity(wParam, lParam);
	case WT_INFOCHANGE:				return WtInfoChange(wParam, lParam);
	case WT_CTXUPDATE:				return WtContextUpdate(wParam, lParam);
	case WT_CSRCHANGE:				return WtCursorChange(wParam, lParam);
	case WM_KEYDOWN:				return WmKeyDown(wParam, lParam, keyCode, wchar);
	case WM_SYSKEYDOWN:				return WmKeyDown(wParam, lParam, keyCode, wchar);
	case WM_KEYUP:					return WmKeyUp(wParam, lParam, keyCode, wchar);
	case WM_SYSKEYUP:				return WmKeyUp(wParam, lParam, keyCode, wchar);
	case WM_CHAR:					return WmChar(wParam, lParam, wchar);
	case WM_SYSCHAR:				return WmChar(wParam, lParam, wchar);
	case WM_DEADCHAR:				return WmChar(wParam, lParam, wchar);
	case WM_CLOSE:					return WmClose(wParam, lParam);
	case WM_DESTROY:				return WmDestroy(wParam, lParam);
	case WM_TIMER:					return WmTimer(wParam, lParam);
	case WM_CLIPBOARDUPDATE:		return WmClipboardUpdate();
	case WM_DESTROYCLIPBOARD:		return WmDestroyClipboard();
	//case WM_USER_CALLFUNCTION:	return WmUserCallFunction(wParam, lParam);
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

/****************************************************************************************************************/

LRESULT Window::WmWindowPosChanging(WPARAM wParam, LPARAM lParam)
{
	// Update the window frame if necessary

	WINDOWPOS* wp = (WINDOWPOS*)lParam;

	if (m_frame && m_frame != &WindowFrame::None && !m_frame->IsSystemFrame)
	{	
		Point position(wp->x, wp->y);
		_Size size(wp->cx, wp->cy);

		m_frame->Update(
			position, 
			size, 
			true, // update position
			true, // update size
			m_hwnd == ::GetActiveWindow());
	}

	return ::DefWindowProc(m_hwnd, WM_WINDOWPOSCHANGING, wParam, lParam);
}

LRESULT Window::WmWindowPosChanged(WPARAM /*wParam*/, LPARAM lParam)
{
	WINDOWPOS* wp = (WINDOWPOS*)lParam;

	m_suppressRectChanges = true;

	Control::SetRect(
		wp->x,
		wp->y,
		wp->cx,
		wp->cy);

	m_suppressRectChanges = false;

	return 0;
}

LRESULT Window::WmNcActivate(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_frame && m_frame != &WindowFrame::None && !m_frame->IsSystemFrame)
	{
		m_frame->Update(
			GetPosition(), 
			GetSize(), 
			false, // don't update position
			true, // update size
			wParam == TRUE);

		SetWindowPos(m_frame->m_hwndBottom, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(m_frame->m_hwndLeft,   HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(m_frame->m_hwndRight,  HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(m_frame->m_hwndTop,    HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	}

	return TRUE;
}

LRESULT Window::WmActivate(WPARAM wParam, LPARAM /*lParam*/)
{
	bool activated = LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE;

	if (activated)
	{
		s_currentWindow = this;
		Clipboard::SetOwnerWindow(this);

		/*	As the window is shown just now, the mouse 
			"shouldn't have been moved" while it was hidden. */		
		m_prevMousePos = Mouse::GetPosition();
	}

	if (m_tablet)
	{
		m_tablet->Enable(activated);
		m_tablet->Overlap(activated);
	}

	// Reset the popups

	ShowFloating(null);

	//if (Tooltip::s_wnd && Tooltip::s_wnd != this)
	//	Tooltip::Show(null, 0, 0);

	//Menu::s_wnd = this;
	//Tooltip::s_wnd = this;

	return 0;
}

LRESULT Window::WmMoving(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

LRESULT Window::WmMove(WPARAM /*wParam*/, LPARAM lParam)
{
	// If a menu is shown, remove it

	//if (menus.Count > 0)
	//	Menu::Show(null, 0, 0);

	SuspendUpdates();
	m_suppressRectChanges = true;

	Control::SetPosition(
		(int)(short)LOWORD(lParam),
		(int)(short)HIWORD(lParam));

	m_suppressRectChanges = false;
	ResumeUpdates();

	return 0;
}

LRESULT Window::WmSizing(WPARAM wParam, LPARAM lParam)
{
	RECT* r = (RECT*)lParam;

	Control::_SizingMessage::_Edge edge = Control::_SizingMessage::None;

	switch (wParam)
	{
	case WMSZ_TOPLEFT:
		if (MinSize.Height > 0) r->top  = min(r->top, r->bottom - MinSize.Height);
		if (MaxSize.Height > 0) r->top  = max(r->bottom - MaxSize.Height, r->top);

		if (MinSize.Width  > 0) r->left = min(r->left, r->right - MinSize.Width);
		if (MaxSize.Width  > 0) r->left = max(r->right - MaxSize.Width, r->left);

		edge = Control::_SizingMessage::TopLeft;

		break;
	
	case WMSZ_TOP:			
		if (MinSize.Height > 0) r->top = min(r->top, r->bottom - MinSize.Height);
		if (MaxSize.Height > 0) r->top = max(r->bottom - MaxSize.Height, r->top);

		edge = Control::_SizingMessage::Top;			

		break;
	
	case WMSZ_TOPRIGHT:		
		if (MinSize.Height > 0) r->top   = min(r->top, r->bottom - MinSize.Height);
		if (MaxSize.Height > 0) r->top   = max(r->bottom - MaxSize.Height, r->top);

		if (MinSize.Width  > 0) r->right = max(r->left + MinSize.Width, r->right);
		if (MaxSize.Width  > 0) r->right = min(r->right, r->left + MaxSize.Width);

		edge = Control::_SizingMessage::TopRight;		

		break;
	
	case WMSZ_LEFT:			
		if (MinSize.Width > 0) r->left = min(r->left, r->right - MinSize.Width);
		if (MaxSize.Width > 0) r->left = max(r->right - MaxSize.Width, r->left);

		edge = Control::_SizingMessage::Left;			

		break;
	
	case WMSZ_RIGHT:		
		if (MinSize.Width > 0) r->right = max(r->left + MinSize.Width, r->right);
		if (MaxSize.Width > 0) r->right = min(r->right, r->left + MaxSize.Width);

		edge = Control::_SizingMessage::Right;			

		break;
	
	case WMSZ_BOTTOMLEFT:
		if (MinSize.Height > 0) r->bottom = max(r->top + MinSize.Height, r->bottom);
		if (MaxSize.Height > 0) r->bottom = min(r->bottom, r->top + MaxSize.Height);

		if (MinSize.Width  > 0) r->left   = min(r->left, r->right - MinSize.Width);
		if (MaxSize.Width  > 0) r->left   = max(r->right - MaxSize.Width, r->left);

		edge = Control::_SizingMessage::BottomLeft;		

		break;
	
	case WMSZ_BOTTOM:
		if (MinSize.Height > 0) r->bottom = max(r->top + MinSize.Height, r->bottom);
		if (MaxSize.Height > 0) r->bottom = min(r->bottom, r->top + MaxSize.Height);

		edge = Control::_SizingMessage::Bottom;			

		break;
	
	case WMSZ_BOTTOMRIGHT:	
		if (MinSize.Height > 0) r->bottom = max(r->top + MinSize.Height, r->bottom);
		if (MaxSize.Height > 0) r->bottom = min(r->bottom, r->top + MaxSize.Height);

		if (MinSize.Width  > 0) r->right  = max(r->left + MinSize.Width, r->right);
		if (MaxSize.Width  > 0) r->right  = min(r->right, r->left + MaxSize.Width);

		edge = Control::_SizingMessage::BottomRight;	

		break;
	}

	_Rect rect(
		r->left, 
		r->top, 
		r->right - r->left, 
		r->bottom - r->top);

	_SizingMessage msg(this, rect, edge);
	SizingEvent.Send(msg);

	// Return the results back to WM_SIZING

	r->left   = rect.X;
	r->top    = rect.Y;
	r->right  = rect.X + rect.Width;
	r->bottom = rect.Y + rect.Height;

	return TRUE;
}

LRESULT Window::WmSize(WPARAM /*wParam*/, LPARAM lParam)
{
	// If a menu is shown, remove it

	if (m_floating.Count > 0)
		ShowFloating(null);

	m_suppressRectChanges = true;

	Control::SetSize(
		(int)LOWORD(lParam),
		(int)HIWORD(lParam));

	m_suppressRectChanges = false;

	return 0;
}

LRESULT Window::WmSettingChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	Textbox::SetCaretBlinkInterval((double)GetCaretBlinkTime() / 1000.0);
	UpdateSystemSettings();
	Update();

	return 0;
}

LRESULT Window::WmInputLangChangeRequest(WPARAM wParam, LPARAM lParam)
{
	::UnloadKeyboardLayout(m_hkl);
	m_hkl = null;

	wchar_t wcLayout[9];
	swprintf_s(wcLayout, sizeof(wcLayout) / sizeof(wchar_t), L"%04X%04X", 0, GetUserDefaultLangID());

	m_hkl = ::LoadKeyboardLayout(wcLayout, KLF_ACTIVATE | KLF_REPLACELANG | KLF_SUBSTITUTE_OK);

	return ::DefWindowProc(m_hwnd, WM_INPUTLANGCHANGEREQUEST, wParam, lParam);
}

LRESULT Window::WmEraseBkgnd(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Do nothing; I do that sheeet myself yo! Fo sho!
	return 1;
}

// BUG: make sure toggle display on the tablet causes Play 
// to reevaluate the reference rectangle and draw correctly.
// This is a generic issue for multi-monitor systems.

LRESULT Window::WmPaint(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (!TryLock())
		return 0;

	HRGN updateRgn = ::CreateRectRgn(0, 0, 0, 0);
	int updateRgnResult = ::GetUpdateRgn(m_hwnd, updateRgn, false);
	
	if (updateRgnResult != NULLREGION 
		&& updateRgnResult != ERROR)
	{
		/*	Create update region. */

		int rgndataSize = ::GetRegionData(
			updateRgn, 
			sizeof(RGNDATA), 
			null);

		RGNDATA* rgndata = (RGNDATA*)malloc(rgndataSize);
		rgndata->rdh.dwSize = rgndataSize;

		Region rgnPaint;

		if (::GetRegionData(
			updateRgn,
			rgndataSize,
			rgndata) != 0)
		{
			RECT* rects = (RECT*)rgndata->Buffer;

			for (uint i = 0; i < rgndata->rdh.nCount; i++)
			{
				rgnPaint.Add(_Rect(
					rects[i].left,
					rects[i].top,
					rects[i].right - rects[i].left,
					rects[i].bottom - rects[i].top));
			}
		}

		free(rgndata);

		/*	Paint the window, which will
			paint all of its children. */
		Paint(rgnPaint);
		
#ifdef _DEBUG
		if (showUpdateRgn)
		{
			HBRUSH hb = ::CreateSolidBrush(0x8080FF);
			::FrameRgn(Buffer->Hdc, updateRgn, hb, 1, 1);
			::DeleteObject(hb);
		}
#endif

		// Blit back buffer to window

		PAINTSTRUCT ps;
		HDC hdcPaint = ::BeginPaint(m_hwnd, &ps);

		::SelectClipRgn(hdcPaint, updateRgn); 

		RECT bounds;
		::GetRgnBox(updateRgn, &bounds);

		::BitBlt(
			hdcPaint, 
			bounds.left, 
			bounds.top, 
			bounds.right - bounds.left, 
			bounds.bottom - bounds.top, 
			Buffer->Hdc, 
			bounds.left, 
			bounds.top, 
			SRCCOPY);

		::SelectClipRgn(hdcPaint, null);

		::EndPaint(m_hwnd, &ps);
	}

	// BUG: when an animation is finished, sometimes the whole window repaints. Also before an animation starts.
	// NOTE: I think this bug has been fixed and was due to the Paint(rgn ? rgn : PaintRect) stuff, which
	// should now be avoided. However more testing is needed.

	::DeleteObject(updateRgn);

	Unlock();

	/*	Update the animator if there is 
		more stuff to animate. */

	::KillTimer(m_hwnd, IDT_ANIMATOR);

	if (Animation::GetAnimator().IsBusy)
		Animation::GetAnimator().Update();

	return 0;
}

LRESULT Window::WmNcHitTest(WPARAM wParam, LPARAM lParam)
{
	Point p = Mouse::GetPosition();

	Control* hit = Control::Captured 
		? Control::Captured 
		: HitTest(p);

	if (hit && typeof(hit) == TYPEOF(Grip))
	{
		// Return the hit result

		Grip::Result result = ((Grip*)hit)->Result_;

		switch (result)
		{
		case Grip::Caption:			return HTCAPTION;
		case Grip::Icon:			return HTSYSMENU;
		case Grip::Minimize:		return HTMINBUTTON;
		case Grip::Maximize:		return HTMAXBUTTON;
		case Grip::Close:			return HTCLOSE;

		case Grip::Left:			return HTLEFT;
		case Grip::Right:			return HTRIGHT;
		case Grip::Top:				return HTTOP;
		case Grip::Bottom:			return HTBOTTOM;

		case Grip::TopLeft:			return HTTOPLEFT;
		case Grip::TopRight:		return HTTOPRIGHT;
		case Grip::BottomLeft:		return HTBOTTOMLEFT;
		case Grip::BottomRight:		return HTBOTTOMRIGHT;

		default:
		case Grip::Client:			return HTCLIENT;
		}
	}
	
	return DefWindowProc(m_hwnd, WM_NCHITTEST, wParam, lParam);
}

LRESULT Window::WmNcMouseMove(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	Point p = Mouse::GetPosition();

	this->m_overNonClient = true;
	this->CheckMouseEnter(p, m_prevMousePos, false); // Window

	Control* hit = Control::Captured 
		? Control::Captured 
		: HitTest(p);

	if (hit)
	{
		if (hit == this && Control::Over)
		{
			Control::Over->CheckMouseLeave(p, m_prevMousePos);
			Control::Over = null;
		}
		
		if (hit->Enabled)
		{
			hit->CheckMouseEnter(p, m_prevMousePos); // Control
		}
	}

	m_prevMousePos = p;

	return 0;
}

LRESULT Window::WmMouseMove(const Point& p, bool fromTablet)
{
	this->m_overClient = true;
	this->CheckMouseEnter(p, m_prevMousePos, true); // Window

	Control* hit = Control::Captured
		? Control::Captured
		: HitTest(p);

	if (hit == this && Control::Over)
	{
		Control::Over->CheckMouseLeave(p, m_prevMousePos);
		Control::Over = null;
	}

	// Shake
	
	if (hit && TestShake(p))
	{
		// Fire shake event

		Point _p = p;
		hit->PointScreenToClient(&_p);

		Mouse::Message msg;

		msg.Object       = hit;
		msg.Position     = _p;
		msg.PrevPosition = m_prevMousePos;

		hit->MouseShakeEvent.Send(msg);
		
		return 0;
	}

	// Tooltip

	//if (hit && hit->GetTooltip() && hit != m_hitTooltip
	//	&& !Key::LMB.IsDown && !Key::MMB.IsDown && !Key::RMB.IsDown) // no tooltip is shown if any mouse button is pressed
	//{
	//	m_hitTooltip = hit;
	//	::SetTimer(hwnd, IDT_TOOLTIP, (UINT)(m_tooltipDelay * 1000.0), null);

		//// BUG: fix the timer glitch (I think the animation isn't nudging itself properly; the bug occurs only the first time on every control once)
	//}
	//else if (hit && !hit->GetTooltip() && m_hitTooltip)
	//{
		//m_hitTooltip = null;
		//::KillTimer(hwnd, IDT_TOOLTIP);
		//Tooltip::Show(null, 0, 0);
	//}

	if (hit->Enabled)
	{
		if (hit != this) // The mouse is already in the window
			hit->CheckMouseEnter(p, m_prevMousePos);

		// Fire events

		if (!m_tablet 
			|| (m_tablet && !m_tablet->InContext) 
			|| fromTablet) // unless the tablet is being used
		{
			Point _p = p;
			hit->PointScreenToClient(&_p);

			/*	Stop the hold timer. */
			
			if (IsTimerRunning(m_holdTimerId)
				&& (_p.X < m_pHoldStart.X - (m_doubleClickSize.Width / 2) || _p.X > m_pHoldStart.X + (m_doubleClickSize.Width / 2) 
				|| _p.Y < m_pHoldStart.Y - (m_doubleClickSize.Height / 2) || _p.Y > m_pHoldStart.Y + (m_doubleClickSize.Height / 2)))
			{
				StopTimer(m_holdTimerId);
			}

			Point _prevMousePos = m_prevMousePos;
			hit->PointScreenToClient(&_prevMousePos);

			//

			Mouse::Message msg;

			msg.Object       = hit;
			msg.Position     = _p;
			msg.PrevPosition = _prevMousePos;

			hit->MouseMoveEvent.Send(msg);
		}

		Mouse::SetCursor(hit->GetCursor());
	}
	else
	{
		Mouse::SetCursor(Cursor::Dot);
	}

	m_prevMousePos = p;

	return 0;
}

LRESULT Window::WmNcMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	/*	Both in this and in the next message we check for 
		the status of the other one. If either one is true,
		then the mouse is still in the window. */

	Point p = Mouse::GetPosition();

	this->m_overNonClient = false;
	this->CheckMouseLeave(p, m_prevMousePos);

	return 0;
}

LRESULT Window::WmMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	/*	Both in this and in the previous message we check for 
		the status of the other one. If either one is true,
		then the mouse is still in the window. */
	
	Point p = Mouse::GetPosition();

	this->m_overClient = false;
	this->CheckMouseLeave(p, m_prevMousePos);

	return 0;
}

LRESULT Window::WmLButtonDown(const Point& _p)
{
	m_mouseDown = true;

	if (m_tablet)
	{
		m_tablet->Enable(true);
		m_tablet->Overlap(true);
		m_tablet->Flush();
	}

	if (!m_lockedCapture)
		::SetCapture(m_hwnd);

	Point p = _p;

	Control* hit = Control::Captured 
		? Control::Captured 
		: HitTest(p);

	// If a tooltip is visible, kill it

	//if (m_hitTooltip)
	//{
	//	::KillTimer(hwnd, IDT_TOOLTIP);
	//	m_hitTooltip = null;
	//}

	//if (GetTooltip())
	//	Tooltip::Show(null, 0, 0);

	if (hit && hit->Enabled /* && hit->HitTestPart(p.x, p.y) != Control::Transparent */)
	{
		/*	If a floating control is shown, remove it, 
			unless it contains the hit. */

		bool inFloating = false;

		for (int i = 0; i < m_floating.Count; i++)
		{
			if (m_floating[i] == hit || m_floating[i]->HasChild(hit))
			{
				inFloating = true;
				break;
			}
		}

		if (m_floating.Count > 0 && !inFloating)
			ShowFloating(null);

		// Unfocus currently focused control
		if (Control::Focused && Control::Focused != hit)
			Control::Focused->LoseFocus();

		hit->Capture();

		hit->PointScreenToClient(&p);

		// Create event args

		Mouse::Message msg;

		msg.Object       = hit;
		msg.Position     = p;
		msg.PrevPosition = m_prevMousePos;

		// If the tablet stylus is being used
		if (m_tablet && m_tablet->InContext)
		{
			msg.IsPen = true;
			msg.CursorId = m_tablet->curStylusId;
		}

		/*	Start the hold timer. */

		m_pHoldStart = p;
		m_holdArgs = msg;

		StartTimer(
			m_holdTimerId,
			hit->HoldDelay,
			hit,
			holdTimer_OnTick);

		/*	The ButtonDown event is always fired
			regardless of the double clicks. */

		hit->LButtonDownEvent.Send(msg);

		// Set focus to the control

		if (hit->AllowFocus)
			hit->SetFocus();

		// Handle the double clicks

		if (m_LDoubleClicking)
		{
			if (p.X >= m_pDoubleClick.X - (m_doubleClickSize.Width / 2) && p.X <= m_pDoubleClick.X + (m_doubleClickSize.Width / 2) &&
			    p.Y >= m_pDoubleClick.Y - (m_doubleClickSize.Height / 2) && p.Y <= m_pDoubleClick.Y + (m_doubleClickSize.Height / 2))
			{
				hit->LDoubleClickEvent.Send(msg);
			}

			m_LDoubleClicking = false;
			::KillTimer(m_hwnd, IDT_DBLCLK);
		}
		else if (hit->AllowDoubleClick)
		{
			m_LDoubleClicking = true;
			m_pDoubleClick = p;

			::KillTimer(m_hwnd, IDT_DBLCLK);
			::SetTimer(m_hwnd, IDT_DBLCLK, (UINT)(m_doubleClickDuration * 1000.0), NULL);
		}
	}

	return 0;
}

LRESULT Window::WmLButtonUp(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	/*	Stop the hold timer. */
	StopTimer(m_holdTimerId);

	if (!m_mouseDown)
		return 0;

	m_mouseDown = false;
	Point p = Mouse::GetPosition();

	Control* hitPhysical = HitTest(p);
	Control* hit = Control::Captured 
		? Control::Captured 
		: hitPhysical;

	if (hit && hit->Enabled /* && hit->HitTestPart(p.x, p.y) != Control::Transparent */)
	{
		hit->PointScreenToClient(&p);

		/*	If the button is released outside of the double-click 
			rectangle, cancel the double-click. */

		if (m_LDoubleClicking &&
			!(p.X >= m_pDoubleClick.X - (m_doubleClickSize.Width / 2) && p.X <= m_pDoubleClick.X + (m_doubleClickSize.Width / 2) &&
		      p.Y >= m_pDoubleClick.Y - (m_doubleClickSize.Height / 2) && p.Y <= m_pDoubleClick.Y + (m_doubleClickSize.Height / 2)))
		{
			m_LDoubleClicking = false;
			::KillTimer(m_hwnd, IDT_DBLCLK);
		}

		// Fire the OnLButtonUpMessage event

		Mouse::Message msg;

		msg.Object       = hit;
		msg.Position     = p;
		msg.PrevPosition = m_prevMousePos;

		// If the tablet stylus is being used
		
		if (m_tablet && m_tablet->InContext)
			msg.IsPen = true;

		hit->LButtonUpEvent.Send(msg);

		/*	If msg.Result is true on return, it means that the 
			the ButtonUp event used up the action for itself, 
			and therefore no Click event should be fired. */

		if (!msg.Result && hit == hitPhysical && !m_holdIgnoreButtonUp)
		{
			/*	This should work only if the mouse button is released over 
				the object that was originally pressed. This means that if 
				the mouse button is pressed over a control, then moved out 
				of its bounds, and then released, this does not constitute 
				a click.

				THIS SHOULD CONSTITUTE A CLICK BY DEFAULT, BUT SHOULD BE 
				DECIDED BY EACH CONTROL SEPARATELY. */

			hit->LClickEvent.Send(msg);
		}

		m_holdIgnoreButtonUp = false;
	}

	Control::Captured->Uncapture();

	if (!m_lockedCapture)
		::ReleaseCapture();

	return 0;
}

LRESULT Window::WmMouseWheel(const Point& _p, double delta)
{
	Point p = _p;

	Control* hit = Control::Captured 
		? Control::Captured 
		: HitTest(p);

	if (hit && hit->Enabled)
	{
		hit->PointScreenToClient(&p);

		// Fire events

		Mouse::Message msg;

		msg.Object       = hit;
		msg.Position     = p;
		msg.PrevPosition = p;
		msg.WheelDelta   = delta;

		// If the tablet stylus is being used
		if (m_tablet && m_tablet->InContext)
		{
			msg.IsPen = true;
			msg.CursorId = m_tablet->curStylusId;
		}

		hit->MouseWheelEvent.Send(msg);
	}

	return 0;
}

LRESULT Window::WmMouseHWheel(const Point& _p, double delta)
{
	Point p = _p;

	Control* hit = Control::Captured 
		? Control::Captured 
		: HitTest(p);

	if (hit && hit->Enabled)
	{
		hit->PointScreenToClient(&p);

		// Fire events

		Mouse::Message msg;

		msg.Object       = hit;
		msg.Position     = p;
		msg.PrevPosition = p;
		msg.WheelDelta   = delta;

		// If the tablet stylus is being used
		if (m_tablet && m_tablet->InContext)
		{
			msg.IsPen = true;
			msg.CursorId = m_tablet->curStylusId;
		}

		hit->MouseHWheelEvent.Send(msg);
	}

	return 0;
}

LRESULT Window::WtPacket(WPARAM /*wParam*/, LPARAM lParam)
{
	PACKET packets[TABLET_PACKET_QUEUE_SIZE];
	Array<Tablet::Message::Point> inputPoints(TABLET_PACKET_QUEUE_SIZE);

	int nPacketsRetrieved = Tablet::dll_WTPacketsGet(
		(HCTX)lParam, 
		m_tablet->packetQueueSize, 
		packets);

	if (nPacketsRetrieved > 0)
	{
		_Rect screenRect = Screen::GetRect();
		_Rect rect = m_tablet->ContextRect;

		for (int i = 0; i < nPacketsRetrieved; i++)
		{
			inputPoints[i].CursorId = packets[i].pkCursor;
			inputPoints[i].X = screenRect.X + (double)(packets[i].pkX - rect.X) / (double)rect.Width * (double)screenRect.Width;
			inputPoints[i].Y = screenRect.Y + (double)screenRect.Height - (double)(packets[i].pkY - rect.Y) / (double)rect.Height * (double)screenRect.Height;
			inputPoints[i].Pressure = (double)(packets[i].pkNormalPressure - m_tablet->minPressure) / (double)(m_tablet->maxPressure - m_tablet->minPressure);
			inputPoints[i].Azimuth = (double)packets[i].pkOrientation.orAzimuth / 10.0;
			inputPoints[i].Elevation = (double)packets[i].pkOrientation.orAltitude / 10.0;
			inputPoints[i].Twist = (double)packets[i].pkOrientation.orTwist / 10.0;
			inputPoints[i].dTime = packets[i].pkTime;

			m_tablet->curStylusId = packets[i].pkCursor;
		}

		int x = (int)inputPoints[nPacketsRetrieved - 1].X;
		int y = (int)inputPoints[nPacketsRetrieved - 1].Y;

		Control* hitPhysical = HitTest(Point(x, y));
		Control* hit = Control::Captured 
			? Control::Captured 
			: hitPhysical;

		if (hit && hit->Enabled)
		{
			Point p(
				Math::Round(inputPoints[nPacketsRetrieved - 1].X), 
				Math::Round(inputPoints[nPacketsRetrieved - 1].Y)); // for the mouse event later, just in case

			Point p0(0, 0);
			hit->PointToScreen(&p0);

			for (int i = 0; i < nPacketsRetrieved; i++)
			{
				inputPoints[i].X -= (double)p0.X;
				inputPoints[i].Y -= (double)p0.Y;
			}

			Tablet::Message msg;
			
			msg.Object = hit;
			msg.Points = inputPoints;
			msg.Result = false;

			hit->PenMoveEvent.Send(msg);

			/*	If the pen event wasn't handled and the cursor
				has moved, send the event to the mouse. */
			if (!msg.Result && p != m_prevMousePos)
				WmMouseMove(p, true);
		}
	}

	return 0;
}

LRESULT Window::WtProximity(WPARAM wParam, LPARAM lParam)
{
	if (m_tablet && m_tablet->GetContext() == (HCTX)wParam)
	{
		m_tablet->Flush();
		m_tablet->SetInContext(LOWORD(lParam) != 0);

		// if (HIWORD(lParam) != 0) // ignore hardware proximity

		// TODO: send this message to the window with some appropriate info (don't know what that is yet)
	}
	
	return 0;
}

LRESULT Window::WtInfoChange(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_tablet && m_tablet->GetContext() == (HCTX)wParam)
	{
		m_tablet->Flush();
		m_tablet->UpdateInfo();
	}

	return 0;
}

LRESULT Window::WtContextUpdate(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_tablet && m_tablet->GetContext() == (HCTX)wParam)
	{
		m_tablet->Flush();
		m_tablet->UpdateInfo();
	}

	return 0;
}

LRESULT Window::WtCursorChange(WPARAM /*wParam*/, LPARAM lParam)
{
	if (m_tablet && m_tablet->GetContext() == (HCTX)lParam)
	{
		//m_tablet->Flush();
	}

	return 0;
}

LRESULT Window::WmKeyDown(WPARAM /*wParam*/, LPARAM lParam, int keyCode, wchar_t wchar)
{
	// Handle the pressed key

	Point p = Mouse::GetPosition();

	Control* hit = Control::Focused
		? Control::Focused 
		: HitTest(p);

	if (hit)
	{
		Key key = Key::FromCode(keyCode);
		bool keyWasDown = (lParam & 0x40000000) != 0;

		// Process shortcuts

		if (!Control::s_keyboardOwner // this means that text shortcuts will have to be implemented separately (Ctrl+C, etc.)
			&& hit->ProcessKey(key, keyWasDown))
			return 0;

		// Process the key

		if (keyWasDown && !hit->AllowKeyRepeat) // Ignore repeated messages
			return 0;

		Key::Message msg;
	
		msg.Object   = hit;
		msg.Key      = key;
		msg.Char     = wchar;
		msg.SendChar = true;

		hit->KeyDownEvent.Send(msg);

		/*	The event handler can prevent a char from being sent. msg.SendChar is stored in 
			a temporary variable and consulted by WM_CHAR, which is a hack, but it works. */
		m_sendChar = msg.SendChar;
	}

	return 0;
}

LRESULT Window::WmKeyUp(WPARAM /*wParam*/, LPARAM /*lParam*/, int keyCode, wchar_t wchar)
{
	// Process shortcuts

	Key key = Key::FromCode(keyCode);

	if (KeyCombo::s_pressed && KeyCombo::s_pressed->Contains(key))
	{
		bool isCombo = true;

		for (int i = 0; i < KeyCombo::s_pressed->Count; i++)
		{
			if ((*KeyCombo::s_pressed)[i] != key && !(*KeyCombo::s_pressed)[i].IsDown)
			{
				isCombo = false;
				break;
			}
		}

		if (isCombo)
		{
			::Message msg;
			KeyCombo::s_pressed->ReleaseEvent.Send(msg);
			KeyCombo::s_pressed = null;
			return 0;
		}
	}

	// Handle the released key

	Point p = Mouse::GetPosition();

	Control* _focused = Control::Focused 
		? Control::Focused 
		: HitTest(p);

	if (_focused)
	{
		Key::Message msg;
	
		msg.Object   = _focused;
		msg.Key      = key;
		msg.Char     = wchar;
		msg.SendChar = true;

		Focused->KeyUpEvent.Send(msg);
	}

	return 0;
}

LRESULT Window::WmChar(WPARAM /*wParam*/, LPARAM lParam, wchar_t wchar)
{
	if (!m_sendChar)
		return 0;

	// OnCharMessage messages are only sent to controls that are in focus

	if (Control::Focused)
	{
		if ((lParam & 0x40000000) != 0 && !Control::Focused->AllowKeyRepeat) // Ignore repeated messages
			return 0;

		Key::Message msg(null, wchar, true);
		Control::Focused->CharEvent.Send(msg);
	}

	return 0;
}

LRESULT Window::WmClose(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// If the main window is being closed, quit the application,
	// otherwise just destroy the window.

	::DestroyWindow(m_hwnd);

	if (Window::s_wndMain == this)
		::PostQuitMessage(0);

	return 0;
}

LRESULT Window::WmDestroy(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	//m_hwnd = null;
	return 0;
}

LRESULT Window::WmTimer(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (wParam)
	{
	case IDT_DBLCLK:
		::KillTimer(m_hwnd, IDT_DBLCLK);
		
		m_LDoubleClicking = false;
		m_MDoubleClicking = false;
		m_RDoubleClicking = false;

		break;

	case IDT_ANIMATOR:
		::KillTimer(m_hwnd, IDT_ANIMATOR);

		/*	This is the easiest way I can figure out to do this.
			Because WM_PAINT messages are not sent if the update
			region is outside of the window's client area, when
			an animated control starts outside the client area
			nothing is painted because the WM_PAINT messages are
			not sent.
			
			Using a timer I force the animator to update at least
			once in case the control is outside of the client area. */

		if (Animation::GetAnimator().IsBusy)
			Animation::GetAnimator().Update();

		break;

	//case IDT_TOOLTIP:
	//	{
	//		::KillTimer(hwnd, IDT_TOOLTIP);

	//		Tooltip* tooltip = m_hitTooltip->GetTooltip();

	//		Rect hitRect = m_hitTooltip->PaintRect;

	//		Tooltip::Show(
	//			tooltip,
	//			hitRect.X + (hitRect.Width - tooltip->Width) / 2 /*+ m_hitTooltip->tooltipOffset.x*/,
	//			hitRect.Y + (hitRect.Height - tooltip->Height) / 2 /*+ m_hitTooltip->tooltipOffset.y*/);
	//		
	//		//OutputDebugString(L"Tooltip SHOW\n");

	//		break;
	//	}
	default:
		for (int i = 0; i < m_timers.Count; i++)
		{
			if ((int)wParam == m_timers[i].Id)
			{
				m_timers[i].Proc(m_timers[i].Control);
				break;
			}
		}

		break;
	}

	return 0;
}

LRESULT Window::WmClipboardUpdate()
{
	Clipboard::Update();
	return 0;
}

LRESULT	Window::WmDestroyClipboard()
{
	Clipboard::Clear();
	return 0;
}

//LRESULT Window::WmUserCallFunction(WPARAM wParam, LPARAM lParam)
//{
//	ScheduledFunction proc = (ScheduledFunction)wParam;
//	void* data = (void*)lParam;
//
//	proc(data);
//
//	return 0;
//}