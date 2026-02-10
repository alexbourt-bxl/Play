#ifndef WINDOW_H
#define WINDOW_H

/*	Window class
	------------

	Creation

		A derived window must call either Update() or ResumeUpdates(true) (if updates were
		previously suspended) as the last thing in the constructor to properly initialize the 
		window for the first time.

	Rectangles

		Windows differ from normal controls in their treatment of rectangles in that a window's
		control rectangle holds the dimensions of the window relative to the screen, but its
		client and paint rectangles relative to the top left corner is always (0, 0).

		A window's paint rectangle is always the same as the window's native client area, but
		its client rectangle can be smaller.

	Hierarchy

		A window cannot have a parent, and is always the topmost parent in a functional control 
		hierarchy.

	Timers

		A window can provide child controls with timers, so that all the controls run in the
		same thread. This removes the conflicts between different threads and the necessity to
		prevent them.
*/

#include <Native\_Hinstance.h>

#include <Controls\Control.h>
#include <Controls\Window\WindowFrame.h>
#include <Animation\Animation.h>

//#include "Menu.h"
//#include "Textbox.h"
//#include "Grip.h"
#include <Types/Graphics\GdiImage.h>
#include <Types/Graphics\GdiImage8.h>
#include <Devices\Keyboard\KeyCombo.h>

#define STYLE_SYSTEMSIZABLE	WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
#define STYLE_SYSTEMFIXED	WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX
#define STYLE_NONE			WS_POPUP

#define _SWP_UPDATE			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOSENDCHANGING

#define IDT_DBLCLK			100
#define IDT_TOOLTIP			101
#define IDT_ANIMATOR		102

//#define WM_USER_CALLFUNCTION WM_APP + 1
//typedef (*ScheduledProc)(void* data);

//class Tooltip;
class Tablet;

