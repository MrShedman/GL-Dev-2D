#pragma once

#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\RenderTarget2D.h"

#include "..\gl\Buffer.hpp"

#include <vector>

class Quad : public Drawable
{
public:

	Quad()
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
	{
		m_vertices.emplace_back(Vector3f(-1.0f, -1.0f, 0.f));
		m_vertices.emplace_back(Vector3f(1.0f, -1.0f, 0.f));
		m_vertices.emplace_back(Vector3f(-1.0f, 1.0f, 0.f));

		m_vertices.emplace_back(Vector3f(1.0f, -1.0f, 0.f));
		m_vertices.emplace_back(Vector3f(1.0f, 1.0f, 0.f));
		m_vertices.emplace_back(Vector3f(-1.0f, 1.0f, 0.f));

		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
		m_indices.push_back(3);
		m_indices.push_back(4);
		m_indices.push_back(5);

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());
	}
	
	Quad(Quad&& other) :
		Drawable(std::move(other)),
		m_verticesBuffer(std::move(other.m_verticesBuffer)),
		m_indicesBuffer(std::move(other.m_indicesBuffer)),
		m_vertices(std::move(other.m_vertices)),	
		m_indices(std::move(other.m_indices))
	
	{

	}

	Quad& operator=(Quad&& other)
	{
		if (this != &other)
		{
			m_verticesBuffer = std::move(other.m_verticesBuffer);
			m_indicesBuffer = std::move(other.m_indicesBuffer);
			m_vertices = std::move(other.m_vertices);
			m_indices = std::move(other.m_indices);
		}

		return *this;
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform = Matrix4f().InitIdentity();

		target.drawDeferred(&m_vertices[0], m_vertices.size(), Triangles, states);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();
	}


private:

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
};