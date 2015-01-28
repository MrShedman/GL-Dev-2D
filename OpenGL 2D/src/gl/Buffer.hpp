#pragma once

#include "GLCheck.h"

class Buffer
{
public:
	
	Buffer(GLenum target, GLenum usage) : m_target(target), m_usage(usage)
	{
		glGenBuffers(1, &m_name);
	}

	~Buffer()
	{
		glDeleteBuffers(1, &m_name);
	}

	Buffer(const Buffer& other) = delete;

	Buffer operator = (const Buffer&) = delete;

	Buffer(Buffer&& other) :
		m_name(0u),
		m_target(0u),
		m_usage(0u)
	{
		std::swap(m_name, other.m_name);
		std::swap(m_target, other.m_target);
		std::swap(m_usage, other.m_usage);
	}

	Buffer& operator=(Buffer&& other)
	{
		if (this != &other)
		{
			m_name = 0u;
			m_target = 0u;
			m_usage = 0u;

			std::swap(m_name, other.m_name);
			std::swap(m_target, other.m_target);
			std::swap(m_usage, other.m_usage);
		}

		return *this;
	}

	GLuint name() const 
	{
		return m_name;
	}

	void bind() const
	{
		check_gl_error(glBindBuffer(m_target, m_name));
	}

	void unbind() const
	{
		check_gl_error(glBindBuffer(m_target, NULL));
	}

	void data(GLsizeiptr data_size, const GLvoid* data_ptr)
	{
		assert(data_size >= 0);

		bind();

		check_gl_error(glBufferData(m_target, data_size, data_ptr, m_usage));

		unbind();
	}

private:

	GLenum m_target;
	GLenum m_usage;

	GLuint m_name;
};