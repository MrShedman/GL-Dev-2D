#include "Text.hpp"

#include "..\rendering\RenderTarget2D.h"

void Text::draw(RenderTarget2D& target, RenderStates states) const
{
	if (m_font)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);

		states.transform = states.transform * getTransform();
		states.texture = m_font->getTexture();
		target.draw(m_vertices.data(), m_vertices.size(), Triangles, states);
	}
}

void Text::renderMesh()
{
	// prevent errors
	if (!mInvalid)
	{
		return;
	}
	else
	{
		Vector2i pos(0, 0);
		renderString(m_text, pos);
	}
/*	if (!m_font) return;
	if (m_text.empty())	return;

	// initialize variables
	const float		space = m_font->getAdvance(32, mFontSize);
	const float		height = getHeight() > 0.0f ? (getHeight() - mFont->getDescent(mFontSize)) : 0.0f;
	float			width, linewidth;
	size_t			index = 0;
	std::u16string	trimmed, chunk;

	// initialize cursor position
	Vector2f cursor(0.0f, std::floorf(mFont->getAscent(mFontSize) + 0.5f));

	// get word/line break information from Cinder's Unicode class if not available
	if (mMust.empty() || mAllow.empty())
		findBreaksUtf16(mText, &mMust, &mAllow);

	//double t = app::getElapsedSeconds();

	// reserve some room in the buffers, to prevent excessive resizing. Do not use the full string length,
	// because the text may contain white space characters that don't need to be rendered.
	size_t sz = mText.length() / 2;
	mVertices.reserve(4 * sz);
	mTexcoords.reserve(4 * sz);
	mIndices.reserve(6 * sz);

	// process text in chunks
	std::vector<size_t>::iterator	mitr = mMust.begin();
	std::vector<size_t>::iterator	aitr = mAllow.begin();
	while (aitr != mAllow.end() && mitr != mMust.end() && (height == 0.0f || cursor.y <= height))
	{
		// calculate the maximum allowed width for this line
		linewidth = getWidthAt(cursor.y);

		switch (mBoundary)
		{
		case LINE:
			// render the whole paragraph
			trimmed = boost::trim_copy(mText.substr(index, *mitr - index + 1));
			width = mFont->measureWidth(trimmed, mFontSize, true);

			// advance iterator
			index = *mitr;
			++mitr;

			break;
		case WORD:
			// measure the first chunk on this line
			chunk = (mText.substr(index, *aitr - index + 1));
			width = mFont->measureWidth(chunk, mFontSize, false);

			// if it fits, add the next chunk until no more chunks fit or are available
			while (linewidth > 0.0f && width < linewidth && *aitr != *mitr)
			{
				++aitr;

				if (aitr == mAllow.end())
					break;

				chunk = (mText.substr(*(aitr - 1) + 1, *aitr - *(aitr - 1)));
				width += mFont->measureWidth(chunk, mFontSize, false);
			}

			// end of line encountered
			if (aitr == mAllow.begin() || *(aitr - 1) <= index)
			{	// not a single chunk fits on this line, just render what we have				
			}
			else if (linewidth > 0.0f && width > linewidth)
			{	// remove the last chunk				
				--aitr;
			}

			if (aitr != mAllow.end())
			{
				// 
				trimmed = boost::trim_copy(mText.substr(index, *aitr - index + 1));
				width = mFont->measureWidth(trimmed, mFontSize);

				// end of paragraph encountered, move to next
				if (*aitr == *mitr)
					++mitr;
				/*else if( mAlignment == JUSTIFIED )
				{
				// count spaces
				uint32_t c = std::count( trimmed.begin(), trimmed.end(), 32 );
				if( c == 0 ) break;
				// remaining whitespace
				float remaining = getWidthAt( cursor.y ) - width;
				float space = mFont->getAdvance( 32, mFontSize );
				//
				stretch = (remaining / c + space) / space;
				if( stretch > 3.0f ) stretch = 1.0f;
				}

				// advance iterator
				index = *aitr;
				++aitr;
			}

			break;
		}

		// adjust alignment
		switch (mAlignment) {
		case CENTER:
			cursor.x = 0.5f * (linewidth - width);
			break;
		case RIGHT:
			cursor.x = (linewidth - width);
			break;
			break;
		}

		// add this fitting part of the text to the mesh 
		renderString(trimmed, &cursor);

		// advance cursor to new line
		if (!newLine(&cursor)) break;
	}
	*/
	//app::console() << ( app::getElapsedSeconds() - t ) << std::endl;
}

