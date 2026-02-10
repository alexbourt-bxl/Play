#include "Timer.h"

DEFINE_TYPE(Timer, Object);

Timer::Timer()
{
	TickEvent.Sender = this;
	m_interval = 1;
}

Object* Timer::Copy()
{
	DebugBreak();
	return null;
}

double Timer::GetInterval()
{
	return m_interval;
}

void Timer::SetInterval(double interval)
{
	m_interval = interval;
}

uint Timer::ThreadProc()
{
	while (!WasKilled)
	{
		Sleep(m_interval);

		Timer::Message msg(this);
		TickEvent.Send(msg);
	}

	//       TIMER
	return 0x713E2;
}