class Window 
	: public Control
{
	DECLARE_TYPE;

	//friend class Menu;
	friend class Control;
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

public:
	typedef int WindowState;

	enum
	{
		Normal,
		Minimized,
		Maximized
	};

	struct Timer
	{
	private:
		typedef Control _Control;
	
	public:
		int Id;
		_Control* Control;
		void (*Proc)(_Control* control);
	};

#ifdef _DEBUG
	bool		showUpdateRgn;
	KeyCombo	keysShowUpdateRgn;
	void		keysShowUpdateRgn_OnPress(::Message& msg);
#endif

public:
	Window(bool isMainWindow = true);
	~Window();

	__declspec(property(get = GetHwnd))
	HWND Hwnd;
	HWND GetHwnd();

	static Window* GetCurrentWindow();

	void	UpdateSystemSettings();

	//
	// Control base methods
	// 
	Window* GetWindow();

	_Rect	GetRect();
	void	SetRect(int x, int y, int width, int height);

	Point	GetPosition();
	void	SetPosition(int x, int y);

	_Size	GetSize();
	void	SetSize(int width, int height);

	_Rect	GetClientRect();
	_Rect	GetFullPaintRect();

	bool	GetIsVisible();
	void	SetIsVisible(bool visible);

	Control* HitTest(const Point& p);

	void	Update(Region& rgn = Region::Null);
	void	Paint(Region& rgn);

	//
	// Handling
	//

	void	Show(WindowState state = Normal);

	void	Maximize();
	void	Restore();
	void	Minimize();

	bool	SwitchToFullScreen();
	bool	SwitchToWindow();

	void	Close();

	__declspec(property(get = GetState))
	WindowState State;
	WindowState	GetState();

	//
	// Control
	//

	void	LockCapture();
	void	UnlockCapture();

	__declspec(property(get = GetCaptureIsLocked))
	bool	CaptureIsLocked;
	bool	GetCaptureIsLocked();

protected:
	bool	CheckMouseEnter(const Point& p, const Point& pPrev, int mouseEnterdata = null);
	bool	CheckMouseLeave(const Point& p, const Point& pPrev);

	//
	// Appearance
	//

public:
	__declspec(property(get = GetTitle, put = SetTitle))
	String	Title;
	String	GetTitle();
	void	SetTitle(const String& title);

	__declspec(property(get = GetFrame, put = SetFrame))
	WindowFrame* Frame;
	WindowFrame* GetFrame();
	void	SetFrame(WindowFrame* frame);

	__declspec(property(get = GetShowInTaskbar, put = SetShowInTaskbar))
	bool	ShowInTaskbar;
	bool	GetShowInTaskbar();
	void	SetShowInTaskbar(bool showInTaskbar);

	__declspec(property(get = GetBuffer))
	GdiImage8* Buffer;
	GdiImage8* GetBuffer();

	//
	// Timers
	//

	static int CreateTimerId();

	void	StartTimer(int timerId, double interval, Control* control, void (*timerProc)(Control* control));
	void	StopTimer(int timerId);
	bool	IsTimerRunning(int timerId);

	//void SetTooltip(Tooltip* tooltip);

	//
	// Shake gesture
	//

	bool	TestShake(const Point& p);
	void	ResetShake();

	//
	// Floating controls
	//

	void	ShowFloating(Control* control);
	void	HideFloating(Control* control);

	//
	// User functions
	//

	//void ScheduleFunction(ScheduledProc proc, void* data);

	//
	// Window proc
	//

public:
	static 
	LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	virtual LRESULT	WmWindowPosChanging(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WmWindowPosChanged(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WmNcActivate(WPARAM wParam, LPARAM lParam);
	virtual	LRESULT	WmActivate(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WmMoving(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmMove(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmSizing(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmSize(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmSettingChange(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmInputLangChangeRequest(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmEraseBkgnd(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmPaint(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmNcHitTest(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmNcMouseMove(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmMouseMove(const Point& p, bool fromTablet = false);
	virtual LRESULT	WmNcMouseLeave(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT	WmMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WmLButtonDown(const Point& p);		
	virtual LRESULT	WmLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WmMouseWheel(const Point& p, double delta);
	virtual LRESULT WmMouseHWheel(const Point& p, double delta);
	virtual LRESULT	WtPacket(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WtProximity(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WtInfoChange(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WtContextUpdate(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WtCursorChange(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WmKeyDown(WPARAM wParam, LPARAM lParam, int keyCode, wchar_t wchar);		
	virtual LRESULT	WmKeyUp(WPARAM wParam, LPARAM lParam, int keyCode, wchar_t wchar);		
	virtual LRESULT	WmChar(WPARAM wParam, LPARAM lParam, wchar_t wchar);		
	virtual LRESULT	WmClose(WPARAM wParam, LPARAM lParam);		
	virtual LRESULT WmDestroy(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WmTimer(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WmClipboardUpdate();
	virtual LRESULT	WmDestroyClipboard();
	//virtual LRESULT WmUserCallFunction(WPARAM wParam, LPARAM lParam);

private:
	String		m_title;

	HWND		m_hwnd;
	HKL			m_hkl; // The keyboard layout for this window

	GdiImage8	m_buffer; // This is a pointer to the buffer in which the control is to be drawn.

	bool		m_mouseDown;
	bool		m_lockedCapture;
	
	/*	When the moust pointer leaves the control, it often skips 
		past the edge. This option forces the window to calculate 
		the theoretical crossing point with the edge and force the 
		mouse to acknowledge it. */
	bool m_forceEdgeOnMouseLeave;
	
	Array<Window::Timer> m_timers;

	bool		m_fullScreen;
	bool		m_wasMaximized;
	_Rect		m_rectBeforeFullScreen;
	LONG_PTR	m_styleBeforeFullScreen;

	WindowFrame* m_frame;

	bool		m_sendChar;
	int			m_tempPressed;

	Control*	m_currentHit;
	Control*	m_mouseHit;

	bool		m_overClient;
	bool		m_overNonClient;
	
	Region		m_rgnUpdate;

	Tablet*		m_tablet;

	bool		m_LDoubleClicking;
	bool		m_MDoubleClicking;
	bool		m_RDoubleClicking;

	Point		m_pDoubleClick;
	_Size		m_doubleClickSize;
	double		m_doubleClickDuration;

	Point		m_prevMousePos;

	//double	m_tooltipDelay;
	//Control*	m_hitTooltip;

	int			m_shakeMoveCount;
	int			m_minShakeDistance;
	int			m_maxShakeDistance;

	Array<FPoint> m_shakeTrail;
	FPoint		m_shakeMoveStart;
	int			m_nShakeMoves;
	bool		m_possibleShake;

	int			m_shakeTimerId;
	double		m_shakeTimerInterval;
	static void	shakeTimer_OnTick(Control* control);

	int			m_holdTimerId;
	static void	holdTimer_OnTick(Control* control);
	bool		m_holdIgnoreButtonUp;
	Point		m_pHoldStart;
	Mouse::Message m_holdArgs;

	Array<Control*> m_floating;
	void		floating_OnEndAnimation(Animation::Message& msg);
		
	//Array<Control*> m_dragged;

	bool		m_suppressRectChanges; // internal flag for user window resizing and moving

private:
	static WNDCLASSEX* s_wndClass;
	static Window* s_wndMain;
	static Window* s_wndOver;

	static Window* s_currentWindow;

#ifdef _DEBUG
	static bool paintUpdateRegion;
#endif

	void OnPaint(Graphics::Message& msg);
	void OnMove(Control::Message& msg);
	void OnSize(Control::Message& msg);

	void position_OnChange(Property::Message& msg);
	void size_OnChange(Property::Message& msg);

	Event<Control::Message&> StateChangeEvent;
};

/*	TODO:

	the events thing is getting out of hand, must be simplified a lot,
	also issues with event handlers trying to affect the size. */

#endif