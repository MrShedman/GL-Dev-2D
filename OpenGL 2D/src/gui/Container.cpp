#include "Container.hpp"

#include "..\window\Event.h"
#include "..\rendering\RenderStates.hpp"
#include "..\rendering\RenderTarget2D.h"

namespace GUI
{

Container::Container()
: mChildren()
{
}

void Container::pack(Component::Ptr component)
{
	mChildren.push_back(component);
}

void Container::clear()
{
	mChildren.clear();
}

void Container::handleEvent(const Event& event)
{
	for (auto &child : mChildren)
	{
		child->handleEvent(event);
	}
}

void Container::update()
{
	for (auto &child : mChildren)
	{
		child->update();
	}
}

void Container::draw(RenderTarget2D& target, RenderStates states) const
{
    states.transform *= getTransform();

	for (auto &child : mChildren)
	{
		target.draw(*child, states);
	}
}

}