#include "Color.h"

#include <algorithm>
#include <functional>
#include <string>

const Color Color::Black = rgb(0, 0, 0);
const Color Color::White = rgb(255, 255, 255);
const Color Color::Red = rgb(255, 0, 0);
const Color Color::Green = rgb(0, 255, 0);
const Color Color::Blue = rgb(0, 0, 255);
const Color Color::Yellow = rgb(255, 255, 0);
const Color Color::Magenta = rgb(255, 0, 255);
const Color Color::Cyan = rgb(0, 255, 255);
const Color Color::Transparent = RGBA(0, 0, 0, 0);

Color::Color() :
r(0),
g(0),
b(0),
a(255)
{

}

Color Color::rgb(unsigned char red, unsigned char green, unsigned char blue)
{
	Color temp;
	temp.r = red;
	temp.g = green;
	temp.b = blue;
	return temp;
}

Color Color::RGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	Color temp;
	temp.r = red;
	temp.g = green;
	temp.b = blue;
	temp.a = alpha;
	return temp;
}

Color Color::HSL(float h, float s, float l)
{
	float r, g, b;

	if (s == 0.0f)
	{
		r = l;
		g = l;
		b = l;
	}
	else
	{
		std::function<float(float, float, float)> hue2rgb =
			[](float p, float q, float t)
		{
			if (t < 0.0f) t += 1.0f;
			if (t > 1.0f) t -= 1.0f;
			if (t < (1.0f / 6.0f)) return p + (q - p) * 6.0f * t;
			if (t < (1.0f / 2.0f)) return q;
			if (t < (2.0f / 3.0f)) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
			return p;
		};

		float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
		float p = 2.0f * l - q;

		r = hue2rgb(p, q, h + 1.0f / 3.0f);
		g = hue2rgb(p, q, h);
		b = hue2rgb(p, q, h - 1.0f / 3.0f);
	}

	unsigned char cr = static_cast<unsigned char>(std::round(r * 255.0f));
	unsigned char cg = static_cast<unsigned char>(std::round(g * 255.0f));
	unsigned char cb = static_cast<unsigned char>(std::round(b * 255.0f));

	return rgb(cr, cg, cb);
}

Color Color::HSV(float h, float s, float v)
{
	float r, g, b;

	int i = std::floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);

	switch (i % 6)
	{
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
	}

	return rgb(r * 255, g * 255, b * 255);
}

Color Color::HEX(std::string hex)
{
	hex.erase(std::remove(hex.begin(), hex.end(), '#'), hex.end());

	int num = std::stoi(hex, 0, 16);

	int r = num / 0x10000;
	int g = (num / 0x100) % 0x100;
	int b = num % 0x100;

	return rgb(r, g, b);
}

Vector3f Color::toHSL() const
{
	float nr = static_cast<float>(r) / 255.f;
	float ng = static_cast<float>(g) / 255.f;
	float nb = static_cast<float>(b) / 255.f;

	float max = Vector3f(nr, ng, nb).Max();
	float min = Vector3f(nr, ng, nb).Min();

	float h, s, l = (max + min) / 2.f;

	if (max == min)
	{
		h = 0.0f;
		s = 0.0f;
	}
	else
	{
		float d = max - min;

		s = l > 0.5f ? (2.0f - max - min) : d / (max + min);

		if (max == nr)
		{
			h = (ng - nb) / d + (g < b ? 6.0f : 0.0f);
		}
		else if (max == ng)
		{
			h = (nb - nr) / d + 2.0f;
		}
		else
		{
			h = (nr - ng) / d + 4.0f;
		}

		h /= 6.0f;
	}

	return Vector3f(h, s, l);
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
	return Color::RGBA(std::min(left.r + right.r, 255),
		std::min(left.g + right.g, 255),
		std::min(left.b + right.b, 255),
		std::min(left.a + right.a, 255));
}

Color operator -(const Color& left, const Color& right)
{
	return Color::RGBA(std::max(left.r - right.r, 0),
		std::max(left.g - right.g, 0),
		std::max(left.b - right.b, 0),
		std::max(left.a - right.a, 0));
}

Color operator *(const Color& left, const Color& right)
{
	return Color::RGBA(left.r * right.r / 255,
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