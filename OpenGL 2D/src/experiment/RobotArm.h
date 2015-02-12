#pragma once

#include "Matrix33.h"
#include "..\math\MathHelper.h"

#include <iostream>

class RobotArm
{
public:

	RobotArm(float x, float y, int minAngle, int maxAngle)
	{
		m_trans = Vector2f(x, y);

		m_minAngle = minAngle;
		m_maxAngle = maxAngle;
	}

	Matrix33 toMatrix(int angle) const
	{
		float a = static_cast<float>(angle);
		float c = std::cos(ToRadians(a));
		float s = std::sin(ToRadians(a));

		return Matrix33(c, -s, m_trans.x,
			s, c, m_trans.y,
			0.0f, 0.0f, 1.0f);
	}

	int getMinAngle() const
	{
		return m_minAngle;
	}

	int getMaxAngle() const
	{
		return m_maxAngle;
	}

private:

	int m_minAngle;
	int m_maxAngle;

	Vector2f m_trans;
};