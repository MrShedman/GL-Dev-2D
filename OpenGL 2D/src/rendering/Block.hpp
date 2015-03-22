#pragma once

#include <vector>

#include "..\rendering\RenderTarget2D.h"
#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\math\Vector.h"
#include "..\math\MathHelper.h"
#include "..\math\Ray.hpp"

#include "..\gl\Buffer.hpp"

#include "..\experiment\Collidable.hpp"

class Block : public Drawable, public Transform, public Collidable
{
public:

	enum Face
	{
		Bottom,
		Top,
		Front,
		Back,
		Left,
		Right
	};

	Block()
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_texture(nullptr)
	{}

	Block(Block&& other) :
		Drawable(std::move(other)),
		Transform(std::move(other)),
		Collidable(std::move(other)),
		m_verticesBuffer(std::move(other.m_verticesBuffer)),
		m_indicesBuffer(std::move(other.m_indicesBuffer)),
		m_vertices(std::move(other.m_vertices)),
		m_indices(std::move(other.m_indices)),
		m_size(std::move(other.m_size))
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
			m_size = std::move(other.m_size);

			m_texture = other.m_texture;
		}

		return *this;
	}
		
	void create(float size)
	{
		m_size = size;

		//bottom
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
		addVertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		addVertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		addVertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		addVertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		addVertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		addVertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

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

	Collidable::Result doesIntersect(const Rayf& ray)
	{
		int index = -1;

		float max = std::numeric_limits<float>::max();
		Vector3f closestIntersection(max, max, max);

		bool hasCollided = false;

		for (int i = 0; i < m_vertices.size(); i += 3)
		{
			Vector3f v0 = getTransform().transformPoint(m_vertices[i+0].position);
			Vector3f v1 = getTransform().transformPoint(m_vertices[i+1].position);
			Vector3f v2 = getTransform().transformPoint(m_vertices[i+2].position);

			// Compute the offset origin, edges, and normal.
			Vector3f diff = ray.origin - v0;
			Vector3f edge1 = v1 - v0;
			Vector3f edge2 = v2 - v0;
			Vector3f normal = edge1.Cross(edge2);

			// Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
			// E1 = edge1, E2 = edge2, N = Cross(E1,E2)) by
			//   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
			//   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
			//   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
			float DdN = ray.direction.Dot(normal);
			float sign;
			if (DdN > 0.0f)
			{
				sign = 1.0f;
			}
			else if (DdN < 0.0f)
			{
				sign = -1.0f;
				DdN = -DdN;
			}
			else
			{
				// Ray and triangle are parallel, call it a "no intersection"
				// even if the ray does intersect.
				continue;
			}

			float DdQxE2 = sign*ray.direction.Dot(diff.Cross(edge2));
			if (DdQxE2 >= 0.0f)
			{
				float DdE1xQ = sign*ray.direction.Dot(edge1.Cross(diff));
				if (DdE1xQ >= 0.0f)
				{
					if (DdQxE2 + DdE1xQ <= DdN)
					{
						// Line intersects triangle, check whether ray does.
						float QdN = -sign*diff.Dot(normal);
						if (QdN >= 0.0f)
						{
							float inv = 1.0f / DdN;
							Vector3f intersection = ray.origin + (ray.direction * QdN * inv);

							if (std::abs(intersection.DistanceSq(ray.origin)) < std::abs(closestIntersection.DistanceSq(ray.origin)))
							{
								closestIntersection = intersection;
								index = i;
							}

							// Ray intersects triangle.
							hasCollided = true;
						}
					}
				}
			}
		}

		Collidable::Result result;

		if (hasCollided)
		{
			int i = std::floor(float(index) / 6.f);
			i = std::min(5, i);
			/*std::vector<std::string> names = { "bottom", "top", "front", "back", "left", "righ" };

			std::cout << names[i] << std::endl;*/
			result.normal = m_vertices[index].normals;
			result.intersection = closestIntersection;
		}

		result.intersects = hasCollided;

		return result;
	}

	void setColour(const Color& color)
	{
		std::cout << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << std::endl;

		for (auto &v : m_vertices)
		{
			v.color = color;
		}
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.texture = m_texture;
		states.transform *= getTransform();

		target.drawDeferred(&m_vertices[0], m_vertices.size(), Triangles, states);// , false, true, false);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();
	}

	void addVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
	{
		m_vertices.push_back(Vertex(Vector3f(x, y, z) * m_size, Color::White, Vector2f(u, v), Vector3f(nx, ny, nz)));
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

	float m_size;

	const Texture* m_texture;

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
};