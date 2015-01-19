#pragma once

#include "..\state\State.hpp"
#include "..\gui\Container.hpp"

#include "rendering\Sprite.hpp"

class MenuState : public State
{
	public:
								MenuState(StateStack& stack, Context context);

		virtual void			draw();
		virtual bool			update(Time dt);
		virtual bool			handleEvent(const Event& event);

	private:

		void initializeButtons();

		Sprite				mBackgroundSprite;
		GUI::Container		mGUIContainer;
};