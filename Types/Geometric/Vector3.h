#ifndef VECTOR3_H
#define VECTOR3_H

#pragma warning(disable:4201)

#include <Types/Object.h>

class Matrix33;

class Vector3
	: public Object
{
	DECLARE_TYPE;

public:
	union
	{
		struct
		{
			double X;
			double Y;
			double Z;
		};

		double e[3];
	};

public:
	Vector3();
	Vector3(const Vector3& v);
	Vector3(double x, double y, double z);

	Object* Copy();

	Vector3& operator += (const Vector3& v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator *= (double s);
	Vector3& operator /= (double s);
	
	Vector3 operator +  (const Vector3& v);
	Vector3 operator -  (const Vector3& v);
	Vector3 operator *  (double s);
	Vector3 operator /  (double s);

	Vector3& operator *= (const Matrix33& m);

	__declspec(property(get = GetLength))
	double	Length;
	double	GetLength();
	
	void    Normalize();

	double  operator ^ (const Vector3& v); // Dot product
	Vector3 operator * (const Vector3& v); // Cross product
};

#endif