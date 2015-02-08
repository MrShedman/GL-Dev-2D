#pragma once

#include "Component.hpp"
#include <vector>
#include <memory>
#include "..\state\State.hpp"
#include "..\rendering\RenderStates.hpp"


class RenderTarget2D;

namespace GUI
{

class Container : public Component
{
    public:
        typedef std::shared_ptr<Container> Ptr;
            

	public:
		Container();

        void				pack(Component::Ptr component);
		void				clear();

        virtual bool		handleEvent(const Event& event);
		virtual void		update();

    private:
		virtual void		draw(RenderTarget2D& target, RenderStates states) const;

  
    private:
        std::vector<Component::Ptr>		mChildren;
};

}