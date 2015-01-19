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

#include "..\gui\Button.hpp"
#include "..\gui\Slider.h"

#include "..\rendering\Block.hpp"
#include "..\rendering\CameraPersp.hpp"

class GameState : public State
{
	public:
							GameState(StateStack& stack, Context context);

		virtual void		draw();
		virtual bool		update(Time dt);
		virtual bool		handleEvent(const Event& event);

	private:

		CameraPersp cam;

		std::vector<Block>	mBlocks;

		Sprite				mBackgroundSprite;
		Font				mFont;
		Text				mText;

		Clock clock;
};