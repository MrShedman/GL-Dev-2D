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

	void create(Camera& cam);

	inline void setCamera(Camera& cam)
	{
		camera = cam;
	}

	void draw(const Drawable& drawable, const RenderStates& states);

	void draw(const Vertex* vertices, unsigned int vertexCount, PrimitiveType type, const RenderStates& states);

private:

	Texture nullTexture;
	Camera camera;
};