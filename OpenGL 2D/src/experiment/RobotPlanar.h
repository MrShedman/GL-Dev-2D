#pragma once

#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"
#include "..\rendering\RenderTarget2D.h"

#include "RobotArm.h"
#include "Cuboid.hpp"

#include "..\gl\Buffer.hpp"

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
		RobotArm arm2(113, 0, -90, 90);
		RobotArm arm3(33, 0, -45, 45);

		pushBack(arm0);
		pushBack(arm1);
		pushBack(arm2);
		pushBack(arm3);

		m_cuboid.setPosition(Vector3f(200, 0, 0));

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

		std::vector<Vertex> m_vertices2;

		for (int i = m_links[1].getMinAngle(); i <= m_links[1].getMaxAngle(); i+=p)
		{
			for (int j = m_links[2].getMinAngle(); j <= m_links[2].getMaxAngle(); j += p)
			{
				for (int k = m_links[3].getMinAngle(); k < m_links[3].getMaxAngle(); k += p)
				{
					auto v = m_links[1].toMatrix(i).getPosition() + m_links[2].toMatrix(j).getPosition() + m_links[3].toMatrix(k).getPosition();
					
					m_vertices2.push_back(Vertex(v + m_links[0].toMatrix(0).getPosition(), Color::Black));
				}				
			}
		}

		m_vertices = convex_hull(m_vertices2);
		
		size_t ringSize = m_vertices.size();

		std::cout << ringSize << std::endl;

		for (int theta = 0; theta <= 360; ++theta)
		{
			rotate(Vector3f(0, 0, 0), Vector3f(0.f, 1.f, 0.f), ToRadians(1));

			Matrix4f transform = getTransform();

			for (int i = 0; i < ringSize; ++i)
			{
				Vector3f pos = transform.transformPointAffine(m_vertices[i].position);

				m_vertices.push_back(Vertex(pos, Color(160, 160, 160)));
			}
		}

		std::vector<Vertex> v;

		for (int theta = 0; theta <= 180; ++theta)
		{
			for (int i = 0; i < ringSize; ++i)
			{
				v.push_back(m_vertices[i + ringSize*theta]);
				v.push_back(m_vertices[(i)+ ringSize*(theta+1)]);
				v.push_back(m_vertices[(i+1) + ringSize*(theta+1)]);

				v.push_back(m_vertices[i + ringSize*theta]);
				v.push_back(m_vertices[(i + 1) + ringSize*(theta + 1)]);
				v.push_back(m_vertices[(i+1) + ringSize*(theta )]);
			}
		}

		m_vertices = v;

		m_verticesOutline.resize(m_vertices.size());

		for (int i = 0; i < v.size(); ++i)
		{
			m_verticesOutline[i] = v[i];
			m_verticesOutline[i].color = Color::Black;
			m_indices.push_back(i);
		}

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());

		setScale(0.1f, 0.1f, 0.1f);
	}

	float cross(const Vertex &O, const Vertex &A, const Vertex &B)
	{
		return (A.position.x - O.position.x) * (B.position.y - O.position.y) - (A.position.y - O.position.y) * (B.position.x - O.position.x);
	}

	// Returns a list of points on the convex hull in counter-clockwise order.
	// Note: the last point in the returned list is the same as the first one.
	std::vector<Vertex> convex_hull(std::vector<Vertex>& P)
	{
		int n = P.size(), k = 0;
		std::vector<Vertex> H(2 * n);

		// Sort points lexicographically
		std::sort(P.begin(), P.end(), [](Vertex v1, Vertex v2)
		{
			return v1.position.x < v2.position.x || (v1.position.x == v2.position.x && v1.position.y < v2.position.y);
		});

		// Build lower hull
		for (int i = 0; i < n; ++i) 
		{
			while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]) <= 0) k--;
			H[k++] = P[i];
		}

		// Build upper hull
		for (int i = n - 2, t = k + 1; i >= 0; i--) 
		{
			while (k >= t && cross(H[k - 2], H[k - 1], P[i]) <= 0) k--;
			H[k++] = P[i];
		}

		H.resize(k);
		return H;
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform *= getTransform();
		
		target.draw(&m_vertices[0], m_vertices.size(), Triangles, states, false);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		target.draw(&m_verticesOutline[0], m_verticesOutline.size(), Triangles, states, false);
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