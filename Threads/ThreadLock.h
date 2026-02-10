#ifndef THREAD_LOCK_H
#define THREAD_LOCK_H

#include <windows.h>

class ThreadLock
{
public:
	ThreadLock();
	~ThreadLock();

	void Lock();
	bool TryLock();
	void Unlock();

	void Wait(); // Waits an infinitely long time
	void Wait(double seconds);

private:
	CRITICAL_SECTION m_cs;
};

#endif