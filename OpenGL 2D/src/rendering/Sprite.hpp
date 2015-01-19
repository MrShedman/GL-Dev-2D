#pragma once

#include "Transform.hpp"
#include "Drawable.hpp"
#include "Texture.hpp"
#include "Color.h"
#include "..\math\Rect.h"

#include <memory>
#include <iostream>
class VBO
{
public:

	VBO()
	{
		//Vertex data
		Vertex vData[4];
		GLuint iData[4];

		//Set rendering indices
		iData[0] = 0;
		iData[1] = 1;
		iData[2] = 2;
		iData[3] = 3;

		//Create VBO
		glGenBuffers(1, &mVBOID);
		glBindBuffer(GL_ARRAY_BUFFER, mVBOID);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vData, GL_STATIC_DRAW);

		//Create IBO
		glGenBuffers(1, &mIBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_STATIC_DRAW);

	}

	~VBO()
	{
		std::cout << "destroying vbo!" << std::endl;

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

		if (mVBOID != 0)
		{
			glDeleteBuffers(1, &mVBOID);
			glDeleteBuffers(1, &mIBOID);
		}
	}

	void bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);
	}

private:

	GLuint mVBOID;
	GLuint mIBOID;
};


class Sprite : public Drawable, public Transform
{
public:

	Sprite();

	explicit Sprite(const Texture& texture);

	Sprite(const Texture& texture, const RectI& rectangle);

	~Sprite();

	void setTexture(const Texture& texture, bool resetRect = false);

	void setTextureRect(const RectI& rectangle);

	void setColor(const Color& color);

	const Texture* getTexture() const;

	const RectI& getTextureRect() const;

	const Color& getColor() const;

	RectF getLocalBounds() const;

	RectF getGlobalBounds() const;

	static void bind(const Sprite* sprite);

private:

	std::shared_ptr<VBO> m_vbo;
	//static GLuint mVBOID;
	//static GLuint mIBOID;

	static void generateVBO();

	void draw(RenderTarget2D& target, RenderStates states) const;

	void updatePositions();

	void updateTexCoords();

	Vertex		m_vertices[4]; ///< Vertices defining the sprite's geometry
	const Texture*	m_texture;     ///< Texture of the sprite
	RectI		m_textureRect; ///< Rectangle defining the area of the source texture to display
};