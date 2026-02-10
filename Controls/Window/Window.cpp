#include "Window.h"

//#include "Tooltip.h"

#include <Types/Graphics\GdiImage8.h>
#include <Graphics\Graphics.h>

#include <Devices\Tablet\Tablet.h>
#include <System\System.h>
#include <Native\Gdi.h>
#include <Resources\resource.h>

#include <locale.h>
#include <assert.h>
#include <stdio.h>
#include <typeinfo.h>
#include <dwmapi.h>
#include <msctf.h>

DEFINE_TYPE(Window, Control);

#ifdef _DEBUG
bool		Window::paintUpdateRegion = false;
#endif

WNDCLASSEX* Window::s_wndClass		  = null;
Window*		Window::s_wndMain		  = null;
Window*		Window::s_wndOver		  = null;
Window*		Window::s_currentWindow	  = null;

Window::Window(bool isMainWindow)
{
	StateChangeEvent.Sender = this;

	m_mouseDown				= false;

	if (isMainWindow && !Window::s_wndMain)
		Window::s_wndMain = this;

	m_hwnd					= null;
	m_hkl					= null;

#ifdef _DEBUG
	showUpdateRgn = false;
	keysShowUpdateRgn.Add(Key::Oem3); // tilde
	keysShowUpdateRgn.PressEvent += EventHandler(this, &Window::keysShowUpdateRgn_OnPress);
	Keys.Add(&keysShowUpdateRgn);
#endif

	/*	TODO:

		need 1) global window shortcuts
			 2) shortcuts to be activated without focus */

	m_frame					= null;
	m_sendChar				= true;
	m_overClient			= false;
	m_overNonClient			= false;
	m_LDoubleClicking		= false;
	m_MDoubleClicking		= false;
	m_RDoubleClicking		= false;

	//m_tooltipDelay		= 0.5;
	//m_hitTooltip			= null;

	m_shakeMoveCount		= 3;
	m_possibleShake			= false;
	m_nShakeMoves			= 0;
	m_minShakeDistance		= 10; // px
	m_maxShakeDistance		= 80; // px

	m_shakeTimerId			= CreateTimerId();
	m_shakeTimerInterval	= 0.3;

	m_holdTimerId			= CreateTimerId();

	m_holdIgnoreButtonUp	= false;
	
	m_fullScreen			= false;
	m_lockedCapture			= false;

	m_suppressRectChanges	= false;

	MoveEvent				+= EventHandler(this, &Window::OnMove);
	SizeEvent				+= EventHandler(this, &Window::OnSize);
	PaintEvent				+= EventHandler(this, &Window::OnPaint);

	m_forceEdgeOnMouseLeave	= false;

	m_prevMousePos			= Mouse::GetPosition();

	Opacity.ChangeEvent.Clear();

	X.ChangeEvent		+= EventHandler(this, &Window::position_OnChange);
	Y.ChangeEvent		+= EventHandler(this, &Window::position_OnChange);
	Width.ChangeEvent	+= EventHandler(this, &Window::size_OnChange);
	Height.ChangeEvent	+= EventHandler(this, &Window::size_OnChange);

	UpdateSystemSettings();

	// Register the window class

	if (!s_wndClass)
	{
		s_wndClass = (WNDCLASSEX*)malloc(sizeof(WNDCLASSEX));
		
		s_wndClass->cbSize			= sizeof(WNDCLASSEX);
		s_wndClass->style			= null;
		s_wndClass->lpfnWndProc		= WindowProcStatic;
		s_wndClass->cbClsExtra		= 0;
		s_wndClass->cbWndExtra		= 0;
		s_wndClass->hInstance		= HINST_PLAY;
		s_wndClass->hIcon			= ::LoadIcon(HINST_PLAY, MAKEINTRESOURCE(IDI_PLAY));
		s_wndClass->hCursor			= null;
		s_wndClass->hbrBackground	= null;
		s_wndClass->lpszMenuName	= null;
		s_wndClass->lpszClassName	= L"Window";
		s_wndClass->hIconSm			= null;

		::RegisterClassEx(s_wndClass);
	}

	if (s_wndClass)
	{
		m_hwnd = ::CreateWindowEx(
			WS_EX_ACCEPTFILES,
			s_wndClass->lpszClassName,
			null,
			STYLE_SYSTEMSIZABLE, // sizable by default
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(HWND)null,
			(HMENU)null,
			HINST_PLAY,
			(LPVOID)this); // pointer to WndProc
	}

	// Load the keyboard layout

	wchar_t wcLayout[9];
	swprintf_s(wcLayout, sizeof(wcLayout) / sizeof(wchar_t), L"%04X%04X", 0, ::GetUserDefaultLangID());

	m_hkl = ::LoadKeyboardLayout(wcLayout, KLF_ACTIVATE | KLF_REPLACELANG | KLF_SUBSTITUTE_OK);

	// Initialize the tablet if present

	m_tablet = Tablet::IsPresent() 
		? new Tablet(m_hwnd) 
		: null;

	//

	SuspendUpdates();

	BackColor = Color::Back;
	Cursor = Cursor::Dot;

	ResumeUpdates();
}

