#include "Component.hpp"

namespace GUI
{

	Component::Component() : mIsActive(false), m_parent(nullptr), m_mouseOver(false), m_theme()
{
}

Component::~Component()
{
}

void Component::setParent(Component* parent)
{
	m_parent = parent;
}

void Component::setTheme(Theme theme)
{
	m_theme = theme;
}

Theme Component::getTheme() const
{
	return m_theme;
}

bool Component::isMouseOver() const
{
	return m_mouseOver;
}

void Component::beginEvent(bool isMouseOver)
{
	m_mouseOver = isMouseOver;
}

bool Component::endEvent()
{
	bool returnVal = m_mouseOver;
	m_mouseOver = false;

	return returnVal;
}

const Matrix4f Component::getParentTransform() const
{
	if (m_parent != nullptr)
	{
		return m_parent->getParentTransform()* getTransform();
	}
	else
	{
		return getTransform();
	}
}

bool Component::isActive() const
{
	return mIsActive;
}

}