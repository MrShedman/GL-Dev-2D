#include "Shape.hpp"

#include "RenderTarget2D.h"
#include "Texture.hpp"
#include <numeric>

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
	return getTransform().transform(getLocalBounds());
}

Shape::Shape() :
m_texture(NULL),
m_textureRect(),
m_fillColor(Color::rgb(255, 255, 255)),
m_outlineColor(Color::rgb(255, 255, 255)),
m_outlineThickness(0),
m_insideBounds(),
m_bounds(),
m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
m_outlineVerticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
m_outlineIndicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
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

	std::vector<GLuint> indices(m_vertices.size());

	std::iota(indices.begin(), indices.end(), 0);

	m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
	m_indicesBuffer.data(indices.size() * sizeof(GLuint), indices.data());

	if (m_outlineThickness != 0.f)
	{
		std::vector<GLuint> outlineIndices(m_outlineVertices.size());

		std::iota(outlineIndices.begin(), outlineIndices.end(), 0);

		m_outlineVerticesBuffer.data(m_outlineVertices.size() * sizeof(Vertex), m_outlineVertices.data());
		m_outlineIndicesBuffer.data(outlineIndices.size() * sizeof(GLuint), outlineIndices.data());
	}
}

void Shape::draw(RenderTarget2D& target, RenderStates states) const
{
	m_verticesBuffer.bind();
	m_indicesBuffer.bind();

	states.transform *= getTransform();

	// Render the inside
	states.texture = m_texture;
	target.draw(&m_vertices[0], m_vertices.size(), TrianglesFan, states);

	// Render the outline
	if (m_outlineThickness != 0)
	{
		m_outlineVerticesBuffer.bind();
		m_outlineIndicesBuffer.bind();

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