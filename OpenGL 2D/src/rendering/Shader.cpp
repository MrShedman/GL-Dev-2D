#include "Shader.hpp"

#include <fstream>
#include <iostream>

#include "GL\glew.h"

namespace
{
	bool getFileContents(const std::string& filename, std::vector<char>& buffer)
	{
		std::ifstream file(filename.c_str(), std::ios_base::binary);
		if (file)
		{
			file.seekg(0, std::ios_base::end);
			std::streamsize size = file.tellg();
			if (size > 0)
			{
				file.seekg(0, std::ios_base::beg);
				buffer.resize(static_cast<std::size_t>(size));
				file.read(&buffer[0], size);
			}
			buffer.push_back('\0');
			return true;
		}
		else
		{
			return false;
		}
	}

	void printProgramLog(GLuint obj)
	{
		GLint infoLogLength = 0;
		GLsizei charsWritten = 0;
		GLchar *infoLog;

		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::cout << infoLogLength << std::endl;

		if (infoLogLength > 0)
		{
			infoLog = (char *)malloc(infoLogLength);
			glGetProgramInfoLog(obj, infoLogLength, &charsWritten, infoLog);
			printf("%s\n", infoLog);
			free(infoLog);
		}
	}
}

Shader2D::Shader2D()
{

}

Shader2D::~Shader2D()
{
	glDeleteProgram(m_programID);
	m_programID = 0;
}

#include "..\window\Clock.h"

bool Shader2D::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, Desc c)
{
	// Read the vertex shader file
	std::vector<char> vertexShader;
	if (!getFileContents(vertexShaderFilename, vertexShader))
	{
		std::cout << "Failed to open vertex shader file \"" << vertexShaderFilename << "\"" << std::endl;
		return false;
	}

	// Read the fragment shader file
	std::vector<char> fragmentShader;
	if (!getFileContents(fragmentShaderFilename, fragmentShader))
	{
		std::cout << "Failed to open fragment shader file \"" << fragmentShaderFilename << "\"" << std::endl;
		return false;
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	vert = glCreateShader(GL_VERTEX_SHADER);

	if (frag == 0)
	{
		std::cout << "Error creating shader type: Fragment" << std::endl;

		return false;
	}
	if (vert == 0)
	{
		std::cout << "Error creating shader type: Vertex" << std::endl;

		return false;
	}

	m_programID = glCreateProgram();

	if (!compile(fragmentShader, frag) || !compile(vertexShader, vert))
	{
		return false;
	}

	glLinkProgram(m_programID);

	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &programSuccess);
	
	if (programSuccess != GL_TRUE)
	{
		std::cout << "Error linking program: " << m_programID << std::endl;
		printProgramLog(m_programID);
		glDeleteProgram(m_programID);
		glDetachShader(m_programID, frag);
		glDetachShader(m_programID, vert);
		m_programID = 0;
		return false;
	}

	m_positionLocation = getAttributeLocation(c.vertex);
	m_normalLocation = getAttributeLocation(c.normal);
	m_texCoordLocation = getAttributeLocation(c.texture);
	m_colourLocation = getAttributeLocation(c.color);

	m_projectionMatrixLocation = getParamLocation("LProjectionMatrix");
	m_modelViewMatrixLocation = getParamLocation("LModelViewMatrix");

	glDetachShader(m_programID, frag);
	glDetachShader(m_programID, vert);
	glDeleteShader(frag);
	glDeleteShader(vert);

	return true;
}

int Shader2D::getProgramID() const
{
	return m_programID;
}

void Shader2D::bind(const Shader2D* shader)
{
	if (shader && shader->getProgramID())
	{
		// Bind the shader
		glUseProgram(shader->getProgramID());
	}
	else
	{
		// Bind no shader
		glUseProgram(0);
	}
}

bool Shader2D::compile(const std::vector<char>& buffer, int shader)
{
	const GLchar* p[1];
	p[0] = &buffer[0];
	GLint lengths[1];
	lengths[0] = buffer.size();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLchar InfoLog[1024];

		glGetShaderInfoLog(shader, 1024, NULL, InfoLog);
	
		std::cout << InfoLog << std::endl;

		return false;
	}

	glAttachShader(m_programID, shader);

	return true;
}