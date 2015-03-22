#pragma once

#include "..\window\Window.hpp"
#include "..\window\Mouse.h"

#include "..\math\Vector.h"
#include "..\math\Ray.hpp"
#include "..\rendering\CameraPersp.hpp"

class MousePicker
{
public:

	static Rayf generateRay(const Window& window, const CameraPersp& camera)
	{
		Matrix4f mat = camera.getTransform();

		Vector3f view(mat[2][0], mat[2][1], mat[2][2]);

		Vector3f pos = camera.getPosition() + view*camera.getNearClippingDistance();

		if (!camera.isEngaged())
		{
			Vector2i mouse = Mouse::getPosition(window);
			Vector2i size = window.getSize();

			Vector3f h = view.Cross(camera.getRotation().GetUp()).Normalized();

			Vector3f v = h.Cross(view).Normalized();

			float fov_rad = camera.getFOV().asRadians();

			float vLength = std::tan(fov_rad / 2.0f) * camera.getNearClippingDistance();
			float hlength = vLength * camera.getAspectRatio();

			v *= vLength;
			h *= hlength;

			float x = mouse.x - (size.x / 2.0f);
			float y = mouse.y - (size.y / 2.0f);

			x /= (size.x / 2.0f);
			y /= (size.y / 2.0f);
			x = -x;
			y = -y;

			pos += h*x + v*y;
		}

		Vector3f dir = pos - camera.getPosition();

		return Rayf(pos, dir);
	}
};