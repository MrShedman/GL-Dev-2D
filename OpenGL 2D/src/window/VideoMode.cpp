#include "VideoMode.hpp"
#include <windows.h>
#include <algorithm>
#include <functional>

VideoMode::VideoMode() :
width       (0),
height      (0),
bitsPerPixel(0)
{

}

VideoMode::VideoMode(unsigned int modeWidth, unsigned int modeHeight, unsigned int modeBitsPerPixel) :
width       (modeWidth),
height      (modeHeight),
bitsPerPixel(modeBitsPerPixel)
{

}

VideoMode VideoMode::getDesktopMode()
{
	DEVMODE win32Mode;
	win32Mode.dmSize = sizeof(win32Mode);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &win32Mode);

	return VideoMode(win32Mode.dmPelsWidth, win32Mode.dmPelsHeight, win32Mode.dmBitsPerPel);
}

const std::vector<VideoMode>& VideoMode::getFullscreenModes()
{
    static std::vector<VideoMode> modes;

    // Populate the array on first call
    if (modes.empty())
    {
		DEVMODE win32Mode;
		win32Mode.dmSize = sizeof(win32Mode);
		for (int count = 0; EnumDisplaySettings(NULL, count, &win32Mode); ++count)
		{
			// Convert to sf::VideoMode
			VideoMode mode(win32Mode.dmPelsWidth, win32Mode.dmPelsHeight, win32Mode.dmBitsPerPel);

			// Add it only if it is not already in the array
			if (std::find(modes.begin(), modes.end(), mode) == modes.end())
				modes.push_back(mode);
		}
        std::sort(modes.begin(), modes.end(), std::greater<VideoMode>());
    }

    return modes;
}

bool VideoMode::isValid() const
{
    const std::vector<VideoMode>& modes = getFullscreenModes();

    return std::find(modes.begin(), modes.end(), *this) != modes.end();
}

bool operator ==(const VideoMode& left, const VideoMode& right)
{
    return (left.width        == right.width)        &&
           (left.height       == right.height)       &&
           (left.bitsPerPixel == right.bitsPerPixel);
}

bool operator !=(const VideoMode& left, const VideoMode& right)
{
    return !(left == right);
}

bool operator <(const VideoMode& left, const VideoMode& right)
{
    if (left.bitsPerPixel == right.bitsPerPixel)
    {
        if (left.width == right.width)
        {
            return left.height < right.height;
        }
        else
        {
            return left.width < right.width;
        }
    }
    else
    {
        return left.bitsPerPixel < right.bitsPerPixel;
    }
}

bool operator >(const VideoMode& left, const VideoMode& right)
{
    return right < left;
}

bool operator <=(const VideoMode& left, const VideoMode& right)
{
    return !(right < left);
}

bool operator >=(const VideoMode& left, const VideoMode& right)
{
    return !(left < right);
}