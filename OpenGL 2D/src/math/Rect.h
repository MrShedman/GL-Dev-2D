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

	bool contains(T x, T y)
	{
		return (x >= left && x <= right && y <= bottom && y >= top);
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