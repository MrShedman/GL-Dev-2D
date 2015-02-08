#pragma once

#include "Vector.h"
#include "Rect.h"

template<typename T>
class Matrix4
{
public:

	Matrix4<T>()
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				if (i == j)
					m_matrix[i][j] = T(1);
				else
					m_matrix[i][j] = T(0);
			}
		}
	}

	Matrix4<T>(T a00, T a01, T a02,
		T a10, T a11, T a12,
		T a20, T a21, T a22)
	{
		m_matrix[0][0] = a00;	m_matrix[1][0] = a01;	m_matrix[2][0] = a02;	m_matrix[3][0] = a03;
		m_matrix[0][1] = a10;	m_matrix[1][1] = a11;	m_matrix[2][1] = a12;	m_matrix[3][1] = a13;
		m_matrix[0][2] = a20;	m_matrix[1][2] = a21;	m_matrix[2][2] = a22;	m_matrix[3][2] = a23;
		m_matrix[0][3] = a30;	m_matrix[1][3] = a31;	m_matrix[2][3] = a32;	m_matrix[3][3] = a33;
	}

	Matrix4<T>(T a00, T a01, T a02, T a03,
		T a10, T a11, T a12, T a13,
		T a20, T a21, T a22, T a23,
		T a30, T a31, T a32, T a33)
	{
		m_matrix[0][0] = a00;	m_matrix[1][0] = a01;	m_matrix[2][0] = a02;	m_matrix[3][0] = a03;
		m_matrix[0][1] = a10;	m_matrix[1][1] = a11;	m_matrix[2][1] = a12;	m_matrix[3][1] = a13;
		m_matrix[0][2] = a20;	m_matrix[1][2] = a21;	m_matrix[2][2] = a22;	m_matrix[3][2] = a23;
		m_matrix[0][3] = a30;	m_matrix[1][3] = a31;	m_matrix[2][3] = a32;	m_matrix[3][3] = a33;
	}

	Matrix4<T>(const Matrix4<T>& rhs)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				m_matrix[i][j] = rhs.m_matrix[i][j];
			}
		}
	}

	inline Matrix4<T> InitIdentity()
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				if (i == j)
					m_matrix[i][j] = T(1);
				else
					m_matrix[i][j] = T(0);
			}
		}

		return *this;
	}

	inline Matrix4<T> InitScale(const Vector3<T>& rhs)
	{
		m_matrix[0][0] = rhs.x;		m_matrix[1][0] = T(0);		m_matrix[2][0] = T(0);		m_matrix[3][0] = T(0);
		m_matrix[0][1] = T(0);		m_matrix[1][1] = rhs.y;		m_matrix[2][1] = T(0);		m_matrix[3][1] = T(0);
		m_matrix[0][2] = T(0);		m_matrix[1][2] = T(0);		m_matrix[2][2] = rhs.z;		m_matrix[3][2] = T(0);
		m_matrix[0][3] = T(0);		m_matrix[1][3] = T(0);		m_matrix[2][3] = T(0);		m_matrix[3][3] = T(1);

		return *this;
	}

	inline Matrix4<T> InitTranslation(const Vector3<T>& rhs)
	{
		m_matrix[0][0] = T(1);	m_matrix[1][0] = T(0);	m_matrix[2][0] = T(0);	m_matrix[3][0] = rhs.x;
		m_matrix[0][1] = T(0);	m_matrix[1][1] = T(1);	m_matrix[2][1] = T(0);	m_matrix[3][1] = rhs.y;
		m_matrix[0][2] = T(0);	m_matrix[1][2] = T(0);	m_matrix[2][2] = T(1);	m_matrix[3][2] = rhs.z;
		m_matrix[0][3] = T(0);	m_matrix[1][3] = T(0);	m_matrix[2][3] = T(0);	m_matrix[3][3] = T(1);

		return *this;
	}

	inline Matrix4<T> InitRotationEuler(Vector3<T> rotation)
	{
		Matrix4<T> rx, ry, rz;

		const T cosx = std::cos(rotation.x);
		const T sinx = std::sin(rotation.x);
		const T cosy = std::cos(rotation.y);
		const T siny = std::sin(rotation.y);
		const T cosz = std::cos(rotation.z);
		const T sinz = std::sin(rotation.z);

		rx[0][0] = T(1);	rx[1][0] = T(0);	rx[2][0] = T(0);	rx[3][0] = T(0);
		rx[0][1] = T(0);	rx[1][1] = cosx;	rx[2][1] = -sinx;	rx[3][1] = T(0);
		rx[0][2] = T(0);	rx[1][2] = sinx;	rx[2][2] = cosx;	rx[3][2] = T(0);
		rx[0][3] = T(0);	rx[1][3] = T(0);	rx[2][3] = T(0);	rx[3][3] = T(1);

		ry[0][0] = cosy;	ry[1][0] = T(0);	ry[2][0] = -siny;	ry[3][0] = T(0);
		ry[0][1] = T(0);	ry[1][1] = T(1);    ry[2][1] = T(0);	ry[3][1] = T(0);
		ry[0][2] = siny;	ry[1][2] = T(0);    ry[2][2] = cosy;	ry[3][2] = T(0);
		ry[0][3] = T(0);	ry[1][3] = T(0);    ry[2][3] = T(0);	ry[3][3] = T(1);

		rz[0][0] = cosz;	rz[1][0] = -sinz;	rz[2][0] = T(0);    rz[3][0] = T(0);
		rz[0][1] = sinz;	rz[1][1] = cosz;	rz[2][1] = T(0);    rz[3][1] = T(0);
		rz[0][2] = T(0);	rz[1][2] = T(0);	rz[2][2] = T(1);    rz[3][2] = T(0);
		rz[0][3] = T(0);	rz[1][3] = T(0);	rz[2][3] = T(0);    rz[3][3] = T(1);

		*this = rz * ry * rx;

		return *this;
	}

	inline Matrix4<T> InitRotationFromVectors(const Vector3<T>& n, const Vector3<T>& v, const Vector3<T>& u)
	{
		m_matrix[0][0] = u.x;	m_matrix[1][0] = u.y;   m_matrix[2][0] = u.z;   m_matrix[3][0] = T(0);
		m_matrix[0][1] = v.x;   m_matrix[1][1] = v.y;   m_matrix[2][1] = v.z;   m_matrix[3][1] = T(0);
		m_matrix[0][2] = n.x;   m_matrix[1][2] = n.y;   m_matrix[2][2] = n.z;   m_matrix[3][2] = T(0);
		m_matrix[0][3] = T(0);  m_matrix[1][3] = T(0);  m_matrix[2][3] = T(0);  m_matrix[3][3] = T(1);

		return *this;
	}

	inline Matrix4<T> InitRotationFromDirection(const Vector3<T>& forward, const Vector3<T>& up)
	{
		Vector3<T> n = forward.Normalized();
		Vector3<T> u = up.Normalized().Cross(n);
		Vector3<T> v = n.Cross(u);

		return InitRotationFromVectors(n, v, u);
	}

	inline Matrix4<T> InitPerspective(T fov, T aspectRatio, T zNear, T zFar)
	{
		const T zRange = zNear - zFar;
		const T tanHalfFOV = std::tan(fov / T(2));
		const T m1 = T(1) / (tanHalfFOV * aspectRatio);
		const T m2 = T(1) / tanHalfFOV;
		const T m3 = (-zNear - zFar) / zRange;
		const T m4 = (T(2) * zFar * zNear) / zRange;

		m_matrix[0][0] = m1;	m_matrix[1][0] = T(0);	m_matrix[2][0] = T(0);  m_matrix[3][0] = T(0);
		m_matrix[0][1] = T(0);	m_matrix[1][1] = m2;	m_matrix[2][1] = T(0);  m_matrix[3][1] = T(0);
		m_matrix[0][2] = T(0);	m_matrix[1][2] = T(0);  m_matrix[2][2] = m3;	m_matrix[3][2] = m4;
		m_matrix[0][3] = T(0);	m_matrix[1][3] = T(0);  m_matrix[2][3] = T(1);  m_matrix[3][3] = T(0);

		return *this;
	}
