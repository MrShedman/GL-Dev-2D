#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "..\math\Matrix.h"
#include "Color.h"
#include "GL\glew.h"

void outputMatrix(const Matrix4f& m);

class Shader2D
{
public:

	Shader2D();

	~Shader2D();

	bool loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	int getProgramID() const;

	static void bind(const Shader2D* shader);
	
	void setVertexPointer(GLsizei stride, const GLvoid* data)
	{
		glVertexAttribPointer(m_positionLocation, 2, GL_FLOAT, GL_FALSE, stride, data);
	}

	void setTexCoordPointer(GLsizei stride, const GLvoid* data)
	{
		glVertexAttribPointer(m_texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, data);
	}

	void setColourPointer(GLsizei stride, const GLvoid* data)
	{
		glVertexAttribPointer(m_colourLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, data);
	}

	void enableVertexPointer()
	{
		glEnableVertexAttribArray(m_positionLocation);
	}

	void disableVertexPointer()
	{
		glDisableVertexAttribArray(m_positionLocation);
	}

	void enableTexCoordPointer()
	{
		glEnableVertexAttribArray(m_texCoordLocation);
	}

	void disableTexCoordPointer()
	{
		glDisableVertexAttribArray(m_texCoordLocation);
	}

	void enableColourPointer()
	{
		glEnableVertexAttribArray(m_colourLocation);
	}

	void disableColourPointer()
	{
		glDisableVertexAttribArray(m_colourLocation);
	}

	void setProjectionMatrix(const Matrix4f& mProjectionMatrix)
	{
		glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, mProjectionMatrix[0]);
	}

	void setModelViewMatrix(const Matrix4f& mModelViewMatrix)
	{
		glUniformMatrix4fv(m_modelViewMatrixLocation, 1, GL_FALSE, mModelViewMatrix[0]);
	}
	
private:

	GLint m_positionLocation;
	GLint m_texCoordLocation;
	GLint m_colourLocation;

	//Projection matrix
	GLint m_projectionMatrixLocation;

	//Modelview matrix
	GLint m_modelViewMatrixLocation;

	int m_programID;

	bool compile(const std::vector<char>& buffer, int shader);
};