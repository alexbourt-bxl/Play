#ifndef INT_ANIMATION_H
#define INT_ANIMATION_H

#include <Animation\Animation.h>

class IntProperty;

class IntAnimation 
	: public Animation
{
	DECLARE_TYPE;

public:
	IntAnimation();
	IntAnimation(const IntAnimation& anim);

	Object* Copy();

	__declspec(property(get = GetProperty, put = SetProperty))
	IntProperty* Property;
	IntProperty* GetProperty() const;
	void SetProperty(IntProperty* prop);
	void SetProperty(IntProperty& prop);

	__declspec(property(get = GetFrom, put = SetFrom))
	int		From;
	int		GetFrom();
	void	SetFrom(int from);

	__declspec(property(get = GetTo, put = SetTo))
	int		To;
	int		GetTo();
	void	SetTo(int to);

private:
	IntProperty* m_property;
	
	double	m_from;
	double	m_to;

	ParamCurve m_valueCurve;
	void	UpdateValueCurve();

	void	CheckFrom();
	double	GetDistance();
	
	void	PerformUpdate();

	void	MatchFromToEnd(Animation* anim);
	bool	Match(Animation* anim);
	void	Inherit(Animation* anim);

	double	GetEndTime();
};

#endif