#undef far
#undef near
	inline Matrix4<T> InitOrthographic(T left, T right, T bottom, T top, T near, T far)
	{
		const T width = right - left;
		const T height = top - bottom;
		const T depth = far - near;

		m_matrix[0][0] = T(2) / width;	m_matrix[1][0] = T(0);			m_matrix[2][0] = T(0);			m_matrix[3][0] = -(right + left) / width;
		m_matrix[0][1] = T(0);			m_matrix[1][1] = T(2) / height;	m_matrix[2][1] = T(0);			m_matrix[3][1] = -(top + bottom) / height;
		m_matrix[0][2] = T(0);			m_matrix[1][2] = T(0);			m_matrix[2][2] = T(-2) / depth;	m_matrix[3][2] = -(far + near) / depth;
		m_matrix[0][3] = T(0);			m_matrix[1][3] = T(0);			m_matrix[2][3] = T(0);			m_matrix[3][3] = T(1);

		return *this;
	}
	
	inline Matrix4<T> Transpose() const
	{
		return Matrix4<T>(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2], m_matrix[0][3],
						  m_matrix[1][0], m_matrix[1][1], m_matrix[1][2], m_matrix[1][3],
						  m_matrix[2][0], m_matrix[2][1], m_matrix[2][2], m_matrix[2][3],
						  m_matrix[3][0], m_matrix[3][1], m_matrix[3][2], m_matrix[3][3]);
	}

	inline Matrix4<T> Inverse() const
	{
		T det = Determinant();

		if (det != T(0))
		{
			Matrix4<T> inverted;

			inverted[0][0] = m_matrix[1][2] * m_matrix[2][3] * m_matrix[3][1] - m_matrix[1][3] * m_matrix[2][2] * m_matrix[3][1] + m_matrix[1][3] * m_matrix[2][1] * m_matrix[3][2] - m_matrix[1][1] * m_matrix[2][3] * m_matrix[3][2] - m_matrix[1][2] * m_matrix[2][1] * m_matrix[3][3] + m_matrix[1][1] * m_matrix[2][2] * m_matrix[3][3];
			inverted[0][1] = m_matrix[0][3] * m_matrix[2][2] * m_matrix[3][1] - m_matrix[0][2] * m_matrix[2][3] * m_matrix[3][1] - m_matrix[0][3] * m_matrix[2][1] * m_matrix[3][2] + m_matrix[0][1] * m_matrix[2][3] * m_matrix[3][2] + m_matrix[0][2] * m_matrix[2][1] * m_matrix[3][3] - m_matrix[0][1] * m_matrix[2][2] * m_matrix[3][3];
			inverted[0][2] = m_matrix[0][2] * m_matrix[1][3] * m_matrix[3][1] - m_matrix[0][3] * m_matrix[1][2] * m_matrix[3][1] + m_matrix[0][3] * m_matrix[1][1] * m_matrix[3][2] - m_matrix[0][1] * m_matrix[1][3] * m_matrix[3][2] - m_matrix[0][2] * m_matrix[1][1] * m_matrix[3][3] + m_matrix[0][1] * m_matrix[1][2] * m_matrix[3][3];
			inverted[0][3] = m_matrix[0][3] * m_matrix[1][2] * m_matrix[2][1] - m_matrix[0][2] * m_matrix[1][3] * m_matrix[2][1] - m_matrix[0][3] * m_matrix[1][1] * m_matrix[2][2] + m_matrix[0][1] * m_matrix[1][3] * m_matrix[2][2] + m_matrix[0][2] * m_matrix[1][1] * m_matrix[2][3] - m_matrix[0][1] * m_matrix[1][2] * m_matrix[2][3];
			inverted[1][0] = m_matrix[1][3] * m_matrix[2][2] * m_matrix[3][0] - m_matrix[1][2] * m_matrix[2][3] * m_matrix[3][0] - m_matrix[1][3] * m_matrix[2][0] * m_matrix[3][2] + m_matrix[1][0] * m_matrix[2][3] * m_matrix[3][2] + m_matrix[1][2] * m_matrix[2][0] * m_matrix[3][3] - m_matrix[1][0] * m_matrix[2][2] * m_matrix[3][3];
			inverted[1][1] = m_matrix[0][2] * m_matrix[2][3] * m_matrix[3][0] - m_matrix[0][3] * m_matrix[2][2] * m_matrix[3][0] + m_matrix[0][3] * m_matrix[2][0] * m_matrix[3][2] - m_matrix[0][0] * m_matrix[2][3] * m_matrix[3][2] - m_matrix[0][2] * m_matrix[2][0] * m_matrix[3][3] + m_matrix[0][0] * m_matrix[2][2] * m_matrix[3][3];
			inverted[1][2] = m_matrix[0][3] * m_matrix[1][2] * m_matrix[3][0] - m_matrix[0][2] * m_matrix[1][3] * m_matrix[3][0] - m_matrix[0][3] * m_matrix[1][0] * m_matrix[3][2] + m_matrix[0][0] * m_matrix[1][3] * m_matrix[3][2] + m_matrix[0][2] * m_matrix[1][0] * m_matrix[3][3] - m_matrix[0][0] * m_matrix[1][2] * m_matrix[3][3];
			inverted[1][3] = m_matrix[0][2] * m_matrix[1][3] * m_matrix[2][0] - m_matrix[0][3] * m_matrix[1][2] * m_matrix[2][0] + m_matrix[0][3] * m_matrix[1][0] * m_matrix[2][2] - m_matrix[0][0] * m_matrix[1][3] * m_matrix[2][2] - m_matrix[0][2] * m_matrix[1][0] * m_matrix[2][3] + m_matrix[0][0] * m_matrix[1][2] * m_matrix[2][3];
			inverted[2][0] = m_matrix[1][1] * m_matrix[2][3] * m_matrix[3][0] - m_matrix[1][3] * m_matrix[2][1] * m_matrix[3][0] + m_matrix[1][3] * m_matrix[2][0] * m_matrix[3][1] - m_matrix[1][0] * m_matrix[2][3] * m_matrix[3][1] - m_matrix[1][1] * m_matrix[2][0] * m_matrix[3][3] + m_matrix[1][0] * m_matrix[2][1] * m_matrix[3][3];
			inverted[2][1] = m_matrix[0][3] * m_matrix[2][1] * m_matrix[3][0] - m_matrix[0][1] * m_matrix[2][3] * m_matrix[3][0] - m_matrix[0][3] * m_matrix[2][0] * m_matrix[3][1] + m_matrix[0][0] * m_matrix[2][3] * m_matrix[3][1] + m_matrix[0][1] * m_matrix[2][0] * m_matrix[3][3] - m_matrix[0][0] * m_matrix[2][1] * m_matrix[3][3];
			inverted[2][2] = m_matrix[0][1] * m_matrix[1][3] * m_matrix[3][0] - m_matrix[0][3] * m_matrix[1][1] * m_matrix[3][0] + m_matrix[0][3] * m_matrix[1][0] * m_matrix[3][1] - m_matrix[0][0] * m_matrix[1][3] * m_matrix[3][1] - m_matrix[0][1] * m_matrix[1][0] * m_matrix[3][3] + m_matrix[0][0] * m_matrix[1][1] * m_matrix[3][3];
			inverted[2][3] = m_matrix[0][3] * m_matrix[1][1] * m_matrix[2][0] - m_matrix[0][1] * m_matrix[1][3] * m_matrix[2][0] - m_matrix[0][3] * m_matrix[1][0] * m_matrix[2][1] + m_matrix[0][0] * m_matrix[1][3] * m_matrix[2][1] + m_matrix[0][1] * m_matrix[1][0] * m_matrix[2][3] - m_matrix[0][0] * m_matrix[1][1] * m_matrix[2][3];
			inverted[3][0] = m_matrix[1][2] * m_matrix[2][1] * m_matrix[3][0] - m_matrix[1][1] * m_matrix[2][2] * m_matrix[3][0] - m_matrix[1][2] * m_matrix[2][0] * m_matrix[3][1] + m_matrix[1][0] * m_matrix[2][2] * m_matrix[3][1] + m_matrix[1][1] * m_matrix[2][0] * m_matrix[3][2] - m_matrix[1][0] * m_matrix[2][1] * m_matrix[3][2];
			inverted[3][1] = m_matrix[0][1] * m_matrix[2][2] * m_matrix[3][0] - m_matrix[0][2] * m_matrix[2][1] * m_matrix[3][0] + m_matrix[0][2] * m_matrix[2][0] * m_matrix[3][1] - m_matrix[0][0] * m_matrix[2][2] * m_matrix[3][1] - m_matrix[0][1] * m_matrix[2][0] * m_matrix[3][2] + m_matrix[0][0] * m_matrix[2][1] * m_matrix[3][2];
			inverted[3][2] = m_matrix[0][2] * m_matrix[1][1] * m_matrix[3][0] - m_matrix[0][1] * m_matrix[1][2] * m_matrix[3][0] - m_matrix[0][2] * m_matrix[1][0] * m_matrix[3][1] + m_matrix[0][0] * m_matrix[1][2] * m_matrix[3][1] + m_matrix[0][1] * m_matrix[1][0] * m_matrix[3][2] - m_matrix[0][0] * m_matrix[1][1] * m_matrix[3][2];
			inverted[3][3] = m_matrix[0][1] * m_matrix[1][2] * m_matrix[2][0] - m_matrix[0][2] * m_matrix[1][1] * m_matrix[2][0] + m_matrix[0][2] * m_matrix[1][0] * m_matrix[2][1] - m_matrix[0][0] * m_matrix[1][2] * m_matrix[2][1] - m_matrix[0][1] * m_matrix[1][0] * m_matrix[2][2] + m_matrix[0][0] * m_matrix[1][1] * m_matrix[2][2];
				
			return inverted * (T(1) / det);
		}
		else
		{
			return Matrix4<T>().InitIdentity();
		}
	}

	inline T Determinant() const
	{
		return m_matrix[0][3] * m_matrix[1][2] * m_matrix[2][1] * m_matrix[3][0] - m_matrix[0][2] * m_matrix[1][3] * m_matrix[2][1] * m_matrix[3][0] - m_matrix[0][3] * m_matrix[1][1] * m_matrix[2][2] * m_matrix[3][0] + m_matrix[0][1] * m_matrix[1][3] * m_matrix[2][2] * m_matrix[3][0] +
			m_matrix[0][2] * m_matrix[1][1] * m_matrix[2][3] * m_matrix[3][0] - m_matrix[0][1] * m_matrix[1][2] * m_matrix[2][3] * m_matrix[3][0] - m_matrix[0][3] * m_matrix[1][2] * m_matrix[2][0] * m_matrix[3][1] + m_matrix[0][2] * m_matrix[1][3] * m_matrix[2][0] * m_matrix[3][1] +
			m_matrix[0][3] * m_matrix[1][0] * m_matrix[2][2] * m_matrix[3][1] - m_matrix[0][0] * m_matrix[1][3] * m_matrix[2][2] * m_matrix[3][1] - m_matrix[0][2] * m_matrix[1][0] * m_matrix[2][3] * m_matrix[3][1] + m_matrix[0][0] * m_matrix[1][2] * m_matrix[2][3] * m_matrix[3][1] +
			m_matrix[0][3] * m_matrix[1][1] * m_matrix[2][0] * m_matrix[3][2] - m_matrix[0][1] * m_matrix[1][3] * m_matrix[2][0] * m_matrix[3][2] - m_matrix[0][3] * m_matrix[1][0] * m_matrix[2][1] * m_matrix[3][2] + m_matrix[0][0] * m_matrix[1][3] * m_matrix[2][1] * m_matrix[3][2] +
			m_matrix[0][1] * m_matrix[1][0] * m_matrix[2][3] * m_matrix[3][2] - m_matrix[0][0] * m_matrix[1][1] * m_matrix[2][3] * m_matrix[3][2] - m_matrix[0][2] * m_matrix[1][1] * m_matrix[2][0] * m_matrix[3][3] + m_matrix[0][1] * m_matrix[1][2] * m_matrix[2][0] * m_matrix[3][3] +
			m_matrix[0][2] * m_matrix[1][0] * m_matrix[2][1] * m_matrix[3][3] - m_matrix[0][0] * m_matrix[1][2] * m_matrix[2][1] * m_matrix[3][3] - m_matrix[0][1] * m_matrix[1][0] * m_matrix[2][2] * m_matrix[3][3] + m_matrix[0][0] * m_matrix[1][1] * m_matrix[2][2] * m_matrix[3][3];
	}
	
	inline Matrix4<T> operator*(T rhs) const
	{
		Matrix4<T> ret;

		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
					ret.m_matrix[i][j] = m_matrix[i][j] * rhs;
			}
		}

		return ret;
	}

	inline Matrix4<T> operator*(const Matrix4<T>& rhs) const
	{
		Matrix4<T> ret;

		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				ret.m_matrix[i][j] = T(0);

				for (unsigned int k = 0; k < 4; k++)
				{
					ret.m_matrix[i][j] += m_matrix[k][j] * rhs.m_matrix[i][k];
				}
			}
		}
		return ret;
	}

	inline Matrix4<T>& operator*=(const Matrix4<T>& rhs)
	{
		*this = *this * rhs;

		return *this;
	}

	inline Vector3<T> Matrix4<T>::transformVector(const Vector3<T> &rhs) const
	{
		T x = rhs.x * m_matrix[0][0] + rhs.y * m_matrix[1][0] + rhs.z * m_matrix[2][0];
		T y = rhs.x * m_matrix[0][1] + rhs.y * m_matrix[1][1] + rhs.z * m_matrix[2][1];
		T z = rhs.x * m_matrix[0][2] + rhs.y * m_matrix[1][2] + rhs.z * m_matrix[2][2];

		return Vector3<T>(x, y, z);
	}

	inline Vector4<T> transformPoint(const Vector4<T>& rhs) const
	{
		Vector3<T> v4 = transformPoint(Vector3f(rhs.x, rhs.y, rhs.z));
		return Vector4<T>(v4.x, v4.y, v4.z, T(0));
	}
	
	inline Vector3<T> transformPoint(const Vector3<T>& rhs) const
	{
		T x = rhs.x * m_matrix[0][0] + rhs.y * m_matrix[1][0] + rhs.z * m_matrix[2][0] + m_matrix[3][0];
		T y = rhs.x * m_matrix[0][1] + rhs.y * m_matrix[1][1] + rhs.z * m_matrix[2][1] + m_matrix[3][1];
		T z = rhs.x * m_matrix[0][2] + rhs.y * m_matrix[1][2] + rhs.z * m_matrix[2][2] + m_matrix[3][2];
		T w = rhs.x * m_matrix[0][3] + rhs.y * m_matrix[1][3] + rhs.z * m_matrix[2][3] + m_matrix[3][3];

		return Vector3<T>(x / w, y / w, z / w);
	}

	inline Vector3<T> transformPointAffine(const Vector3<T>& rhs) const
	{
		T x = rhs.x * m_matrix[0][0] + rhs.y * m_matrix[1][0] + rhs.z * m_matrix[2][0] + m_matrix[3][0];
		T y = rhs.x * m_matrix[0][1] + rhs.y * m_matrix[1][1] + rhs.z * m_matrix[2][1] + m_matrix[3][1];
		T z = rhs.x * m_matrix[0][2] + rhs.y * m_matrix[1][2] + rhs.z * m_matrix[2][2] + m_matrix[3][2];

		return Vector3<T>(x, y, z);
	}

	inline Vector2<T> transformPoint(const Vector2<T>& rhs) const
	{
		Vector3<T> v3 = transformPoint(Vector3<T>(rhs.x, rhs.y, T(0)));

		return Vector2<T>(v3.x, v3.y);
	}

	inline Rect<T> transform(const Rect<T>& rhs) const
	{
		const std::vector<Vector2<T>> points =
		{
			transformPoint(Vector2<T>(rhs.left, rhs.top)),
			transformPoint(Vector2<T>(rhs.left, rhs.bottom)),
			transformPoint(Vector2<T>(rhs.right, rhs.top)),
			transformPoint(Vector2<T>(rhs.right, rhs.bottom))
		};

		return RectF(points);
	}

	inline const T* operator[](int index) const
	{
		return m_matrix[index];
	}

	inline T* operator[](int index)
	{
		return m_matrix[index];
	}

public:

	T m_matrix[4][4];
};

typedef Matrix4<float> Matrix4f;

std::ostream& operator<< (std::ostream& os, const Matrix4f& m);