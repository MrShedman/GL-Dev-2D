#pragma once

#include <ostream>

class Matrix33
{
public:

	Matrix33();

	Matrix33(	float a00, float a01, float a02,
				float a10, float a11, float a12,
				float a20, float a21, float a22);

	Matrix33 InitIdentity();

	Matrix33 Transpose() const;

	float getDeterminant() const;

	Matrix33 getInverse() const;

	Matrix33 operator*(const Matrix33& r) const;

	void set(unsigned int x, unsigned int y, float val);

	const float* operator[](int index) const;

	float* operator[](int index);

	static const Matrix33 Identity;

private:

	float m[3][3];
};

std::ostream& operator<< (std::ostream& os, const Matrix33& m);