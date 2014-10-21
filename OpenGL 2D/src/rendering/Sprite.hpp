#pragma once

#include "Transform.hpp"
#include "Drawable.hpp"
#include "Texture.hpp"
#include "Color.h"
#include "..\math\Rect.h"


class Sprite : public Drawable, public Transform
{
public:

	Sprite();

	explicit Sprite(Texture& texture);

	Sprite(Texture& texture, const RectI& rectangle);

	~Sprite();

	void setTexture(Texture& texture, bool resetRect = false);

	void setTextureRect(const RectI& rectangle);

	void setColor(const Color& color);

	const Texture* getTexture() const;

	const RectI& getTextureRect() const;

	const Color& getColor() const;

	RectF getLocalBounds() const;

	RectF getGlobalBounds() const;

	void bindVBO();

private:

	void generateVBO();

	void draw(RenderTarget2D& target, RenderStates states) const;

	void updatePositions();

	void updateTexCoords();

	GLuint mVBOID;
	GLuint mIBOID;

	Vertex		m_vertices[4]; ///< Vertices defining the sprite's geometry
	Texture*	m_texture;     ///< Texture of the sprite
	RectI		m_textureRect; ///< Rectangle defining the area of the source texture to display
};