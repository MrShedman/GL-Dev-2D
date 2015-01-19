#pragma once

#include <math.h>
#include <vector>
#include <algorithm>

template<typename T>
class Vector2
{
public:

	Vector2<T>()
		:
		x(0),
		y(0)
	{ }

	Vector2<T>(T x, T y)
		:
		x(x),
		y(y)
	{ }

	Vector2<T>(const Vector2<T>& rhs)
		:
		x(rhs.x),
		y(rhs.y)
	{}

	template<typename U>
	Vector2<T>(const Vector2<U>& rhs)
		:
		x(static_cast<T>(rhs.x)),
		y(static_cast<T>(rhs.y))
	{}

	inline T Dot(const Vector2<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}

	inline T Cross(const Vector2<T>& rhs) const
	{
		return x * rhs.y - y * rhs.x;
	}

	inline Vector2<T> rotate(const T& angle) const
	{
		const T cosine = std::cos(angle);
		const T sine = std::sin(angle);
		return Vector2<T>(x * cosine - y * sine, x * sine + y * cosine);
	}

	inline Vector2<T> inverted() const
	{
		return Vector2<T>(-x, -y);
	}

	inline Vector2<T> Max(const Vector2<T>& rhs) const
	{
		Vector2<T> result;

		result.x = std::max(x, rhs.x);
		result.y = std::max(y, rhs.y);

		return result;
	}

	inline T Max() const
	{
		return std::max(x, y);
	}

	inline T LengthSq() const
	{
		return this->Dot(*this);
	}

	inline T Length() const
	{
		return std::sqrt(LengthSq());
	}
	inline Vector2<T> Normalized() const
	{
		return *this / Length();
	}

	inline Vector2<T> Lerp(const Vector2<T>& rhs, T lerpFactor) const
	{
		return (rhs - *this) * lerpFactor + *this;
	}

