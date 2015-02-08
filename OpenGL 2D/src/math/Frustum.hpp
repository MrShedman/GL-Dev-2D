#pragma once

#include "Plane.hpp"
#include "Matrix.h"

class Frustum
{
public:
	Frustum(const Matrix4f& matrix);

	Frustum(const Frustum& rhs)
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			p[i] = rhs.p[i];
		}
	}

#undef FAR
#undef  NEAR
	enum Type
	{
		NEAR = 0,
		FAR = 1,
		LEFT = 2,
		RIGHT = 3,
		UP = 4,
		DOWN = 5
	};

	bool intersection(const Vector3f& point);
	bool intersection(const Vector3f& centre, float radius);

	bool contains(const Vector3f& centre, float radius);

	Plane p[6];
};