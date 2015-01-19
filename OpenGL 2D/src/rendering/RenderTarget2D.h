#pragma once

class Drawable;
class RenderStates;
class Vertex;

#include "Camera.hpp"
#include "Texture.hpp"
#include "PrimitiveType.hpp"

class RenderTarget2D
{
public:

	RenderTarget2D();

	void create(Camera& cam, unsigned int width, unsigned int height);

	inline void setCamera(Camera& cam)
	{
		camera = cam;
		m_size = cam.getSize();
	}

	const Camera& getCamera() const
	{
		return camera;
	}

	Vector2u getSize() const;

	RectI getViewport(const Camera& view) const;

	Vector2f mapPixelToCoords(const Vector2i& point, const Camera& view) const;

	void draw(const Drawable& drawable, const RenderStates& states);

	void draw(const Vertex* vertices, unsigned int vertexCount, PrimitiveType type, const RenderStates& states, bool d = true);

private:

	Vector2u m_size;

	Texture nullTexture;
	Camera camera;
};