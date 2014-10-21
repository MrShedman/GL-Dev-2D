#pragma once

#include "..\math\Matrix.h"

class Texture;
class Shader2D;

class RenderStates
{
public:

	RenderStates();

	//RenderStates(const BlendMode& theBlendMode);

	RenderStates(const Matrix4f& theTransform);

	RenderStates(Texture* theTexture);

	RenderStates(Shader2D* theShader);

	//RenderStates(const BlendMode& theBlendMode, const Transform& theTransform, const Texture* theTexture, const Shader* theShader);


	//BlendMode      blendMode; ///< Blending mode
	Matrix4f	transform; ///< Transform
	Texture*	texture;   ///< Texture
	Shader2D*	shader;    ///< Shader
};