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

class Application
{
public:

	Application(unsigned int width, unsigned int height, const std::string& title);

	void getInput();
	void update(Time dt);
	void render();

	void run();

	void close();

private:

	std::string txt;

	GLuint vao;

	Font font;
	Text text;

	RenderTarget2D target;

	Sprite sprite2;
	Sprite sprite;

	Sprite backgroundSprite;

	Texture background;

	Texture texture2;
	Texture texture;
	
	Shader2D sdfShader;
	Shader2D shader;

	static const Time timePerFrame;

	Window window;

	RectangleShape shape;
};