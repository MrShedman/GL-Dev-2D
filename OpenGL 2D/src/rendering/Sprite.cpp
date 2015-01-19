#include "Sprite.hpp"

#include "RenderTarget2D.h"

#include <memory>

namespace
{

	std::shared_ptr<VBO> m_sharedVBO;

	const std::shared_ptr<VBO>& initVBO()
	{
		if (!m_sharedVBO)
		{
			m_sharedVBO.reset(new VBO());
		}

		return m_sharedVBO;
	}
}


Sprite::Sprite() :
m_texture(NULL),
m_textureRect()
{
	m_vbo = initVBO();
//	mVBOID = 0;
	//mIBOID = 0;
}

Sprite::Sprite(const Texture& texture) :
m_texture(NULL),
m_textureRect()
{
	//mVBOID = 0;
	//mIBOID = 0;
	setTexture(texture);
}

Sprite::Sprite(const Texture& texture, const RectI& rectangle) :
m_texture(NULL),
m_textureRect()
{
	//mVBOID = 0;
	//mIBOID = 0;
	setTexture(texture);
	setTextureRect(rectangle);
}

Sprite::~Sprite()
{
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

//	if (mVBOID != 0)
	//{
	//	glDeleteBuffers(1, &mVBOID);
	//	glDeleteBuffers(1, &mIBOID);
	//}
}

void Sprite::setTexture(const Texture& texture, bool resetRect)
{
	// Assign the new texture
	m_texture = &texture;

	// Recompute the texture area if requested, or if there was no valid texture & rect before
	if (resetRect || (m_textureRect == RectI()))
	{
		setTextureRect(RectI(0, texture.getSize().y, 0, texture.getSize().x));
	}

	m_vbo = initVBO();
	//generateVBO();
}

void Sprite::setTextureRect(const RectI& rectangle)
{
	if (rectangle != m_textureRect)
	{
		m_textureRect = rectangle;
		updatePositions();
		updateTexCoords();
	}
}

void Sprite::setColor(const Color& color)
{
	// Update the vertices' color
	m_vertices[0].color = color;
	m_vertices[1].color = color;
	m_vertices[2].color = color;
	m_vertices[3].color = color;
}

const Texture* Sprite::getTexture() const
{
	return m_texture;
}

const RectI& Sprite::getTextureRect() const
{
	return m_textureRect;
}

const Color& Sprite::getColor() const
{
	return m_vertices[0].color;
}

RectF Sprite::getLocalBounds() const
{
	float right = static_cast<float>(std::abs(m_textureRect.right));
	float bottom = static_cast<float>(std::abs(m_textureRect.bottom));

	return RectF(0.f, bottom, 0.f, right);
}

RectF Sprite::getGlobalBounds() const
{
	return getTransform().Transform(getLocalBounds());
}

void Sprite::generateVBO()
{
	/*Vertex vData[4];
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
	
	//Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);*/
}

void Sprite::bind(const Sprite* sprite)
{
	/*if (sprite)
	{
		glBindBuffer(GL_ARRAY_BUFFER, sprite->mVBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->mIBOID);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}*/
}


void Sprite::draw(RenderTarget2D& target, RenderStates states) const
{
	if (m_texture)
	{
	//	Sprite::bind(this);
		m_vbo->bind();

		states.transform *= getTransform();
		states.texture = m_texture;
		target.draw(m_vertices, 4, TrianglesStrip, states);

		//Sprite::bind(NULL);
	}
}

void Sprite::updatePositions()
{
	RectF bounds = getLocalBounds();

	m_vertices[0].position = Vector2f(0.f, 0.f);
	m_vertices[1].position = Vector2f(0.f, bounds.getHeight());
	m_vertices[2].position = Vector2f(bounds.getWidth() , 0.f);
	m_vertices[3].position = Vector2f(bounds.getWidth(), bounds.getHeight());
}

void Sprite::updateTexCoords()
{
	float left = static_cast<float>(m_textureRect.left) / m_texture->getSize().x;
	float right = static_cast<float>(m_textureRect.right) / m_texture->getSize().x;;
	float top = static_cast<float>(m_textureRect.top) / m_texture->getSize().y;
	float bottom = static_cast<float>(m_textureRect.bottom) / m_texture->getSize().y;

	m_vertices[0].texCoords = Vector2f(left, top);
	m_vertices[1].texCoords = Vector2f(left, bottom);
	m_vertices[2].texCoords = Vector2f(right, top);
	m_vertices[3].texCoords = Vector2f(right, bottom);
}