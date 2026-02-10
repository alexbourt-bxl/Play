#ifndef POINT_ANIMATION_H
#define POINT_ANIMATION_H

#include <Animation\Animation.h>
#include <Types/Geometric\Point.h>
#include <Types/Geometric\FPoint.h>

class IntProperty;

class PointAnimation 
	: public Animation
{
	DECLARE_TYPE;

public:
	PointAnimation();
	PointAnimation(const PointAnimation& anim);

	Object* Copy();

	__declspec(property(get = GetPropertyX, put = SetPropertyX))
	IntProperty* PropertyX;
	IntProperty* GetPropertyX() const;
	void SetPropertyX(IntProperty* prop);
	void SetPropertyX(IntProperty& prop);

	__declspec(property(get = GetPropertyY, put = SetPropertyY))
	IntProperty* PropertyY;
	IntProperty* GetPropertyY() const;
	void SetPropertyY(IntProperty* prop);
	void SetPropertyY(IntProperty& prop);

	__declspec(property(get = GetFrom, put = SetFrom))
	Point	From;
	Point	GetFrom();
	void	SetFrom(Point from);

	__declspec(property(get = GetTo, put = SetTo))
	Point	To;
	Point	GetTo();
	void	SetTo(Point to);

private:
	IntProperty* m_propertyX;
	IntProperty* m_propertyY;
	
	FPoint	m_from;
	FPoint	m_to;

	ParamCurve m_valueCurveX;
	ParamCurve m_valueCurveY;
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