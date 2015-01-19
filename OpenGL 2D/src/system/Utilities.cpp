#include "Utilities.h"
#include "..\rendering\Sprite.hpp"
#include "..\window\Window.hpp"
#include "..\window\VideoMode.hpp"

std::vector<std::string> Util::Split(const std::string& s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = (unsigned int)s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}

void Util::scaleSpriteToFitWindow(Sprite &sprite, const Window &window)
{
	Vector2i tSize = sprite.getTexture()->getSize();
	Vector2i wSize = window.getSize();

	float sx = static_cast<float>(wSize.x) / static_cast<float>(tSize.x);
	float sy = static_cast<float>(wSize.y) / static_cast<float>(tSize.y);

	sprite.setScale(sx, sy);
}

std::vector<VideoMode> Util::getValidVideoModes()
{
	std::vector<VideoMode> modes = VideoMode::getFullscreenModes();

	// get the best bits per pixel supported
	unsigned int bpp = modes.front().bitsPerPixel;

	// remove all modes that have different bits per pixel
	auto end = std::remove_if(modes.begin(), modes.end(), [&](const VideoMode &v)
	{
		return v.bitsPerPixel != bpp;
	});

	std::vector<VideoMode> validModes;

	// populate new vector with found modes
	for (auto i = modes.begin(); i != end; ++i)
	{
		validModes.emplace_back(i->width, i->height, i->bitsPerPixel);
		//validModes.push_back(sf::VideoMode(i->width, i->height, bpp));
	}

	// reverse so worst is at front and best is at back
	std::reverse(validModes.begin(), validModes.end());

	return validModes;
}