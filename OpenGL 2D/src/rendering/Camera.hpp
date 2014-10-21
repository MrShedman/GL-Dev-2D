#pragma once

#include "..\math\Matrix.h"
#include "..\math\Rect.h"
#include <iostream>
class Camera
{
public:

	Camera(RectF bounds = RectF())
	{
		if (bounds != RectF())
		{
			ortho.InitOrthographic(bounds.left, bounds.right, bounds.bottom, bounds.top, 1.f, -1.f);
		}
		else
		{
			ortho.InitIdentity();
		}
	}

	Matrix4f& getTransform()
	{
		return ortho;
	}

private:

	Matrix4f ortho;
};