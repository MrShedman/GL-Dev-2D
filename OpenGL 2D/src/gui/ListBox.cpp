#include "ListBox.hpp"

#include "..\window\Event.h"
#include "..\rendering\RenderStates.hpp"
#include "..\rendering\RenderTarget2D.h"

#include <numeric>

namespace GUI
{


ListBox::ListBox(State::Context context)
: m_size(240.0f, 240.0f)
, mShape(m_size)
, mText("", context.fonts->get(Fonts::Main), 18)
, window(*context.window)
, isPressed(false)
{
	m_slider = GUI::Slider<float>::create(context, GUI::Slider<float>::Orientation::Vertical);

	mShape.setFillColor(Color::rgb(150, 150, 150));

	std::vector<float> num(100);
	std::iota(num.begin(), num.end(), 0.0f);

	m_slider->setPossibleValues(num);
	m_slider->setCurrentValue(0.0f);
	m_slider->setDisplayFunction([](float d)
	{
		return ""; Util::toString(d);
	});
	m_slider->setCallback([this](int id)
	{
	
	});
	m_slider->setParent(this);
	m_slider->setPosition(m_size.x - 5.f, 0.f);
	mShape.setOutlineThickness(-5.f);
	mShape.setOutlineColor(Color::Black);
	
	int numEntries = 20;

	for (int i = 0; i < numEntries; ++i)
	{
		auto toggle = Toggle::create(context);
		toggle->setText("Toggle" + Util::toString(i) + " off", "Toggle" + Util::toString(i) + " on");
		
		toggle->setCallback([this](bool flag)
		{
		
		});
		toggle->setSize(Vector2f(240.0f, 40.0f));
		if (i == 5)
		{
			toggle->setSize(Vector2f(240.0f, 120.0f));
		}

		addComponent(toggle);
	}

	float size = 1.0f - (((numEntries * 65.0f) - m_size.y) / (numEntries * 65.0f));
	Util::clamp(size, 0.05f, 0.95f);
	size *= m_size.y;

	m_slider->setSize(Vector2f(30.0f, m_size.y), size);
	updatePositions();
}

ListBox::Ptr ListBox::create(State::Context context)
{
	std::shared_ptr<ListBox> listbox(new ListBox(context));
	return listbox;
}

void ListBox::addComponent(Component::Ptr component)
{
	component->setParent(this);
	m_components.push_back(component);	
	updatePositions();
}

void ListBox::setCallback(Callback callback)
{
	m_slider->setCallback(std::move(callback));
}

void ListBox::setText(const std::string& text)
{

}

void ListBox::setSize(Vector2f size)
{
	m_size = size;
	updatePositions();
}

bool ListBox::handleEvent(const Event& event)
{
	bool slider = m_slider->handleEvent(event);

	bool mouse = mouseOver();

	if (slider || mouse)
	{
		float pos = m_slider->getPositionFactor();

		if (event.type == Event::MouseWheelMoved && mouse)
		{
			float size = 1.f / (m_size.y - m_slider->getHandleSize());

			float dir = -std::copysign(4.0f*size, event.mouseWheel.delta);
			m_slider->setPositionFactor(pos + dir);
		}

		updatePositions();

		for (auto&t : m_components)
		{
			if (t->handleEvent(event))
			{
				//break;
			}
		}

		return true;
	}

	return false;
}

void ListBox::update()
{
	//if (mouseOver())
	{
		for (auto &t : m_components)
		{
			t->update();
		}
	}
}

bool ListBox::mouseOver()
{
	return getParentTransform().transform(mShape.getGlobalBounds()).contains(Mouse::getPosition(window));
}

void ListBox::mouseMoved()
{

}

void ListBox::mousePressed()
{

}

void ListBox::mouseReleased()
{

}

void ListBox::updatePositions()
{
	float totalSize = 0.f;

	for (auto &t : m_components)
	{
		totalSize += t->getSize().y - 5.0f;
	}

	totalSize += 5;

	float pos = m_slider->getPositionFactor();
	float orig = m_components.front()->getPosition().x;

	m_components.front()->setPosition(orig, (totalSize - m_size.y)*-pos);
	float size = m_components.size();
	for (int i = 1; i < size; ++i)
	{
		m_components[i]->setPosition(orig, m_components[i - 1]->getPosition().y + m_components[i - 1]->getSize().y - 5.0f);
	}
}

void ListBox::draw(RenderTarget2D& target, RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mShape, states);
	target.draw(*m_slider, states);
	
	auto pos = getParentTransform().transformPoint(mShape.getPosition());

	RectF viewport;
	viewport.top = (pos.y + 5.0f) / window.getSize().y;
	viewport.bottom = (pos.y + m_size.y - 5) / window.getSize().y;
	viewport.left = pos.x / window.getSize().x;
	viewport.right = (pos.x + m_size.x) / window.getSize().x;

	Camera* camera = &target.getCamera();
	
	Camera cam;
	cam.setCenter(pos.x + m_size.x * 0.5f, pos.y + m_size.y * 0.5f);
	cam.setSize(m_size.x, m_size.y - 10.0f);
	cam.setViewport(viewport);
	states.cam = &cam;

	for (auto&t : m_components)
	{
		target.draw(*t, states);
	}

	states.cam = camera;
	states.shader = &states.shaderHolder->get(Shaders::SDFF);

	target.draw(mText, states);
}

}