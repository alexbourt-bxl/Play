#ifndef POLY_H
#define POLY_H

#include <Types/Object.h>
#include <Containers/Array.h>
#include <Types/Geometric/FPoint.h>

class FRect;

class Poly
	: public Object
{
	DECLARE_TYPE;

public:
	Poly();
	Poly(const Poly& poly);
	~Poly();

	Object* Copy();

	Poly&	operator = (const Poly& poly);

	__declspec(property(get = GetPoints))
	Array<FPoint>& Points;
	Array<FPoint>& GetPoints();

	__declspec(property(get = GetBounds))
	FRect	Bounds;
	FRect	GetBounds();

	__declspec(property(get = GetCenter))
	FPoint	Center;
	FPoint	GetCenter();

	bool	Contains(const FPoint& p);
	void	Offset(const FPoint& p);

	void	Inset(double dist);
	void	InsetCorner(FPoint& p1, FPoint& p2, FPoint& p3, FPoint* p, double dist);

	void	Reverse();

private:
	Array<FPoint> m_points;
};

#endif