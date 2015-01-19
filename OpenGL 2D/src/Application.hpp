#pragma once

#include <string>

#include "window\Window.hpp"
#include "window\Time.h"
#include "rendering\Texture.hpp"
#include "rendering\Shader.hpp"
#include "rendering\Color.h"
#include "math\Matrix.h"
#include "rendering\Sprite.hpp"
#include "rendering\RenderTarget2D.h"
#include "text\Font.hpp"
#include "text\Text.hpp"

#include "rendering\RectangleShape.hpp"
#include "rendering\RenderTexture.hpp"

#include "state\StateStack.hpp"
#include "resource management\ResourceHolder.hpp"
#include "app\ResourceIdentifiers.hpp"

class Application
{
public:

	Application(Window& window);

	void getInput();
	void update(Time dt);
	void render();

	void loadResources();

	void run();

	void close();

private:

	static const Time timePerFrame;

	bool m_isOpen;

	Clock clock;

	GLuint vao;

	TextureHolder	mTextures;
	FontHolder		mFonts;
	ShaderHolder	mShaders;

	RenderTarget2D	target;

	StateStack		mStateStack;

	Window&			window;
};