#pragma once

#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"
#include "..\rendering\RenderTarget2D.h"

#include "RobotArm.h"

#include "..\gl\Buffer.hpp"

#include <vector>

class RobotPlanar : public Drawable, public Transform
{
public:

	RobotPlanar()
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
	{
		RobotArm arm1(0, 50, 0, 0, 360);
		RobotArm arm2(90, 50, 0, -45, 45);
		RobotArm arm3(-90, 50, 0, -225, -180);
		RobotArm arm4(0, 50, 0, -45, 45);

		pushBack(arm1);
		pushBack(arm2);
		pushBack(arm3);
		pushBack(arm4);

		plot();
	}

	void pushBack(RobotArm& arm)
	{
		m_links.push_back(arm);
	}

	void plot()
	{
		int count = 0;
		int p = 5;

		std::vector<GLuint> m_indices2;
		std::vector<Vertex> m_vertices2;

		for (int i = m_links[0].getMinAngle(); i < m_links[0].getMaxAngle(); i+=p)
		{
			for (int j = m_links[1].getMinAngle(); j < m_links[1].getMaxAngle(); j += p)
			{
				for (int k = m_links[2].getMinAngle(); k < m_links[2].getMaxAngle(); k += p)
				{
					for (int l = m_links[3].getMinAngle(); l < m_links[3].getMaxAngle(); l += p)
					{
						Matrix33 m = m_links[0].toMatrix(i) * m_links[1].toMatrix(j)*m_links[2].toMatrix(k)*m_links[3].toMatrix(l);

						float x = m[2][0];
						float y = m[2][1];

						m_vertices.push_back(Vertex(Vector3f(x, y, 0), Color::Black));
						m_indices.push_back(count++);
					}
				}
			}
		}
		std::cout << m_vertices.size() << std::endl;

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform *= getTransform();

		target.draw(&m_vertices[0], m_vertices.size(), Points, states, false);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();
	}
	
private:

	std::vector<RobotArm> m_links;

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
};