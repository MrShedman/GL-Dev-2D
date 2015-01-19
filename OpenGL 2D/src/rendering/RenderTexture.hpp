#pragma once

#include "Texture.hpp"
#include "..\math\Vector.h"

class RenderTexture
{
public:

	RenderTexture();

	~RenderTexture();

	bool create(unsigned int width, unsigned int height);

	void display();

	static void bind(const RenderTexture* texture);

	void bind();

	Vector2u getSize() const;

	const Texture& getTexture() const;

private:

	GLuint m_frameBuffer;

	Vector2i m_size;
	Texture m_texture;
};