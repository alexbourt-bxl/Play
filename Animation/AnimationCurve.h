#ifndef ANIMATION_CURVE_H
#define ANIMATION_CURVE_H

#include <Types/Geometric\ParamCurve.h>

class AnimationCurve
	: public ParamCurve
{
private:
	AnimationCurve(int nPoints, ...);

public:
	static AnimationCurve Step;
	static AnimationCurve Linear;
	static AnimationCurve Relaxed;
	static AnimationCurve Smooth;
	static AnimationCurve EaseOut;
	static AnimationCurve EaseIn;
};

#endif