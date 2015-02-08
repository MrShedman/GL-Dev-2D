#include "Matrix33.h"

#include <iostream>
#include <iomanip>

const Matrix33 Matrix33::Identity;

Matrix33::Matrix33()
{
	m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0;
	m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0;
	m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0;
}

Matrix33::Matrix33(	float a00, float a10, float a20,
					float a01, float a11, float a21,
					float a02, float a12, float a22)
{
	m[0][0] = a00; m[1][0] = a10; m[2][0] = a20;
	m[0][1] = a01; m[1][1] = a11; m[2][1] = a21;
	m[0][2] = a02; m[1][2] = a12; m[2][2] = a22;
}

Matrix33 Matrix33::Transpose() const
{
	Matrix33 t;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			t[i][j] = m[j][i];
		}
	}
	return t;
}

float Matrix33::getDeterminant() const
{
	return	m[0][0] * (m[2][2] * m[1][1] - m[2][1] * m[1][2]) -
		m[1][0] * (m[2][2] * m[0][1] - m[2][1] * m[0][2]) +
		m[2][0] * (m[1][2] * m[0][1] - m[1][1] * m[0][2]);
}

Matrix33 Matrix33::getInverse() const
{
	int i, j, k;
	Matrix33 s;
	Matrix33 t(*this);

	// Forward elimination
	for (i = 0; i < 2; i++) {
		int pivot = i;

		float pivotsize = t[i][i];

		if (pivotsize < 0)
			pivotsize = -pivotsize;

		for (j = i + 1; j < 3; j++) {
			float tmp = t[j][i];

			if (tmp < 0)
				tmp = -tmp;

			if (tmp > pivotsize) {
				pivot = j;
				pivotsize = tmp;
			}
		}

		if (pivotsize == 0) {
			//if (singExc)
			//	throw ::Imath::SingMatrixExc ("Cannot invert singular matrix.");

			return Matrix33();
		}

		if (pivot != i) {
			for (j = 0; j < 3; j++) {
				float tmp;

				tmp = t[i][j];
				t[i][j] = t[pivot][j];
				t[pivot][j] = tmp;

				tmp = s[i][j];
				s[i][j] = s[pivot][j];
				s[pivot][j] = tmp;
			}
		}

		for (j = i + 1; j < 3; j++) {
			float f = t[j][i] / t[i][i];

			for (k = 0; k < 3; k++) {
				t[j][k] -= f * t[i][k];
				s[j][k] -= f * s[i][k];
			}
		}
	}

	// Backward substitution
	for (i = 3 - 1; i >= 0; --i) {
		float f;

		if ((f = t[i][i]) == 0) {
			//if (singExc)
			//	throw ::Imath::SingMatrixExc ("Cannot invert singular matrix.");

			return Matrix33();
		}

		for (j = 0; j < 3; j++) {
			t[i][j] /= f;
			s[i][j] /= f;
		}

		for (j = 0; j < i; j++) {
			f = t[j][i];

			for (k = 0; k < 3; k++) {
				t[j][k] -= f * t[i][k];
				s[j][k] -= f * s[i][k];
			}
		}
	}

	return s;
}

Matrix33 Matrix33::operator*(const Matrix33& r) const
{
	Matrix33 ret;
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			ret.m[i][j] = 0.0;

			for (unsigned int k = 0; k < 3; k++)
			{
				ret.m[i][j] += m[k][j] * r.m[i][k];
			}
		}
	}
	return ret;
}

void Matrix33::set(unsigned int x, unsigned int y, float val)
{
	m[x][y] = val;
}

const float* Matrix33::operator[](int index) const
{
	return m[index];
}

float* Matrix33::operator[](int index)
{
	return m[index];
}

std::ostream& operator << (std::ostream& os, const Matrix33& m)
{
	for (int i = 0; i < 3; ++i)
	{
		std::cout << std::fixed << std::setprecision(5);
		std::cout << "[";
		for (int j = 0; j < 2; ++j)
		{
			std::cout << std::left << std::setfill(' ') << std::setw(10) << m[i][j];
		}
		std::cout << m[i][2] << "]" << std::endl;
	}

	return os;
}
