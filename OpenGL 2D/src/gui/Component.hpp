#pragma once

#include "..\window\NonCopyable.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"

#include <memory>

class Event;

namespace GUI
{

class Component : public Drawable, public Transform, private NonCopyable
{
    public:
        typedef std::shared_ptr<Component> Ptr;


	public:
							Component();
        virtual				~Component();

        virtual bool		isActive() const;

        virtual bool		handleEvent(const Event& event) = 0;
		virtual void		update() = 0;

    private:
        bool				mIsActive;
};

}