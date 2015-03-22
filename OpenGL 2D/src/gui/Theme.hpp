#pragma once

#include "..\rendering\Color.h"

namespace GUI
{
	static Color pressedColor = Color::rgb(255, 177, 0);
	static Color hoverColor = Color::rgb(181, 230, 29);
	static Color normalColor = Color::rgb(31, 177, 76);

	class Theme
	{
	public:
		Theme()
			:
			Theme(pressedColor, hoverColor, normalColor)
		{
		}

		Theme(Color pressed, Color hover, Color normal)
			:
			pressed(pressed),
			hover(hover),
			normal(normal)
		{
		}

		void setTheme(Color pressed, Color hover, Color normal)
		{
			this->pressed = pressed;
			this->hover = hover;
			this->normal = normal;
		}

		Color getPressed() const
		{
			return pressed;
		}

		Color getHover() const
		{
			return hover;
		}

		Color getNormal() const
		{
			return normal;
		}

	private:

		Color pressed;//Color(255, 177, 0);
		Color hover;//Color(181, 230, 29);
		Color normal;//Color(31, 177, 76);
	};

}