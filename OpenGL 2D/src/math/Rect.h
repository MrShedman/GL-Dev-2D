#pragma once

#include "Vector.h"

#undef max

template <class T>
class Rect
{
public:
	Rect()
		:
		top(0),
		bottom(0),
		left(0),
		right(0)
	{}

	Rect(T top, T bottom, T left, T right)
		:
		top(top),
		bottom(bottom),
		left(left),
		right(right)
	{}

	Rect(const Rect<T> &rhs)
		:
		top(rhs.top),
		bottom(rhs.bottom),
		left(rhs.left),
		right(rhs.right)
	{}

	Rect(const std::vector<Vector2<T>> &points)
	{
		left = std::numeric_limits<T>::max();
		right = -std::numeric_limits<T>::max();
		top = std::numeric_limits<T>::max();
		bottom = -std::numeric_limits<T>::max();

		for (size_t s = 0; s < points.size(); ++s)
			include(points[s]);
	}

	void include(const Vector2<T> &point)
	{
		if (left > point.x) left = point.x;
		if (right < point.x) right = point.x;
		if (top > point.y) top = point.y;
		if (bottom < point.y) bottom = point.y;
	}

	void include(const std::vector<Vector2<T>> &points)
	{
		for (size_t s = 0; s < points.size(); ++s)
			include(points[s]);
	}

	void translate(T dx, T dy)
	{
		top += dy;
		bottom += dy;
		left += dx;
		right += dx;
	}

	bool lineIntersects(const Vector2<T>& begin, const Vector2<T>& end)
	{
		bool contB = contains(begin);
		bool contE = contains(end);

		if (contB != contE)
		{
			return true;
		}
		else if (contB && contE)
		{
			return false;
		}

		Vector2<T> topLeft(left, top);
		Vector2<T> topRight(right, top);
		Vector2<T> bottomLeft(left, bottom);
		Vector2<T> bottomRight(right, bottom);

		std::vector<std::pair<Vector2<T>, Vector2<T>>> lines;

		lines.emplace_back(topLeft, topRight);
		lines.emplace_back(topRight, bottomRight);
		lines.emplace_back(bottomRight, bottomLeft);
		lines.emplace_back(bottomLeft, topLeft);

		for (size_t i = 0; i < 4; ++i)
		{
			Vector2<T> CmP = lines[i].first - begin;
			Vector2<T> r = end - begin;
			Vector2<T> s = lines[i].second - lines[i].first;

			T CmPxr = CmP.Cross(r);
			T CmPxs = CmP.Cross(s);
			T rxs = r.Cross(s);

			if (CmPxr == 0.f)
			{
				// Lines are collinear, and so intersect if they have any overlap

				if (((lines[i].first.x - begin.x < 0) != (lines[i].first.x - end.x < 0))
					|| ((lines[i].first.y - begin.y < 0) != (lines[i].first.y - end.y < 0)))
				{
						return true;
				}
			}

			if (rxs == 0)
				continue; // Lines are parallel.

			T rxsr = 1 / rxs;
			T t = CmPxs * rxsr;
			T u = CmPxr * rxsr;

			if ((t >= 0) && (t <= 1) && (u >= 0) && (u <= 1))
			{
				return true;
			}
		}

		return false;
	}

	bool contains(T x, T y)
	{
		return (x >= left && x <= right && y <= bottom && y >= top);
	}

	bool contains(const Vector2<T>& point)
	{
		return contains(point.x, point.y);
	}

	T getWidth() const
	{
		return right - left;
	}

	T getHeight() const
	{
		return bottom - top;
	}

	Vector2<T> getCenter() const
	{
		T middleX = left + (right - left) / 2;
		T middleY = top + (bottom - top) / 2;

		return Vector2<T>(middleX, middleY);
	}

	inline bool operator==(const Rect<T>& rhs) const
	{
		return top == rhs.top && bottom == rhs.bottom && left == rhs.left && right == rhs.right;
	}

	inline bool operator!=(const Rect<T>& rhs) const
	{
		return !operator==(rhs);
	}

public:
	T top;
	T bottom;
	T left;
	T right;
};

typedef Rect<float> RectF;
typedef Rect<int> RectI;