
namespace GUI
{
	template<class T>
	Slider<T>::Slider(State::Context context, Orientation orientation)
		: mCallback()
		, mDisplayFunction()
		, m_orientation(orientation)
		, mShape(orientation == Horizontal ? Vector2f(30.0f, 70.0f) : Vector2f(70.0f, 30.0f))
		, mOutline(orientation == Horizontal ? Vector2f(240.0f, 70.0f) : Vector2f(70.0f, 240.0f))
		, mText("", context.fonts->get(Fonts::Main), 18.0f)
		, window(*context.window)
		, isPressed(false)
	{
		changeState(Normal);

		mOutline.setFillColor(Color::rgb(150, 150, 150));
		mOutline.setOutlineThickness(-5.0f);
		mOutline.setOutlineColor(Color::Black);

		mShape.setOutlineThickness(-5.0f);
		mShape.setOutlineColor(Color::Black);

		mText.setColor(Color::White);
		RectF bounds = mOutline.getLocalBounds();
		mText.setPosition(bounds.getCenter());

		updateRange();
	}

	template<class T>
	typename Slider<T>::Ptr Slider<T>::create(State::Context context, Orientation orientation)
	{
		std::shared_ptr<Slider<T>> slider(new Slider<T>(context, orientation));
		return slider;
	}

	template<class T>
	void Slider<T>::setCallback(Callback callback)
	{
		mCallback = std::move(callback);
	}

	template<class T>
	void Slider<T>::setDisplayFunction(DisplayFunction func)
	{
		mDisplayFunction = std::move(func);
		setText(getDisplay());
	}

	template<class T>
	void Slider<T>::setPossibleValues(std::vector<T> values)
	{
		possibleValues = std::move(values);
	}

	template<class T>
	void Slider<T>::setCurrentValue(T value)
	{
		int size = static_cast<int>(possibleValues.size());

		for (int i = 0; i < size; ++i)
		{
			if (value == possibleValues[i])
			{
				float percent = static_cast<float>(i) / static_cast<float>((size - 1));

				float position = (maxRange - minRange) * percent + minRange;

				if (m_orientation == Horizontal)
				{
					mShape.setPosition(position, mShape.getPosition().y);
				}
				else
				{
					mShape.setPosition(mShape.getPosition().x, position);
				}
			}
		}

		mCurrent = possibleValues[getIndex()];
	}

	template<class T>
	void Slider<T>::setText(const std::string& text)
	{
		mText.setText(text);
		mText.renderMesh();
		mText.setOrigin(mText.getBounds().getCenter());
	}

	template<class T>
	void Slider<T>::updateRange()
	{
		minRange = 0;
		RectF bounds = mOutline.getLocalBounds();

		if (m_orientation == Horizontal)
		{
			maxRange = bounds.right - mShape.getSize().x;
		}
		else
		{
			maxRange = bounds.bottom - mShape.getSize().y;
		}
	}

	template<class T>
	void Slider<T>::setSize(Vector2f size, float thickness)
	{
		if (m_orientation == Horizontal)
		{
			mShape.setSize(Vector2f(thickness, size.y));
		}
		else
		{
			mShape.setSize(Vector2f(size.x, thickness));			
		}
	
		mOutline.setSize(size);
		RectF bounds = mOutline.getLocalBounds();
		mText.setPosition(bounds.getCenter());

		updateRange();
	}

	template<class T>
	void Slider<T>::update()
	{

	}

	template<class T>
	void Slider<T>::setPositionFactor(float factor)
	{
		Util::clamp(factor, 0.0f, 1.0f);

		float newPos = factor * (maxRange - minRange) + minRange;

		if (m_orientation == Horizontal)
		{
			mShape.setPosition(newPos, mShape.getPosition().y);
		}
		else
		{
			mShape.setPosition(mShape.getPosition().x, newPos);
		}
	}

	template<class T>
	float Slider<T>::getPositionFactor() const
	{
		float position = (m_orientation == Horizontal) ? mShape.getPosition().x : mShape.getPosition().y;

		return (position - minRange) / (maxRange - minRange);
	}

