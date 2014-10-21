#include "Application.hpp"
#include "window\Event.h"
#include "rendering\Color.h"
#include <iostream>

void scaleSpriteToFitWindow(Sprite &sprite, const Window &window)
{
	Vector2u tSize = sprite.getTexture()->getSize();
	Vector2u wSize = window.getSize();

	float sx = static_cast<float>(wSize.x) / static_cast<float>(tSize.x);
	float sy = static_cast<float>(wSize.y) / static_cast<float>(tSize.y);

	sprite.setScale(sx, sy);
}

const Time Application::timePerFrame = seconds(1.f / 60.f);

static float pt = 14.f;

Application::Application(unsigned int width, unsigned int height, const std::string& title)
{
	ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 2;
	settings.core = true;

	window.create(VideoMode(width, height), title, Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	std::cout << "GPU: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
	
	glGenVertexArrays(1, &vao);

	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray(vao);

	//Set blending
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);
	glClearColor(35 / 255.f, 127 / 255.f, 229 / 255.f, 255 / 255.f);

	target.create(Camera(RectF(0.f, height, 0.f, width)));

	if (!background.loadFromFile("res/textures/MenuBackground.png"))
	{
		std::cout << "Failed to load texture: MenuBackground.png" << std::endl;
	}

	if (!texture.loadFromFile("res/textures/opengl.png"))
	{
		std::cout << "Failed to load texture: opengl.png" << std::endl;
	}

	if (!texture2.loadFromFile("res/textures/aluminium.png"))
	{
		std::cout << "Failed to load texture: default.png" << std::endl;
	}

	if (!shader.loadFromFile("res/shaders/program.vert", "res/shaders/program.frag"))
	{
		std::cout << "Failed to load shader: main" << std::endl;
	}

	
	if (!sdfShader.loadFromFile("res/shaders/program.vert", "res/shaders/sdf.frag"))
	{
		std::cout << "Failed to load shader: sdf" << std::endl;
	}
	
	backgroundSprite.setTexture(background);
	background.setSmooth(false);

	sprite.setTexture(texture);
	sprite.setTextureRect(RectI(0, 235, 0, 255));
	sprite.setScale(0.5, 0.5f);
	sprite.setColor(Color::Magenta);
	sprite.setOrigin(235.f / 2.f, 255.f / 2.f);
	sprite2.setTexture(texture2);

		//case Pressed: color = sf::Color(255, 177, 0);  break;
	//
	//	case Hover: color = sf::Color(181, 230, 29); break;

		//case Normal: color = sf::Color(31, 177, 76);  break;

	//shape.setFillColor(Color(31, 177, 76));
	//shape.setTexture(&texture2);
	shape.setOutlineThickness(-5);
	shape.setOutlineColor(Color::Black);
	shape.setSize(Vector2f(240, 70));
	RectF rect = shape.getLocalBounds();
	shape.setOrigin(rect.getCenter());
	shape.setPosition(640, 390);
	//shape.setTexture(&texture2);
	shape.generateVBO();

	font.loadFromFile("res/fonts/Sansation.sdff");
	text.setFont(font);
	Vector2i pos(0, 0);
	text.setFontSize(20);
	text.setText("Settings");
	text.setColor(Color::Black);
	text.renderMesh();
	//text.setStringColor(Color::Red, "i");

	Vector2f centre = text.getBounds().getCenter();
	text.setOrigin(centre);
	text.setPosition(640, 390);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::getInput()
{
	Event event;

	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			close();
		}
		if (event.type == Event::Resized)
		{
			glViewport(0, 0, event.size.width, event.size.height);
			target.setCamera(Camera(RectF(0.f, event.size.height, 0.f, event.size.width)));

			scaleSpriteToFitWindow(backgroundSprite, window);
		}
		if (event.type == Event::MouseWheelMoved)
		{
			pt += event.mouseWheel.delta;
			text.setFontSize(pt);
			Vector2i pos(0, 0);
			text.renderString("Settings", pos);
			text.setColor(Color::Black);
		}
	}
}

static float dx = 0.f;

void Application::update(Time dt)
{
	int x = Mouse::getPosition(window).x;
	int y = Mouse::getPosition(window).y;

	if (shape.getGlobalBounds().contains(x, y))
	{
		shape.setFillColor(Color(181, 230, 29));
	}
	else
	{
		shape.setFillColor(Color(31, 177, 76));
	}

	dx += 0.05f;
	float scaled = std::abs(std::sin(dx)) + 0.5f;
	
	float r = 100.f;
	float dxc = std::cos(dx) * r;
	float dys = std::sin(dx) * r;
	sprite.setPosition(dxc + 640, dys + 360);
	//sprite.rotate(5.f);
	sprite2.setScale(0.5f, 0.5f);
	sprite2.setOrigin(Vector2f(256.f, 256.f));
	sprite2.rotate(1.f);
	sprite.rotate(5.f);
	sprite2.setPosition(x + 300, y);
	sprite2.setColor(Color(255, 255, 255, 120));
}

void Application::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	target.draw(backgroundSprite, RenderStates(&shader));

	//target.draw(sprite, RenderStates(&shader));
	
	//target.draw(sprite2, RenderStates(&shader));
	
	target.draw(shape, RenderStates(&shader));
	text.renderMesh();
	//Texture::bind(&texture2, 1);

	target.draw(text, RenderStates(&sdfShader));

	window.display();
}

void Application::run()
{
	Clock clock;
	Time timeSinceLastUpdate = Time::Zero;

	while (window.isOpen())
	{
		Time dt;
		dt = clock.restart();
		timeSinceLastUpdate += dt;

		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			getInput();
			update(timePerFrame);
		}

		render();
	}
}

void Application::close()
{
	window.close();
}