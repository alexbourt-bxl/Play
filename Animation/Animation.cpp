#include "Animation.h"

#include <Types/Geometric\ParamCurve.h>
#include <Properties\Property.h>
#include <limits>

DEFINE_ABSTRACT_TYPE(Animation, Object);

Animation::Animation()
{
	StartEvent.Sender = this;
	EndEvent.Sender = this;

	m_speed			= 0.0;
	m_duration		= 0.0;

	m_random		= 0.0;
	m_randomValue	= 0.0;

	m_curve			= AnimationCurve::Linear;

	m_conflictAction = InheritCurrent;
	m_scaleInertia	= 1.0;

	Reset();
}

Animation::Animation(const Animation& anim)
{
	StartEvent.Sender = this;
	EndEvent.Sender = this;

	m_speed			= anim.m_speed;
	m_duration		= anim.m_duration;
	
	m_random		= anim.m_random;
	m_randomValue	= anim.m_randomValue;

	m_curve			= anim.m_curve;

	m_conflictAction = anim.m_conflictAction;
	m_scaleInertia	= anim.m_scaleInertia;

	Reset();
}

Animation::~Animation()
{

}

void Animation::SetSpeed(double speed)
{
	m_speed = speed;
}

double Animation::GetDuration()
{
	return m_duration;
}

void Animation::SetDuration(double duration)
{
	assert(duration >= 0.0);
	m_duration = max(0.0, duration);
}

double Animation::GetRandom()
{
	return m_random;
}

void Animation::SetRandom(double random)
{
	m_random = random;
	m_randomValue = Math::Random();
}

ParamCurve& Animation::GetCurve()
{
	return m_curve;
}

void Animation::SetCurve(const ParamCurve& curve)
{
	m_curve = curve;
}

Animation::ConflictAction Animation::GetConflictAction()
{
	return m_conflictAction;
}

void Animation::SetConflictAction(ConflictAction conflictAction)
{
	m_conflictAction = conflictAction;
}

double Animation::GetScaleInertia()
{
	return m_scaleInertia;
}

void Animation::SetScaleInertia(double scale)
{
	m_scaleInertia = scale;
}

//

Animator& Animation::GetAnimator()
{
	static Animator animator;
	return animator;
}

void Animation::Suspend()
{
	Animation::GetAnimator().Lock();
}

void Animation::Add(Animation* anim)
{
	GetAnimator().Add(anim);
}

void Animation::Remove(Animation* anim)
{
	for (int i = 0; i < GetAnimator().Count; i++)
	{
		if (GetAnimator()[i]->Match(anim))
			GetAnimator().RemoveAt(i);
	}
}

void Animation::Resume()
{
	GetAnimator().Unlock();
	GetAnimator().Update();
}

//

bool Animation::GetStarted()
{
	return m_started;
}

bool Animation::GetFinished()
{
	return m_started && ElapsedTime >= EndTime;
}

void Animation::Reset()
{
	m_started = false;
	m_startCount = 0;
}

double Animation::GetElapsedTime()
{
	double time = (double)(GetAnimator().CurrentCount - m_startCount) / (double)GetAnimator().CounterFrequency;
	return max(0.0, time);
}

double Animation::GetActualDuration()
{
	CheckFrom();

	double duration = m_speed > 0.0
		? GetDistance() / m_speed
		: m_duration;

	if (m_random == 0.0)
		return duration;

	/*	Add randomness. */

	double range = duration * m_random;
	double random = duration + range * (2.0 * m_randomValue - 1.0);

	return max(0.0, random);
}

void Animation::SetCount()
{
	if (m_startCount == 0)
		m_startCount = GetAnimator().CurrentCount;
}

void Animation::Update()
{
	PreUpdate();
	PerformUpdate();
	PostUpdate();
}

void Animation::PreUpdate()
{
	if (!m_started)
	{
		Animation::Message msg(this);
		StartEvent.Send(msg);

		UpdateValueCurve();

		m_started = true;
	}
}

void Animation::PostUpdate()
{
	if (Finished)
	{
		Animation::Message msg(this);
		EndEvent.Send(msg);

		GetAnimator().Remove(this);
	}
}