#pragma once

#include <vector>

#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"
#include "..\rendering\RenderTarget2D.h"

#include "..\gl\Buffer.hpp"

class Cuboid : public Drawable, public Transform
{
public:
	
	Cuboid()
		:
		Cuboid(Vector3f())
	{}

	Cuboid(Vector3f size, Color c = Color::Black)
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_size(size)
	{
		float x = m_size.x;
		float y = m_size.y;
		float z = m_size.z;

		Vector3f p1 = Vector3f(0, 0, 0);
		Vector3f p2 = Vector3f(0, y, 0);
		Vector3f p3 = Vector3f(x, y, 0);
		Vector3f p4 = Vector3f(x, 0, 0);
		Vector3f p5 = Vector3f(0, 0, z);
		Vector3f p6 = Vector3f(0, y, z);
		Vector3f p7 = Vector3f(x, y, z);
		Vector3f p8 = Vector3f(x, 0, z);

		m_vertices.push_back(p1);
		m_vertices.push_back(p2);
		m_vertices.push_back(p2);
		m_vertices.push_back(p3);
		m_vertices.push_back(p3);
		m_vertices.push_back(p4);
		m_vertices.push_back(p4);
		m_vertices.push_back(p1);

		m_vertices.push_back(p5);
		m_vertices.push_back(p6);
		m_vertices.push_back(p6);
		m_vertices.push_back(p7);
		m_vertices.push_back(p7);
		m_vertices.push_back(p8);
		m_vertices.push_back(p8);
		m_vertices.push_back(p5);

		m_vertices.push_back(p1);
		m_vertices.push_back(p5);

		m_vertices.push_back(p2);
		m_vertices.push_back(p6);

		m_vertices.push_back(p3);
		m_vertices.push_back(p7);

		m_vertices.push_back(p4);
		m_vertices.push_back(p8);

		for (int i = 0; i < 24; ++i)
		{
			m_vertices[i].color = c;
			m_indices.push_back(i);
		}

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());
	}


	Cuboid(Cuboid&& other) :
		Drawable(std::move(other)),
		Transform(std::move(other)),
		m_verticesBuffer(std::move(other.m_verticesBuffer)),
		m_indicesBuffer(std::move(other.m_indicesBuffer)),
		m_vertices(std::move(other.m_vertices)),
		m_indices(std::move(other.m_indices)),
		m_size(std::move(other.m_size))
	{

	}

	Cuboid& operator=(Cuboid&& other)
	{
		if (this != &other)
		{
			m_verticesBuffer = std::move(other.m_verticesBuffer);
			m_indicesBuffer = std::move(other.m_indicesBuffer);
			m_vertices = std::move(other.m_vertices);
			m_indices = std::move(other.m_indices);
			m_size = std::move(other.m_size);
		}

		return *this;
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform *= getTransform();

		target.drawDeferred(&m_vertices[0], m_vertices.size(), Lines, states);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();
	}

private:

	Vector3f m_size;

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
};