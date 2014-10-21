#pragma once

class Color
{
public:

	Color();

	Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);

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