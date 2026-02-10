#include "Matrix33.h"

#include <Math\Math.h>

DEFINE_TYPE(Matrix33, Object);

Matrix33::Matrix33()
{
	e[0][0] = 1.0; e[0][1] = 0.0; e[0][2] = 0.0;
	e[1][0] = 0.0; e[1][1] = 1.0; e[1][2] = 0.0;
	e[2][0] = 0.0; e[2][1] = 0.0; e[2][2] = 1.0;
}

Matrix33::Matrix33(const Matrix33& m)
{
	e[0][0] = m.e[0][0]; e[0][1] = m.e[0][1]; e[0][2] = m.e[0][2];
	e[1][0] = m.e[1][0]; e[1][1] = m.e[1][1]; e[1][2] = m.e[1][2];
	e[2][0] = m.e[2][0]; e[2][1] = m.e[2][1]; e[2][2] = m.e[2][2];
}

Matrix33::Matrix33(
	double e11, double e12, double e13,
	double e21, double e22, double e23,
	double e31, double e32, double e33)
{
	e[0][0] = e11; e[0][1] = e12; e[0][2] = e13;
	e[1][0] = e21; e[1][1] = e22; e[1][2] = e23;
	e[2][0] = e31; e[2][1] = e32; e[2][2] = e33;
}

Object* Matrix33::Copy()
{
	return new Matrix33(*this);
}

Matrix33& Matrix33::operator *= (const Matrix33& M)
{
	double d;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
			d = 0.0;

			/*	Calculate the dot product of ith row 
				of this and jth column of M. */
            
			for (int k = 0; k < 3; k++)
                d += e[i][k] * M.e[k][j];

			e[i][j] = d;
        }
    }

	return *this;
}

Matrix33 Matrix33::operator * (const Matrix33& m)
{
	return Matrix33(*this) *= m;
}

Matrix33& Matrix33::operator *= (double f)
{
	e[0][0] *= f;	e[0][1] *= f;	e[0][2] *= f;
	e[1][0] *= f;	e[1][1] *= f;	e[1][2] *= f;
	e[2][0] *= f;	e[2][1] *= f;	e[2][2] *= f;

	return *this;
}

Matrix33& Matrix33::operator /= (double f)
{
	e[0][0] /= f;	e[0][1] /= f;	e[0][2] /= f;
	e[1][0] /= f;	e[1][1] /= f;	e[1][2] /= f;
	e[2][0] /= f;	e[2][1] /= f;	e[2][2] /= f;

	return *this;
}

double Matrix33::GetDeterminant()
{
	return
		e[0][0] * e[1][1] * e[2][2]
	+	e[0][1] * e[1][2] * e[2][0]
	+	e[0][2] * e[1][0] * e[2][1]
	-	e[0][0] * e[1][2] * e[2][1]
	-	e[0][1] * e[1][0] * e[2][2]
	-	e[0][2] * e[1][1] * e[2][0];
}

Matrix33 Matrix33::GetInverse()
{
	Matrix33 inv(
		  e[2][2] * e[1][1] - e[2][1] * e[1][2],	-(e[2][2] * e[1][0] - e[2][0] * e[1][2]),	  e[2][1] * e[1][0] - e[2][0] * e[1][1],
		-(e[2][2] * e[0][1] - e[2][1] * e[0][2]),	  e[2][2] * e[0][0] - e[2][0] * e[0][2],	-(e[2][1] * e[0][0] - e[2][0] * e[0][1]),
		  e[1][2] * e[0][1] - e[1][1] * e[0][2],	-(e[1][2] * e[0][0] - e[1][0] * e[0][2]),	  e[1][1] * e[0][0] - e[1][0] * e[0][1]);

	inv /= Determinant;

	return inv;
}

void Matrix33::Transpose()
{
	SWAPF(e[1][0], e[0][1]);
	SWAPF(e[2][0], e[0][2]);
	SWAPF(e[2][1], e[1][2]);
}