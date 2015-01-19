#include "Application.hpp"
#include "window\Event.h"
#include "rendering\Color.h"
#include <iostream>

#include "state\LoadingState.hpp"
#include "app\MenuState.hpp"
#include "app\GameState.hpp"
#include "app\SettingsState.hpp"

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

Application::Application(Window& window)
:
window(window),
mTextures(),
mFonts(),
mShaders(),
mStateStack(State::Context(window, target, mTextures, mFonts, mShaders)),
m_isOpen(true)
{
	//window.setVerticalSyncEnabled(true);

	std::cout << "GPU: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Vector2i size = window.getSize();
	glViewport(0, 0, size.x, size.y);
	glClearColor(35 / 255.f, 127 / 255.f, 229 / 255.f, 255 / 255.f);

	Camera cam(RectF(0.f, static_cast<float>(size.y), 0.f, static_cast<float>(size.x)));

	target.create(cam, size.x, size.y);

	loadResources();

	mStateStack.registerState<LoadingState>(States::Loading);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<SettingsState>(States::Settings);

	mStateStack.pushState(States::Loading);
}

void Application::loadResources()
{
	mFonts.load(Fonts::Main, "res/fonts/Sansation.sdff");

	mTextures.load(Textures::Loading, "res/textures/splash.png");

	Shader2D::Desc d;
	d.vertex = "LVertexPos";
	d.texture = "LTexCoord";
	d.color = "LVertexColor";

	mShaders.load(Shaders::Default, "res/shaders/program.vert", "res/shaders/program.frag", d);
	mShaders.load(Shaders::PostFX, "res/shaders/program.vert", "res/shaders/postFX.frag", d);
	mShaders.load(Shaders::SDFF, "res/shaders/program.vert", "res/shaders/sdf.frag", d);
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
			target.setCamera(Camera(RectF(0.f, static_cast<float>(event.size.height), 0.f, static_cast<float>(event.size.width))));
		}
		if (event.type == Event::FileDrop)
		{
			int size = event.fileDrop.fileCount;

			std::cout << size << " files dropped at: " << event.fileDrop.x << ", " << event.fileDrop.y << std::endl;

			for (int i = 0; i < size; ++i)
			{
				std::cout << event.fileDrop.files[i] << std::endl;
			}
		}
		
		mStateStack.handleEvent(event);
	}
}

void Application::update(Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	window.clear(Color::White);//Color(35, 127, 229));

	mStateStack.draw();

	window.display();
}

void Application::run()
{
	Clock clock;
	Time timeSinceLastUpdate = Time::Zero;

	while (m_isOpen)
	{
		render();

		Time dt;
		dt = clock.restart();
		timeSinceLastUpdate += dt;

		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			getInput();
			update(timePerFrame);

			if (mStateStack.isEmpty())
			{
				close();
			}
		}
	}
}

void Application::close()
{
	m_isOpen = false;
}