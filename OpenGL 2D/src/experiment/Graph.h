#pragma once

#include <vector>
#include <algorithm>
#include <functional>

#include "Range.h"

#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\rendering\Image.hpp"

#include "..\gl\Buffer.hpp"

class Graph : public Drawable, public Transform
{
public:

	enum Palette
	{
		Jet,
		Hot,
		Gray,
		Cool
	};

	typedef std::function<float(float, float)> Equation;

	Graph()
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_verticesBuffer2(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer2(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
	{
		m_image.loadFromFile("res/textures/palette.png");
		m_palette = Jet;
	}

	Graph(Graph&& other) :
		Drawable(std::move(other)),
		Transform(std::move(other)),
		m_verticesBuffer(std::move(other.m_verticesBuffer)),
		m_indicesBuffer(std::move(other.m_indicesBuffer)),
		m_verticesBuffer2(std::move(other.m_verticesBuffer2)),
		m_indicesBuffer2(std::move(other.m_indicesBuffer2)),
		m_vertices(std::move(other.m_vertices)),
		m_indices(std::move(other.m_indices)),
		m_vertices2(std::move(other.m_vertices2)),
		m_indices2(std::move(other.m_indices2)),
		m_equation(std::move(other.m_equation)),
		m_image(std::move(other.m_image)),
		m_xRange(std::move(other.m_xRange)),
		m_yRange(std::move(other.m_yRange)),
		m_palette(std::move(other.m_palette)),
		m_size(std::move(other.m_size))
	{

	}

	Graph& operator=(Graph&& other)
	{
		if (this != &other)
		{
			m_verticesBuffer = std::move(other.m_verticesBuffer);
			m_indicesBuffer = std::move(other.m_indicesBuffer);
			m_vertices = std::move(other.m_vertices);
			m_indices = std::move(other.m_indices);
			m_verticesBuffer2 = std::move(other.m_verticesBuffer2);
			m_indicesBuffer2 = std::move(other.m_indicesBuffer2);
			m_vertices2 = std::move(other.m_vertices2);
			m_indices2 = std::move(other.m_indices2);
			m_equation = std::move(other.m_equation);
			m_image = std::move(other.m_image);
			m_xRange = std::move(other.m_xRange);
			m_yRange = std::move(other.m_yRange);
			m_palette = std::move(other.m_palette);
			m_size = std::move(other.m_size);
		}

		return *this;
	}


	void setEquation(Equation equation)
	{
		m_equation = std::move(equation);
	}

	void setSize(Vector3f size)
	{
		m_size = size;

		setOrigin(m_size * 0.5f);
	}

	void setRange(Range xRange, Range yRange)
	{
		m_xRange = xRange;
		m_yRange = yRange;
	}

	void setPalette(Palette palette)
	{
		m_palette = palette;
	}

	bool plot()
	{
		int xLimit = m_xRange.getUpperLimit();
		int yLimit = m_yRange.getUpperLimit();

		std::vector<std::vector<float>> heightMap;

		heightMap.resize(xLimit + 1);

		for (int i = 0; i <= xLimit; ++i)
		{
			heightMap[i].resize(yLimit + 1);
		}

		for (int x = 0; x <= xLimit; ++x)
		{
			for (int y = 0; y <= yLimit; ++y)
			{
				heightMap[x][y] = m_equation(m_xRange.map(x), m_yRange.map(y));
			}
		}

		int count = 0;

		for (int x = 0; x <= xLimit - 1; ++x)
		{
			for (int y = 0; y <= yLimit - 1; ++y)
			{
				float xf = static_cast<float>(x);
				float yf = static_cast<float>(y);
				float xf1 = static_cast<float>(x + 1);
				float yf1 = static_cast<float>(y + 1);

				Vector3f topLeft = Vector3f(xf, heightMap[x][y + 1], yf1);
				Vector3f topRight = Vector3f(xf1, heightMap[x + 1][y + 1], yf1);
				Vector3f bottomLeft = Vector3f(xf, heightMap[x][y], yf);
				Vector3f bottomRight = Vector3f(xf1, heightMap[x + 1][y], yf);

				m_vertices.push_back(bottomLeft);
				m_vertices.push_back(topRight);
				m_vertices.push_back(bottomRight);
				
				m_vertices.push_back(bottomLeft);
				m_vertices.push_back(topLeft);
				m_vertices.push_back(topRight);

				m_indices.push_back(count++);
				m_indices.push_back(count++);
				m_indices.push_back(count++);
				m_indices.push_back(count++);
				m_indices.push_back(count++);
				m_indices.push_back(count++);
			}
		}

		std::cout << m_vertices.size() << std::endl;

		normalize();

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());

		doGrid();

		return true;
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform *= getTransform();

		target.draw(&m_vertices[0], m_vertices.size(), Triangles, states, false);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();

		m_verticesBuffer2.bind();
		m_indicesBuffer2.bind();

		target.draw(&m_vertices2[0], m_vertices2.size(), Lines, states, false);

		m_verticesBuffer2.unbind();
		m_indicesBuffer2.unbind();
	}

private:

	void doGrid()
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

		m_vertices2.push_back(p1);
		m_vertices2.push_back(p2);
		m_vertices2.push_back(p2);
		m_vertices2.push_back(p3);
		m_vertices2.push_back(p3);
		m_vertices2.push_back(p4);
		m_vertices2.push_back(p4);
		m_vertices2.push_back(p1);

		m_vertices2.push_back(p5);
		m_vertices2.push_back(p6);
		m_vertices2.push_back(p6);
		m_vertices2.push_back(p7);
		m_vertices2.push_back(p7);
		m_vertices2.push_back(p8);
		m_vertices2.push_back(p8);
		m_vertices2.push_back(p5);

		m_vertices2.push_back(p1);
		m_vertices2.push_back(p5);

		m_vertices2.push_back(p2);
		m_vertices2.push_back(p6);

		m_vertices2.push_back(p3);
		m_vertices2.push_back(p7);

		m_vertices2.push_back(p4);
		m_vertices2.push_back(p8);

		for (int i = 0; i < 24; ++i)
		{
			m_vertices2[i].color = Color(150, 150, 150);
			m_indices2.push_back(i);
		}

		m_verticesBuffer2.data(m_vertices2.size() * sizeof(Vertex), m_vertices2.data());
		m_indicesBuffer2.data(m_indices2.size() * sizeof(GLuint), m_indices2.data());

	}

