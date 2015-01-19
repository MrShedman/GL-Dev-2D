
#include "RenderTexture.hpp"

#include <iostream>

RenderTexture::RenderTexture()
{
	m_frameBuffer = 0;
}

RenderTexture::~RenderTexture()
{
	// Destroy the frame buffer
	if (m_frameBuffer)
	{
		glDeleteFramebuffers(1, &m_frameBuffer);
	}
}

bool RenderTexture::create(unsigned int width, unsigned int height)
{
	if (!m_texture.loadFromMemory(NULL, width, height))
	{
		std::cout << "Error creating texture for rendertexture" << std::endl;
		return false;
	}

	glGenFramebuffers(1, &m_frameBuffer);

	if (!m_frameBuffer)
	{
		std::cout << "Impossible to create render texture (failed to create the frame buffer object)" << std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	// Link the texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.getID(), 0);

	// A final check, just to be sure...
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		std::cout << "Impossible to create render texture (failed to link the target texture to the frame buffer)" << std::endl;
		return false;
	}

	m_size = Vector2u(width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void RenderTexture::bind(const RenderTexture* texture)
{
	if (texture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, texture->m_frameBuffer);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void RenderTexture::display()
{
	glFlush();
}

Vector2u RenderTexture::getSize() const
{
	return m_size;
}

const Texture& RenderTexture::getTexture() const
{
	return m_texture;
}