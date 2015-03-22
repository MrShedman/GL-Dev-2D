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
		m_palette(Jet),
		m_showWireframe(false)
	{
		m_image.loadFromFile("res/textures/palette.png");
	}

	Graph(Graph&& other) :
		Drawable(std::move(other)),
		Transform(std::move(other)),
		m_verticesBuffer(std::move(other.m_verticesBuffer)),
		m_indicesBuffer(std::move(other.m_indicesBuffer)),
		m_vertices(std::move(other.m_vertices)),
		m_wireframe(std::move(other.m_wireframe)),
		m_indices(std::move(other.m_indices)),
		m_equation(std::move(other.m_equation)),
		m_image(std::move(other.m_image)),
		m_xRange(std::move(other.m_xRange)),
		m_yRange(std::move(other.m_yRange)),
		m_palette(std::move(other.m_palette)),
		m_size(std::move(other.m_size)),
		m_cuboid(std::move(other.m_cuboid)),
		m_showWireframe(other.m_showWireframe)
	{

	}

	Graph& operator=(Graph&& other)
	{
		if (this != &other)
		{
			m_verticesBuffer = std::move(other.m_verticesBuffer);
			m_indicesBuffer = std::move(other.m_indicesBuffer);
			m_vertices = std::move(other.m_vertices);
			m_wireframe = std::move(other.m_wireframe);
			m_indices = std::move(other.m_indices);
			m_equation = std::move(other.m_equation);
			m_image = std::move(other.m_image);
			m_xRange = std::move(other.m_xRange);
			m_yRange = std::move(other.m_yRange);
			m_palette = std::move(other.m_palette);
			m_size = std::move(other.m_size);
			m_cuboid = std::move(other.m_cuboid);
			m_showWireframe = other.m_showWireframe;
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

		m_cuboid = std::move(Cuboid(size));
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

	void showWireframe(bool flag)
	{
		m_showWireframe = flag;
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
		m_vertices.clear();
		m_wireframe.clear();
		m_indices.clear();

		for (int x = 0; x < xLimit; ++x)
		{
			for (int y = 0; y < yLimit; ++y)
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

		calcNormals();

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());

		return true;
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

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform *= getTransform();

		target.drawDeferred(&m_vertices[0], m_vertices.size(), Triangles, states);
		
		if (m_showWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			target.drawDeferred(&m_wireframe[0], m_wireframe.size(), Triangles, states);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();

		target.draw(m_cuboid, states);
	}

private:
	
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

			m_wireframe.emplace_back(v.position, Color::Black);

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

	Cuboid m_cuboid;

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

	bool m_showWireframe;
	std::vector<Vertex> m_wireframe;
};