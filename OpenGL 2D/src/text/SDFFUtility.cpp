#include "SDFFUtility.hpp"

#include <iostream>

bool loadSDFF(const std::string& filename, MetricsData& metrics, std::vector<unsigned char>& pixels)
{
	metrics.clear();
	pixels.clear();

	BinaryStreamRead in(filename);

	std::string sdf = in.readString(4);

	if (sdf != "SDFF")
	{
		return false;
	}

	in.advance(1);

	size_t size = in.read<size_t>();

	std::string name = in.readString(size);
	in.advance(1);

	int glyphCount = in.read<int>();

	std::cout << "\nLoading SDF Font: " << name << std::endl;
	std::cout << "Glyph #: " << glyphCount << std::endl;

	for (int i = 0; i < glyphCount; ++i)
	{
		Metrics m;

		char charcode = static_cast<char>(in.read<int>());
		m.x = in.read<float>();
		m.y = in.read<float>();
		m.w = in.read<float>();
		m.h = in.read<float>();
		m.dx = in.read<float>();
		m.dy = in.read<float>();
		m.d = in.read<float>();

		metrics[charcode] = m;

		in.advance(1);
	}

	in.advance(1);

	int imageSize = in.read<int>();

	int realImagesize = std::sqrt(imageSize);
	std::cout << "Image dimensions: " << realImagesize << "x" << realImagesize << std::endl << std::endl;
	imageSize *= 4;
	pixels.resize(imageSize);


	for (unsigned int i = 0; i < imageSize; i += 4)
	{
		unsigned char temp = in.read<unsigned char>();

		pixels[i + 0] = temp;
		pixels[i + 1] = temp;
		pixels[i + 2] = temp;
		pixels[i + 3] = temp;
	}

	return true;
}