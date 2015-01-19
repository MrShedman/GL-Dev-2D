#include "MenuState.hpp"
#include "..\resource management\ResourceHolder.hpp"
#include "..\gui\Button.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\system\Utilities.h"

#include <iostream>
#include <memory>

MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::Texture2));

	Util::scaleSpriteToFitWindow(mBackgroundSprite, *context.window);

	initializeButtons();
}

void MenuState::initializeButtons()
{
	Context context = getContext();

	mGUIContainer.clear();

	float y = 0.5f * context.window->getSize().y;
	float x = 0.5f * context.window->getSize().x;

	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(x - 120, y);
	playButton->setText("Play");
	playButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});

	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(x - 120, y + 90);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Settings);
	});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(x - 120, y + 180);
	exitButton->setText("Exit");
	exitButton->setCallback([this]()
	{
		requestStateClear();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(exitButton);
	mGUIContainer.pack(settingsButton);
}

void MenuState::draw()
{
	RenderTarget2D& target = *getContext().target;

	RenderStates states;
	states.shaderHolder = getContext().shaders;

	target.draw(mBackgroundSprite, RenderStates(&getContext().shaders->get(Shaders::Default)));
	target.draw(mGUIContainer, states);
}

bool MenuState::update(Time dt)
{
	mGUIContainer.update();
	return true;
}

bool MenuState::handleEvent(const Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}