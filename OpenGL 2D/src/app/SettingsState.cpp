
#include "SettingsState.hpp"
#include "..\resource management\ResourceHolder.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\system\Utilities.h"

#include "..\gui\Button.hpp"
#include "..\gui\Slider.h"
#include "..\gui\Toggle.h"
#include "..\gui\ListBox.hpp"

#include <iostream>

SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
, mIsChanged(false)
, mIsWindowChanged(false)
, mIsWindowResChanged(false)
, mWindowFullscreen(false)
{
	desiredMode.width = context.window->getSize().x;
	desiredMode.height = context.window->getSize().y;
	desiredMode.bitsPerPixel = 32;

	mBackgroundSprite.setTexture(context.textures->get(Textures::MenuBackground));

	Util::scaleSpriteToFitWindow(mBackgroundSprite, *context.window);

	validModes = Util::getValidVideoModes();

	m_graph.setEquation([](float x)
	{
		if (x == 0)
		{
			return 1.f;
		}

		return std::sin(x) / x;
	});
	m_graph.setRange(Range(-std::atan(1) * 4.f, 0.1f, std::atan(1) * 4.f));
	m_graph.setSize(Vector2f(200.f, 160.f));
	m_graph.setPosition(Vector3f(200, 0, 0));
	m_graph.plot();
	m_graph.setPosition(500, 100);

	initializeButtons();
}

void SettingsState::initializeButtons()
{
	mGUIContainer.clear();

	float y = 0.5f * getContext().window->getSize().y;
	float x = 0.5f * getContext().window->getSize().x;
	
	auto list = GUI::ListBox::create(getContext());
	list->setPosition(100.0f, 100.0f);
	list->setCallback([this](int id)
	{
		float x = id * 0.01f;
		float y = id * 0.01f;
	//	m_graph.setScale(x + 1, y+1);
		m_graph.setPosition(500, 250 * y + 100);
	});

	auto button1 = GUI::Button::create(getContext());
	button1->setSize(Vector2f(200.0f, 40.0f));
	button1->setText("testing");
	button1->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Menu);
	});
	list->addComponent(button1);

	auto resolution = GUI::Slider<VideoMode>::create(getContext());
	resolution->setSize(Vector2f(500.0f, 70.0f));
	resolution->setPosition(Vector2f(x - 250.0f, y + 90.0f));
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

	auto apply = GUI::Button::create(getContext());
	apply->setPosition(x + 10.0f, y + 270.0f);
	apply->setText("Apply");
	apply->setCallback([this]()
	{
		mIsChanged = true;
	});

	auto toggleVsyncButton = GUI::Toggle::create(getContext());
	toggleVsyncButton->setPosition(x - 250.0f, y + 180.0f);
	toggleVsyncButton->setText("Drag & Drop: OFF", "Drag & Drop: ON");
	toggleVsyncButton->setState(getContext().window->isDragDropAllowed());
	toggleVsyncButton->setCallback([this](bool flag)
	{
		getContext().window->setDragDropAllowed(flag);
	});

	auto toggleFS = GUI::Toggle::create(getContext());
	toggleFS->setPosition(x + 10.0f, y + 180.0f);
	toggleFS->setText("Windowed", "Full Screen");
	toggleFS->setState(getContext().window->isFullscreen());
	toggleFS->setCallback([this](bool flag)
	{
		mWindowFullscreen = flag;

	/*	if (flag)
		{
			getContext().window->switchToFullscreen(desiredMode);
		}
		else
		{
			getContext().window->switchToWindowed(desiredMode, Style::Close, true);
		}*/
		mIsWindowResChanged = true;
		//mIsChanged = true;
	});

	auto backButton = GUI::Button::create(getContext());
	backButton->setPosition(x - 250.0f, y + 270.0f);
	backButton->setText("Back");
	backButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Menu);
	});

	mGUIContainer.pack(list);
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
	states.cam = &target.getCamera();
	target.draw(mBackgroundSprite, states);
	target.draw(mGUIContainer, states);
	states.shader = &states.shaderHolder->get(Shaders::Default);
	target.draw(m_graph, states);
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
	
	if (mIsWindowResChanged)
	{
		if (mWindowFullscreen)
		{
			getContext().window->switchToFullscreen(desiredMode);
		}
		else
		{
			getContext().window->switchToWindowed(desiredMode, Style::Close, true);
		}

		mIsWindowResChanged = false;
	}
	else if (mIsWindowChanged)
	{
		window.setSize(Vector2i(desiredMode.width, desiredMode.height));
		mIsWindowChanged = false;
	}

	Util::scaleSpriteToFitWindow(mBackgroundSprite, *getContext().window);
		
	initializeButtons();

	mIsChanged = false;
}