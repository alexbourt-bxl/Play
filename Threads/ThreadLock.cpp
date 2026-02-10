#include "ThreadLock.h"

#include <stdio.h>
#include <limits>

ThreadLock::ThreadLock()
{
	::InitializeCriticalSection(&m_cs);
}

ThreadLock::~ThreadLock()
{
	::DeleteCriticalSection(&m_cs);
}

void ThreadLock::Lock()
{
	::EnterCriticalSection(&m_cs);
}

bool ThreadLock::TryLock()
{
	return ::TryEnterCriticalSection(&m_cs) != FALSE;
}

void ThreadLock::Unlock()
{
	::LeaveCriticalSection(&m_cs);
}

void ThreadLock::Wait()
{
	Wait(std::numeric_limits<double>::infinity());
}

void ThreadLock::Wait(double seconds)
{
	DWORD waitTime = 
		seconds == std::numeric_limits<double>::infinity()
		? INFINITE
		: (DWORD)(seconds * 1000);

	::WaitForSingleObject(&m_cs, waitTime);
}