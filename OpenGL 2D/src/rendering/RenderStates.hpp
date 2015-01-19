#pragma once

#include "..\math\Matrix.h"
#include "..\app\ResourceIdentifiers.hpp"

class Texture;
class Shader2D;
class CameraPersp;

class RenderStates
{
public:

	RenderStates();

	//RenderStates(const BlendMode& theBlendMode);

	RenderStates(const Matrix4f& theTransform);

	RenderStates(const Texture* theTexture);

	RenderStates(const Shader2D* theShader);

	//RenderStates(const BlendMode& theBlendMode, const Transform& theTransform, const Texture* theTexture, const Shader* theShader);

	CameraPersp* cam;

	//BlendMode      blendMode; ///< Blending mode
	Matrix4f	transform; ///< Transform

	const Texture*	texture;   ///< Texture
	const TextureHolder* textureHolder;

	const Shader2D* shader;
	const ShaderHolder* shaderHolder;
};