#pragma once

#include <string>
#include <vector>

#include "..\math\Vector.h"

class Image
{
public:

	bool loadFromFile(const std::string& filename);

	const unsigned char* Image::getData() const;
	Vector2i getSize() const;

private:

	int bytesPerPixel;
	std::vector<unsigned char> m_pixels;
	Vector2i m_size;
};