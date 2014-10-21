#pragma once

#include "Drawable.hpp"
#include "Transform.hpp"
#include "Vertex.hpp"
#include "Color.h"
#include "..\math\Rect.h"
#include "GL\glew.h"

#include <vector>

class RenderTarget2D;
class Texture;

class Shape : public Drawable, public Transform
{
public:

	virtual ~Shape();

	void setTexture( Texture* texture, bool resetRect = false);

	void setTextureRect(const RectI& rect);

	void setFillColor(const Color& color);

	void setOutlineColor(const Color& color);

	void setOutlineThickness(float thickness);

	const Texture* getTexture() const;

	const RectI& getTextureRect() const;

	const Color& getFillColor() const;

	const Color& getOutlineColor() const;

	float getOutlineThickness() const;

	virtual unsigned int getPointCount() const = 0;

	virtual Vector2f getPoint(unsigned int index) const = 0;

	RectF getLocalBounds() const;

	RectF getGlobalBounds() const;

	void generateVBO();

protected:

	Shape();

	void bindVBO() const;

	void update();

private:

	virtual void draw(RenderTarget2D& target, RenderStates states) const;

	void updateFillColors();

	void updateTexCoords();

	void updateOutline();

	void updateOutlineColors();

private:

	GLuint m_verticesVBO;
	GLuint m_indicesVBO;

	GLuint m_outlineVerticesVBO;
	GLuint m_outlineIndicesVBO;

private:

	Texture*			m_texture;          ///< Texture of the shape
	RectI				m_textureRect;      ///< Rectangle defining the area of the source texture to display
	Color				m_fillColor;        ///< Fill color
	Color				m_outlineColor;     ///< Outline color
	float				m_outlineThickness; ///< Thickness of the shape's outline
	std::vector<Vertex>	m_vertices;         ///< Vertex array containing the fill geometry
	std::vector<Vertex>	m_outlineVertices;  ///< Vertex array containing the outline geometry
	RectF				m_insideBounds;     ///< Bounding rectangle of the inside (fill)
	RectF				m_bounds;           ///< Bounding rectangle of the whole shape (outline + fill)
};