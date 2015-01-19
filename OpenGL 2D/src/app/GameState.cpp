
#include "GameState.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\resource management\ResourceHolder.hpp"
#include "..\system\Utilities.h"

GameState::GameState(StateStack& stack, Context context)
:
State(stack, context),
mFont(context.fonts->get(Fonts::Main)),
mText("Game", mFont),
mBlocks()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::MenuBackground));

	Util::scaleSpriteToFitWindow(mBackgroundSprite, *context.window);

	
	mText.renderMesh();
	mText.setColor(Color::Magenta);
	//text.setColor(Color::White);

	cam.init(context.window);

	auto t = context.textures->get(Textures::Texture2);

	int size = 1;
	float scale = 1.1f;

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			for (int k = 0; k < size; ++k)
			{
				Block b;
				b.create(1);
				b.setPosition(i*scale, j*scale, k*scale);
				//b.setScale(2, 4, 6);
				//b.setRotation(Vector3f(1.0f, 1.0f, 0.0f), 45.0f);
				b.setTexture(&t);
					
				mBlocks.push_back(b);
			}
		}
	}

}

#include "..\rendering\Block.hpp"

void GameState::draw()
{
	RenderTarget2D& target = *getContext().target;

	RenderStates states;
	states.shaderHolder = getContext().shaders;
	states.shader = &states.shaderHolder->get(Shaders::Default);

	//target.draw(mBackgroundSprite, states);

	glEnable(GL_DEPTH_TEST);
	states.cam = &cam;
	for (auto &b : mBlocks)
	{
		//b.move(Vector3f(0.1f, 0.1f, 0.1f));
		target.draw(b, states);
	}

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
			requestStackPush(States::Menu);
		}
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