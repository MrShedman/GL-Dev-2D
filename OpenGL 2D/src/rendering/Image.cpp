#include "Image.hpp"
//#define STB_IMAGE_IMPLEMENTATION
#include "..\staticLibs\stb_image.h"

#include <iostream>

bool Image::loadFromFile(const std::string& filename)
{
	unsigned char* data = stbi_load(filename.c_str(), &m_size.x, &m_size.y, &bytesPerPixel, 4);

	if (data == nullptr)
	{
		std::cout << "Unable to load texture: " << filename << std::endl;
		return false;
	}

	m_pixels.assign(data, data + (m_size.x * m_size.y * bytesPerPixel));

	return true;
}

const unsigned char* Image::getData() const
{
	return &m_pixels[0];
}

Vector2i Image::getSize() const
{
	return m_size;
}