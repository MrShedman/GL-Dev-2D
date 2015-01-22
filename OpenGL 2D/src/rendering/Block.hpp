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
		vao = 0;
		m_texture = nullptr;
	}

		
	void create(float size)
	{
		/*addVertex(-1.0f, -1.0f, -1.0f); // triangle 1 : begin
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
		addVertex(1.0f, 1.0f, -1.0f);
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
		addVertex(1.0f, -1.0f, 1.0f);*/
		

			// bottom
		addVertex(-size, -size, -size, 0.0f, 0.0f);
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
		addVertex(size, size, size, 0.0f, 1.0f);

		indices.resize(m_vertices.size());

		for (size_t i = 0; i < m_vertices.size(); ++i)
		{
			indices[i] = i;
		}


		genVBO();
	}

	~Block()
	{
		if (m_verticesVBO != 0)
		{
			//glDeleteBuffers(1, &m_verticesVBO);
		}
		if (m_indicesVBO != 0)
		{
			//glDeleteBuffers(1, &m_indicesVBO);
		}

		//std::cout << "destruction" << std::endl;
	}

	void genVBO()
	{
		if (vao != 0)
		{
			glDeleteVertexArrays(1, &vao);
		}

		// Create and bind a BO for vertex data
		glGenBuffers(1, &m_verticesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);

		// copy data into the buffer object7
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
		
		// Create and bind a BO for index data
		
		glGenBuffers(1, &m_indicesVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVBO);

		// copy data into the buffer object
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		// At this point the VAO is set up with two vertex attributes
		// referencing the same buffer object, and another buffer object
		// as source for index data. We can now unbind the VAO, go do
		// something else, and bind it again later when we want to render
		// with it.
		//glBindVertexArray(0);
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		//std::cout << "before block vao: " << glGetError() << std::endl;
	//	glBindVertexArray(0);
		//glBindVertexArray(vao);
		
		//glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVBO);
		//std::cout << "after block vao: " << glGetError() << std::endl;
		states.transform *= getTransform();

		// Render the insid
		//states.texture = m_texture;
		target.draw(&m_vertices[0], m_vertices.size(), Triangles, states, false);
	}

	void addVertex(float x, float y, float z, float u, float v)
	{
		static int count = 0;

		static Color c = Color::Magenta;

		if (count == 6)
		{
			float r = randomize(0, 255);
			float g = randomize(0, 255);
			float b = randomize(0, 255);
			c = Color(r, g, b);
			count = 0;
		}
		//c = Color::White;
		count++;
		m_vertices.push_back(Vertex(Vector3f(x, y, z), c, Vector2f(u, v)));
	}

	void setTexture(Texture *t)
	{
		m_texture = t;
	}

	Texture* m_texture;

	GLuint vao;

	GLuint m_verticesVBO;
	GLuint m_indicesVBO;

	std::vector<GLuint> indices;
	std::vector<Vertex> m_vertices;
};