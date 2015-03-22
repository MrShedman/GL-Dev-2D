#include "Vertex.hpp"

Vertex::Vertex() :
position(0.0f, 0.0f, 0.0f),
color(Color::White),
texCoords(0, 0)
{
}

Vertex::Vertex(const Vector2f& thePosition) :
position(thePosition),
color(Color::White),
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
color(Color::White),
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
color(Color::White),
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
color(Color::White),
texCoords(theTexCoords)
{
}

Vertex::Vertex(const Vector3f& thePosition, const Color& theColor, const Vector2f& theTexCoords, const Vector3f& theNormals) :
position(thePosition),
normals(theNormals),
color(theColor),
texCoords(theTexCoords)
{
}