	template<class T>
	float Slider<T>::getHandleSize() const
	{
		if (m_orientation == Horizontal)
		{
			return mShape.getSize().x;// mShape.setSize(Vector2f(thickness, size.y));
		}
		else
		{
			return mShape.getSize().y; // mShape.setSize(Vector2f(size.x, thickness));
		}
	}

	template<class T>
	int Slider<T>::getIndex()
	{
		float size = static_cast<float>(possibleValues.size() - 1);

		float index = std::round(getPositionFactor() * size);
	
		Util::clamp(index, 0.f, size);

		return static_cast<int>(index);
	}

	template<class T>
	std::string Slider<T>::getDisplay()
	{
		mCurrent = possibleValues[getIndex()];
		return mDisplayFunction(mCurrent);
	}

	template<class T>
	float Slider<T>::getOffset()
	{
		if (m_orientation == Horizontal)
		{
			return Mouse::getPosition(window).x - mShape.getPosition().x;
		}
		else
		{
			return Mouse::getPosition(window).y - mShape.getPosition().y;
		}
	}

	template<class T>
	bool Slider<T>::handleEvent(const Event& event)
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

		return mouseOver(mShape.getGlobalBounds());
	}

	template<class T>
	bool Slider<T>::mouseOver(RectF rect)
	{
		return getParentTransform().transform(rect).contains(Mouse::getPosition(window));
	}

	template<class T>
	void Slider<T>::mouseMoved()
	{
		if (isPressed)
		{
			float mouse = 0.0f;
			
			if (m_orientation == Horizontal)
			{
				mouse = static_cast<float>(Mouse::getPosition(window).x);
			}
			else
			{
				mouse = static_cast<float>(Mouse::getPosition(window).y);
			}

			float newPos = mouse - offset;
			float oldPos = newPos;

			Util::clamp(newPos, minRange, maxRange);

			// if we are at edge calculate new offset
			if (oldPos != newPos)
			{
				offset = getOffset();
			}

			if (m_orientation == Horizontal)
			{
				mShape.setPosition(newPos, mShape.getPosition().y);
			}
			else
			{
				mShape.setPosition(mShape.getPosition().x, newPos);
			}

			setText(getDisplay());
			mCallback(getIndex());
		}

		if (mouseOver(mShape.getGlobalBounds()))
		{
			isPressed ? changeState(Pressed) : changeState(Hover);
		}
		else
		{
			changeState(Normal);
			isPressed = false;
		}
	}

	template<class T>
	void Slider<T>::mousePressed()
	{
		if (mouseOver(mShape.getGlobalBounds()))
		{
			//find offset between first click and sliders top left corner
			offset = getOffset();

			changeState(Pressed);
			isPressed = true;
		}
		else if (mouseOver(mOutline.getGlobalBounds()))
		{
			float newPos = 0;

			if (m_orientation == Horizontal)
			{
				newPos = Mouse::getPosition(window).x - getPosition().x - mShape.getSize().x / 2.0f;
			}
			else
			{
				newPos = Mouse::getPosition(window).y - getPosition().y - mShape.getSize().y / 2.0f;
			}

			Util::clamp(newPos, minRange, maxRange);

			if (m_orientation == Horizontal)
			{
				mShape.setPosition(newPos, mShape.getPosition().y);
			}
			else
			{
				mShape.setPosition(mShape.getPosition().x, newPos);
			}

			setText(getDisplay());

			mCallback(getIndex());

			offset = getOffset();

			changeState(Pressed);
			isPressed = true;
		}
	}

	template<class T>
	void Slider<T>::mouseReleased()
	{
		if (mouseOver(mShape.getGlobalBounds()) && isPressed)
		{
			changeState(Hover);
			isPressed = false;
		}
	}

	template<class T>
	void Slider<T>::draw(RenderTarget2D& target, RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mOutline, states);
		target.draw(mShape, states);

		states.shader = &states.shaderHolder->get(Shaders::SDFF);

		target.draw(mText, states);
	}

	template<class T>
	void Slider<T>::changeState(Type buttonType)
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