	inline Vector2<T> operator+(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x + rhs.x, y + rhs.y);
	}

	inline Vector2<T> operator-(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x - rhs.x, y - rhs.y);
	}

	inline Vector2<T> operator*(const T& rhs) const
	{
		return Vector2<T>(x * rhs, y * rhs);
	}

	inline Vector2<T> operator/(const T& rhs) const
	{
		return Vector2<T>(x / rhs, y / rhs);
	}

	inline Vector2<T>& operator+=(const Vector2<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	inline Vector2<T>& operator-=(const Vector2<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	inline Vector2<T>& operator*=(const T& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	inline Vector2<T>& operator/=(const T& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this
	}

	inline bool operator==(const Vector2<T>& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	inline bool operator!=(const Vector2<T>& rhs) const
	{
		return !operator==(rhs);
	}

public:

	T x;
	T y;
};

class Quaternion;

template<typename T>
class Vector3
{
public:

	Vector3<T>()
		:
		x(0),
		y(0),
		z(0)
	{ }

	Vector3<T>(T x, T y, T z)
		:
		x(x),
		y(y),
		z(z)
	{ }

	Vector3<T>(const Vector3<T>& rhs)
		:
		x(rhs.x),
		y(rhs.y),
		z(rhs.z)
	{}

	Vector3<T>(const Vector2<T>& rhs, float z = 0.f)
		:
		x(rhs.x),
		y(rhs.y),
		z(z)
	{}

	template<typename U>
	Vector3<T>(const Vector3<U>& rhs)
		:
		x(static_cast<T>(rhs.x)),
		y(static_cast<T>(rhs.y)),
		z(static_cast<T>(rhs.z))
	{}


	inline T Dot(const Vector3<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	inline Vector3<T> Cross(const Vector3<T>& rhs) const
	{
		T nx = y * rhs.z - z * rhs.y;
		T ny = z * rhs.x - x * rhs.z;
		T nz = x * rhs.y - y * rhs.x;

		return Vector3<T>(nx, ny, nz);
	}

	inline Vector3<T> Rotate(T angle, const Vector3<T>& axis) const
	{
		const T cosine = std::cos(-angle);
		const T sine = std::sin(-angle);

		return this->Cross(axis * sine) + (*this * cosine) + axis * this->Dot(axis * (1 - cosine));
	}

	inline Vector3<T> inverted() const
	{
		return Vector3<T>(-x, -y, -z);
	}

	inline Vector3<T> Reflect(const Vector3<T>& normal) const
	{
		return *this - (normal * (this->Dot(normal) * 2));
	}

	Vector3<T> Rotate(const Quaternion& rotation) const;

	inline Vector3<T> Max(const Vector3<T>& rhs) const
	{
		Vector3<T> result;

		result.x = std::max(x, rhs.x);
		result.y = std::max(y, rhs.y);
		result.z = std::max(z, rhs.z);

		return result;
	}

	inline T Max() const
	{
		T max = (x < y) ? y : x;
		return ((max < z) ? z : max);
	}

	inline T LengthSq() const
	{
		return this->Dot(*this);
	}

	inline T Length() const
	{
		return std::sqrt(LengthSq());
	}

	inline Vector3<T> Normalized() const
	{
		return *this / Length();
	}

	inline Vector3<T> Lerp(const Vector3<T>& rhs, T lerpFactor) const
	{
		return (rhs - *this) * lerpFactor + *this;
	}

	inline Vector3<T> operator+(const Vector3<T>& rhs) const
	{
		return Vector3f(x + rhs.x, y + rhs.y, z + rhs.z);

	}
	inline Vector3<T> operator-(const Vector3<T>& rhs) const
	{
		return Vector3f(x - rhs.x, y - rhs.y, z - rhs.z);

	}
	inline Vector3<T> operator*(T rhs) const
	{
		return Vector3f(x * rhs, y * rhs, z * rhs);
	}

	inline Vector3<T> operator/(T rhs) const
	{
		return Vector3f(x / rhs, y / rhs, z / rhs);
	}

	inline bool operator==(const Vector3<T>& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	inline bool operator!=(const Vector3<T>& rhs) const
	{
		return !operator==(rhs);
	}

	inline Vector3<T>& operator+=(const Vector3<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	inline Vector3<T>& operator-=(const Vector3<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	inline Vector3<T>& operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;

		return *this;
	}

	inline Vector3<T>& operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;

		return *this;
	}

public:

	T x;
	T y;
	T z;
};

template<typename T>
class Vector4
{
public:

	Vector4<T>()
		:
		x(0),
		y(0),
		z(0),
		w(0)
	{ }

	Vector4<T>(T x, T y, T z, T w)
		:
		x(x),
		y(y),
		z(z),
		w(w)
	{ }

	Vector4<T>(const Vector3<T>& rhs)
		:
		x(rhs.x),
		y(rhs.y),
		z(rhs.z),
		w(T(1))
	{}

	Vector4<T>(const Vector4<T>& rhs)
		:
		x(rhs.x),
		y(rhs.y),
		z(rhs.z),
		w(rhs.w)
	{}

	template<typename U>
	Vector4<T>(const Vector4<U>& rhs)
		:
		x(static_cast<T>(rhs.x)),
		y(static_cast<T>(rhs.y)),
		z(static_cast<T>(rhs.z)),
		w(static_cast<T>(rhs.w))
	{}

	inline T Dot(const Vector4<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	inline Vector4<T> Max(const Vector4<T>& rhs) const
	{
		Vector4<T> result;

		result.x = std::max(x, rhs.x);
		result.y = std::max(y, rhs.y);
		result.z = std::max(z, rhs.z);
		result.w = std::max(w, rhs.w);

		return result;
	}

	inline T Max() const
	{
		std::vector<T> values = { x, y, z, w };

		return *std::max_element(values.begin(), values.end());
	}

	inline T LengthSq() const
	{
		return this->Dot(*this);
	}

	inline T Length() const
	{
		return std::sqrt(LengthSq());
	}

	inline Vector4<T> Normalized() const
	{
		return *this / Length();
	}

	inline Vector4<T> Lerp(const Vector4<T>& rhs, T lerpFactor) const
	{
		return (rhs - *this) * lerpFactor + *this;
	}

	inline Vector4<T> operator+(const Vector4<T>& rhs) const
	{
		return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	inline Vector4<T> operator-(const Vector4<T>& rhs) const
	{
		return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	inline Vector4<T> operator*(T rhs) const
	{
		return Vector4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	inline Vector4<T> operator/(T rhs) const
	{
		return Vector4<T>(x / rhs, y / rhs, z / rhs, w / rhs);
	}

	inline bool operator==(const Vector4<T>& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}

	inline bool operator!=(const Vector4<T>& rhs) const
	{
		return !operator==(rhs);
	}

	inline Vector4<T>& operator+=(const Vector4<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;

		return *this;
	}

	inline Vector4<T>& operator-=(const Vector4<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;

		return *this;
	}

	inline Vector4<T>& operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;

		return *this;
	}

	inline Vector4<T>& operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;

		return *this;
	}

public:

	T x;
	T y;
	T z;
	T w;
};

typedef Vector2<unsigned int> Vector2u;
typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;

std::ostream& operator<< (std::ostream& os, const Vector3f& v);