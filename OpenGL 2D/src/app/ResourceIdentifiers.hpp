#pragma once

// Forward declaration of SFML classes
class Texture;
class Font;
class Shader2D;

namespace Textures
{
	enum ID
	{
		Loading,
		MenuBackground,
		Texture1,
		Texture2
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}

namespace Shaders
{
	enum ID
	{
		Default,
		Instanced,
		PostFX,
		SDFF,
		Lighting,
		DeferredGeometry,
		DeferredLighting
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<Font, Fonts::ID>			FontHolder;
typedef ResourceHolder<Shader2D, Shaders::ID>	ShaderHolder;