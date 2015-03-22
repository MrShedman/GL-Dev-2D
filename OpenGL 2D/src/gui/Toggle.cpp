#include "Toggle.h"
#include <iostream>

#include "..\window\Event.h"
#include "..\rendering\RenderStates.hpp"
#include "..\rendering\RenderTarget2D.h"

namespace GUI
{

Toggle::Toggle(State::Context context)
	: mCallback()
	, mShape(Vector2f(240, 70))
	, mText("", context.fonts->get(Fonts::Main), 18)
	, window(*context.window)
	, isSelected(false)
{
	changeState(Normal);
	mShape.setOutlineThickness(-5.f);
	mShape.setOutlineColor(Color::Black);

	RectF bounds = mShape.getLocalBounds();
	mText.setPosition(bounds.getCenter());
	mText.setColor(Color::Black);
}

Toggle::Ptr Toggle::create(State::Context context)
{
	std::shared_ptr<Toggle> toggle(new Toggle(context));
	return toggle;
}

void Toggle::setCallback(Callback callback)
{
	mCallback = std::move(callback);
}

void Toggle::setText(const std::string& offText, const std::string& onText)
{
	this->offText = offText;
	this->onText = onText;

	updateText();
}

void Toggle::updateText()
{
	std::string text = isSelected ? onText : offText;
	mText.setText(text);
	mText.renderMesh();
	mText.setOrigin(mText.getBounds().getCenter());
}

void Toggle::setSize(Vector2f size)
{
	mShape.setSize(size);
	RectF bounds = mShape.getLocalBounds();
	mText.setPosition(bounds.getCenter());
}

void Toggle::setState(bool flag)
{
	isSelected = flag;
	updateText();
	isSelected ? changeState(Pressed) : changeState(Normal);
}

Vector2f Toggle::getSize() const
{
	return mShape.getSize();
}

bool Toggle::handleEvent(const Event& event)
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

void Toggle::update()
{
	static float factor = 0.f;

	if (mouseOver())
	{
		Uint8 rgb = static_cast<Uint8>(std::abs(std::sin(factor)) * 255);
		mText.setColor(Color::rgb(rgb, rgb, rgb));
		factor += 0.05f;
		isSelected ? changeState(Pressed) : changeState(Hover);
	}
	else
	{
		mText.setColor(Color::Black);
		isSelected ? changeState(Pressed) : changeState(Normal);
	}
	//mouseMoved();
}

bool Toggle::mouseOver()
{
	return getParentTransform().transform(mShape.getGlobalBounds()).contains(Mouse::getPosition(window));
}

void Toggle::mouseMoved()
{
	if (isMouseOver())
	{
		isSelected ? changeState(Pressed) : changeState(Hover);
	}
	else
	{
		isSelected ? changeState(Pressed) : changeState(Normal);
	}
}

void Toggle::mousePressed()
{
	if (isMouseOver())
	{
		isSelected = !isSelected;
		isSelected ? changeState(Pressed) : changeState(Hover);
	
		updateText();
			
		//mSounds.play(SoundEffect::Button);
	}
}

void Toggle::mouseReleased()
{
	if (isMouseOver())
	{
		mCallback(isSelected);
	}
}

void Toggle::draw(RenderTarget2D& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mShape, states);

	states.shader = &states.shaderHolder->get(Shaders::SDFF);

	target.draw(mText, states);
}

void Toggle::changeState(Type buttonType)
{
	Color color;

	switch (buttonType)
	{

	case Pressed: color = pressedColor;  break;

	case Hover: color = hoverColor; break;

	case Normal: color = normalColor;  break;

	}

	mShape.setFillColor(color);
}

}