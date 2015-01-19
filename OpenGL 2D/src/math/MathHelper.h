#pragma once

#define MATH_PI 3.1415926535897932384626433832795
#define ToRadians(x) (float)(((x) * MATH_PI / 180.0f))
#define ToDegrees(x) (float)(((x) * 180.0f / MATH_PI))

#define INVALID_VALUE 0xFFFFFFFF

template<typename T>
inline T Clamp(const T &a, const T &min, const T &max)
{
	if (a < min) return min;
	else if (a > max) return max;
	else return a;
}

float randomize(float min, float max);