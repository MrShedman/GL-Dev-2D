#pragma once

#include "Component.hpp"
#include "..\app\ResourceIdentifiers.hpp"
#include "..\state\State.hpp"
#include "..\resource management\ResourceHolder.hpp"

#include "..\rendering\RectangleShape.hpp"
#include "..\text\Text.hpp"
#include "..\window\Window.hpp"

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace GUI
{

class Button : public Component
{
    public:
        typedef std::shared_ptr<Button>		Ptr;
        typedef std::function<void()>		Callback;

		enum Type
		{
			Normal,
			Hover,
			Pressed,
			ButtonCount
		};

	public:
								Button(State::Context context);

        void					setCallback(Callback callback);
        void					setText(const std::string& text);
		void					setSize(Vector2f size);

		virtual bool			handleEvent(const Event& event);
		virtual void			update();

    private:

		bool mouseOver();
		void mouseMoved();
		void mousePressed();
		void mouseReleased();

		virtual void			draw(RenderTarget2D& target, RenderStates states) const;
		void					changeState(Type buttonType);

    private:
		bool isPressed;

        Callback				mCallback;
        RectangleShape			mShape;
		Text					mText;
		Window&					window;
};

}