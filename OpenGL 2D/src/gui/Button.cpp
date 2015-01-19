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

	mShape.generateVBO();
}

void Button::setCallback(Callback callback)
{
	mCallback = std::move(callback);
}

void Button::setText(const std::string& text)
{
	mText.setText(text);

	mText.renderMesh();
	mText.setOrigin(mText.getBounds().getCenter());

}

void Button::setSize(Vector2f size)
{
	mShape.setSize(size);
	RectF bounds = mShape.getLocalBounds();
	mText.setPosition(bounds.getCenter());
}

void Button::handleEvent(const Event& event)
{
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
}

void Button::update()
{	
	static float factor = 0.f;

	if (mouseOver())
	{
		Uint8 rgb = static_cast<Uint8>(std::abs(std::sin(factor)) * 255);
		mText.setColor(Color(rgb, rgb, rgb));
		factor += 0.05f;
		mText.renderMesh();
	}
	else
	{
		mText.setColor(Color::Black);
	}
}

bool Button::mouseOver()
{
	return getTransform().Transform(mShape.getLocalBounds()).contains(Mouse::getPosition(window));
}

void Button::mouseMoved()
{
	if (mouseOver())
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
	if (mouseOver())
	{
		changeState(Pressed);
		isPressed = true;

		//mSounds.play(SoundEffect::Button);
	}
}

void Button::mouseReleased()
{
	if (mouseOver() && isPressed)
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

	case Pressed: color = Color(255, 177, 0);  break;

	case Hover: color = Color(181, 230, 29); break;

	case Normal: color = Color(31, 177, 76);  break;

	}

	mShape.setFillColor(color);
}

}