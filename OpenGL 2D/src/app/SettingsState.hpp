#pragma once

#include "..\state\State.hpp"
#include "..\gui\Container.hpp"

#include "..\rendering\Sprite.hpp"

class SettingsState : public State
{
	public:
										SettingsState(StateStack& stack, Context context);

		virtual void					draw();
		virtual bool					update(Time dt);
		virtual bool					handleEvent(const Event& event);

	private:

		bool							mIsWindowChanged;
		bool							mIsChanged;

		void initializeButtons();
		void applyChanges();
		
		VideoMode				desiredMode;
		std::vector<VideoMode>	validModes;

		Sprite					mBackgroundSprite;
		GUI::Container			mGUIContainer;
};