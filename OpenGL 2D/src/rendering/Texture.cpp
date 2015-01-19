#include "Texture.hpp"

#include <iostream>

#include "..\Application.hpp"
#include "..\math\Rect.h" 
#define STB_IMAGE_IMPLEMENTATION
#include "..\staticLibs\stb_image.h"

Texture::Texture()
{
	m_isSmooth = false;

	m_textureID = 0;

	m_width = 0;
	m_height = 0;
}

Texture::~Texture()
{
	if (m_textureID != 0)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
	}
}

Texture::Texture(const Texture& copy)
{
	if (copy.m_textureID)
	{
		m_isSmooth = copy.m_isSmooth;

		m_textureID = copy.m_textureID;

		m_width = copy.m_width;
		m_height = copy.m_height;
	}
}

bool Texture::loadFromFile(const std::string& filename)
{
	Image image;

	if (!image.loadFromFile(filename))
	{
		return false;
	}

	return loadFromImage(image);
}

bool Texture::loadFromImage(const Image& image)
{
	return loadFromMemory(image.getData(), image.getSize().x, image.getSize().y);
}

bool Texture::loadFromMemory(const std::vector<unsigned char>& pixels, int width, int height)
{
	return loadFromMemory(pixels.data(), width, height);
}

bool Texture::loadFromMemory(const unsigned char* pixels, int width, int height)
{
	m_width = width;
	m_height = height;

	glGenTextures(1, &m_textureID);

	//Bind texture ID
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	//Generate texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, NULL);

	m_isSmooth = true;

	return true;
}

Vector2i Texture::getSize() const
{
	return Vector2i(m_width, m_height);
}

int Texture::getID() const
{
	return m_textureID;
}

void Texture::bind(const Texture* texture, int unit)
{
	if (texture && texture->getID())
	{
		// Bind the texture
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texture->getID());
	}
	else
	{
		// Bind no texture
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
}

void Texture::setSmooth(bool smooth)
{
	if (smooth != m_isSmooth)
	{
		m_isSmooth = smooth;

		if (m_textureID)
		{
			glBindTexture(GL_TEXTURE_2D, m_textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
		}
	}
}