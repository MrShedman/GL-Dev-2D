#pragma once

#include "..\window\NonCopyable.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"

#include "Theme.hpp"

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

		virtual void		setSize(Vector2f size){}
		void				setParent(Component* parent);

		virtual Vector2f	getSize() const{ return Vector2f(); }

		void				setTheme(Theme theme);

		Theme				getTheme() const;

	protected:

		Theme				m_theme;

		bool				m_mouseOver;

		bool				isMouseOver() const;

		void				beginEvent(bool isMouseOver);
		bool				endEvent();

		Component*			m_parent;

		const Matrix4f		getParentTransform() const;

    private:

        bool				mIsActive;
};

}