Window::~Window()
{
	if (m_frame 
		&& m_frame != &WindowFrame::None 
		&& !m_frame->IsSystemFrame)
		delete m_frame;

	if (m_tablet)
		delete m_tablet;

	if (m_hkl)
		::UnloadKeyboardLayout(m_hkl);

	if (m_hwnd)
		::DestroyWindow(m_hwnd);
}

HWND Window::GetHwnd()
{
	return m_hwnd;
}

Window* Window::GetCurrentWindow()
{
	return s_currentWindow;
}

void Window::UpdateSystemSettings()
{
	System::Locale = _create_locale(LC_ALL, "");

	m_doubleClickSize.Width = GetSystemMetrics(SM_CXDOUBLECLK);
	m_doubleClickSize.Height = GetSystemMetrics(SM_CYDOUBLECLK);

	m_doubleClickDuration = (double)::GetDoubleClickTime() / 1000.0;
}

Window* Window::GetWindow()
{
	return this;
}

///	<summary>Returns the window's bounding rectangle.</summary>
///	<returns>Returns a reference to a <see cref="Rect" />.</returns>
///	<remarks>A window's bounding rectangle is given in screen coordinates.</remarks>
///	<seealso cref="GetClientRect" />
///	<seealso cref="GetPaintRect" />
Rect Window::GetRect()
{
	RECT r;
	::GetWindowRect(m_hwnd, &r);

	return _Rect(
		r.left,
		r.top,
		r.right - r.left,
		r.bottom - r.top);
}

