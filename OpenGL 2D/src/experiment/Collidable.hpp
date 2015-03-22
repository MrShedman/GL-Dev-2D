#pragma once

#include "..\math\Vector.h"
#include "..\math\Ray.hpp"

class Collidable
{
public:

	struct Result
	{
		bool intersects;
		Vector3f intersection;
		Vector3f normal;
	};

	virtual ~Collidable(){}

	virtual Result doesIntersect(const Rayf& ray) = 0;
};