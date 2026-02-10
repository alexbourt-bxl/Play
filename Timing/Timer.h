#ifndef TIMER_H
#define TIMER_H

#include <Types/Object.h>
#include <Threads/Thread.h>
#include <Events/Event.h>

class Timer 
	: public Object, 
	  public Thread, public EventSender
{
	DECLARE_TYPE;

public:
	class Message;

public:
	Timer();

	Object*	Copy();

	__declspec(property(get = GetInterval, put = SetInterval))
	double Interval;
	double GetInterval();
	void  SetInterval(double interval);

	Event<Message&> TickEvent;

private:
	double	m_interval;

	uint	ThreadProc();
};

class Timer::Message 
	: public ::Message
{
	typedef Timer _Timer;

public:
	_Timer* Timer;

	Message(_Timer* timer)
		: Timer(timer) {}
};

#endif