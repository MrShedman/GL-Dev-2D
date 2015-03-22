#pragma once

#include "Component.hpp"
#include "..\app\ResourceIdentifiers.hpp"
#include "..\state\State.hpp"
#include "..\resource management\ResourceHolder.hpp"

#include "..\rendering\RectangleShape.hpp"
#include "..\text\Text.hpp"

#include "..\system\Utilities.h"

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace GUI
{

template<class T>
class Slider : public Component
{
public:
	typedef std::shared_ptr<Slider<T>>			Ptr;
	typedef std::function<void(int)>		Callback;
	typedef std::function<std::string(T)>	DisplayFunction;

	enum Orientation
	{
		Horizontal,
		Vertical
	}
	m_orientation;

	enum Type
	{
		Normal,
		Hover,
		Pressed,
		ButtonCount
	};

public:


	static Ptr				create(State::Context context, Orientation = Horizontal);

	void					setCallback(Callback callback);
	void					setText(const std::string& text);
	void					setSize(Vector2f size, float thickness = 30);

	void					setDisplayFunction(DisplayFunction func);
	void					setCurrentValue(T value);
	void					setPossibleValues(std::vector<T> values);

	virtual bool			handleEvent(const Event& event);
	virtual void			update();

	void					setPositionFactor(float factor);
	float					getPositionFactor() const;

	float					getHandleSize() const;

private:

	Slider(State::Context context, Orientation = Horizontal);

	bool					mouseOver(RectF rect);
	void					mouseMoved();
	void					mousePressed();
	void					mouseReleased();

	void					changeState(Type buttonType);
	void					updateRange();

	int						getIndex();
	float					getOffset();
	std::string				getDisplay();

	virtual void			draw(RenderTarget2D& target, RenderStates states) const;

private:

	bool					isPressed;
	float					minRange;
	float					maxRange;
	float					offset;

private:

	T						mCurrent;
	std::vector<T>			possibleValues;
	DisplayFunction			mDisplayFunction;
	Callback				mCallback;

private:

	RectangleShape		mOutline;
	RectangleShape		mShape;
	Text				mText;
	Window&				window;
};

}

#include "Slider.inl"