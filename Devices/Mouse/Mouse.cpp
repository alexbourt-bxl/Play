#include "Mouse.h"

#include <windows.h>

Point Mouse::GetPosition()
{
	//HDESK hdesk = ::OpenInputDesktop(0, false, READ_CONTROL);

	//if (hdesk)
	//	::SetThreadDesktop(hdesk);

	POINT p;
	::GetCursorPos(&p);

	return Point(p.x, p.y);
}

void Mouse::SetPosition(const Point& p)
{
	::SetCursorPos(p.X, p.Y);
}

void Mouse::SetCursor(Cursor& cursor)
{
	::SetCursor(cursor.ToHCURSOR());
}

void Mouse::ShowCursor()
{
	while (::ShowCursor(true) < 0);
}

void Mouse::HideCursor()
{
	while (::ShowCursor(false) >= 0);
}