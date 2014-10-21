#include "RenderStates.hpp"
#include "Texture.hpp"

RenderStates::RenderStates() :
//blendMode(BlendAlpha),
transform(),
texture(NULL),
shader(NULL)
{
}

RenderStates::RenderStates(const Matrix4f& theTransform) :
//blendMode(BlendAlpha),
transform(theTransform),
texture(NULL),
shader(NULL)
{
}

/*
RenderStates::RenderStates(const BlendMode& theBlendMode) :
//blendMode(theBlendMode),
transform(),
texture(NULL),
shader(NULL)
{
}
*/

RenderStates::RenderStates(Texture* theTexture) :
//blendMode(BlendAlpha),
transform(),
texture(theTexture),
shader(NULL)
{
}

RenderStates::RenderStates(Shader2D* theShader) :
//blendMode(BlendAlpha),
transform(),
texture(NULL),
shader(theShader)
{
}

/*
RenderStates::RenderStates(const BlendMode& theBlendMode, const Transform& theTransform,
	const Texture* theTexture, const Shader* theShader) :
	blendMode(theBlendMode),
	transform(theTransform),
	texture(theTexture),
	shader(theShader)
{
}
*/