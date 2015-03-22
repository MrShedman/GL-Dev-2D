#pragma once


#include "..\rendering\Block.hpp"

#include <vector>

class CollidableContainer
{
public:
	CollidableContainer()
		:
		m_bestIndex(-1)
	{

	}

	bool testCollisions(const Rayf& ray)
	{
		m_bestIndex = -1;

		m_bestResult.intersection = Vector3f();
		m_bestResult.normal = Vector3f();
		m_bestResult.intersects = false;

		std::vector<std::pair<Collidable::Result, int>> results;

		std::cout << m_collidables.size() << std::endl;

		for (int i = 0; i < m_collidables.size(); ++i)
		{
			auto r = m_collidables[i]->doesIntersect(ray);

			if (r.intersects)
			{
				results.push_back(std::make_pair(r, i));
			}
		}

		if (!results.empty())
		{
			std::sort(results.begin(), results.end(), [&ray](std::pair<Collidable::Result, int>& p1, std::pair<Collidable::Result, int>& p2)
			{
				return std::abs(ray.origin.DistanceSq(p1.first.intersection)) < std::abs(ray.origin.DistanceSq(p2.first.intersection));
			});

			m_bestIndex = results.front().second;
			m_bestResult = results.front().first;
		}

		return hasCollision();
	}

	bool hasCollision() const
	{
		return m_bestIndex >= 0;
	}

	Collidable::Result getBestCollisionResult() const
	{
		return m_bestResult;
	}

	Collidable* getBestCollision()
	{
		return m_collidables[m_bestIndex];
	}

	void pushBack(Collidable* collidable)
	{
		m_collidables.push_back(collidable);
	}

	void eraseCollided()
	{
		m_collidables.erase(m_collidables.begin() + m_bestIndex);
	}

private:

	Collidable::Result m_bestResult;
	int m_bestIndex;

	std::vector<Collidable*> m_collidables;
};