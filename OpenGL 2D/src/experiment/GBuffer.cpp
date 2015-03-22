#include "GBuffer.hpp"

#include <iostream>

#include "..\gl\GLCheck.h"

GBuffer::GBuffer()
{
	m_fbo = 0;
	m_depthTexture = 0;
	m_textures[0] = 0;
	m_textures[1] = 0;
	m_textures[2] = 0;
}

GBuffer::~GBuffer()
{
	if (m_fbo != 0) {
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_textures[0] != 0) {
		glDeleteTextures(GBUFFER_NUM_TEXTURES, m_textures);
	}

	if (m_depthTexture != 0) {
		glDeleteTextures(1, &m_depthTexture);
	}
}


bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	// Create the FBO
	check_gl_error(glGenFramebuffers(1, &m_fbo));
	check_gl_error(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo));

	// Create the gbuffer textures
	glGenTextures(GBUFFER_NUM_TEXTURES, m_textures);
	glGenTextures(1, &m_depthTexture);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		check_gl_error(glBindTexture(GL_TEXTURE_2D, m_textures[i]));
		check_gl_error(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL));
		check_gl_error(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		check_gl_error(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		check_gl_error(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0));
	}

	// depth
	check_gl_error(glBindTexture(GL_TEXTURE_2D, m_depthTexture));
	check_gl_error(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	check_gl_error(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0));

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2 };

	check_gl_error(glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers));

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FB error, status: " << Status << std::endl;
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}


void GBuffer::BindForWriting()
{
	check_gl_error(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo));
}


void GBuffer::BindForReading()
{
	check_gl_error(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++)
	{
		check_gl_error(glActiveTexture(GL_TEXTURE0 + i));
		check_gl_error(glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]));
	}
}


void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}
