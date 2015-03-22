#pragma once

#include "Vector.h"

template <class T>
class Ray
{
public:

	Ray<T>()
		:
		origin(Vector3<T>()),
		direction(Vector3<T>())
	{}

	Ray<T>(const Vector3<T>& origin, const Vector3<T>& direction)
		:
		origin(origin),
		direction(direction.Normalized())
	{}

	Vector3<T> origin;
	Vector3<T> direction;
};

typedef Ray<float> Rayf;