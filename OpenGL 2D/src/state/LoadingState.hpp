#pragma once

#include "..\state\State.hpp"
#include "..\gui\Container.hpp"

#include "..\rendering\Image.hpp"
#include "..\rendering\Sprite.hpp"
#include "..\rendering\RectangleShape.hpp"

#include <iostream>     // std::cout
#include <future>       // std::packaged_task, std::future
#include <chrono>       // std::chrono::seconds
#include <thread>
#include <functional>

class LoadingState : public State
{
public:
	LoadingState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(Time dt);
	virtual bool			handleEvent(const Event& event);

private:

	std::vector<Image> images;

	
	bool loadParallel(Context context);

	std::atomic<int> percent;

	std::packaged_task<bool()> task;   // set up packaged_task
	
	std::thread thread;   // spawn thread to count down from 10 to 0

	RectangleShape outer;
	RectangleShape rectangle;

	Clock clock;

	int percentDone;
	bool doneLoading;

	void initializeButtons();

	Sprite				mBackgroundSprite;
	GUI::Container		mGUIContainer;
};