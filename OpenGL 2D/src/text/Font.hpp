#pragma once

#include "MetricsData.hpp"
#include "..\rendering\Texture.hpp"
#include "SDFFUtility.hpp"
#include "..\math\Vector.h"
#include "..\math\Rect.h"

#include <iostream>

class Font
{
public:

	Font();

	Font(const Font& copy);

	bool loadFromFile(const std::string& filename);
	
	std::string getFamily() const { return mFamily; }

	float		getAscent(float fontSize = 12.0f) const { return mAscent * (fontSize / mFontSize); }

	float		getDescent(float fontSize = 12.0f) const { return mDescent * (fontSize / mFontSize); }

	inline float		getLeading(float fontSize = 12.0f) const 
	{
		return /*mLeading * */(fontSize / mFontSize); 
	}

	float		getSpaceWidth(float fontSize = 12.0f) const { return mSpaceWidth * (fontSize / mFontSize); }

	bool		contains(char charcode) const { return (m_metrics.find(charcode) != m_metrics.end()); }

	Metrics		getMetrics(char charcode) const;

	RectF		getBounds(char charcode, float fontSize = 12.0f) const;

	inline RectF getBounds(const Metrics &metrics, float fontSize = 12.0f) const;

	RectF		getTexCoords(char charcode) const;

	inline RectF getTexCoords(const Metrics &metrics) const;

	float		getAdvance(char charcode, float fontSize = 12.0f) const;

	inline float getAdvance(const Metrics &metrics, float fontSize = 12.0f) const;

	void		enableAndBind() const { }

	void		bind(GLuint textureUnit = 0) const {  }

	void		unbind(GLuint textureUnit = 0) const  {  }

	RectF	measure(const std::string &text, float fontSize = 12.0f) const;

	float		measureWidth(const std::string &text, float fontSize = 12.0f, bool precise = true) const;

	const Texture* getTexture() const;

private:

	bool				mInvalid;

	std::string			mFamily;

	float				mFontSize;
	float				mLeading;
	float				mAscent;
	float				mDescent;
	float				mSpaceWidth;

	Vector2i		m_textureSize;
	Texture			m_texture;
	MetricsData		m_metrics;
};