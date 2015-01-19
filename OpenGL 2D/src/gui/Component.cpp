#include "Component.hpp"

namespace GUI
{

Component::Component() : mIsActive(false)
{
}

Component::~Component()
{
}

bool Component::isActive() const
{
	return mIsActive;
}

}