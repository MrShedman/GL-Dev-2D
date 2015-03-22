#pragma once

#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"
#include "Font.hpp"

#include "..\gl\Buffer.hpp"

class Text : public Drawable, public Transform
{
public:

	typedef enum { LEFT, CENTER, RIGHT } Alignment;
	//! Specifies the boundary used to break up text in word wrapping and other algorithms
	typedef enum { LINE, WORD } Boundary;

	Text();

	Text(const std::string& string, const Font& font, unsigned int characterSize = 30);

	void setStringColor(const Color& color, const std::string& find);
	
	void setColor(const Color& color);

	std::string	getFontFamily() const { if (m_font) return m_font->getFamily(); else return std::string(); }
	void		setFont(const Font& font) { m_font = &font; mInvalid = true; }

	float		getFontSize() const { return mFontSize; }
	void		setFontSize(float size) { mFontSize = size; mInvalid = true; }

	float		getLineSpace() const { return mLineSpace; }
	void		setLineSpace(float value) { mLineSpace = value; mInvalid = true; }

	float		getLeading() const 
	{
		return (m_font ? std::floorf(m_font->getLeading(mFontSize) * mLineSpace + 0.5f) : 0.0f); 
	}

	Alignment	getAlignment() const { return mAlignment; }
	void		setAlignment(Alignment alignment) { mAlignment = alignment; mInvalid = true; }

	Boundary	getBoundary() const { return mBoundary; }
	void		setBoundary(Boundary boundary) { mBoundary = boundary; mInvalid = true; }

	void		setText(const std::string &text) { m_text = text; mMust.clear(); mAllow.clear(); mInvalid = true; }

	RectF	getBounds();

	float	getWidthAt(float y) { return 0.0f; }
	
	float	getHeight() { return 0.0f; }

	bool newLine(Vector2i& cursor)
	{
		cursor.x = 0;
		cursor.y += getLeading();

		return (getHeight() == 0.0f || cursor.y < getHeight());
	}

	void draw(RenderTarget2D& target, RenderStates states) const;

	virtual void		renderMesh();
	//! helper to render a non-word-wrapped string
	void		renderString(const std::string &str, Vector2i& cursor, float stretch = 1.0f);

	bool isWhitespaceUtf16(const char ch);

private:

	std::vector<size_t>		mMust, mAllow;

	bool		mInvalid;

	bool		mBoundsInvalid;

	RectF		mBounds;

	float		mFontSize;

	float		mLineSpace;

	Alignment				mAlignment;
	Boundary				mBoundary;

	std::string m_text;

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<Vertex> m_vertices;
	std::vector<GLuint>	m_indices;

	bool m_bufferNeedsUpdate;

	const Font* m_font;
};