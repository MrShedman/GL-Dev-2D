#pragma once

#include "Shape.hpp"
#include "GL\glew.h"

class RectangleShape : public Shape
{
public:

	explicit RectangleShape(const Vector2f& size = Vector2f(0, 0));

	void setSize(const Vector2f& size);

	const Vector2f& getSize() const;

	virtual unsigned int getPointCount() const;

	virtual Vector2f getPoint(unsigned int index) const;

private:

	Vector2f m_size; ///< Size of the rectangle
};