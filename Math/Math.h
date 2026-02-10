#ifndef MATH_H
#define MATH_H

#include <Types/BaseTypes.h>

#define ZERO					0.000000001 //000000000000000000001 //0.0000000000001f

#define PI_2					1.5707963267949
#define PI						3.1415926535897
#define _2PI					6.2831853071795

#define PHI						1.6180339887498
#define _1_PHI					1 / PHI

#define E_						2.7182818284590

#define INT_NAN					0x7FFFFFFF

#define SGN(a)					(a >= 0 ? 1 : -1)

#define SWAP(a, b)				{ a ^= b; b ^= a; a ^= b; }
#define SWAPF(a, b)				{ a += b; b = a - b; a -= b; }

#define SQR(x)					((x) * (x))

#define INT_MULT(a, b, t)		((t) = (a) * (b) + 0x80, ((((t) >> 8) + (t)) >> 8))
#define INT_DIV(a, b)			((b) == 0 ? 0xFF : ((((a) << 8) | (a)) / (b)))
#define INT_DIV_M(a, b)			((b) == 0 ? 0xFF : min((((a) << 8) | (a)) / (b), 0xFF))

#define INT_OVER(a, b, f, t)	((b) + (a) - INT_MULT((a), (f), (t))) // premultiplied
#define INT_LERP(a, b, f, t)	((a) + INT_MULT(((b) - (a)), (f), (t)))
#define INT_LERP_MM(a, b, f, t)	(min(max(0, (a) + INT_MULT(((b) - (a)), (f), (t))), 0xFF)) // same as INT_LERP, but with min/max
#define INT_CLERP(a, b, f, t)	(INT_MULT((a), (f), (t)) + INT_MULT((b), (0xFF - (f)), (t))) // for color blending modes (alpha is backwards)

#define FROUND(x)				((double)(int)((x) + 0.5))
#define IPART(x)				((int)(x))
#define FPART(x)				((x) - (double)IPART((x)))
#define RFPART(x)				(1 - FPART((x)))

#ifndef min
#define min(a, b)				((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b)				((a) > (b) ? (a) : (b))
#endif

class Point;
class FPoint;
class Poly;
class Matrix33;
class Curve;
class Shape;

class Math
{
public:
	static void		Randomize(int seed);
	static double	Random(); // Returns a random value between 0.0 and 1.0
	static double	Random(double from, double to);

	static __forceinline int	Round(double f);
	static __forceinline int	ToInt8(double f);

	static __forceinline int	GetPopulationCount(int i);
	static __forceinline int	GetLeadingZeroCount(int i);
	static __forceinline uint	Log10(uint i);
	static __forceinline uint	GetDigitCount(int i);
	static __forceinline int	GetPrevPowerOf2(int i);
	static __forceinline int	GetNextPowerOf2(int i);

	static __forceinline float  InvSqrt(float x);
	static __forceinline double InvSqrt(double x);

	static __forceinline double	GetDistance(const Point& p1, const Point& p2);
	static __forceinline double	GetDistance(const FPoint& p1, const FPoint& p2);
	static __forceinline double	GetAngle(const Point& p1, const Point& p2);
	static __forceinline double	GetAngle(const FPoint& p1, const FPoint& p2);
	static __forceinline double	GetAngleDifference(double angle1, double angle2);
	static __forceinline double	GetDistanceToLine(FPoint& p, FPoint& p1, FPoint& p2, bool segment = false);

	static __forceinline double	CubeRoot(double x);
	static __forceinline double	LerpBezier(double f0, double f1, double f2, double f3, double t);

	static int		SolveQuadratic(double a, double b, double c, double* roots);
	static int		SolveCubic(double a, double b, double c, double d, double* roots);
	static double	SolveCubicNR(double a, double b, double c, double d, double minRoot, double maxRoot, double accuracy = 0.00001);

	static bool		IntersectLines(FPoint& p1, FPoint& p2, FPoint& p3, FPoint& p4, FPoint& p, bool segment = false);
	
	static void		Transform(FPoint& p, Matrix33& m);
	static void		Transform(Poly& poly, Matrix33& m);
	static void		Transform(Curve& curve, Matrix33& m);
	static void		Transform(Shape& shape, Matrix33& m);
	static FPoint	Transform(const FPoint& p, const FPoint& offset, double rotate, const FPoint& scale, FPoint* center); // [FPoint* center] to be able to pass null
	static void		Transform(Poly& poly, const FPoint& offset, double rotate, const FPoint& scale, FPoint* center); // [FPoint* center] to be able to pass null
	static void		Transform(Curve& curve, const FPoint& offset, double rotate, const FPoint& scale, FPoint* center);
	static void		Transform(Shape& shape, const FPoint& offset, double rotate, const FPoint& scale, FPoint* center);

	static bool	GetProjectiveMapping(
		//const FPoint& sp0, const FPoint& sp1, const FPoint& sp2, const FPoint& sp3, // source points
		const FPoint& dp0, const FPoint& dp1, const FPoint& dp2, const FPoint& dp3, // target points
		Matrix33* invMatrix);
};

#include "Math.inl"

#endif
