#pragma once

#include "Component.hpp"
#include "Button.hpp"
#include "Slider.h"
#include "Toggle.h"
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

	class ListBox : public Component
	{
	public:
		typedef std::shared_ptr<ListBox>		Ptr;
		typedef std::function<void(int)>		Callback;

	public:

		static Ptr				create(State::Context context);

		void					setCallback(Callback callback);
		void					setText(const std::string& text);
		void					setSize(Vector2f size);

		virtual bool			handleEvent(const Event& event);
		virtual void			update();

		void					addComponent(Component::Ptr component);

	private:

		ListBox(State::Context context);

		bool mouseOver();
		void mouseMoved();
		void mousePressed();
		void mouseReleased();

		void updatePositions();

		virtual void			draw(RenderTarget2D& target, RenderStates states) const;

	private:
		bool isPressed;

		Vector2f				m_size;

		std::vector<Component::Ptr>	m_components;
		Slider<float>::Ptr			m_slider;
		RectangleShape			mShape;
		Text					mText;
		Window&					window;
	};

}