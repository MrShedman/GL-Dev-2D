#pragma once

#include "Vector.h"
#include "Matrix.h"
#include <iostream>

class Quaternion : public Vector4<float>
{
public:
	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
		:
		Vector4(x, y, z, w)
	{}

	Quaternion(const Vector4<float>& rhs)
		:
		Vector4(rhs)
	{}

	Quaternion(const Vector3f& axis, float angle)
	{
		float sinHalfAngle = std::sin(angle / 2.f);
		float cosHalfAngle = std::cos(angle / 2.f);

		x = axis.x * sinHalfAngle;
		y = axis.y * sinHalfAngle;
		z = axis.z * sinHalfAngle;
		w = cosHalfAngle;
	}

	Quaternion(const Matrix4f& m)
	{
		float trace = m[0][0] + m[1][1] + m[2][2];

		if (trace > 0)
		{
			float s = 0.5f / std::sqrt(trace + 1.0f);
			x = (m[1][2] - m[2][1]) * s;
			y = (m[2][0] - m[0][2]) * s;
			z = (m[0][1] - m[1][0]) * s;
			w = 0.25f / s;
		}
		else if (m[0][0] > m[1][1] && m[0][0] > m[2][2])
		{
			float s = 2.0f * std::sqrt(1.0f + m[0][0] - m[1][1] - m[2][2]);
			x = 0.25f * s;
			y = (m[1][0] + m[0][1]) / s;
			z = m[0][2] / s;
			w = (m[1][2] - m[2][1]) / s;
		}
		else if (m[1][1] > m[2][2])
		{
			float s = 2.0f * std::sqrt(1.0f + m[1][1] - m[0][0] - m[2][2]);
			x = (m[1][0] + m[0][1]) / s;
			y = 0.25f * s;
			z = (m[2][1] + m[1][2]) / s;
			w = (m[2][0] - m[0][2]) / s;
		}
		else
		{
			float s = 2.0f * std::sqrt(1.0f + m[2][2] - m[1][1] - m[0][0]);
			x = (m[2][0] + m[0][2]) / s;
			y = (m[1][2] + m[2][1]) / s;
			z = 0.25f * s;
			w = (m[0][1] - m[1][0]) / s;
		}

		float length = Length();

		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	inline Quaternion NLerp(const Quaternion& quat, float lerpFactor, bool shortestPath) const
	{
		Quaternion correctedDest;

		if (shortestPath && this->Dot(quat) < 0)
		{
			correctedDest = quat * -1;
		}
		else
		{
			correctedDest = quat;
		}

		return Quaternion(Lerp(correctedDest, lerpFactor).Normalized());
	}

	inline Quaternion SLerp(const Quaternion& quat, float lerpFactor, bool shortestPath) const
	{
		const float EPSILON = 1e3;

		float cos = this->Dot(quat);
		Quaternion correctedDest;

		if (shortestPath && cos < 0)
		{
			cos *= -1;
			correctedDest = quat * -1;
		}
		else
		{
			correctedDest = quat;
		}

		if (std::abs(cos) > (1 - EPSILON))
		{
			return NLerp(correctedDest, lerpFactor, false);
		}

		float sin = std::sqrt(1.0f - cos * cos);
		float angle = std::atan2(sin, cos);
		float invSin = 1.0f / sin;

		float srcFactor = std::sin((1.0f - lerpFactor) * angle) * invSin;
		float destFactor = std::sin(lerpFactor* angle) * invSin;

		return Quaternion((*this) * srcFactor + correctedDest * destFactor);
	}

	inline Matrix4f ToRotationMatrix() const
	{
		Vector3f forward = Vector3f(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
		Vector3f up = Vector3f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));
		Vector3f right = Vector3f(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y));

		return Matrix4f().InitRotationFromVectors(forward, up, right);
	}

	inline Vector3f GetForward() const
	{
		return Vector3f(0, 0, 1).Rotate(*this);
	}

	inline Vector3f GetBack() const
	{
		return Vector3f(0, 0, -1).Rotate(*this);
	}

	inline Vector3f GetUp() const
	{
		return Vector3f(0, 1, 0).Rotate(*this);
	}

	inline Vector3f GetDown() const
	{
		return Vector3f(0, -1, 0).Rotate(*this);
	}

	inline Vector3f GetRight() const
	{
		return Vector3f(1, 0, 0).Rotate(*this);
	}

	inline Vector3f GetLeft() const
	{
		return Vector3f(-1, 0, 0).Rotate(*this);
	}

	inline Quaternion Conjugate() const 
	{ 
		return Quaternion(-x, -y, -z, w)/LengthSq();
	}
	
	inline Quaternion operator*(float rhs) const
	{
		return Quaternion(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	inline Quaternion operator*(const Quaternion& q) const
	{
		const float nw = (w * q.w) - (x * q.x) - (y * q.y) - (z * q.z);
		const float nx = (x * q.w) + (w * q.x) + (y * q.z) - (z * q.y);
		const float ny = (y * q.w) + (w * q.y) + (z * q.x) - (x * q.z);
		const float nz = (z * q.w) + (w * q.z) + (x * q.y) - (y * q.x);

		return Quaternion(nx, ny, nz, nw);
	}

	inline Quaternion operator*(const Vector3<float>& v) const
	{
		return *this * Quaternion(v.x, v.y, v.z, 0.0f);
	}
};

std::ostream& operator<< (std::ostream& os, const Quaternion& q);