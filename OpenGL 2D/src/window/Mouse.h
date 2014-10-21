#pragma once

#include <Windows.h>

#include "Window.hpp"

class Mouse
{
public:

	enum Button
	{
		Left,       ///< The left mouse button
		Right,      ///< The right mouse button
		Middle,     ///< The middle (wheel) mouse button
		XButton1,   ///< The first extra mouse button
		XButton2,   ///< The second extra mouse button

		ButtonCount ///< Keep last -- the total number of mouse buttons
	};

	static bool isButtonPressed(Button button);

	static Vector2i getPosition();

	static Vector2i getPosition(const Window& relativeTo);

	static void setPosition(const Vector2i& position);

	static void setPosition(const Vector2i& position, const Window& relativeTo);
};