#ifndef STROKEPOINT_H
#define STROKEPOINT_H

#include <Types/Geometric\FPoint.h>

class StrokePoint
	: public FPoint
{
public:
	double Pressure; // (0 - 1)
	double Speed; // Normalized to 0-1; used for interpolation only, should be recalculated anew each time.

	double dTime; // Time elapsed sice the last stroke point (in seconds)

	double Azimuth;
	double Elevation;
	double Twist;

public:
	StrokePoint();
	StrokePoint(const StrokePoint& p);
	StrokePoint(double x, double y, double pressure = 1, double azimuth = 0, double elevation = 0, double twist = 0, double dt = 0);

	~StrokePoint();

	StrokePoint& operator += (const StrokePoint& p);
	StrokePoint  operator +  (const StrokePoint& p) const;
	StrokePoint& operator -= (const StrokePoint& p);
	StrokePoint  operator -  (const StrokePoint& p) const;
	StrokePoint& operator *= (double s);
	StrokePoint  operator *  (double s);
	StrokePoint& operator /= (double s);
	StrokePoint  operator /  (double s);

public:
	bool _flowing; // indicates if this point was created as a result of flow
};

#endif