	void normalize()
	{
		auto xResult = std::minmax_element(m_vertices.begin(), m_vertices.end(), [&](Vertex v1, Vertex v2)->bool { return v1.position.x < v2.position.x; });
		auto yResult = std::minmax_element(m_vertices.begin(), m_vertices.end(), [&](Vertex v1, Vertex v2)->bool { return v1.position.y < v2.position.y; });
		auto zResult = std::minmax_element(m_vertices.begin(), m_vertices.end(), [&](Vertex v1, Vertex v2)->bool { return v1.position.z < v2.position.z; });
		
		float xMin = xResult.first->position.x;
		float xMax = xResult.second->position.x;

		float yMin = yResult.first->position.y;
		float yMax = yResult.second->position.y;

		float zMin = zResult.first->position.z;
		float zMax = zResult.second->position.z;

		for (auto& v : m_vertices)
		{
			float newX = translate(v.position.x, xMin, xMax, 0.0, 1.0);
			v.position.x = newX  * m_size.x;

			float newY = translate(v.position.y, yMin, yMax, 0.0, 1.0);
			v.position.y = newY  * m_size.y;

			float newZ = translate(v.position.z, zMin, zMax, 0.0, 1.0);
			v.position.z = newZ  * m_size.z;

			unsigned int pixel = static_cast<unsigned int>(std::round(newY * static_cast<float>(m_image.getSize().x - 1)));

			v.color = m_image.getPixel(pixel, m_palette);
		}
	}

	float translate(float value, float leftMin, float leftMax, float rightMin, float rightMax)
	{
		float leftSpan = leftMax - leftMin;
		float rightSpan = rightMax - rightMin;

		float valueScaled = (value - leftMin) / leftSpan;

		return rightMin + (valueScaled * rightSpan);
	}

	Buffer m_verticesBuffer2;
	Buffer m_indicesBuffer2;

	std::vector<GLuint> m_indices2;
	std::vector<Vertex> m_vertices2;

	Image m_image;
	Palette m_palette;

	Vector3f m_size;

	Equation m_equation;

	Range m_xRange;
	Range m_yRange;

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
};