#include "Shape.hpp"

#include "RenderTarget2D.h"
#include "Texture.hpp"

namespace
{
	RectF getBounds(const std::vector<Vertex>& vertices)
	{
		if (!vertices.empty())
		{
			float left = vertices[0].position.x;
			float top = vertices[0].position.y;
			float right = vertices[0].position.x;
			float bottom = vertices[0].position.y;

			for (std::size_t i = 1; i < vertices.size(); ++i)
			{
				Vector2f position(vertices[i].position.x, vertices[i].position.y);

				// Update left and right
				if (position.x < left)
					left = position.x;
				else if (position.x > right)
					right = position.x;

				// Update top and bottom
				if (position.y < top)
					top = position.y;
				else if (position.y > bottom)
					bottom = position.y;
			}

			return RectF(top, bottom, left, right);
		}
		else
		{
			// Array is empty
			return RectF();
		}
	}
}

Shape::~Shape()
{
	if (m_verticesVBO != 0)
	{
		glDeleteBuffers(1, &m_verticesVBO);
		glDeleteBuffers(1, &m_indicesVBO);
	}

	if (m_outlineVerticesVBO != 0)
	{
		glDeleteBuffers(1, &m_outlineVerticesVBO);
		glDeleteBuffers(1, &m_outlineIndicesVBO);
	}
}

void Shape::setTexture(Texture* texture, bool resetRect)
{
	if (texture)
	{
		// Recompute the texture area if requested, or if there was no texture & rect before
		if (resetRect || (!m_texture && (m_textureRect == RectI())))
		{
			setTextureRect(RectI(0, texture->getSize().y, 0, texture->getSize().x));
		}
	}

	// Assign the new texture
	m_texture = texture;
}

const Texture* Shape::getTexture() const
{
	return m_texture;
}

void Shape::setTextureRect(const RectI& rect)
{
	m_textureRect = rect;
	updateTexCoords();
}

const RectI& Shape::getTextureRect() const
{
	return m_textureRect;
}

void Shape::setFillColor(const Color& color)
{
	m_fillColor = color;
	updateFillColors();
}

const Color& Shape::getFillColor() const
{
	return m_fillColor;
}

void Shape::setOutlineColor(const Color& color)
{
	m_outlineColor = color;
	updateOutlineColors();
}

const Color& Shape::getOutlineColor() const
{
	return m_outlineColor;
}

void Shape::setOutlineThickness(float thickness)
{
	m_outlineThickness = thickness;
	update(); // recompute everything because the whole shape must be offset
}

float Shape::getOutlineThickness() const
{
	return m_outlineThickness;
}

RectF Shape::getLocalBounds() const
{
	return m_bounds;
}

RectF Shape::getGlobalBounds() const
{
	return getTransform().Transform(getLocalBounds());
}

Shape::Shape() :
m_texture(NULL),
m_textureRect(),
m_fillColor(255, 255, 255),
m_outlineColor(255, 255, 255),
m_outlineThickness(0),
m_insideBounds(),
m_bounds()
{
}

void Shape::update()
{
	// Get the total number of points of the shape
	unsigned int count = getPointCount();
	if (count < 3)
	{
		m_vertices.resize(0);
		m_outlineVertices.resize(0);
		return;
	}

	m_vertices.resize(count + 2); // + 2 for center and repeated first point

	// Position
	for (unsigned int i = 0; i < count; ++i)
		m_vertices[i + 1].position = getPoint(i);
	m_vertices[count + 1].position = m_vertices[1].position;

	// Update the bounding rectangle
	m_vertices[0] = m_vertices[1]; // so that the result of getBounds() is correct
	m_insideBounds = getBounds(m_vertices);

	// Compute the center and make it the first vertex
	m_vertices[0].position.x = m_insideBounds.left + m_insideBounds.getWidth() / 2;
	m_vertices[0].position.y = m_insideBounds.top + m_insideBounds.getHeight() / 2;

	// Color
	updateFillColors();

	// Texture coordinates
	updateTexCoords();

	// Outline
	updateOutline();
}

void Shape::draw(RenderTarget2D& target, RenderStates states) const
{
	bindVBO();

	states.transform *= getTransform();

	// Render the inside
	states.texture = m_texture;
	target.draw(&m_vertices[0], m_vertices.size(), TrianglesFan, states);

	// Render the outline
	if (m_outlineThickness != 0)
	{
		states.texture = NULL;
		target.draw(&m_outlineVertices[0], m_outlineVertices.size(), TrianglesStrip, states);
	}
}

void Shape::updateFillColors()
{
	for (unsigned int i = 0; i < m_vertices.size(); ++i)
		m_vertices[i].color = m_fillColor;
}

