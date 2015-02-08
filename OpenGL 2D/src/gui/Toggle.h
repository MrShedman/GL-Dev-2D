#pragma once

#include "Component.hpp"
#include "..\app\ResourceIdentifiers.hpp"
#include "..\state\State.hpp"
#include "..\resource management\ResourceHolder.hpp"

#include "..\rendering\RectangleShape.hpp"
#include "..\text\Text.hpp"

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace GUI
{
	class Toggle : public Component
	{
	public:
		typedef std::shared_ptr<Toggle>		Ptr;
		typedef std::function<void(bool)>	Callback;

		enum Type
		{
			Normal,
			Hover,
			Pressed,
			ButtonCount
		};


	public:
		Toggle(State::Context context);

		void					setCallback(Callback callback);
		void					setText(const std::string& offText, const std::string& onText);
		void					setSize(Vector2f size);
		void					setState(bool flag);
		virtual bool			handleEvent(const Event& event);
		virtual void			update();

	private:

		bool mouseOver();
		void mouseMoved();
		void mousePressed();
		void mouseReleased();

		virtual void			draw(RenderTarget2D& target, RenderStates states) const;
		void					changeState(Type buttonType);
		void					updateText();

	private:
		bool isSelected;

		std::string				offText;
		std::string				onText;
		Callback				mCallback;
		RectangleShape			mShape;
		Text					mText;
		Window&					window;
	};

}