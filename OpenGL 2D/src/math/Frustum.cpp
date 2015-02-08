#include "Frustum.hpp"

Frustum::Frustum(const Matrix4f& m)
{
	Vector3f right = Vector3f(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0]);

	p[RIGHT] = Plane(right, m[3][3] - m[3][0]);

	Vector3f left = Vector3f(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0]);

	p[LEFT] = Plane(left, m[3][3] + m[3][0]);

	Vector3f down = Vector3f(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1]);

	p[DOWN] = Plane(down, m[3][3] + m[3][1]);

	Vector3f up = Vector3f(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1]);

	p[UP] = Plane(up, m[3][3] - m[3][1]);

	Vector3f far = Vector3f(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2]);

	p[FAR] = Plane(far, m[3][3] - m[3][2]);

	Vector3f near = Vector3f(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2]);

	p[NEAR] = Plane(near, m[3][3] + m[3][2]);
}

bool Frustum::intersection(const Vector3f& point)
{
	for (int i = 0; i < 6; i++)
	{
		if (point.Dot(p[i].getNormal()) + p[i].getDistance() <= 0)
		{
			// The point was behind a side, so it ISN'T in the frustum
			return false;
		}
	}
}

bool Frustum::intersection(const Vector3f& centre, float radius)
{
	// Loop through each plane that comprises the frustum.
	for (int i = 0; i < 6; i++)
	{
		/*if (p[i].intersects(centre, radius))
		{
			return true;
		}*/

		if (centre.Dot(p[i].getNormal()) + p[i].getDistance() + radius <= 0)
		{
			return false;
		}
	}

	return true;
}


bool Frustum::contains(const Vector3f& centre, float radius)
{
	float distance;
	for (size_t i = 0; i < 6; ++i)
	{
		distance = p[i].distance(centre);
		if (distance < -radius)
			return false;
		else if (distance < radius)
			return false;
	}

	return true;
}