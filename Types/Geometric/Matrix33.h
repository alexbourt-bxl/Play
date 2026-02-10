#ifndef MATRIX33_h
#define MATRIX33_h

#include <Types/Object.h>

class Matrix33
	: public Object
{
	DECLARE_TYPE;

public:
	double e[3][3];

public:
	Matrix33();
	Matrix33(const Matrix33& m);
	Matrix33(
		double e11, double e12, double e13,
		double e21, double e22, double e23,
		double e31, double e32, double e33);

	Object* Copy();

	Matrix33& operator *= (const Matrix33& m);
	Matrix33  operator *  (const Matrix33& m);

	Matrix33& operator *= (double f);
	Matrix33& operator /= (double f);

	__declspec(property(get = GetDeterminant))
	double Determinant;
	double GetDeterminant();

	__declspec(property(get = GetInverse))
	Matrix33 Inverse;
	Matrix33 GetInverse();

	void Transpose();
};

#endif