void Shape::updateTexCoords()
{
	for (unsigned int i = 0; i < m_vertices.size(); ++i)
	{
		float xratio = m_insideBounds.getWidth() > 0 ? (m_vertices[i].position.x - m_insideBounds.left) / m_insideBounds.getWidth() : 0;
		float yratio = m_insideBounds.getHeight() > 0 ? (m_vertices[i].position.y - m_insideBounds.top) / m_insideBounds.getHeight() : 0;
		m_vertices[i].texCoords.x = m_textureRect.left + m_textureRect.getWidth() * xratio;
		m_vertices[i].texCoords.y = m_textureRect.top + m_textureRect.getHeight() * yratio;
		
		if (m_textureRect != RectI())
		{
			m_vertices[i].texCoords.x /= m_textureRect.getWidth();
			m_vertices[i].texCoords.y /= m_textureRect.getHeight();
		}
	}
}

void Shape::updateOutline()
{
	unsigned int count = m_vertices.size() - 2;
	m_outlineVertices.resize((count + 1) * 2);

	for (unsigned int i = 0; i < count; ++i)
	{
		unsigned int index = i + 1;

		Vector3f p0 = (i == 0) ? m_vertices[count].position : m_vertices[index - 1].position;
		Vector3f p1 = m_vertices[index].position;
		Vector3f p2 = m_vertices[index + 1].position;

		Vector3f n1 = (p0 - p1).Normalized();
		Vector3f n2 = (p1 - p2).Normalized();

		if (n1.Dot(m_vertices[0].position - p1) > 0)
		{
			n1 = n1.inverted();
		}
		if (n2.Dot(m_vertices[0].position - p1) > 0)
		{
			n2 = n2.inverted();
		}

		float factor = 1.f + n1.Dot(n2);//(n1.x * n2.x + n1.y * n2.y);
		Vector3f normal = (n1 + n2) / factor; 

	/*	Vector2f p0;

		// Get the two segments shared by the current point
		if (i == 0)
		{
			p0.x = m_vertices[count].position.x;
			p0.y = m_vertices[count].position.y;
		}
		else
		{
			p0.x = m_vertices[index - 1].position.x;
			p0.y = m_vertices[index - 1].position.y;
		}

		Vector2f p1(m_vertices[index].position.x, m_vertices[index].position.y);
		Vector2f p2(m_vertices[index + 1].position.x, m_vertices[index + 1].position.y);

		// Compute their normal
		Vector2f n1 = (p0 - p1).Normalized();
		Vector2f n2 = (p1 - p2).Normalized();

		// Make sure that the normals point towards the outside of the shape
		// (this depends on the order in which the points were defined)
		if (n1.Dot(m_vertices[0].position - p1) > 0)
		{
			n1 = n1.inverted();
		}
		if (n2.Dot(m_vertices[0].position - p1) > 0)
		{
			n2 = n2.inverted();
		}

		// Combine them to get the extrusion direction
		float factor = 1.f + n1.Dot(n2);//(n1.x * n2.x + n1.y * n2.y);
		Vector2f normal = (n1 + n2) / factor;*/

		// Update the outline points
		m_outlineVertices[i * 2 + 0].position = p1;
		m_outlineVertices[i * 2 + 1].position = p1 + normal * m_outlineThickness;
	}

	// Duplicate the first point at the end, to close the outline
	m_outlineVertices[count * 2 + 0].position = m_outlineVertices[0].position;
	m_outlineVertices[count * 2 + 1].position = m_outlineVertices[1].position;

	// Update outline colors
	updateOutlineColors();

	// Update the shape's bounds
	m_bounds = getBounds(m_outlineVertices);
}

void Shape::updateOutlineColors()
{
	for (unsigned int i = 0; i < m_outlineVertices.size(); ++i)
		m_outlineVertices[i].color = m_outlineColor;
}

void Shape::bindVBO() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVBO);
	
	if (m_outlineThickness != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_outlineVerticesVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_outlineIndicesVBO);
	}
}

void Shape::generateVBO()
{
	if (m_verticesVBO != 0)
	{
		glDeleteBuffers(1, &m_verticesVBO);
		glDeleteBuffers(1, &m_indicesVBO);
	}

	if (m_outlineVerticesVBO != 0)
	{
		glDeleteBuffers(1, &m_outlineVerticesVBO);
		glDeleteBuffers(1, &m_outlineIndicesVBO);
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
	glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

	//Create IBO
	glGenBuffers(1, &m_indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesSize * sizeof(GLuint), &indices[0], GL_DYNAMIC_DRAW);

	//Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

	//Outline VBO
	if (m_outlineThickness != 0)
	{
		size_t outlineVerticesSize = m_outlineVertices.size();

		//Vertex data
		std::vector<GLuint> outlineIndices(outlineVerticesSize);

		for (size_t i = 0; i < outlineVerticesSize; ++i)
		{
			outlineIndices[i] = i;
		}

		//Create VBO
		glGenBuffers(1, &m_outlineVerticesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_outlineVerticesVBO);
		glBufferData(GL_ARRAY_BUFFER, outlineVerticesSize * sizeof(Vertex), &m_outlineVertices[0], GL_DYNAMIC_DRAW);

		//Create IBO
		glGenBuffers(1, &m_outlineIndicesVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_outlineIndicesVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, outlineVerticesSize * sizeof(GLuint), &outlineIndices[0], GL_DYNAMIC_DRAW);

		//Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}
}