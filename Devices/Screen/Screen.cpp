#include "Screen.h"

#include <Controls\Window\Window.h>

#include <windows.h>

Size Screen::GetSize()
{
	HMONITOR hMonitor;
	MONITORINFO mi;

	hMonitor = ::MonitorFromWindow(
		Window::GetCurrentWindow()->Hwnd,
		MONITOR_DEFAULTTONEAREST);

	mi.cbSize = sizeof(MONITORINFO);
	
	::GetMonitorInfo(hMonitor, &mi);

	return Size(
		mi.rcMonitor.right - mi.rcMonitor.left,
		mi.rcMonitor.bottom - mi.rcMonitor.top);
}

Rect Screen::GetRect()
{
	HMONITOR hMonitor;
	MONITORINFO mi;

	hMonitor = ::MonitorFromWindow(
		Window::GetCurrentWindow()->Hwnd,
		MONITOR_DEFAULTTONEAREST);

	mi.cbSize = sizeof(MONITORINFO);
	
	::GetMonitorInfo(hMonitor, &mi);

	return Rect(
		mi.rcMonitor.left,
		mi.rcMonitor.top,
		mi.rcMonitor.right - mi.rcMonitor.left,
		mi.rcMonitor.bottom - mi.rcMonitor.top);
}

Point Screen::GetMonitorOffsetFromPoint(const Point& p)
{
	HMONITOR hMonitor;
	MONITORINFO mi;

	POINT _p = { p.X, p.Y };

	hMonitor = ::MonitorFromPoint(
		_p,
		MONITOR_DEFAULTTONEAREST);

	mi.cbSize = sizeof(MONITORINFO);
	
	::GetMonitorInfo(hMonitor, &mi);

	return Point(
		mi.rcMonitor.left, 
		mi.rcMonitor.top);
}
