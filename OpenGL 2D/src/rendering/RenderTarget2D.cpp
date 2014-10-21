#include "RenderTarget2D.h"

#include "Drawable.hpp"
#include "RenderStates.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

RenderTarget2D::RenderTarget2D()
{

}

void RenderTarget2D::create(Camera& cam)
{
	camera = cam;

	std::vector<unsigned char> white(4);
	white[0] = 255;
	white[1] = 255;
	white[2] = 255;
	white[3] = 255;

	if (!nullTexture.loadFromMemory(white, 1, 1))
	{
		std::cout << "error: loading null texture" << std::endl;
	}
}

void RenderTarget2D::draw(const Drawable& drawable, const RenderStates& states)
{
	drawable.draw(*this, states);
}

void RenderTarget2D::draw(const Vertex* vertices, unsigned int vertexCount, PrimitiveType type, const RenderStates& states)
{
	Shader2D::bind(states.shader);

	states.shader->setProjectionMatrix(camera.getTransform());
	states.shader->setModelViewMatrix(states.transform);

	//Enable vertex and texture coordinate arrays
	states.shader->enableVertexPointer();
	states.shader->enableTexCoordPointer();
	states.shader->enableColourPointer();

	if (states.texture != NULL)
	{
		Texture::bind(states.texture);
	}
	else
	{
		Texture::bind(&nullTexture);
	}

	//Update vertex buffer data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(Vertex), vertices);

	//Set texture coordinate data
	states.shader->setTexCoordPointer(sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
	
	states.shader->setVertexPointer(sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

	states.shader->setColourPointer(sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

	static const GLenum modes[] = { GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
		GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN};
	
	GLenum mode = modes[type];

	//Draw using vertex data and index data
	glDrawElements(mode, vertexCount, GL_UNSIGNED_INT, NULL);

	//Disable vertex and texture coordinate arrays
	states.shader->disableVertexPointer();
	states.shader->disableTexCoordPointer();
	states.shader->disableColourPointer();

	Shader2D::bind(NULL);

	Texture::bind(NULL);
}
