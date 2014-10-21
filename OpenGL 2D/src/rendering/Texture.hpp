#pragma once

#include <string>

#include "GL\glew.h"
#include "..\math\Vector.h"
#include "Color.h"
#include "Vertex.hpp"

class Texture
{
public:

	Texture();

	~Texture();

	bool loadFromFile(const std::string& filename);

	bool loadFromMemory(unsigned char* pixels, int width, int height);

	bool loadFromMemory(std::vector<unsigned char>& pixels, int width, int height);

	Vector2i getSize() const;

	int getID() const;

	static void bind(const Texture* texture, int unit = 0);

	void setSmooth(bool smooth);
	
private:
	
	bool m_isSmooth;

	GLuint m_textureID;

	//Texture dimensions
	int m_width;
	int m_height;
};