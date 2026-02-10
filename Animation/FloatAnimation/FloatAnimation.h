#ifndef FLOAT_ANIMATION_H
#define FLOAT_ANIMATION_H

#include <Animation\Animation.h>

class FloatProperty;

class FloatAnimation 
	: public Animation
{
	DECLARE_TYPE;

public:
	FloatAnimation();
	FloatAnimation(const FloatAnimation& anim);

	Object* Copy();

	__declspec(property(get = GetProperty, put = SetProperty))
	FloatProperty* Property;
	FloatProperty* GetProperty() const;
	void SetProperty(FloatProperty* prop);
	void SetProperty(FloatProperty& prop);

	__declspec(property(get = GetFrom, put = SetFrom))
	double	From;
	double	GetFrom();
	void	SetFrom(double from);

	__declspec(property(get = GetTo, put = SetTo))
	double	To;
	double	GetTo();
	void	SetTo(double to);

private:
	FloatProperty* m_property;
	
	double	m_from;
	double	m_to;

	double	m_prev;
	double	m_current;

	ParamCurve m_valueCurve;
	void	UpdateValueCurve();

	void	CheckFrom();
	void	Initialize();

	double	GetDistance();
	
	void	PerformUpdate();

	void	MatchFromToEnd(Animation* anim);
	bool	Match(Animation* anim);
	void	Inherit(Animation* anim);

	double	GetEndTime();
};

#endif