void Text::renderString(const std::string &str, Vector2i& cursor, float stretch)
{
	m_vertices.clear();
	mIndices.clear();

	std::string::const_iterator itr;
	for (itr = str.begin(); itr != str.end(); ++itr) {
		// retrieve character code
		char id = (char)*itr;

		if (m_font->contains(id)) {
			// get metrics for this character to speed up measurements
			Metrics m = m_font->getMetrics(id);

			// skip whitespace characters
			if (!isWhitespaceUtf16(id))
			{
				size_t index = m_vertices.size();
				
				RectF pBounds = m_font->getBounds(m, mFontSize);
				RectF tBounds = m_font->getTexCoords(m);

				m_vertices.push_back(Vertex(cursor + Vector2f(pBounds.left, pBounds.top), Vector2f(tBounds.left, tBounds.top)));
				m_vertices.push_back(Vertex(cursor + Vector2f(pBounds.right, pBounds.top), Vector2f(tBounds.right, tBounds.top)));
				m_vertices.push_back(Vertex(cursor + Vector2f(pBounds.left, pBounds.bottom), Vector2f(tBounds.left, tBounds.bottom)));
				m_vertices.push_back(Vertex(cursor + Vector2f(pBounds.left, pBounds.bottom), Vector2f(tBounds.left, tBounds.bottom)));
				m_vertices.push_back(Vertex(cursor + Vector2f(pBounds.right, pBounds.top), Vector2f(tBounds.right, tBounds.top)));
				m_vertices.push_back(Vertex(cursor + Vector2f(pBounds.right, pBounds.bottom), Vector2f(tBounds.right, tBounds.bottom)));

				mIndices.push_back(index + 0); //mIndices.push_back(index + 3); mIndices.push_back(index + 1);
				mIndices.push_back(index + 1); 
				mIndices.push_back(index + 2);
				mIndices.push_back(index + 3);
				mIndices.push_back(index + 4);
				mIndices.push_back(index + 5);
			}
			else
			{
				std::cout << "whitespace" << std::endl;
			}

			if (id == 32)
				cursor.x += stretch * m_font->getAdvance(m, mFontSize);
			else
				cursor.x += m_font->getAdvance(m, mFontSize);
		}
		else if (id == '\n')
		{
			newLine(cursor);
		}
	}

	if (mVBOID != 0)
	{
		glDeleteBuffers(1, &mVBOID);
		glDeleteBuffers(1, &mIBOID);
	}
	//Create VBO
	glGenBuffers(1, &mVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, mVBOID);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

	//Create IBO
	glGenBuffers(1, &mIBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_DYNAMIC_DRAW);

	//Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

	mBoundsInvalid = true;
	mInvalid = false;
}

bool Text::isWhitespaceUtf16(const char ch)
{
	// see: http://en.wikipedia.org/wiki/Whitespace_character, 
	// make sure the values are in ascending order, 
	// otherwise the binary search won't work
	static const wchar_t arr[] = {
		0x0009, 0x000A, 0x000B, 0x000C, 0x000D,
		0x0020, 0x0085, 0x00A0, 0x1680, 0x180E,
		0x2000, 0x2001, 0x2002, 0x2003, 0x2004,
		0x2005, 0x2006, 0x2007, 0x2008, 0x2009,
		0x200A, 0x2028, 0x2029, 0x202F, 0x205F, 0x3000
	};
	static const std::vector<wchar_t> whitespace(arr, arr + sizeof(arr) / sizeof(arr[0]));

	return std::binary_search(whitespace.begin(), whitespace.end(), ch);
}

RectF	Text::getBounds()
{
	mBounds = RectF(0.f, 0.f, 0.f, 0.f);

	auto itr = m_vertices.begin();

	while (itr != m_vertices.end())
	{
		mBounds.left = std::min(itr->position.x, mBounds.left);
		mBounds.top = std::min(itr->position.y, mBounds.top);
		mBounds.right = std::max(itr->position.x, mBounds.right);
		mBounds.bottom = std::max(itr->position.y, mBounds.bottom);
		++itr;
	}
	
	return mBounds;
}