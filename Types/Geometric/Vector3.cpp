#include "Vector3.h"

#include <Types/Geometric/Matrix33.h>
#include <Math/Math.h>

DEFINE_TYPE(Vector3, Object);

Vector3::Vector3()
{
	e[0] = 0.0;
	e[1] = 0.0;
	e[2] = 0.0;
}

Vector3::Vector3(const Vector3& v)
{
	e[0] = v.e[0];
	e[1] = v.e[1];
	e[2] = v.e[2];
}

Vector3::Vector3(double x, double y, double z)
{
	e[0] = x;
	e[1] = y;
	e[2] = z;
}

Object* Vector3::Copy()
{
	return new Vector3(*this);
}

Vector3& Vector3::operator += (const Vector3& v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];

	return *this;
}

Vector3& Vector3::operator -= (const Vector3& v)
{
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];

	return *this;
}

Vector3& Vector3::operator *= (double s)
{
	e[0] *= s;
	e[1] *= s;
	e[2] *= s;

	return *this;
}

Vector3& Vector3::operator /= (double s)
{
	e[0] /= s;
	e[1] /= s;
	e[2] /= s;

	return *this;
}

Vector3 Vector3::operator + (const Vector3& v)
{
	return Vector3(*this) += v;
}

Vector3 Vector3::operator - (const Vector3& v)
{
	return Vector3(*this) -= v;
}

Vector3 Vector3::operator * (double s)
{
	return Vector3(*this) *= s;
}

Vector3 Vector3::operator / (double s)
{
	return Vector3(*this) /= s;
}

Vector3& Vector3::operator *= (const Matrix33& m)
{
    Vector3 v;

    for (int i = 0; i < 3; i++)
    {
        // Calculate the dot product of the ith row of this and v
 
		for (int j = 0; j < 3; j++)
            v.e[i] += m.e[i][j] * e[j];
    }

	for (int i = 0; i < 3; i++)
		e[i] = v.e[i];

	return *this;
}

double Vector3::GetLength()
{
	return sqrt(SQR(e[0]) + SQR(e[1]) + SQR(e[2]));
}

void Vector3::Normalize()
{
	*this /= Length;
}

double Vector3::operator ^ (const Vector3& v)
{
	return e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2];
}

Vector3 Vector3::operator * (const Vector3& v)
{
	return Vector3(
		e[1] * v.e[2] - e[2] * v.e[1],
		e[2] * v.e[0] - e[0] * v.e[2],
		e[0] * v.e[1] - e[1] * v.e[0]);
}