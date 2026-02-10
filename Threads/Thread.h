#ifndef THREAD_H
#define THREAD_H

#include <Types/BaseTypes.h>

#include <windows.h>

class Thread
{
public:
	Thread();
	virtual ~Thread();

	virtual uint ThreadProc() = 0;

	virtual bool Start();									// Starts the thread
	virtual bool Start(int priority);						// Starts the thread and sets its priority
	virtual bool Start(int priority, DWORD affinityMask);	// Starts the thread, sets its priority and assigns it to specific processors
	
	virtual bool Stop();	
	virtual bool Stop(bool wait);	

	__declspec(property(get = GetIsRunning))
	bool	IsRunning;
	bool	GetIsRunning();

	__declspec(property(get = GetWasKilled))
	bool	WasKilled;
	bool	GetWasKilled();

	void	Sleep();
	void	Sleep(double seconds);
	void	WakeUp();

	//uint	ThreadProc();

	/*	TODO:

	bool	ResolveDeadlock(Thread* otherThread); */
	
private:
	HANDLE	m_thread;
	UINT	m_threadID;

	bool	m_running;
	bool	m_killed;

	HANDLE	m_hWakeEvent;

private:
	static uint __stdcall StaticThreadProc(void* arg);
};

#endif