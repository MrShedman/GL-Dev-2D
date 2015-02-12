#pragma once

#include <ostream>

#include "..\math\Vector.h"

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

	Vector3f getPosition() const
	{
		float x = m[0][0] * m[2][0] + m[0][1] * m[2][1] + m[0][2] * m[2][2];
		float y = m[1][0] * m[2][0] + m[1][1] * m[2][1] + m[1][2] * m[2][2];	

		return Vector3f(x, y, 0.f);
	}

	Vector3f transformPointAffine(const Vector3f& rhs) const
	{
		float x = rhs.x * m[0][0] + rhs.y * m[1][0] + rhs.z * m[2][0];
		float y = rhs.x * m[0][1] + rhs.y * m[1][1] + rhs.z * m[2][1];
		float z = rhs.x * m[0][2] + rhs.y * m[1][2] + rhs.z * m[2][2];

		return Vector3f(x, y, z);
	}

	inline Vector3f transformVector(const Vector3f &rhs) const
	{
		float x = rhs.x * m[0][0] + rhs.y * m[1][0] + rhs.z * m[2][0];
		float y = rhs.x * m[0][1] + rhs.y * m[1][1] + rhs.z * m[2][1];
		float z = rhs.x * m[0][2] + rhs.y * m[1][2] + rhs.z * m[2][2];

		return Vector3f(x, y, z);
	}

	void set(unsigned int x, unsigned int y, float val);

	const float* operator[](int index) const;

	float* operator[](int index);

	static const Matrix33 Identity;

private:

	float m[3][3];
};

std::ostream& operator<< (std::ostream& os, const Matrix33& m);