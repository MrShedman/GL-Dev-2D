#include "Font.hpp"

Font::Font()
: 
mInvalid(true), 
mFamily("Unknown"),
mFontSize(12.0f),
mLeading(0.0f),
mAscent(0.0f), 
mDescent(0.0f), 
mSpaceWidth(0.0f)
{
}

Font::Font(const Font& copy)
:
mInvalid(copy.mInvalid),
mFamily(copy.mFamily),
mFontSize(copy.mFontSize),
mLeading(copy.mLeading),
mAscent(copy.mAscent),
mDescent(copy.mDescent),
mSpaceWidth(copy.mSpaceWidth),
m_textureSize(copy.m_textureSize),
m_texture(copy.m_texture),
m_metrics(copy.m_metrics)
{

}

#include "..\window\Clock.h"

bool Font::loadFromFile(const std::string& filename)
{
	Clock t1;
	std::vector<unsigned char> pixels;

	loadSDFF(filename, m_metrics, pixels);

	std::cout << "sdff: " << t1.getElapsedTime().asMilliseconds() << "ms" << std::endl;

	int dimensions = static_cast<int>(std::sqrt(pixels.size() / 4));
	m_textureSize = Vector2i(dimensions, dimensions);

	return m_texture.loadFromMemory(pixels, m_textureSize.x, m_textureSize.y);
}

Metrics Font::getMetrics(char charcode) const
{
	MetricsData::const_iterator itr = m_metrics.find(charcode);
	if (itr == m_metrics.end()) return Metrics();

	return itr->second;
}

RectF Font::getBounds(char charcode, float fontSize) const
{
	MetricsData::const_iterator itr = m_metrics.find(charcode);
	if (itr != m_metrics.end())
		return getBounds(itr->second, fontSize);
	else
		return RectF();
}

RectF Font::getBounds(const Metrics &metrics, float fontSize) const
{
	float	scale = (fontSize / mFontSize);

	float top = -metrics.dy * scale;
	float bottom = (metrics.h - metrics.dy) * scale;
	float left = metrics.x * scale;
	float right = (metrics.dx + metrics.w) * scale;

	//return RectF(top, bottom, left, right);
	
	std::vector<Vector2f> points;
	points.push_back(Vector2f(metrics.dx, -metrics.dy) * scale);
	points.push_back(Vector2f(metrics.dx + metrics.w, metrics.h - metrics.dy) * scale);

	return RectF(points);
}

RectF Font::getTexCoords(char charcode) const
{
	MetricsData::const_iterator itr = m_metrics.find(charcode);
	if (itr != m_metrics.end())
		return getTexCoords(itr->second);
	else
		return RectF();
}

RectF Font::getTexCoords(const Metrics &metrics) const
{
	float top = metrics.y / m_textureSize.y;
	float bottom = (metrics.y + metrics.h) / m_textureSize.y;
	float left = metrics.x / m_textureSize.x;
	float right = (metrics.x + metrics.w) / m_textureSize.x;

	return RectF(top, bottom, left, right);

	std::vector<Vector2f> points;
	points.push_back(Vector2f(metrics.x / m_textureSize.x, metrics.y / m_textureSize.y));
	points.push_back(Vector2f((metrics.x + metrics.w) / m_textureSize.x, (metrics.y + metrics.h) / m_textureSize.y));

	return RectF(points);

/*	return Rectf(
		Vec2f(metrics.x1, metrics.y1) / mTextureSize,
		Vec2f(metrics.x2, metrics.y2) / mTextureSize
		);*/
}

float Font::getAdvance(char charcode, float fontSize) const
{
	MetricsData::const_iterator itr = m_metrics.find(charcode);
	if (itr != m_metrics.end())
		return getAdvance(itr->second, fontSize);

	return 0.0f;
}

float Font::getAdvance(const Metrics &metrics, float fontSize) const
{
	return metrics.d * fontSize / mFontSize;
}

RectF Font::measure(const std::string &text, float fontSize) const
{
	/*float offset = 0.0f;
	RectF result(0.0f, 0.0f, 0.0f, 0.0f);

	std::vector<Vector2f> points;

	std::string::const_iterator citr;
	for (citr = text.begin(); citr != text.end(); ++citr) {
		char charcode = (char)*citr;

		// TODO: handle special chars like /t

		MetricsData::const_iterator itr = m_metrics.find(charcode);
		if (itr != m_metrics.end()) 
		{
			points.push_back()
			result.include(
				RectF(offset + itr->second.dx, -itr->second.dy,
				offset + itr->second.dx + itr->second.w, itr->second.h - itr->second.dy)
				);
			offset += itr->second.d;
		}
	}

	// return
	return result.scaled(fontSize / mFontSize);
	return result;*/

	return RectF();
}

float Font::measureWidth(const std::string &text, float fontSize, bool precise) const
{
	float offset = 0.0f;
	float adjust = 0.0f;

	std::string::const_iterator citr;
	for (citr = text.begin(); citr != text.end(); ++citr) {
		char charcode = (char)*citr;

		// TODO: handle special chars like /t

		MetricsData::const_iterator itr = m_metrics.find(charcode);
		if (itr != m_metrics.end()) {
			offset += itr->second.d;

			// precise measurement takes into account that the last character 
			// contributes to the total width only by its own width, not its advance
			if (precise)
				adjust = itr->second.dx + itr->second.w - itr->second.d;
		}
	}

	return (offset + adjust) * (fontSize / mFontSize);
}

const Texture* Font::getTexture() const
{
	return &m_texture;
}