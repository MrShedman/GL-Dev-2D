#pragma once

#include "..\state\State.hpp"

#include "window\Time.h"
#include "rendering\Texture.hpp"
#include "rendering\Color.h"
#include "rendering\Sprite.hpp"
#include "rendering\RectangleShape.hpp"
#include "rendering\RenderTexture.hpp"
#include "text\Font.hpp"
#include "text\Text.hpp"

#include "..\gui\Container.hpp"
#include "..\gui\Button.hpp"
#include "..\gui\Slider.h"

#include "..\rendering\Block.hpp"
#include "..\rendering\CameraPersp.hpp"

#include "..\experiment\Graph.h"

class GameState : public State
{
	public:
							GameState(StateStack& stack, Context context);

		virtual void		draw();
		virtual bool		update(Time dt);
		virtual bool		handleEvent(const Event& event);

	private:

		bool polygonMode;

		void initializeButtons();

		float speed;

		std::vector<Graph> m_graphs;

		CameraPersp cam;

		std::vector<Block>	mBlocks;

		Sprite				mBackgroundSprite;
		Text				mText;

		Clock clock;
		GUI::Container		mGUIContainer;
};