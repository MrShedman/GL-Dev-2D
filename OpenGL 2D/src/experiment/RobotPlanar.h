#pragma once

#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"
#include "..\rendering\RenderTarget2D.h"

#include "RobotArm.h"
#include "Cuboid.hpp"
#include "ConvexHull.hpp"

#include "..\gl\Buffer.hpp"

#include "..\window\Clock.h"

#include <vector>

class RobotPlanar : public Drawable, public Transform
{
public:

	RobotPlanar()
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_cuboid(Vector3f(60, 60, 60), Color::Black)
	{
		RobotArm arm0(-40.5, 65, 0, 0);
		RobotArm arm1(214.6, 40.5, -45, 15);
		RobotArm arm2(113, 0, -135, 135);
		RobotArm arm3(33, 0, -90, 90);
		RobotArm arm4(0, -60, 0, 0);

		pushBack(arm0);
		pushBack(arm1);
		pushBack(arm2);
		pushBack(arm3);
		pushBack(arm4);

		m_cuboid.setOrigin(-30.0f, 0.0f, 0.0f);
		m_cuboid.setPosition(Vector3f(120, 0, 0));

		//plot();
	}

	void pushBack(RobotArm& arm)
	{
		m_links.push_back(arm);
	}

	void plot()
	{
		Clock clock;

		int p = 1;

		RectF cube(60.f, 0.f, 150.f, 210.f);

		int count = 0;

		for (int i = m_links[0].getMinAngle(); i <= m_links[0].getMaxAngle(); i+=p)
		{
			for (int j = m_links[1].getMinAngle(); j <= m_links[1].getMaxAngle(); j += p)
			{
				for (int k = m_links[2].getMinAngle(); k <= m_links[2].getMaxAngle(); k += p)
				{
					for (int l = m_links[3].getMinAngle(); l <= m_links[3].getMaxAngle(); l += p)
					{
						count++;

						auto v0 = m_links[0].toMatrix(i).getPosition();
						auto v1 = m_links[1].toMatrix(i + j).getPosition();
						auto v2 = m_links[2].toMatrix(i + j + k).getPosition();
						auto v3 = m_links[3].toMatrix(i + j + k + l).getPosition();
						auto v4 = m_links[4].toMatrix(i + j + k + l).getPosition();
						
						if (cube.lineIntersects(v0, v0 + v1)
							|| cube.lineIntersects(v0 + v1, v0 + v1 + v2)
							|| cube.lineIntersects(v0 + v1 + v2, v0 + v1 + v2 + v3)
							|| cube.lineIntersects(v0 + v1 + v2 + v3, v0 + v1 + v2 + v3 + v4))
						{
							continue;
						}

						m_vertices.push_back(v0 + v1 + v2 + v3 + v4);						
					}
				}				
			}
		}

		std::cout << "# points tested: " << count << std::endl;

		auto yResult = std::max_element(m_vertices.begin(), m_vertices.end(), [&](Vertex v1, Vertex v2)->bool { return v1.position.y < v2.position.y; });

		int height = std::ceil(yResult->position.y);
		int interval = 2;

		std::vector<Vector3f> min;
		std::vector<Vector3f> max;

		for (int i = 0; i <= height / interval; ++i)
		{
			std::vector<Vector3f> temps;

			for (int j = 0; j < m_vertices.size(); ++j)
			{
				if (m_vertices[j].position.y > i*interval && m_vertices[j].position.y < (i + 1)*interval)
				{
					temps.push_back(m_vertices[j].position);
				}
			}
			if (!temps.empty())
			{
				auto xResult = std::minmax_element(temps.begin(), temps.end(), [&](Vector3f v1, Vector3f v2)->bool { return v1.x < v2.x; });
				
				Vector3f m = *xResult.first;
				if (m.x < 0.f)
				{
					min.emplace_back(0.f, m.y, m.z);
				}
				else
				{
					min.push_back(m);
				}
				max.push_back(*xResult.second);
			}
		}

		m_vertices.clear();
		m_vertices.reserve(min.size() + max.size());
		
		for (auto &m : min)
		{
			m_vertices.emplace_back(m, Color::Red);
		}

		std::reverse(max.begin(), max.end());
		
		for (auto &m : max)
		{
			m_vertices.emplace_back(m, Color::Green);
		}
		
		size_t ringSize = m_vertices.size();

		int accuracyDeg = 4;

		std::vector<Vertex> v;
		v.reserve(ringSize / (360 / accuracyDeg));
	
		for (int theta = 0; theta < 360 / accuracyDeg; ++theta)
		{
			rotate(Vector3f(0, 0, 0), Vector3f(0.f, 1.f, 0.f), degrees(accuracyDeg));

			Matrix4f transform = getTransform();

			for (int i = 0; i < ringSize; ++i)
			{
				Vector3f pos = transform.transformPointAffine(m_vertices[i].position);

				v.emplace_back(pos, m_vertices[i].color);
			}
		}
		std::cout << "v: " << v.size() << std::endl;

		m_vertices.clear();

		for (int theta = 0; theta <= 45; ++theta)
		{
			for (int i = 0; i < ringSize; ++i)
			{
				const Vertex bottomLeft = v[i + ringSize*theta];
				const Vertex bottomRight = v[i + ringSize*(theta + 1)];
				const Vertex topRight = v[(i + 1) + ringSize*(theta + 1)];
				const Vertex topLeft = v[(i + 1) + ringSize*(theta)];

				m_vertices.push_back(bottomLeft);
				m_vertices.push_back(bottomRight);
				m_vertices.push_back(topRight);

				m_vertices.push_back(bottomLeft);
				m_vertices.push_back(topRight);
				m_vertices.push_back(topLeft);
			}
		}

		size_t size = m_vertices.size();
		std::cout << "# points accepted: " << size << std::endl;
		m_verticesOutline.resize(size);
		m_indices.resize(size);

		for (size_t i = 0; i < size; ++i)
		{
			m_verticesOutline[i] = m_vertices[i];
			m_verticesOutline[i].color = Color::Black;
			m_indices[i] = i;
		}

		calcNormals();

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());

