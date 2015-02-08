#pragma once

#include "Vector.h"

class Plane
{
public:

	Plane() :
		m_normal(Vector3f()),
		m_distance(0.f)
	{}

	Plane(const Vector3f& normal, float distance) :
		m_normal(normal.Normalized()),
		m_distance(distance / normal.Length()) {}

	Plane& Plane::operator=(Plane rhs)
	{
		m_normal = rhs.m_normal;
		m_distance = rhs.m_distance;
		return *this;
	}

	bool intersects(Vector3f centre, float radius);

	float distance(const Vector3f& point)
	{
		return m_normal.Dot(point) - m_distance;
	}

	inline const Vector3f& getNormal() const { return m_normal; }
	inline float getDistance()         const { return m_distance; }

private:

	Vector3f m_normal;
	float    m_distance;
};