#pragma once

#include "Matrix33.h"
#include "..\math\MathHelper.h"

#include <iostream>

class RobotArm
{
public:

	RobotArm(float angle, float x, float y, int minAngle, int maxAngle)
	{
		m_angle = angle;
		m_trans = Vector2f(x, y);

		m_minAngle = minAngle;
		m_maxAngle = maxAngle;
	}

	void outPutMatrix()
	{
		std::cout << toMatrix(m_angle) << std::endl;
	}

	Matrix33 toMatrix(float angle) const
	{
		float c = std::cos(ToRadians(angle));
		float s = std::sin(ToRadians(angle));

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

	float m_angle;
	Vector2f m_trans;
};