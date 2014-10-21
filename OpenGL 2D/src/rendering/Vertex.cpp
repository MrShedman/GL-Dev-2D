#include "Vertex.hpp"

Vertex::Vertex() :
position(0, 0),
color(255.f, 255.f, 255.f),
texCoords(0, 0)
{
}

Vertex::Vertex(const Vector2f& thePosition) :
position(thePosition),
color(255.f, 255.f, 255.f),
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
color(255.f, 255.f, 255.f),
texCoords(theTexCoords)
{
}

Vertex::Vertex(const Vector2f& thePosition, const Color& theColor, const Vector2f& theTexCoords) :
position(thePosition),
color(theColor),
texCoords(theTexCoords)
{
}