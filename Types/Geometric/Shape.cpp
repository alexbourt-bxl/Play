#include "Shape.h"

#include <Types/Geometric\FRect.h>

DEFINE_TYPE(Shape, Object);

Shape::Shape()
{

}

Shape::Shape(const Shape& shape)
{
	for (int i = 0; i < ((Shape&)shape).m_curves.Count; i++)
		m_curves.Add(shape.m_curves[i]);
}

Object* Shape::Copy()
{
	return new Shape(*this);
}

Array<Curve>& Shape::GetCurves()
{
	return m_curves;
}

FRect Shape::GetBounds()
{
	FRect bounds;

	for (int i = 0; i < m_curves.Count; i++)
		bounds.Expand(m_curves[i].Bounds);

	return bounds;
}

void Shape::Offset(const FPoint& p)
{
	for (int i = 0; i < m_curves.Count; i++)
		m_curves[i].Offset(p);
}