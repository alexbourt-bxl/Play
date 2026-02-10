#ifndef ANIMATION_H
#define ANIMATION_H

#include <Types/Object.h>
#include <Events/Event.h>
#include <Animation\Animator.h>
#include <Types/Geometric\ParamCurve.h>

/*	This value is used to represent a value's base "mass". 
	Specifically it is used to scale the distance of the 
	last value update, and by trial and error 1/10 seems 
	a decent base value. */
#define INERTIA_MASS_BASE 1.0 / 10.0

class Property;

class Animation 
	: public Object,
	  public EventSender
{
	DECLARE_ABSTRACT_TYPE;

	friend class Animator;
	
	typedef Property _Property;

public:
	enum ConflictAction;

	class Message;

public:
	Animation();
	Animation(const Animation& anim);
	virtual ~Animation();

	__declspec(property(get = GetDelay, put = SetDelay))
	double	Delay;
	double	GetDelay();
	void	SetDelay(double delay);

	__declspec(property(get = GetSpeed, put = SetSpeed))
	double	Speed;
	double	GetSpeed();
	void	SetSpeed(double speed);

	__declspec(property(get = GetDuration, put = SetDuration))
	double	Duration;
	double	GetDuration();
	void	SetDuration(double duration);

	__declspec(property(get = GetRandom, put = SetRandom))
	double	Random;
	double	GetRandom();
	void	SetRandom(double random);

	__declspec(property(get = GetCurve, put = SetCurve))
	ParamCurve& Curve;
	ParamCurve& GetCurve();
	void SetCurve(const ParamCurve& curve);

	__declspec(property(get = GetConflictAction, put = SetConflictAction))
	ConflictAction OnConflict;
	ConflictAction GetConflictAction();
	void SetConflictAction(ConflictAction conflictAction);

	__declspec(property(get = GetScaleInertia, put = SetScaleInertia))
	double	ScaleInertia;
	double	GetScaleInertia();
	void	SetScaleInertia(double scale);

	Event<Animation::Message&> StartEvent;
	Event<Animation::Message&> EndEvent;

	//

	static Animator& GetAnimator();

	static void Suspend();

	static void Add(Animation* anim);
	static void Remove(Animation* anim);

	static void Resume();

	//

protected:
	__declspec(property(get = GetStarted))
	bool	Started;
	bool	GetStarted();

	__declspec(property(get = GetFinished))
	bool	Finished;
	virtual bool GetFinished();

	void Reset();

	virtual void  UpdateValueCurve() = 0;

	virtual void  CheckFrom() = 0;
	virtual double GetDistance() = 0;

	virtual void MatchFromToEnd(Animation* anim) = 0;
	virtual bool Match(Animation* anim) = 0;
	virtual void Inherit(Animation* anim) = 0;

	__declspec(property(get = GetElapsedTime))
	double	ElapsedTime;
	double	GetElapsedTime();

	__declspec(property(get = GetEndTime))
	double	EndTime;
	virtual double GetEndTime() = 0;

	__declspec(property(get = GetActualDuration))
	double	ActualDuration;
	double	GetActualDuration();

	virtual void SetCount();

	void	Update();

	void	PreUpdate();
	virtual void PerformUpdate() = 0;
	void	PostUpdate();

	__int64	m_startCount;
	bool	m_started;

private:
	/*	The animator tries to use the speed to calculate changes. 
		If the speed is zero, it uses the duration. */
	double	m_speed; // units per second
	double	m_duration; // seconds

	/*	To make animations more "organic" a randomness can be added to 
		the duration, which is helpful for things like group animations 
		where each one can be a little different. 
		
		The randomness value is mirrored around the point of reference, 
		so if the duration is 4.0 and the randomness is 1.0, duration 
		will vary from 0.0 to 8.0. */
	double	m_random;
	double	m_randomValue;

	ParamCurve m_curve;

	ConflictAction m_conflictAction;
	double	m_scaleInertia;
};

//

enum Animation::ConflictAction
{
	ReplaceCurrent,
	InheritCurrent,
	CancelNew,
	DelayNew
};

class Animation::Message
	: public ::Message
{
	typedef Animation _Animation;

public:
	_Animation* Animation;

	Message(_Animation* anim)
		: ::Message(anim) {}
};

#include <Animation\IntAnimation\IntAnimation.h>
#include <Animation\FloatAnimation\FloatAnimation.h>
#include <Animation\PointAnimation\PointAnimation.h>
#include <Animation\AnimationCurve.h>

#endif