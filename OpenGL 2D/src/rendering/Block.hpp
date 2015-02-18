#pragma once

#include <vector>

#include "..\rendering\RenderTarget2D.h"
#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\math\Vector.h"
#include "..\math\MathHelper.h"

#include "..\gl\Buffer.hpp"

class Block : public Drawable, public Transform
{
public:

	Block()
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_texture(nullptr)
	{}

	Block(Block&& other) :
		Drawable(std::move(other)),
		Transform(std::move(other)),
		m_verticesBuffer(std::move(other.m_verticesBuffer)),
		m_indicesBuffer(std::move(other.m_indicesBuffer)),
		m_vertices(std::move(other.m_vertices)),
		m_indices(std::move(other.m_indices))
	{
		m_texture = other.m_texture;
	}

	Block& operator=(Block&& other)
	{
		if (this != &other)
		{
			m_verticesBuffer = std::move(other.m_verticesBuffer);
			m_indicesBuffer = std::move(other.m_indicesBuffer);
			m_vertices = std::move(other.m_vertices);
			m_indices = std::move(other.m_indices);

			m_texture = other.m_texture;
		}

		return *this;
	}
		
	void create(float size)
	{
		//// bottom
		//addVertex(-size, -size, -size, 0.0f, 0.0f);
		//addVertex(size, -size, -size, 1.0f, 0.0f);
		//addVertex(-size, -size, size, 0.0f, 1.0f);
		//addVertex(size, -size, -size, 1.0f, 0.0f);
		//addVertex(size, -size, size, 1.0f, 1.0f);
		//addVertex(-size, -size, size, 0.0f, 1.0f);

		//	// top
		//addVertex(-size, size, -size, 0.0f, 0.0f);
		//addVertex(-size, size, size, 0.0f, 1.0f);
		//addVertex(size, size, -size, 1.0f, 0.0f);
		//addVertex(size, size, -size, 1.0f, 0.0f);
		//addVertex(-size, size, size, 0.0f, 1.0f);
		//addVertex(size, size, size, 1.0f, 1.0f);

		//	// front
		//addVertex(-size, -size, size, 1.0f, 0.0f);
		//addVertex(size, -size, size, 0.0f, 0.0f);
		//addVertex(-size, size, size, 1.0f, 1.0f);
		//addVertex(size, -size, size, 0.0f, 0.0f);
		//addVertex(size, size, size, 0.0f, 1.0f);
		//addVertex(-size, size, size, 1.0f, 1.0f);

		//		// back
		//addVertex(-size, -size, -size, 0.0f, 0.0f);
		//addVertex(-size, size, -size, 0.0f, 1.0f);
		//addVertex(size, -size, -size, 1.0f, 0.0f);
		//addVertex(size, -size, -size, 1.0f, 0.0f);
		//addVertex(-size, size, -size, 0.0f, 1.0f);
		//addVertex(size, size, -size, 1.0f, 1.0f);

		//		// left
		//addVertex(-size, -size, size, 0.0f, 1.0f);
		//addVertex(-size, size, -size, 1.0f, 0.0f);
		//addVertex(-size, -size, -size, 0.0f, 0.0f);
		//addVertex(-size, -size, size, 0.0f, 1.0f);
		//addVertex(-size, size, size, 1.0f, 1.0f);
		//addVertex(-size, size, -size, 1.0f, 0.0f);

		//		// right
		//addVertex(size, -size, size, 1.0f, 1.0f);
		//addVertex(size, -size, -size, 1.0f, 0.0f);
		//addVertex(size, size, -size, 0.0f, 0.0f);
		//addVertex(size, -size, size, 1.0f, 1.0f);
		//addVertex(size, size, -size, 0.0f, 0.0f);
		//addVertex(size, size, size, 0.0f, 1.0f);

		addVertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
		addVertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f);
		addVertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);
		addVertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f);
		addVertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
		addVertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);

		// top
		addVertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		addVertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		addVertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		addVertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		addVertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		addVertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);

		// front
		addVertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		addVertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		addVertex(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		addVertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		addVertex(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		// back
		addVertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		addVertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f);
		addVertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		addVertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		addVertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f);
		addVertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f);

		// left
		addVertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		addVertex(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		addVertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		addVertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		addVertex(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		addVertex(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f);

		// right
		addVertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
		addVertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		addVertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		addVertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
		addVertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		addVertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);

		m_indices.resize(m_vertices.size());

		for (size_t i = 0; i < m_vertices.size(); ++i)
		{
			m_indices[i] = i;
		}
		setScale(size, size, size);
		//calcNormals();

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());
	}

	std::vector<Vector3f> getPositionData() const
	{
		std::vector<Vector3f> positions;

		for (auto &v : m_vertices)
		{
			positions.push_back(v.position);
		}
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.texture = m_texture;
		states.transform *= getTransform();

		target.draw(&m_vertices[0], m_vertices.size(), Triangles, states, false, true);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();
	}

	void addVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
	{
		static int count = 0;

		static Color c = Color::Magenta;

		if (count == 6)
		{
			unsigned char r = static_cast<unsigned char>(randomize(0, 255));
			unsigned char g = static_cast<unsigned char>(randomize(0, 255));
			unsigned char b = static_cast<unsigned char>(randomize(0, 255));
			c = Color(r, g, b);
			count = 0;
		}
		//c = Color::White;
		count++;
		m_vertices.push_back(Vertex(Vector3f(x, y, z), c, Vector2f(u, v), Vector3f(nx, ny, nz)));
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

	void setTexture(const Texture& texture)
	{
		m_texture = &texture;
	}

	const Texture* m_texture;

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
};