#include "Texture.hpp"

#include <iostream>

#include "..\Application.hpp"
#include "..\math\Rect.h"
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

bool Texture::loadFromFile(const std::string& filename)
{
	int bytesPerPixel;
	unsigned char* data = stbi_load(filename.c_str(), &m_width, &m_height, &bytesPerPixel, 4);

	if (data == nullptr)
	{
		std::cout << "Unable to load texture: " << filename << std::endl;
		return false;
	}

	return loadFromMemory(data, m_width, m_height);
}

bool Texture::loadFromMemory(unsigned char* pixels, int width, int height)
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

bool Texture::loadFromMemory(std::vector<unsigned char>& pixels, int width, int height)
{
	m_width = width;
	m_height = height;

	glGenTextures(1, &m_textureID);

	//Bind texture ID
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	//Generate texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

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