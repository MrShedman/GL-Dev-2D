#pragma once

#include "..\math\Vector.h"
#include "Color.h"

class Vertex
{
public:

	Vertex();

	Vertex(const Vector2f& thePosition);

	Vertex(const Vector2f& thePosition, const Color& theColor);

	Vertex(const Vector2f& thePosition, const Vector2f& theTexCoords);

	Vertex(const Vector2f& thePosition, const Color& theColor, const Vector2f& theTexCoords);

	Vertex(const Vector3f& thePosition);

	Vertex(const Vector3f& thePosition, const Color& theColor);

	Vertex(const Vector3f& thePosition, const Vector2f& theTexCoords);

	Vertex(const Vector3f& thePosition, const Color& theColor, const Vector2f& theTexCoords, const Vector3f& theNormals);

	Vector3f position;
	Vector3f normals;
	Vector2f texCoords;
	Color	 color;
};