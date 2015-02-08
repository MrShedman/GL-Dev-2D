#include "Plane.hpp"

bool Plane::intersects(Vector3f centre, float radius)
{
	float distanceFromSphereCenter = std::abs(m_normal.Dot(centre) + m_distance);

	float distanceFromSphere = distanceFromSphereCenter - radius;

	return distanceFromSphere < 0.0f;
}