#include "LoadingState.hpp"
#include "..\resource management\ResourceHolder.hpp"
#include "..\gui\Button.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\system\Utilities.h"

#include <iostream>
#include <memory>
#include <map>

bool LoadingState::loadParallel(Context context)
{
	Image i1;
	i1.loadFromFile("res/textures/opengl.png");

	Image i2;
	i2.loadFromFile("res/textures/aluminium.png");

	Image i3;
	i3.loadFromFile("res/textures/MenuBackground.png");

	images.push_back(i1);
	images.push_back(i2);
	images.push_back(i3);

	int from = 0;
	int to = 100;

	for (int i = from; i <= to; ++i) 
	{
		percent.store(i, std::memory_order_relaxed);
		std::cout << i << '\n';
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	std::cout << "Lift off!\n";
	return true;
}

LoadingState::LoadingState(StateStack& stack, Context context)
: State(stack, context),
doneLoading(false),
percentDone(0),
rectangle(Vector2f(10, 50)),
outer(Vector2f(10, 50)),
task(std::bind(&LoadingState::loadParallel, this, context)),
thread(std::move(task)),
percent(0)
{
	thread.detach();
	clock.restart();

	outer.setFillColor(Color::Black);
	outer.setSize(Vector2f(100 * 3, 50));
	outer.setOutlineThickness(5.f);
	//outer.setOutlineColor()


	rectangle.setFillColor(Color::rgb(35, 127, 229));


	rectangle.setPosition(1280 / 2 - 150, 720 / 2 + 50);

	outer.setPosition(1280 / 2 - 150, 720 / 2 + 50);

	mBackgroundSprite.setTexture(context.textures->get(Textures::Loading));

	//Util::scaleSpriteToFitWindow(mBackgroundSprite, *context.window);

	mBackgroundSprite.setScale(1.0f, 1.0f);
	auto size = mBackgroundSprite.getTexture()->getSize();
	mBackgroundSprite.setOrigin(static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f);
	mBackgroundSprite.setPosition(640.0f, 360.0f);
	initializeButtons();
}

void LoadingState::initializeButtons()
{

}

void LoadingState::draw()
{
	RenderTarget2D& target = *getContext().target;

	RenderStates states;
	states.shaderHolder = getContext().shaders;
	states.shader = &getContext().shaders->get(Shaders::Default);
	states.cam = &target.getCamera();
	target.draw(mBackgroundSprite, states);
	target.draw(outer, states);
	target.draw(rectangle, states);
	target.draw(mGUIContainer, states);
}

bool LoadingState::update(Time dt)
{	
	int p = percent.load();
	if (p == 100)
	{
		getContext().textures->loadFromImage(Textures::Texture1, images[0]);
		getContext().textures->loadFromImage(Textures::Texture2, images[1]);
		//getContext().textures->loadFromMemory(Textures::MenuBackground, images[2].getData(), images[2].getSize().x, images[2].getSize().y);
		getContext().textures->loadFromImage(Textures::MenuBackground, images[2]);
		doneLoading = true;
	}
	rectangle.setSize(Vector2f(p * 3, 50));

	if (doneLoading)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	mGUIContainer.update();
	return true;
}

bool LoadingState::handleEvent(const Event& event)
{
	//outer.setPosition(event.mouseMove.x, event.mouseMove.y);
	//rectangle.setPosition(event.mouseMove.x, event.mouseMove.y);

	mGUIContainer.handleEvent(event);
	return false;
}