void Window::SetRect(int x, int y, int width, int height)
{
	Control::SetRect(x, y, width, height);

	::SetWindowPos(
		m_hwnd,
		NULL,
		X,
		Y,
		Width,
		Height,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

Point Window::GetPosition()
{
	RECT r;
	::GetWindowRect(m_hwnd, &r);

	return Point(r.left, r.top);
}

void Window::SetPosition(int x, int y)
{
	Control::SetPosition(x, y);

	::SetWindowPos(
		m_hwnd, 
		NULL, 
		X, 
		Y, 
		0, 
		0, 
		SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}

Size Window::GetSize()
{
	RECT r;
	::GetWindowRect(m_hwnd, &r);

	return _Size(
		r.right - r.left,
		r.bottom - r.top);
}

void Window::SetSize(int width, int height)
{
	Control::SetSize(width, height);

	::SetWindowPos(
		m_hwnd,
		NULL,
		0,
		0,
		Width,
		Height,
		SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);
}

///	<summary>Returns the window's client rectangle.</summary>
///	<returns>Returns a reference to a <see cref="Rect" />.</returns>
///	<remarks>A window's client rectangle is relative to its bounding rectangle.</remarks>
///	<seealso cref="GetRect" />
///	<seealso cref="GetPaintRect" />
Rect Window::GetClientRect()
{
	RECT cr;
	::GetClientRect(m_hwnd, &cr);

	return _Rect(
		0, 
		0, 
		cr.right - cr.left,
		cr.bottom - cr.top);
}

///	<summary>Returns the window's paint rectangle.</summary>
///	<returns>Returns a reference to a <see cref="Rect" />.</returns>
///	<remarks>A window's paint rectangle is always given in screen coordinates.</remarks>
///	<seealso cref="GetRect" />
///	<seealso cref="GetClientRect" />
Rect Window::GetFullPaintRect()
{
	return GetClientRect();
}


bool Window::GetIsVisible()
{
	//WINDOWPLACEMENT wp;
	//wp.Length = sizeof(WINDOWPLACEMENT);

	//::GetWindowPlacement(m_hwnd, &wp);

	//return wp.showCmd == SW_SHOW || wp.showCmd == SW_SHOWMAXIMIZED;

	return Control::GetVisible();
}

void Window::SetIsVisible(bool visible)
{
	Control::SetVisible(visible);

	//::ShowWindow(m_hwnd, visible ? SW_SHOW : SW_HIDE);

	::SetWindowPos(
		m_hwnd,
		null,
		0, 0, 0, 0,
		SWP_NOSENDCHANGING | SWP_NOMOVE | SWP_NOSIZE | (Control::GetVisible() ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
}

Control* Window::HitTest(const Point& p)
{
	// Floating controls

	for (int i = m_floating.Count - 1; i >= 0; i--)
	{
		Control* hit = m_floating[i]->HitTest(p);
		if (hit) return hit;
	}

	// Controls

	Control* hit = Control::HitTest(p);
	return hit ? hit : this;
}

void Window::Update(Region& rgn)
{
	Lock();

	if (UpdatesSuspended)
	{
		Unlock();
		return;
	}

	if (m_markedForLayout)
		UpdateLayout(false);

	m_rgnUpdate.Add(rgn 
		? rgn 
		: ClientRect);

	HRGN updateRgn = Gdi::ToHRGN(m_rgnUpdate);
	::InvalidateRgn(m_hwnd, updateRgn, false);
	::DeleteObject(updateRgn);

	m_rgnUpdate.Clear();

	/*	Set a timer to force the animator to update 
		in case no WM_PAINT messages are posted. 
		
		100 msec seems like a decent compromise that
		neither taxes the processor nor is too slow
		for the first update. */

	::SetTimer(m_hwnd, IDT_ANIMATOR, 100, null);

	Unlock();
}

void Window::Paint(Region& rgn)
{
	Graphics g(Buffer);
	g.ClipRegion = rgn;

	Lock();

	Graphics::Message msg(g, rgn);
	PaintEvent.Send(msg);

	Unlock();

	/*	Paint the child controls. */

	for (int i = 0; i < Controls.Count; i++)
		Controls[i]->Paint(rgn);
	
	/*	Paint the menus. */

	for (int i = 0; i < m_floating.Count; i++)
		m_floating[i]->Paint(rgn);

	// Paint the dragged controls

	//for (int i = 0; i < dragged.Count; i++)
	//	dragged[i]->Paint(rgn);

	// Paint the tooltip

	//if (m_tooltip)
	//	m_tooltip->Paint(rgn);
}

void Window::Show(WindowState state)
{
	switch (state)
	{
	case Minimized:
		Minimize();
		break;

	case Maximized:
		Maximize();
		break;

	case Normal:
	default:
		Restore();
		break;
	}
}

void Window::Maximize()
{
	::ShowWindow(m_hwnd, SW_MAXIMIZE);
	::UpdateWindow(m_hwnd);

	Control::Message msg(this);
	StateChangeEvent.Send(msg);
}

void Window::Restore()
{
	::ShowWindow(m_hwnd, SW_RESTORE);
	::UpdateWindow(m_hwnd);

	Control::Message msg(this);
	StateChangeEvent.Send(msg);
}

void Window::Minimize()
{
	::ShowWindow(m_hwnd, SW_MINIMIZE);

	Control::Message msg(this);
	StateChangeEvent.Send(msg);
}

bool Window::SwitchToFullScreen()
{
	if (m_fullScreen)
		return false;

	m_wasMaximized = ::IsZoomed(m_hwnd) != FALSE;
	m_rectBeforeFullScreen = GetRect();

	m_styleBeforeFullScreen = ::GetWindowLong(m_hwnd, GWL_STYLE);
	::SetWindowLong(m_hwnd, GWL_STYLE, STYLE_NONE);
	
	HMONITOR hMonitor = ::MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);

	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	
	::GetMonitorInfo(hMonitor, &mi);

	::SetWindowPos(
		m_hwnd, 
		HWND_NOTOPMOST, 
		mi.rcMonitor.left, 
		mi.rcMonitor.top, 
		mi.rcMonitor.right - mi.rcMonitor.left, 
		mi.rcMonitor.bottom - mi.rcMonitor.top, 
		SWP_SHOWWINDOW);

	m_fullScreen = true;

	return true;
}

bool Window::SwitchToWindow()
{
	if (!m_fullScreen)
		return false;

	::SetWindowLong(m_hwnd, GWL_STYLE, m_styleBeforeFullScreen /*| (m_windowWasMaximized ? WS_MAXIMIZE : 0)*/);
	
	::SetWindowPos(
		m_hwnd, 
		HWND_NOTOPMOST, 
		m_rectBeforeFullScreen.X, 
		m_rectBeforeFullScreen.Y, 
		m_rectBeforeFullScreen.Width, 
		m_rectBeforeFullScreen.Height, 
		SWP_SHOWWINDOW);

	m_fullScreen = false;

	return true;
}

void Window::Close()
{
	/*	NOTE: After the window is closed by calling this function
	it basically stops being usable. */

	::PostMessage(m_hwnd, WM_CLOSE, null, null);
}

Window::WindowState Window::GetState()
{
	if (IsZoomed(m_hwnd) != 0)
		return Window::Maximized;
	else if (IsIconic(m_hwnd) != 0)
		return Window::Minimized;
	else
		return Window::Normal;
}

void Window::LockCapture()
{
	if (!m_lockedCapture)
	{
		::SetCapture(m_hwnd);
		m_lockedCapture = true;
	}
}

void Window::UnlockCapture()
{
	if (m_lockedCapture)
	{
		::ReleaseCapture();
		m_lockedCapture = false;
	}
}

bool Window::GetCaptureIsLocked()
{
	return m_lockedCapture;
}

bool Window::CheckMouseEnter(const Point& _p, const Point& _pPrev, int _data)
{
	// Start tracking the MouseLeave event for the window

	TRACKMOUSEEVENT tme;

	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE | (_data < 1 ? TME_NONCLIENT : 0);
	tme.hwndTrack = m_hwnd;
	tme.dwHoverTime = HOVER_DEFAULT;

	::TrackMouseEvent(&tme);

	if (Window::s_wndOver != this)
	{
		Point p = _p;
		PointScreenToClient(&p);

		Point pPrev = _pPrev;
		PointScreenToClient(&pPrev);

		Window::s_wndOver = this;
		//Control::s_over = this;

		Mouse::Message msg(p, pPrev);
		MouseEnterEvent.Send(msg);

		return true;
	}

	return false;
}

bool Window::CheckMouseLeave(const Point& _p, const Point& _pPrev)
{
	if (this == Window::s_wndOver && !m_overClient && !m_overNonClient)
	{
		Point p = _p;
		PointScreenToClient(&p);

		Point pPrev = _pPrev;
		PointScreenToClient(&pPrev);

		// Force the mouse to pass an 
		// edge pixel of the window.
		
		if (m_forceEdgeOnMouseLeave)
		{
			RECT cr;
			::GetClientRect(m_hwnd, &cr);

			POINT __p = { 0, 0 };
			::ClientToScreen(m_hwnd, &__p);

			cr.left   += __p.x;
			cr.top    += __p.y;
			cr.right  += __p.x;
			cr.bottom += __p.y;

			if (!(m_prevMousePos.X == cr.left 
				|| m_prevMousePos.X == cr.right - 1 
				|| m_prevMousePos.Y == cr.top 
				|| m_prevMousePos.Y == cr.bottom - 1))
			{
				Point ep = _p;

				if (_p.X < cr.left && _p.Y < cr.top) // top left
				{
					ep.X = cr.left;
					ep.Y = cr.top;
				}
				else if (_p.X < cr.left && _p.Y >= 0 && _p.Y <= cr.bottom - 1) // left
				{
					ep.X = cr.left;
					ep.Y = m_prevMousePos.Y + (int)((double)(_p.Y - m_prevMousePos.Y) * (double)(cr.left - m_prevMousePos.X) / (double)(_p.X - m_prevMousePos.X));
				}
				else if (_p.X < cr.left && _p.Y >= cr.bottom) // bottom left
				{
					ep.X = cr.left;
					ep.Y = cr.bottom - 1;
				}
				else if (_p.X >= 0 && _p.X <= cr.right - 1 && _p.Y >= cr.bottom) // bottom
				{
					ep.X = m_prevMousePos.X + (int)((double)(_p.X - m_prevMousePos.X) * (double)(cr.bottom - 1 - m_prevMousePos.Y) / (double)(_p.Y - m_prevMousePos.Y));
					ep.Y = cr.bottom - 1;
				}
				else if (_p.Y >= cr.right && _p.Y >= cr.bottom) // bottom right
				{
					ep.X = cr.right - 1;
					ep.Y = cr.bottom - 1;
				}
				else if (_p.X >= cr.right && _p.Y >= 0 && _p.Y <= cr.bottom - 1) // right
				{
					ep.X = cr.right - 1;
					ep.Y = m_prevMousePos.Y + (int)((double)(_p.Y - m_prevMousePos.Y) * (double)(cr.right - 1 - m_prevMousePos.X) / (double)(_p.X - m_prevMousePos.X));
				}
				else if (_p.X >= cr.right && _p.Y < cr.top) // top right
				{
					ep.X = cr.right - 1;
					ep.Y = cr.top;
				}
				else if (_p.X >= 0 && _p.X <= cr.right - 1 && _p.Y < cr.top) // top
				{
					ep.X = m_prevMousePos.X + (int)((double)(_p.X - m_prevMousePos.X) * (double)(cr.top - m_prevMousePos.Y) / (double)(_p.Y - m_prevMousePos.Y));
					ep.Y = cr.top;
				}

				//WmMouseMove(ep);
			}
		}

		Window::s_wndOver = null;

		Mouse::Message msg(_p, _pPrev);
		MouseLeaveEvent.Send(msg);

		return true;
	}

	return false;
}

String Window::GetTitle()
{
	return m_title;
}

void Window::SetTitle(const String& _title)
{
	m_title = _title;
	::SetWindowText(m_hwnd, m_title);
}

WindowFrame* Window::GetFrame()
{
	return m_frame;
}

void Window::SetFrame(WindowFrame* frame)
{
	if (frame == &WindowFrame::SystemSizable)
	{
		::SetWindowLongPtr(m_hwnd, GWL_STYLE, STYLE_SYSTEMSIZABLE);
		::SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0, _SWP_UPDATE);
	}
	else if (frame == &WindowFrame::SystemFixed)
	{
		::SetWindowLongPtr(m_hwnd, GWL_STYLE, STYLE_SYSTEMSIZABLE);
		::SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0, _SWP_UPDATE);
	}
	else // if (m_frame == &WindowFrame::None) // This handles all other frames, including the absense of one
	{
		::SetWindowLongPtr(m_hwnd, GWL_STYLE, STYLE_NONE);
		::SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0, _SWP_UPDATE);
	}

	m_frame = frame;
}

bool Window::GetShowInTaskbar()
{
	LONG_PTR exStyle = ::GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);
	return (exStyle & WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW;
}

void Window::SetShowInTaskbar(bool showInTaskbar)
{
	LONG_PTR exStyle = ::GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);

	if (showInTaskbar)
		exStyle &= ~WS_EX_TOOLWINDOW;
	else
		exStyle |= WS_EX_TOOLWINDOW;

	::SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, exStyle);
	::SetWindowPos(m_hwnd, null, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

GdiImage8* Window::GetBuffer()
{
	return &m_buffer;
}

int Window::CreateTimerId()
{
	static int baseTimerId = 200;
	return baseTimerId++;
}

void Window::StartTimer(int timerId, double interval, Control* control, void (*timerProc)(Control* control))
{
	/*	First check if the timer is already running. */

	bool alreadyRunning = false;

	for (int i = 0; i < m_timers.Count; i++)
	{
		if (m_timers[i].Id == timerId)
		{
			alreadyRunning = true;
			break;
		}
	}
	
	/*	If the timer is already running, stop it first.
		The net effect will be that the timer is restarted. */

	if (alreadyRunning)
	{
		::KillTimer(
			m_hwnd, 
			timerId);
	}

	/*	Otherwise the timer is new. */

	else
	{
		Window::Timer timer;
		timer.Id = timerId;
		timer.Control = control;
		timer.Proc = timerProc;
		m_timers.Add(timer);
	}

	::SetTimer(
		m_hwnd, 
		timerId, 
		(UINT)Math::Round(interval * 1000.0),
		null);
}

void Window::StopTimer(int timerId)
{
	for (int i = 0; i < m_timers.Count; i++)
	{
		if (m_timers[i].Id == timerId)
		{
			::KillTimer(
				m_hwnd, 
				m_timers[i].Id);

			m_timers.RemoveAt(i);
			return;
		}
	}
}

bool Window::IsTimerRunning(int timerId)
{
	for (int i = 0; i < m_timers.Count; i++)
	{
		if (m_timers[i].Id == timerId)
			return true;
	}

	return false;
}

//void Window::SetTooltip(Tooltip* tooltip)
//{
//	if (m_tooltip)
//		m_tooltip->SetParent(null);
//
//	m_tooltip = tooltip;
//	
//	if (m_tooltip)
//		m_tooltip->SetParent(this);
//}

bool Window::TestShake(const Point& _p)
{
	FPoint p(
		(double)_p.X, 
		(double)_p.Y);

	if (m_shakeTrail.Count > 0)
	{
		double filter = _1_PHI;
		p = m_shakeTrail[m_shakeTrail.Count - 1] * filter + p * (1.0 - filter);
	}

	m_shakeTrail.Add(p);

	if (m_shakeTrail.Count == 1)
	{
		m_shakeMoveStart = p;
		m_possibleShake = true;
		
		StartTimer(
			m_shakeTimerId, 
			m_shakeTimerInterval, 
			this,
			&shakeTimer_OnTick);
	}
	else if (m_shakeTrail.Count > 2) // 3+ points needed to compare 2 angles
	{
		FPoint pp = m_shakeTrail[m_shakeTrail.Count - 2];

		double diffAngle = Math::GetAngleDifference(
			Math::GetAngle(m_shakeMoveStart, pp),
			Math::GetAngle(pp, p));

		if (fabs(diffAngle) > PI / 1.5)
		{
			if (m_possibleShake)
			{
				double dist = Math::GetDistance(m_shakeMoveStart, pp);

				if (dist >= (double)m_minShakeDistance
					&& dist <= (double)m_maxShakeDistance)
				{
					m_nShakeMoves++;
					m_shakeMoveStart = pp;
					
					StartTimer(
						m_shakeTimerId, 
						m_shakeTimerInterval, 
						this,
						&shakeTimer_OnTick);
				}

				if (m_nShakeMoves >= m_shakeMoveCount)
				{
					StopTimer(m_shakeTimerId);
					m_possibleShake = false;
					ResetShake();
					return true;
				}
			}
			else
			{
				ResetShake();
			}
		}
	}

	return false;
}

void Window::ResetShake()
{
	m_nShakeMoves = 0;
	m_shakeMoveStart = m_shakeTrail[max(0, m_shakeTrail.Count - 1)];
	m_shakeTrail.Clear();
}

void Window::ShowFloating(Control* control)
{
	for (int i = 0; i < m_floating.Count; i++)
	{
		if (m_floating[i] == control)
			return;
	}

	Animation::Suspend();

	/*	Hide all current floating controls. */

	for (int i = 0; i < m_floating.Count; i++)
	{
		FloatAnimation* anim = new FloatAnimation();
			anim->Property = m_floating[i]->Opacity;
			anim->To       = 0.0;
			anim->Speed    = 10.0;
			anim->EndEvent += EventHandler(this, &Window::floating_OnEndAnimation);

		Animation::Add(anim);
	}

	/*	Show the new floating control. */

	for (int i = 0; i < m_floating.Count; i++)
	{
		if (m_floating[i]->HasChild(control))
			return;
	}

	if (control/* && !m_floating.Contains(control)*/)
	{
		SuspendUpdates();

		control->Parent = this;
		control->Opacity  = 0.0;

		m_floating.Add(control);

		ResumeUpdates();

		/*	Try to fit the control inside the window. */

		//if (x < 0)
		//	x = 0;

		//if (x + control->Width > ClientRect.Width)
		//	x = ClientRect.Width - control->Width;

		//control->SetPosition(x, y);
		//control->data[1] = (void*)1; // 0x01 marks it as a floating menu (instead of a permanent panel)
		//control->data[2] = OnHideMessage;

		FloatAnimation* anim = new FloatAnimation();
			anim->Property = control->Opacity;
			anim->To       = 1.0;
			anim->Speed    = 10.0;

		Animation::Add(anim);
	}

	Animation::Resume();
}

void Window::HideFloating(Control* control)
{
	if (!m_floating.Contains(control))
		return;

	FloatAnimation* anim = new FloatAnimation();
		anim->Property = control->Opacity;
		anim->To       = 0.0;
		anim->Speed    = 10.0;
		anim->EndEvent += EventHandler(this, &Window::floating_OnEndAnimation);

	Animation::Suspend();
	Animation::Add(anim);
	Animation::Resume();
}

//void Window::ScheduleFunction(ScheduledProc proc, void* data)
//{
//	::PostMessage(
//		m_hwnd, 
//		WM_USER_CALLFUNCTION,
//		(WPARAM)proc,
//		(LPARAM)data);
//}