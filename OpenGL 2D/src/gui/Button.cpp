#include "Button.hpp"
#include <iostream>

#include "..\window\Event.h"
#include "..\rendering\RenderStates.hpp"
#include "..\rendering\RenderTarget2D.h"

namespace GUI
{

Button::Button(State::Context context)
: mCallback()
, mShape(Vector2f(240, 70))
, mText("", context.fonts->get(Fonts::Main), 18)
, window(*context.window)
, isPressed(false)
{
	changeState(Normal);
	mShape.setOutlineThickness(-5.f);
	mShape.setOutlineColor(Color::Black);

	RectF bounds = mShape.getLocalBounds();
	mText.setPosition(bounds.getCenter());
	mText.setColor(Color::Black);
}

Button::Ptr	Button::create(State::Context context)
{
	std::shared_ptr<Button> button(new Button(context));
	return button;
}

void Button::setCallback(Callback callback)
{
	mCallback = std::move(callback);
}

void Button::setText(const std::string& text)
{
	mText.setText(text);

	mText.renderMesh();
	mText.setOrigin(std::round(mText.getBounds().getCenter().x), std::round(mText.getBounds().getCenter().y));
}

void Button::setSize(Vector2f size)
{
	mShape.setSize(size);
	RectF bounds = mShape.getLocalBounds();
	mText.setPosition(std::round(bounds.getCenter().x), std::round(bounds.getCenter().y));
}

Vector2f Button::getSize() const
{
	return mShape.getSize();
}

bool Button::handleEvent(const Event& event)
{
	beginEvent(mouseOver());

	if (event.type == Event::MouseMoved)
	{
		mouseMoved();
	}
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		mousePressed();
	}
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
	{
		mouseReleased();
	}

	return endEvent();
}

void Button::update()
{	
	static float factor = 0.f;

	if (mouseOver())
	{
		Uint8 rgb = static_cast<Uint8>(std::abs(std::sin(factor)) * 255);
		mText.setColor(Color::rgb(rgb, rgb, rgb));
		factor += 0.05f;

		isPressed ? changeState(Pressed) : changeState(Hover);
	}
	else
	{
		mText.setColor(Color::Black);
		changeState(Normal);
	}
}

bool Button::mouseOver()
{
	return getParentTransform().transform(mShape.getGlobalBounds()).contains(Mouse::getPosition(window));
}

void Button::mouseMoved()
{
	if (isMouseOver())
	{
		isPressed ? changeState(Pressed) : changeState(Hover);
	}
	else
	{
		changeState(Normal);
		isPressed = false;
	}
}

void Button::mousePressed()
{
	if (isMouseOver())
	{
		changeState(Pressed);
		isPressed = true;
	}
}

void Button::mouseReleased()
{
	if (isMouseOver() && isPressed)
	{
		changeState(Hover);
		isPressed = false;
		mCallback();
	}
}

void Button::draw(RenderTarget2D& target, RenderStates states) const
{
	states.transform *= getTransform();

	states.shader = &states.shaderHolder->get(Shaders::Default);

	target.draw(mShape, states);

	states.shader = &states.shaderHolder->get(Shaders::SDFF);

	target.draw(mText, states);
}

void Button::changeState(Type buttonType)
{
	Color color;

	switch (buttonType)
	{

	case Pressed: color = m_theme.getPressed();  break;

	case Hover: color = m_theme.getHover(); break;

	case Normal: color = m_theme.getNormal();  break;

	}

	mShape.setFillColor(color);
}

}