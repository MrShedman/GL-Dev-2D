#pragma once

#include "Block.hpp"
#include "Transform.hpp"
#include "Drawable.hpp"

#include "..\gl\Buffer.hpp"

class Chunk : public Drawable, public Transform
{
public:
	Chunk()
		:
		m_positionBuffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
	{
		int size = 3;
		float scale = 10.f;

		for (int i = -size; i < size; ++i)
		{
			for (int j = -size; j < size; ++j)
			{
				for (int k = -size; k < size; ++k)
				{
					Block b;
					b.create(1);
					b.setPosition(i*scale, j*scale, k*scale);
					b.setOrigin(0.f, 0.f, 0.f);
					//b.setScale(2, 4, 6);
					//b.setRotation(Vector3f(1.0f, 1.0f, 0.0f), 45.0f);
					//b.setTexture(t);
					m_blocks.push_back(std::move(b));
				}
			}
		}
	}

	void updatePositions()
	{
		for (auto && m : m_blocks)
		{
			auto data = m.getPositionData();

			for (auto &p : data)
			{
				m_positions.push_back(p);
			}
		}
	}

	void setUpInstance()
	{
		m_positionBuffer.data(m_positions.size() * sizeof(Vector3f), m_positions.data());
		//you need to upload sizeof( vec4 ) * number_of_cubes bytes, DYNAMIC_DRAW because it is updated per frame

		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, m_positions.size());
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		states.transform *= getTransform();

	//	target.draw(&m_vertices[0], m_vertices.size(), Triangles, states, false);
	}

private:

	Buffer m_positionBuffer;

	std::vector<Vector3f> m_positions;
	std::vector<Block>	m_blocks;
};