		setScale(0.1f, 0.1f, 0.1f);

		std::cout << "Generating swept volume took: " << clock.getElapsedTime().asMilliseconds() << "ms" << std::endl;
	}

	void calcNormals()
	{
		std::vector<Vector3f> m_normals;
		m_normals.reserve(m_vertices.size());

		for (unsigned int i = 0; i < m_vertices.size(); i++)
			m_normals.push_back(Vector3f(0, 0, 0));

		for (unsigned int i = 0; i < m_indices.size(); i += 3)
		{
			int i0 = m_indices[i];
			int i1 = m_indices[i + 1];
			int i2 = m_indices[i + 2];

			Vector3f v1 = m_vertices[i1].position - m_vertices[i0].position;
			Vector3f v2 = m_vertices[i2].position - m_vertices[i0].position;

			Vector3f normal = v1.Cross(v2).Normalized();

			m_normals[i0] = m_normals[i0] + normal;
			m_normals[i1] = m_normals[i1] + normal;
			m_normals[i2] = m_normals[i2] + normal;
		}

		for (unsigned int i = 0; i < m_normals.size(); i++)
		{
			//m_normals[i] = m_normals[i].Normalized();
			m_vertices[i].normals = m_normals[i].Normalized();
		}
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform *= getTransform();
		
		target.drawDeferred(&m_vertices[0], m_vertices.size(), Triangles, states);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		target.drawDeferred(&m_verticesOutline[0], m_verticesOutline.size(), Triangles, states);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();

		target.draw(m_cuboid, states);
	}
	
private:

	Cuboid m_cuboid;

	std::vector<RobotArm> m_links;

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;

	std::vector<Vertex> m_verticesOutline;
};