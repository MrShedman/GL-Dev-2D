#pragma once

#include "Transform.hpp"
#include "Drawable.hpp"
#include "Texture.hpp"
#include "Color.h"
#include "..\math\Rect.h"

#include <memory>
#include <iostream>

#include "..\gl\Buffer.hpp"

class Sprite : public Drawable, public Transform
{
public:

	Sprite();

	explicit Sprite(const Texture& texture);

	Sprite(const Texture& texture, const RectI& rectangle);

	void setTexture(const Texture& texture, bool resetRect = false);

	void setTextureRect(const RectI& rectangle);

	void setColor(const Color& color);

	const Texture* getTexture() const;

	const RectI& getTextureRect() const;

	const Color& getColor() const;

	RectF getLocalBounds() const;

	RectF getGlobalBounds() const;
	
private:

	Sprite(const Sprite& other) = delete;

	Sprite operator = (const Sprite&) = delete;

	void draw(RenderTarget2D& target, RenderStates states) const;

	void updatePositions();

	void updateTexCoords();

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	Vertex		m_vertices[4]; ///< Vertices defining the sprite's geometry
	const Texture*	m_texture;     ///< Texture of the sprite
	RectI		m_textureRect; ///< Rectangle defining the area of the source texture to display
};