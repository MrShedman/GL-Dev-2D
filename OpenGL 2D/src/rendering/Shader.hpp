#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "..\math\Matrix.h"
#include "Color.h"
#include "GL\glew.h"

#include "..\gl\GLCheck.h"

class Shader2D
{
public:

	struct Desc
	{
		std::string vertex;
		std::string normal;
		std::string texture;
		std::string color;
	};

	Shader2D();

	~Shader2D();

	bool loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, Desc c);

	int getProgramID() const;

	static void bind(const Shader2D* shader);
	
	void setVertexPointer(GLsizei stride, const GLvoid* data) const
	{
		check_gl_error(glVertexAttribPointer(m_positionLocation, 3, GL_FLOAT, GL_FALSE, stride, data));
	}

	void setNormalPointer(GLsizei stride, const GLvoid* data) const
	{
		check_gl_error(glVertexAttribPointer(m_normalLocation, 3, GL_FLOAT, GL_TRUE, stride, data));
	}

	void setTexCoordPointer(GLsizei stride, const GLvoid* data) const
	{
		check_gl_error(glVertexAttribPointer(m_texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, data));
	}

	void setColourPointer(GLsizei stride, const GLvoid* data) const
	{
		check_gl_error(glVertexAttribPointer(m_colourLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, data));
	}

	void enableVertexPointer() const
	{
		check_gl_error(glEnableVertexAttribArray(m_positionLocation));
	}

	void disableVertexPointer() const
	{
		check_gl_error(glDisableVertexAttribArray(m_positionLocation));
	}

	void enableNormalPointer() const
	{
		check_gl_error(glEnableVertexAttribArray(m_normalLocation));
	}

	void disableNormalPointer() const
	{
		check_gl_error(glDisableVertexAttribArray(m_normalLocation));
	}

	void enableTexCoordPointer() const
	{
		check_gl_error(glEnableVertexAttribArray(m_texCoordLocation));
	}

	void disableTexCoordPointer() const
	{
		check_gl_error(glDisableVertexAttribArray(m_texCoordLocation));
	}

	void enableColourPointer() const
	{
		check_gl_error(glEnableVertexAttribArray(m_colourLocation));
	}

	void disableColourPointer() const
	{
		check_gl_error(glDisableVertexAttribArray(m_colourLocation));
	}

	void setProjectionMatrix(const Matrix4f& mProjectionMatrix) const
	{
		check_gl_error(glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, mProjectionMatrix[0]));
	}

	void setModelViewMatrix(const Matrix4f& mModelViewMatrix) const
	{
		check_gl_error(glUniformMatrix4fv(m_modelViewMatrixLocation, 1, GL_FALSE, mModelViewMatrix[0]));
	}
	
	void setTextureUnit(const std::string& name, int unit)
	{
		if (m_programID)
		{
			// Enable program
			Shader2D::bind(this);

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{
				check_gl_error(glUniform1i(location, unit));
			}

			// Disable program
			Shader2D::bind(NULL);
		}
	}

	void setParameter(const std::string& name, float x)
	{
		if (m_programID)
		{
			// Enable program
			Shader2D::bind(this);

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{
				check_gl_error(glUniform1f(location, x));
			}

			// Disable program
			Shader2D::bind(NULL);
		}
	}

	void setParameter(const std::string& name, float x, float y)
	{
		if (m_programID)
		{
			// Enable program
			Shader2D::bind(this);

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{
				check_gl_error(glUniform2f(location, x, y));
			}

			// Disable program
			Shader2D::bind(NULL);
		}
	}

	void setParameter(const std::string& name, float x, float y, float z)
	{
		if (m_programID)
		{
			// Enable program
			Shader2D::bind(this);

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{
				check_gl_error(glUniform3f(location, x, y, z));
			}

			// Disable program
			Shader2D::bind(NULL);
		}
	}

	void setParameter(const std::string& name, const Vector2f& v)
	{
		setParameter(name, v.x, v.y);
	}

	void setParameter(const std::string& name, const Vector3f& v)
	{
		setParameter(name, v.x, v.y, v.z);
	}

	void setParameter(const std::string& name, const Matrix4f& m)
	{
		if (m_programID)
		{
			// Enable program
			Shader2D::bind(this);

			// Get parameter location and assign it new values
			GLint location = getParamLocation(name);
			if (location != -1)
			{

				check_gl_error(glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, m[0]));
			}

			// Disable program
			Shader2D::bind(NULL);
		}
	}

private:

	int getAttributeLocation(const std::string& name)
	{
		int location = glGetAttribLocation(m_programID, name.c_str());

		if (location == -1)
		{
			std::cout << "Parameter \"" << name << "\" not found in shader" << std::endl;
		}

		std::cout << "Paramete: " << name << " is in location: " << location << std::endl;

		return location;
	}

	typedef std::map<std::string, int> ParamTable;

	int getParamLocation(const std::string& name)
	{
		// Check the cache
		ParamTable::const_iterator it = m_params.find(name);
		
		if (it != m_params.end())
		{
			// Already in cache, return it
			return it->second;
		}
		else
		{
			// Not in cache, request the location from OpenGL
			int location = glGetUniformLocation(m_programID, name.c_str());
			m_params.insert(std::make_pair(name, location));

			if (location == -1)
			{
				std::cout << "Parameter \"" << name << "\" not found in shader" << std::endl;
			}

			return location;
		}
	}

	ParamTable m_params;

	GLint m_positionLocation;
	GLint m_normalLocation;
	GLint m_texCoordLocation;
	GLint m_colourLocation;

	//Projection matrix
	GLint m_projectionMatrixLocation;

	//Modelview matrix
	GLint m_modelViewMatrixLocation;

	int m_programID;
	int frag;
	int vert;

	bool compile(const std::vector<char>& buffer, int shader);
};