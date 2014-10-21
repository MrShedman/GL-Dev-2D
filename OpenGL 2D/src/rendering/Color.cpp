#include "Color.h"
#include <algorithm>

const Color Color::Black(0, 0, 0);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);
const Color Color::Transparent(0, 0, 0, 0);

Color::Color() :
r(0),
g(0),
b(0),
a(255)
{

}

Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) :
r(red),
g(green),
b(blue),
a(alpha)
{

}

bool operator ==(const Color& left, const Color& right)
{
	return (left.r == right.r) &&
		(left.g == right.g) &&
		(left.b == right.b) &&
		(left.a == right.a);
}

bool operator !=(const Color& left, const Color& right)
{
	return !(left == right);
}

Color operator +(const Color& left, const Color& right)
{
	return Color(std::min(left.r + right.r, 255),
		std::min(left.g + right.g, 255),
		std::min(left.b + right.b, 255),
		std::min(left.a + right.a, 255));
}

Color operator -(const Color& left, const Color& right)
{
	return Color(std::max(left.r - right.r, 0),
		std::max(left.g - right.g, 0),
		std::max(left.b - right.b, 0),
		std::max(left.a - right.a, 0));
}

Color operator *(const Color& left, const Color& right)
{
	return Color(left.r * right.r / 255,
		left.g * right.g / 255,
		left.b * right.b / 255,
		left.a * right.a / 255);
}

Color& operator +=(Color& left, const Color& right)
{
	return left = left + right;
}

Color& operator -=(Color& left, const Color& right)
{
	return left = left - right;
}

Color& operator *=(Color& left, const Color& right)
{
	return left = left * right;
}