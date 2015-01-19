#include "Vertex.hpp"

Vertex::Vertex() :
position(0.0f, 0.0f, 0.0f),
color(255, 255, 255),
texCoords(0, 0)
{
}

Vertex::Vertex(const Vector2f& thePosition) :
position(thePosition),
color(255, 255, 255),
texCoords(0, 0)
{
}

Vertex::Vertex(const Vector2f& thePosition, const Color& theColor) :
position(thePosition),
color(theColor),
texCoords(0, 0)
{
}

Vertex::Vertex(const Vector2f& thePosition, const Vector2f& theTexCoords) :
position(thePosition),
color(255, 255, 255),
texCoords(theTexCoords)
{
}

Vertex::Vertex(const Vector2f& thePosition, const Color& theColor, const Vector2f& theTexCoords) :
position(thePosition),
color(theColor),
texCoords(theTexCoords)
{
}

Vertex::Vertex(const Vector3f& thePosition) :
position(thePosition),
color(255, 255, 255),
texCoords(0, 0)
{
}

Vertex::Vertex(const Vector3f& thePosition, const Color& theColor) :
position(thePosition),
color(theColor),
texCoords(0, 0)
{
}

Vertex::Vertex(const Vector3f& thePosition, const Vector2f& theTexCoords) :
position(thePosition),
color(255, 255, 255),
texCoords(theTexCoords)
{
}

Vertex::Vertex(const Vector3f& thePosition, const Color& theColor, const Vector2f& theTexCoords) :
position(thePosition),
color(theColor),
texCoords(theTexCoords)
{
}