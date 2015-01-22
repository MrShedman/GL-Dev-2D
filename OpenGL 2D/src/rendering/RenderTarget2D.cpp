#include "RenderTarget2D.h"

#include "Drawable.hpp"
#include "RenderStates.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "..\math\MathHelper.h"

RenderTarget2D::RenderTarget2D()
{

}

void RenderTarget2D::create(Camera& cam, unsigned int width, unsigned int height)
{
	m_size = Vector2u(width, height);
	camera = cam;

	std::vector<unsigned char> white({ 255, 255, 255, 255 });

	if (!nullTexture.loadFromMemory(white, 1, 1))
	{
		std::cout << "error: loading null texture" << std::endl;
	}
}

RectI RenderTarget2D::getViewport(const Camera& view) const
{
	float width = m_size.x;
	float height = m_size.y;
	const RectF& viewport = view.getViewport();

	int top = static_cast<int>(0.5f + height * viewport.top);
	int bottom = static_cast<int>(0.5f + height * viewport.bottom);
	int left = static_cast<int>(0.5f + width  * viewport.left);
	int right = static_cast<int>(0.5f + width  * viewport.right);

	return RectI(top, bottom, left, right);
}

Vector2u RenderTarget2D::getSize() const
{
	return m_size;
}

Vector2f RenderTarget2D::mapPixelToCoords(const Vector2i& point, const Camera& view) const
{
	// First, convert from viewport coordinates to homogeneous coordinates
	Vector2f normalized;
	RectI viewport = getViewport(view);
	normalized.x = -1.f + 2.f * (point.x - viewport.left) / viewport.getWidth();
	normalized.y = 1.f - 2.f * (point.y - viewport.top) / viewport.getHeight();

	// Then transform by the inverse of the view matrix
	return view.getInverseTransform().Transform(normalized);
}

void RenderTarget2D::draw(const Drawable& drawable, const RenderStates& states)
{
	drawable.draw(*this, states);
}

#include "CameraPersp.hpp"

void RenderTarget2D::draw(const Vertex* vertices, unsigned int vertexCount, PrimitiveType type, const RenderStates& states, bool d)
{
	RectI viewport = getViewport(camera);
	int top = getSize().y - viewport.bottom;
	glViewport(viewport.left, top, viewport.getWidth(), viewport.getHeight());
	
	Shader2D::bind(states.shader);

	//Enable vertex and texture coordinate arrays
	states.shader->enableVertexPointer();
	states.shader->enableTexCoordPointer();
	states.shader->enableColourPointer();

	//Set texture coordinate data
	states.shader->setTexCoordPointer(sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
	states.shader->setVertexPointer(sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	states.shader->setColourPointer(sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

	if (d)
	{
		states.shader->setProjectionMatrix(camera.getTransform());
	}
	else
	{
		states.shader->setProjectionMatrix(states.cam->getProjection());
	}
	
	states.shader->setModelViewMatrix(states.transform);

	if (states.texture != NULL)
	{
		Texture::bind(states.texture);
		Texture::bind(&nullTexture, 1);
	}
	else
	{
		Texture::bind(&nullTexture);
	}

	//Update vertex buffer data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(Vertex), vertices);

	static const GLenum modes[] = { GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
		GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN};
	
	GLenum mode = modes[type];
	//Draw using vertex data and index data
	glDrawElements(mode, vertexCount, GL_UNSIGNED_INT, NULL);
	
	//Disable vertex and texture coordinate arrays
	states.shader->disableVertexPointer();
	states.shader->disableTexCoordPointer();
	states.shader->disableColourPointer();

	Shader2D::bind(NULL);

	Texture::bind(NULL);
}
