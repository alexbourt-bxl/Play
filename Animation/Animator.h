#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <Containers/Array.h>
#include <Threads\ThreadLock.h>
//#include <Timing\Timer.h>

class Animation;

class Animator
	: public Array<Animation*>,
	  public ThreadLock
{
	friend class Animation;
	friend class GroupAnimation;
	friend class StepAnimation;
	
public:
	Animator();

	__declspec(property(get = GetIsBusy))
	bool IsBusy;
	bool GetIsBusy();

	__declspec(property(get = GetCounterFrequency))
	__int64 CounterFrequency;
	__int64 GetCounterFrequency();

	__declspec(property(get = GetCurrentCount))
	__int64 CurrentCount;
	__int64 GetCurrentCount();

	__declspec(property(get = GetTimeDelta))
	double TimeDelta;
	double GetTimeDelta();

	void Update();

private:
	LARGE_INTEGER m_counterFrequency;

	LARGE_INTEGER m_currentCount;
	LARGE_INTEGER m_prevCount;

	bool ResolveConflicts(Animation* anim);

	/*	The delete queue holds pointers to animations
		that will be deleted after the current update. */
	Array<Animation*> m_deleteQueue;
	void OnRemoveAnimation(Container::Message& msg);
};

#endif