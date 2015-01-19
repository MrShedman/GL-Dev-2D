
#include "SettingsState.hpp"
#include "..\resource management\ResourceHolder.hpp"
#include "..\gui\Button.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\system\Utilities.h"

#include "..\gui\Slider.h"
#include "..\gui\Toggle.h"

#include <iostream>

SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
, mIsChanged(false)
, mIsWindowChanged(false)
{
	desiredMode.width = context.window->getSize().x;
	desiredMode.height = context.window->getSize().y;
	desiredMode.bitsPerPixel = 32;

	mBackgroundSprite.setTexture(context.textures->get(Textures::MenuBackground));

	Util::scaleSpriteToFitWindow(mBackgroundSprite, *context.window);

	validModes = Util::getValidVideoModes();

	initializeButtons();
}

void SettingsState::initializeButtons()
{
	mGUIContainer.clear();

	float y = 0.5f * getContext().window->getSize().y;
	float x = 0.5f * getContext().window->getSize().x;
	
	auto resolution = std::make_shared<GUI::Slider<VideoMode>>(getContext());
	resolution->setSize(Vector2f(500, 70));
	resolution->setPosition(Vector2f(x - 250, y + 90));
	resolution->setPossibleValues(validModes);
	resolution->setCurrentValue(desiredMode);
	resolution->setDisplayFunction([](VideoMode d)
	{
		return Util::toString(d.width) + "x" + Util::toString(d.height);
	});
	resolution->setCallback([this](int id)
	{
		desiredMode = validModes[id];
		mIsWindowChanged = true;
	});

	auto apply = std::make_shared<GUI::Button>(getContext());
	apply->setPosition(x + 10, y + 270);
	apply->setText("Apply");
	apply->setCallback([this]()
	{
		mIsChanged = true;
	});

	auto toggleVsyncButton = std::make_shared<GUI::Toggle>(getContext());
	toggleVsyncButton->setPosition(x - 250, y + 180);
	toggleVsyncButton->setText("Drag & Drop: OFF", "Drag & Drop: ON");
	toggleVsyncButton->setState(getContext().window->isDragDropAllowed());
	toggleVsyncButton->setCallback([this](bool flag)
	{
		getContext().window->setDragDropAllowed(flag);
	});

	auto toggleFS = std::make_shared<GUI::Toggle>(getContext());
	toggleFS->setPosition(x + 10, y + 180);
	toggleFS->setText("Windowed", "Full Screen");
	toggleFS->setState(getContext().window->isFullscreen());
	toggleFS->setCallback([this](bool flag)
	{
		if (flag)
		{
			getContext().window->switchToFullscreen(desiredMode);
		}
		else
		{
			getContext().window->switchToWindowed(desiredMode, Style::Close, true);
		}

		mIsChanged = true;
	});

	auto backButton = std::make_shared<GUI::Button>(getContext());
	backButton->setPosition(x - 250, y + 270);
	backButton->setText("Back");
	backButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Menu);
	});

	mGUIContainer.pack(resolution);
	mGUIContainer.pack(apply);
	mGUIContainer.pack(toggleFS);
	mGUIContainer.pack(toggleVsyncButton);
	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	RenderTarget2D& target = *getContext().target;

	RenderStates states;
	states.shaderHolder = getContext().shaders;
	states.shader = &states.shaderHolder->get(Shaders::Default);

	target.draw(mBackgroundSprite, states);
	target.draw(mGUIContainer, states);
}

bool SettingsState::update(Time)
{
	mGUIContainer.update();
	return true;
}

bool SettingsState::handleEvent(const Event& event)
{
	mGUIContainer.handleEvent(event);

	if (mIsChanged)
	{
		applyChanges();
	}

	return false;
}

void SettingsState::applyChanges()
{
	Window& window = *getContext().window;
	
	if (mIsWindowChanged)
	{
		window.setSize(Vector2i(desiredMode.width, desiredMode.height));
		mIsWindowChanged = false;
	}

	Util::scaleSpriteToFitWindow(mBackgroundSprite, *getContext().window);
		
	initializeButtons();

	mIsChanged = false;
}