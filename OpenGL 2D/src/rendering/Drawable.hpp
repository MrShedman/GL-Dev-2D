#pragma once

#include "RenderStates.hpp"

class Renderstates;
class RenderTarget2D;

class Drawable
{
public:

	virtual ~Drawable() {}

protected:

	friend class RenderTarget2D;

	virtual void draw(RenderTarget2D& target, RenderStates states) const = 0;
};