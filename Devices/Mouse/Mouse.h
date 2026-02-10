#ifndef MOUSE_H
#define MOUSE_H

#include <Types/Geometric\Point.h>
#include <Devices\Mouse\Cursor.h>
#include <Events/Message.h>

class Mouse
{
public: // Types
	enum
	{
		LeftButton	 = 0x01,
		RightButton	 = 0x02,
		MiddleButton = 0x04
	};

	class Message;

public:
	static Point GetPosition();
	static void	 SetPosition(const Point& p);
	
	static void	 SetCursor(Cursor& cursor);
	
	static void	 ShowCursor();
	static void	 HideCursor();
};

class Mouse::Message
	: public ::Message
{
public:
	Point Position;
	Point PrevPosition;
	double WheelDelta;
	bool  IsPen;
	uint  CursorId;

	Message() 
		: WheelDelta(0.0), IsPen(false), CursorId(0) {}

	Message(const Message& msg)
		: Position(msg.Position), PrevPosition(msg.PrevPosition), WheelDelta(msg.WheelDelta), IsPen(msg.IsPen), CursorId(msg.CursorId) {}

	Message(const Point& p, const Point& pPrev) 
		: Position(p), PrevPosition(pPrev), WheelDelta(0.0), IsPen(false), CursorId(0) {}
};

#endif