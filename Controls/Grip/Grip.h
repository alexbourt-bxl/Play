#ifndef GRIP_H
#define GRIP_H

#include <Controls\Control.h>

class Grip 
	: public Control
{
	DECLARE_TYPE;

public:
	enum Result;

public:
	Grip();
	Grip(Result result);

	__declspec(property(get = GetResult, put = SetResult))
	Result	Result_;
	Result	GetResult();
	void	SetResult(Result result);

private:

	Result	m_result;

protected:	// Messages
	
	void	OnPaint(Graphics::Message& msg);
	//void	OnLButtonDown(void* source, Mouse::Message& msg);
};

//

enum Grip::Result
{
	None,

	Client,

	Caption,
	Icon,
	Minimize,
	Maximize,
	Close,

	Left,
	Right,
	Top,
	Bottom,

	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};

#endif