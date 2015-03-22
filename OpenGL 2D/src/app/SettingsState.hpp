#pragma once

#include "..\state\State.hpp"
#include "..\gui\Container.hpp"

#include "..\rendering\Sprite.hpp"

#include "..\experiment\Graph2D.h"

class SettingsState : public State
{
	public:
										SettingsState(StateStack& stack, Context context);

		virtual void					draw();
		virtual bool					update(Time dt);
		virtual bool					handleEvent(const Event& event);

	private:

		bool							mWindowFullscreen;
		bool							mIsWindowResChanged;
		bool							mIsWindowChanged;
		bool							mIsChanged;

		void initializeButtons();
		void applyChanges();
		
		Graph2D					m_graph;

		VideoMode				desiredMode;
		std::vector<VideoMode>	validModes;

		Sprite					mBackgroundSprite;
		GUI::Container			mGUIContainer;
};