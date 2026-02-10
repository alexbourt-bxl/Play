#ifndef SCREEN_H
#define SCREEN_H

// TODO: create Monitor class so that individual monitors can be dealt with
// and use Monitor objects here.

#include <Types/Geometric\Size.h>
#include <Types/Geometric\Rect.h>
#include <Types/Geometric\Point.h>

class Screen
{
public:
	static Size	GetSize();
	static Rect	GetRect();
	static Point GetMonitorOffsetFromPoint(const Point& p);
};

#endif