#ifndef SHAPE_H
#define SHAPE_H

#include <Types/Object.h>
#include <Containers/Array.h>
#include <Types/Geometric\Curve.h>

class FRect;

class Shape
	: public Object
{
	DECLARE_TYPE;

public:
	Shape();
	Shape(const Shape& shape);

	Object* Copy();

	__declspec(property(get = GetCurves))
	Array<Curve>& Curves;
	Array<Curve>& GetCurves();

	__declspec(property(get = GetBounds))
	FRect Bounds;
	FRect GetBounds();

	void Offset(const FPoint& p);

private:
	Array<Curve> m_curves;
};

#endif