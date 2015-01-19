#pragma once

#include <vector>

#include "..\rendering\RenderTarget2D.h"
#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\math\Vector.h"
#include "..\math\MathHelper.h"


class Block : public Drawable, public Transform
{
public:

	Block()
	{
		m_verticesVBO = 0;
		m_indicesVBO = 0;
		m_texture = nullptr;
	}

		
	void create(float size)
	{
		addVertex(-1.0f, -1.0f, -1.0f); // triangle 1 : begin
		addVertex(-1.0f, -1.0f, 1.0f);
		addVertex(-1.0f, 1.0f, 1.0f); // triangle 1 : end
		addVertex(1.0f, 1.0f, -1.0f); // triangle 2 : begin
		addVertex(-1.0f, -1.0f, -1.0f);
		addVertex(-1.0f, 1.0f, -1.0f); // triangle 2 : end
		addVertex(1.0f, -1.0f, 1.0f);
		addVertex(-1.0f, -1.0f, -1.0f);
		addVertex(1.0f, -1.0f, -1.0f);
		addVertex(1.0f, 1.0f, -1.0f);
		addVertex(1.0f, -1.0f, -1.0f);
		addVertex(-1.0f, -1.0f, -1.0f);
		addVertex(-1.0f, -1.0f, -1.0f);
		addVertex(-1.0f, 1.0f, 1.0f);
		addVertex(-1.0f, 1.0f, -1.0f);
		addVertex(1.0f, -1.0f, 1.0f);
		addVertex(-1.0f, -1.0f, 1.0f);
		addVertex(-1.0f, -1.0f, -1.0f);
		addVertex(-1.0f, 1.0f, 1.0f);
		addVertex(-1.0f, -1.0f, 1.0f);
		addVertex(1.0f, -1.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(1.0f, -1.0f, -1.0f);
		/*addVertex(1.0f, 1.0f, -1.0f);
		addVertex(1.0f, -1.0f, -1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(1.0f, -1.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(1.0f, 1.0f, -1.0f);
		addVertex(-1.0f, 1.0f, -1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(-1.0f, 1.0f, -1.0f);
		addVertex(-1.0f, 1.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f);
		addVertex(-1.0f, 1.0f, 1.0f);
		addVertex(1.0f, -1.0f, 1.0f);
		

			// bottom
		/*addVertex(-size, -size, -size, 0.0f, 0.0f);
		addVertex(size, -size, -size, 1.0f, 0.0f);
		addVertex(-size, -size, size, 0.0f, 1.0f);
		addVertex(size, -size, -size, 1.0f, 0.0f);
		addVertex(size, -size, size, 1.0f, 1.0f);
		addVertex(-size, -size, size, 0.0f, 1.0f);

			// top
		addVertex(-size, size, -size, 0.0f, 0.0f);
		addVertex(-size, size, size, 0.0f, 1.0f);
		addVertex(size, size, -size, 1.0f, 0.0f);
		addVertex(size, size, -size, 1.0f, 0.0f);
		addVertex(-size, size, size, 0.0f, 1.0f);
		addVertex(size, size, size, 1.0f, 1.0f);

			// front
		addVertex(-size, -size, size, 1.0f, 0.0f);
		addVertex(size, -size, size, 0.0f, 0.0f);
		addVertex(-size, size, size, 1.0f, 1.0f);
		addVertex(size, -size, size, 0.0f, 0.0f);
		addVertex(size, size, size, 0.0f, 1.0f);
		addVertex(-size, size, size, 1.0f, 1.0f);

				// back
		addVertex(-size, -size, -size, 0.0f, 0.0f);
		addVertex(-size, size, -size, 0.0f, 1.0f);
		addVertex(size, -size, -size, 1.0f, 0.0f);
		addVertex(size, -size, -size, 1.0f, 0.0f);
		addVertex(-size, size, -size, 0.0f, 1.0f);
		addVertex(size, size, -size, 1.0f, 1.0f);

				// left
		addVertex(-size, -size, size, 0.0f, 1.0f);
		addVertex(-size, size, -size, 1.0f, 0.0f);
		addVertex(-size, -size, -size, 0.0f, 0.0f);
		addVertex(-size, -size, size, 0.0f, 1.0f);
		addVertex(-size, size, size, 1.0f, 1.0f);
		addVertex(-size, size, -size, 1.0f, 0.0f);

				// right
		addVertex(size, -size, size, 1.0f, 1.0f);
		addVertex(size, -size, -size, 1.0f, 0.0f);
		addVertex(size, size, -size, 0.0f, 0.0f);
		addVertex(size, -size, size, 1.0f, 1.0f);
		addVertex(size, size, -size, 0.0f, 0.0f);
		addVertex(size, size, size, 0.0f, 1.0f);*/

		genVBO();
	}

	~Block()
	{
		if (m_verticesVBO != 0)
		{
			glDeleteBuffers(1, &m_verticesVBO);
			glDeleteBuffers(1, &m_indicesVBO);
		}
	}

	void genVBO()
	{
		if (m_verticesVBO != 0)
		{
			glDeleteBuffers(1, &m_verticesVBO);
			glDeleteBuffers(1, &m_indicesVBO);
		}

		//shape VBO
		size_t verticesSize = m_vertices.size();

		//Vertex data
		std::vector<GLuint> indices(verticesSize);

		for (size_t i = 0; i < verticesSize; ++i)
		{
			indices[i] = i;
		}

		//Create VBO
		glGenBuffers(1, &m_verticesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
		glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

		//Create IBO
		glGenBuffers(1, &m_indicesVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesSize * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		//Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVBO);

		states.transform *= getTransform();

		// Render the inside
		states.texture = nullptr;
		target.draw(&m_vertices[0], m_vertices.size(), Triangles, states, false);
	}

	void addVertex(float x, float y, float z, float u = 0, float v = 0)
	{
		float r = randomize(0, 255);
		float g = randomize(0, 255);
		float b = randomize(0, 255);

		m_vertices.push_back(Vertex(Vector3f(x, y, z), Color(r, g, b), Vector2f(u, v)));
	}

	void setTexture(Texture *t)
	{
		m_texture = t;
	}

	Texture* m_texture;

	GLuint m_verticesVBO;
	GLuint m_indicesVBO;

	std::vector<Vertex> m_vertices;
};