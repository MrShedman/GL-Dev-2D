#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "Range.h"
#include "Cuboid.hpp"

#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\rendering\Image.hpp"

#include "..\gl\Buffer.hpp"

class Graph2D : public Drawable, public Transform
{
public:

	typedef std::function<float(float)> Equation;

	Graph2D()
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_outlineVerticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_outlineIndicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
	{
	}

	Graph2D(Graph2D&& other) :
		Drawable(std::move(other)),
		Transform(std::move(other)),
		m_verticesBuffer(std::move(other.m_verticesBuffer)),
		m_indicesBuffer(std::move(other.m_indicesBuffer)),
		m_outlineVerticesBuffer(std::move(other.m_outlineVerticesBuffer)),
		m_outlineIndicesBuffer(std::move(other.m_outlineIndicesBuffer)),
		m_vertices(std::move(other.m_vertices)),
		m_outline(std::move(other.m_outline)),
		m_indices(std::move(other.m_indices)),
		m_equation(std::move(other.m_equation)),
		m_xRange(std::move(other.m_xRange)),
		m_size(std::move(other.m_size))
	{

	}

	Graph2D& operator=(Graph2D&& other)
	{
		if (this != &other)
		{
			m_verticesBuffer = std::move(other.m_verticesBuffer);
			m_indicesBuffer = std::move(other.m_indicesBuffer);
			m_outlineVerticesBuffer = std::move(other.m_outlineVerticesBuffer);
			m_outlineIndicesBuffer = std::move(other.m_outlineIndicesBuffer);
			m_vertices = std::move(other.m_vertices);
			m_outline = std::move(other.m_outline);
			m_indices = std::move(other.m_indices);
			m_equation = std::move(other.m_equation);
			m_xRange = std::move(other.m_xRange);
			m_size = std::move(other.m_size);
		}

		return *this;
	}

	void setEquation(Equation equation)
	{
		m_equation = std::move(equation);
	}

	void setSize(Vector2f size)
	{
		m_size = size;

		setOrigin(m_size.x * 0.5f, m_size.y * 0.5f);
	}

	void setRange(Range xRange)
	{
		m_xRange = xRange;
	}

	bool plot()
	{
		int xLimit = m_xRange.getUpperLimit();

		std::vector<float> heightMap;

		heightMap.resize(xLimit + 1);

		m_vertices.clear();
		m_outline.clear();
		m_indices.clear();

		for (int x = 0; x <= xLimit; ++x)
		{
			float y = m_equation(m_xRange.map(x));
			m_vertices.push_back(Vertex(Vector3f(x, -y, 0.0f), Color::White));
			m_indices.push_back(x);
		}

		std::cout << m_vertices.size() << std::endl;

		normalize();

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());

		std::vector<GLuint> outlineIndices = { 0, 1, 2, 3, 4 };

		RectF rect({ Vector2f(), m_size });
		
		m_outline.emplace_back(Vertex(Vector3f(rect.left, rect.top, 0.f), Color::White));
		m_outline.emplace_back(Vertex(Vector3f(rect.left, rect.bottom, 0.f), Color::White));
		m_outline.emplace_back(Vertex(Vector3f(rect.right, rect.bottom, 0.f), Color::White));
		m_outline.emplace_back(Vertex(Vector3f(rect.right, rect.top, 0.f), Color::White));
		m_outline.emplace_back(Vertex(Vector3f(rect.left, rect.top, 0.f), Color::White));

		m_outlineVerticesBuffer.data(m_outline.size() * sizeof(Vertex), m_outline.data());
		m_outlineIndicesBuffer.data(outlineIndices.size() * sizeof(GLuint), outlineIndices.data());

		return true;
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform *= getTransform();

		target.drawDeferred(&m_vertices[0], m_vertices.size(), LinesStrip, states);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();

		m_outlineVerticesBuffer.bind();
		m_outlineIndicesBuffer.bind();

		target.drawDeferred(&m_outline[0], m_outline.size(), LinesStrip, states);

		m_outlineVerticesBuffer.unbind();
		m_outlineIndicesBuffer.unbind();
	}

private:

	void normalize()
	{
		auto xResult = std::minmax_element(m_vertices.begin(), m_vertices.end(), [&](Vertex v1, Vertex v2)->bool { return v1.position.x < v2.position.x; });
		auto yResult = std::minmax_element(m_vertices.begin(), m_vertices.end(), [&](Vertex v1, Vertex v2)->bool { return v1.position.y < v2.position.y; });

		float xMin = xResult.first->position.x;
		float xMax = xResult.second->position.x;

		float yMin = yResult.first->position.y;
		float yMax = yResult.second->position.y;

		for (auto& v : m_vertices)
		{
			float newY = translate(v.position.y, yMin, yMax, 0.0f, 1.0f);
			v.position.y = newY  * m_size.y;

			float newX = translate(v.position.x, xMin, xMax, 0.0f, 1.0f);
			v.position.x = newX * m_size.x;
		}
	}

	float translate(float value, float leftMin, float leftMax, float rightMin, float rightMax)
	{
		float leftSpan = leftMax - leftMin;
		float rightSpan = rightMax - rightMin;

		float valueScaled = (value - leftMin) / leftSpan;

		return rightMin + (valueScaled * rightSpan);
	}

	Vector2f m_size;

	Equation m_equation;

	Range m_xRange;

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;

	Buffer m_outlineVerticesBuffer;
	Buffer m_outlineIndicesBuffer;

	std::vector<Vertex> m_outline;
};