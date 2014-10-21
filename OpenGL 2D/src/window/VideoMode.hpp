#pragma once

#include <vector>

class VideoMode
{
public:

    VideoMode();

    VideoMode(unsigned int modeWidth, unsigned int modeHeight, unsigned int modeBitsPerPixel = 32);

    static VideoMode getDesktopMode();

    static const std::vector<VideoMode>& getFullscreenModes();

    bool isValid() const;

    unsigned int width;        ///< Video mode width, in pixels
    unsigned int height;       ///< Video mode height, in pixels
    unsigned int bitsPerPixel; ///< Video mode pixel depth, in bits per pixels
};

bool operator ==(const VideoMode& left, const VideoMode& right);

bool operator !=(const VideoMode& left, const VideoMode& right);

bool operator <(const VideoMode& left, const VideoMode& right);

bool operator >(const VideoMode& left, const VideoMode& right);

bool operator <=(const VideoMode& left, const VideoMode& right);

bool operator >=(const VideoMode& left, const VideoMode& right);