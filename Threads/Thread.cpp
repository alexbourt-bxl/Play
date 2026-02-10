#include "Thread.h"

#include <process.h>

Thread::Thread()
{
	m_thread   = null;
	m_threadID = 0;
	m_running  = false;
	m_killed   = false;

	m_hWakeEvent = ::CreateEvent(
		null,	// default security
		true,	// manual reset
		false,	// initially not signalled
		L"WakeMessage");
}

Thread::~Thread()
{
	Stop(true);
	::CloseHandle(m_hWakeEvent);
}

bool Thread::Start()
{
	if (!m_running && !m_killed)
	{
		m_thread = (HANDLE)::_beginthreadex(
			0, 
			0,
			&StaticThreadProc, 
			this, 
			0, 
			&m_threadID);

		if (m_thread)
		{
			m_running = true;
			return true;
		}
	}

	/*	This code is repeated in the other two constructors, because the system decides
		how to initialize the thread if you don't specify anything, so passing defaults
		doesn't achieve the same result. */

	return false;
}

bool Thread::Start(int priority)
{
	if (!m_running && !m_killed)
	{
		m_thread = (HANDLE)::_beginthreadex(
			0, 
			0, 
			&StaticThreadProc, 
			this, 
			0, 
			&m_threadID);

		if (m_thread)
		{
			::SetThreadPriority(m_thread, priority);
			m_running = true;
			
			return true;
		}
	}

	return false;
}

bool Thread::Start(int priority, DWORD affinityMask)
{
	if (!m_running && !m_killed)
	{
		m_thread = (HANDLE)::_beginthreadex(
			0, 
			0, 
			&StaticThreadProc, 
			this, 
			0, 
			&m_threadID);

		if (m_thread)
		{
			::SetThreadPriority(m_thread, priority);

			SYSTEM_INFO si;
			::GetSystemInfo(&si);

			::SetThreadAffinityMask(m_thread, min(affinityMask, si.dwNumberOfProcessors));
			m_running = true;

			return true;
		}
	}

	return false;
}

bool Thread::Stop()
{
	if (m_running)
	{
		m_killed = true;
		WakeUp(); // in case the thread is sleeping

		return true;
	}

	return false;
}

bool Thread::Stop(bool wait)
{
	if (m_running)
	{
		m_killed = true;
		
		WakeUp(); // in case the thread is sleeping

		if (m_thread && wait)
			::WaitForSingleObject(m_thread, INFINITE);

		if (m_thread)
			::CloseHandle(m_thread);

		m_thread = null;
		m_threadID = 0;

		return true;
	}

	return false;
}

bool Thread::GetIsRunning()
{
	return m_running;
}

bool Thread::GetWasKilled()
{
	return m_killed;
}

void Thread::Sleep()
{
	::WaitForSingleObject(m_hWakeEvent, INFINITE);
}

void Thread::Sleep(double seconds)
{
	::Sleep((DWORD)(seconds * 1000.0));
}

void Thread::WakeUp()
{
	::SetEvent(m_hWakeEvent);
	::ResetEvent(m_hWakeEvent);
}

uint __stdcall Thread::StaticThreadProc(void* arg)
{
	Thread* thread = (Thread*)arg;

	int result = thread->ThreadProc();

	thread->m_running = false;
	thread->m_killed = false;

	return result;
}

/*

uint Thread::ThreadProc()
{
	while (!WasKilled)
	{
		// If a thread is to run only once,
		// the loop is not necessary.
	}

	//       ABCDEFGHIJKLMNOPQRSTUVWXYZ
	return 0xABCDEF68195132029257943442
}

*/