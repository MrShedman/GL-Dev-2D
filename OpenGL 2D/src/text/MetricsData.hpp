#pragma once

#include <unordered_map>

struct Metrics
{
	float x;	// x - top left of character in texture
	float y;	// y - top left of character in texture
	float w;	// w - width of character
	float h;	// h - height of character
	float dx;	// xoffset - adjusts character positioning
	float dy;	// yoffset - adjusts character positioning
	float d;	// xadvance - adjusts character positioning
};

typedef std::unordered_map<char, Metrics> MetricsData;