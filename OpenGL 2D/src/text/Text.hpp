#pragma once

#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"
#include "Font.hpp"

class Text : public Drawable, public Transform
{
public:

	typedef enum { LEFT, CENTER, RIGHT } Alignment;
	//! Specifies the boundary used to break up text in word wrapping and other algorithms
	typedef enum { LINE, WORD } Boundary;

	Text()
		:
		mInvalid(true),
		mBoundsInvalid(true),
		mAlignment(LEFT),
		mBoundary(WORD),
		mFontSize(14.0f),
		mLineSpace(1.0f)
	{

	}

	Text(const std::string& string, const Font& font, unsigned int characterSize = 30)
		:
	mInvalid(true),
	mBoundsInvalid(true),
	mAlignment(LEFT),
	mBoundary(WORD),
	mFontSize(characterSize),
	mLineSpace(1.0f)
	{
		setText(string);
		setFont(font);
	}

	~Text()
	{
		if (mVBOID != 0)
		{
			glDeleteBuffers(1, &mVBOID);
			glDeleteBuffers(1, &mIBOID);
		}
	}

	/*void setFont(Font& font)
	{
		m_font = &font;

		RectI bounds(0, 512, 0, 512);
		RectI m_textureRect = bounds;

		m_vertices[0].position = Vector2f(0.f, 0.f);
		m_vertices[1].position = Vector2f(0.f, bounds.getHeight());
		m_vertices[2].position = Vector2f(bounds.getWidth(), 0.f);
		m_vertices[3].position = Vector2f(bounds.getWidth(), bounds.getHeight());


		float left = static_cast<float>(m_textureRect.left) / m_textureRect.getWidth();
		float right = static_cast<float>(m_textureRect.right) / m_textureRect.getWidth();
		float top = static_cast<float>(m_textureRect.top) / m_textureRect.getHeight();
		float bottom = static_cast<float>(m_textureRect.bottom) / m_textureRect.getHeight();

		m_vertices[0].texCoords = Vector2f(left, top);
		m_vertices[1].texCoords = Vector2f(left, bottom);
		m_vertices[2].texCoords = Vector2f(right, top);
		m_vertices[3].texCoords = Vector2f(right, bottom);
	}*/

	inline void setStringColor(const Color& color, const std::string& find)
	{
		int occurrences = 0;
		std::string::size_type start = 0;
		while ((start = m_text.find(find, start)) != std::string::npos) 
		{			
			++occurrences;

			for (size_t i = start; i < start + find.length(); ++i)
			{
				size_t index = (i - 0) * 6;

				m_vertices[index + 0].color = color;
				m_vertices[index + 1].color = color;
				m_vertices[index + 2].color = color;
				m_vertices[index + 3].color = color;
				m_vertices[index + 4].color = color;
				m_vertices[index + 5].color = color;
			}

			start += find.length(); // see the note
		}
	}

	inline void setColor(const Color& color)
	{
		for (auto& v : m_vertices)
		{
			v.color = color;
		}
	}

	std::string	getFontFamily() const { if (m_font) return m_font->getFamily(); else return std::string(); }
	void		setFont(const Font& font) { m_font = &font; mInvalid = true; }

	float		getFontSize() const { return mFontSize; }
	void		setFontSize(float size) { mFontSize = size; mInvalid = true; }

	float		getLineSpace() const { return mLineSpace; }
	void		setLineSpace(float value) { mLineSpace = value; mInvalid = true; }

	float		getLeading() const 
	{
		return mFontSize * 4;
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

	GLuint mVBOID;
	GLuint mIBOID;

	std::vector<size_t>		mMust, mAllow;

	bool		mInvalid;

	bool		mBoundsInvalid;

	RectF		mBounds;

	float		mFontSize;

	float		mLineSpace;

	Alignment				mAlignment;
	Boundary				mBoundary;

	std::string m_text;

	std::vector<Vertex> m_vertices;
	std::vector<GLuint>	mIndices;
	const Font* m_font;
};