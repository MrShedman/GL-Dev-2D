
#include "GameState.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\resource management\ResourceHolder.hpp"
#include "..\system\Utilities.h"

GameState::GameState(StateStack& stack, Context context)
	:
	State(stack, context),
	//mFont(context.fonts->get(Fonts::Main)),
	mText("Game", context.fonts->get(Fonts::Main)),
	mBlocks()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::MenuBackground));

	Util::scaleSpriteToFitWindow(mBackgroundSprite, *context.window);


	mText.renderMesh();
	mText.setColor(Color::Magenta);
	//text.setColor(Color::White);

	cam.init(context.window);

	Texture& t = context.textures->get(Textures::Texture1);

	int size = 6;
	float scale = 10.f;

	for (int i = -size; i < size; ++i)
	{
		for (int j = -size; j < size; ++j)
		{
			for (int k = -size; k < size; ++k)
			{
				Block b;
				b.create(1);
				b.setPosition(i*scale, j*scale, k*scale);
				//b.setTexture(t);
				mBlocks.push_back(std::move(b));
			}
		}
	}

//	glEnable(GL_CULL_FACE); // cull face
//	glCullFace(GL_BACK); // cull back face
//	glFrontFace(GL_CW);
}

static float m = 0.01f;

void GameState::draw()
{
	RenderTarget2D& target = *getContext().target;

	RenderStates states;
	states.shaderHolder = getContext().shaders;
	states.shader = &states.shaderHolder->get(Shaders::Default);

	//target.draw(mBackgroundSprite, states);
	//states.texture = &getContext().textures->get(Textures::ID::Texture1);
	glEnable(GL_DEPTH_TEST);
	states.cam = &cam;
	static float x1 = 0;
	float x = std::sin(2 * 3.1415* x1) + 2.f;
	x /= 2.f;
	x1 += 0.01f;
	for (auto &b : mBlocks)
	{
		//b.move(Vector3f(0.1f, 0.1f, 0.1f));
		b.setScale(x, x, x);
		b.rotate(b.getOrigin(), Vector3f(1.0f, 0.0f, 0.f), m);
		
		target.draw(b, states);
	}

	//std::cout << mBlocks[100].getPosition() << std::endl;

	glDisable(GL_DEPTH_TEST);

	states.shader = &states.shaderHolder->get(Shaders::SDFF);
	target.draw(mText, states);
}

bool GameState::update(Time dt)
{
	Window& window = *getContext().window;

	cam.Update(dt.asSeconds());

	mText.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y, 0.f);

	return true;
}

bool GameState::handleEvent(const Event& event)
{
	cam.handleEvent(event, 1.0f);

	if (event.type == Event::Resized)
	{
		//camera.setViewSize(static_cast<sf::Vector2f>(getContext().window->getSize()));
	}
	if (event.type == Event::KeyPressed)
	{
		// Escape pressed, trigger the pause screen
		if (event.key.code == Keyboard::Escape)
		{
			requestStackPop();
			requestStackPush(States::Menu);
		}
	}
	if (event.type == Event::MouseWheelMoved)
	{
		m += event.mouseWheel.delta / 1000.f;
	}


	Window& window = *getContext().window;

	if (event.type == Event::KeyPressed)
	{
		if (event.key.code == Keyboard::Return && event.key.alt)
		{
			if (window.isFullscreen())
			{
				window.switchToWindowed(VideoMode(1280, 720), Style::Default, true);
			}
			else
			{
				window.switchToFullscreen(VideoMode::getFullscreenModes()[0]);
			}
		}
	}
	
	return true;
}