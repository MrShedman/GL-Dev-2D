#pragma once

#include "..\math\Vector.h"

class Color
{
public:

	Color();

	static Color rgb(unsigned char red, unsigned char green, unsigned char blue);

	static Color RGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

	static Color HSL(float h, float s, float l);

	static Color HSV(float h, float s, float v);

	static Color HEX(std::string hex);

	Vector3f toHSL() const;

	static const Color Black;       ///< Black predefined color
	static const Color White;       ///< White predefined color
	static const Color Red;         ///< Red predefined color
	static const Color Green;       ///< Green predefined color
	static const Color Blue;        ///< Blue predefined color
	static const Color Yellow;      ///< Yellow predefined color
	static const Color Magenta;     ///< Magenta predefined color
	static const Color Cyan;        ///< Cyan predefined color
	static const Color Transparent; ///< Transparent (black) predefined color

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

bool operator ==(const Color& left, const Color& right);

bool operator !=(const Color& left, const Color& right);

Color operator +(const Color& left, const Color& right);

Color operator -(const Color& left, const Color& right);

Color operator *(const Color& left, const Color& right);

Color& operator +=(Color& left, const Color& right);

Color& operator -=(Color& left, const Color& right);

Color& operator *=(Color& left, const Color& right);