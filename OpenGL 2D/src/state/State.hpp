#pragma once

#include "..\app\StateIdentifiers.hpp"
#include "..\app\ResourceIdentifiers.hpp"

#include "..\window\Time.h"
#include "..\window\Event.h"

#include <memory>


class Window;
class RenderTarget2D;
class StateStack;

class State
{
	public:
		typedef std::unique_ptr<State> Ptr;

		struct Context
		{
			Context(Window& window, RenderTarget2D &target, TextureHolder &textures, FontHolder &fonts, ShaderHolder &shaders);

			Window* window;
			RenderTarget2D *target;
			TextureHolder *textures;
			FontHolder *fonts;
			ShaderHolder *shaders;
		};


	public:
		
		State(StateStack& stack, Context context);

		virtual ~State();

		virtual void draw() = 0;
		virtual bool update(Time dt) = 0;
		virtual bool handleEvent(const Event& event) = 0;


	protected:
		void requestStackPush(States::ID stateID);
		void requestStackPop();
		void requestStateClear();

		Context	getContext() const;

	private:
		StateStack* mStack;
		Context mContext;
};
