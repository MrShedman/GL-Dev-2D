#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include <array>

class Event
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Size events parameters (Resized)
	///
	////////////////////////////////////////////////////////////
	struct SizeEvent
	{
		unsigned int width;  ///< New width, in pixels
		unsigned int height; ///< New height, in pixels
	};

	////////////////////////////////////////////////////////////
	/// \brief Keyboard event parameters (KeyPressed, KeyReleased)
	///
	////////////////////////////////////////////////////////////
	struct KeyEvent
	{
		Keyboard::Key code;    ///< Code of the key that has been pressed
		bool          alt;     ///< Is the Alt key pressed?
		bool          control; ///< Is the Control key pressed?
		bool          shift;   ///< Is the Shift key pressed?
		bool          system;  ///< Is the System key pressed?
	};

	////////////////////////////////////////////////////////////
	/// \brief Text event parameters (TextEntered)
	///
	////////////////////////////////////////////////////////////
	struct TextEvent
	{
		unsigned int unicode; ///< UTF-32 unicode value of the character
	};

	////////////////////////////////////////////////////////////
	/// \brief Mouse move event parameters (MouseMoved)
	///
	////////////////////////////////////////////////////////////
	struct MouseMoveEvent
	{
		int x; ///< X position of the mouse pointer, relative to the left of the owner window
		int y; ///< Y position of the mouse pointer, relative to the top of the owner window
	};

	////////////////////////////////////////////////////////////
	/// \brief Raw mouse movement event parameters (MouseMotion)
	///
	////////////////////////////////////////////////////////////
	struct MouseMotionEvent
	{
		int dx; ///< X delta of the mouse pointer, relative to its last position
		int dy; ///< Y delta of the mouse pointer, relative to its last position
	};

	////////////////////////////////////////////////////////////
	/// \brief Mouse buttons events parameters
	///        (MouseButtonPressed, MouseButtonReleased)
	///
	////////////////////////////////////////////////////////////
	struct MouseButtonEvent
	{
		Mouse::Button button; ///< Code of the button that has been pressed
		int           x;      ///< X position of the mouse pointer, relative to the left of the owner window
		int           y;      ///< Y position of the mouse pointer, relative to the top of the owner window
	};

	////////////////////////////////////////////////////////////
	/// \brief Mouse wheel events parameters (MouseWheelMoved)
	///
	////////////////////////////////////////////////////////////
	struct MouseWheelEvent
	{
		int delta; ///< Number of ticks the wheel has moved (positive is up, negative is down)
		int x;     ///< X position of the mouse pointer, relative to the left of the owner window
		int y;     ///< Y position of the mouse pointer, relative to the top of the owner window
	};

	struct FileDropEvent
	{
		int x;
		int y;
		int fileCount;
		std::string* files;
	};

	////////////////////////////////////////////////////////////
	/// \brief Enumeration of the different types of events
	///
	////////////////////////////////////////////////////////////
	enum EventType
	{
		Closed,                 ///< The window requested to be closed (no data)
		Resized,                ///< The window was resized (data in event.size)
		LostFocus,              ///< The window lost the focus (no data)
		GainedFocus,            ///< The window gained the focus (no data)
		TextEntered,            ///< A character was entered (data in event.text)
		KeyPressed,             ///< A key was pressed (data in event.key)
		KeyReleased,            ///< A key was released (data in event.key)
		MouseWheelMoved,        ///< The mouse wheel was scrolled (data in event.mouseWheel)
		MouseButtonPressed,     ///< A mouse button was pressed (data in event.mouseButton)
		MouseButtonReleased,    ///< A mouse button was released (data in event.mouseButton)
		MouseMoved,             ///< The mouse cursor moved (data in event.mouseMove)
		MouseMotion,			///< The mouse cursor moved (data in event.mouseMove)
		MouseEntered,           ///< The mouse cursor entered the area of the window (no data)
		MouseLeft,              ///< The mouse cursor left the area of the window (no data)
		FileDrop,

		Count                   ///< Keep last -- the total number of event types
	};

	////////////////////////////////////////////////////////////
	// Member data
	////////////////////////////////////////////////////////////
	EventType type; ///< Type of the event

	union
	{
		SizeEvent            size;            ///< Size event parameters (Event::Resized)
		KeyEvent             key;             ///< Key event parameters (Event::KeyPressed, Event::KeyReleased)
		TextEvent            text;            ///< Text event parameters (Event::TextEntered)
		MouseMoveEvent       mouseMove;       ///< Mouse move event parameters (Event::MouseMoved)
		MouseMotionEvent     mouseMotion;
		MouseButtonEvent     mouseButton;     ///< Mouse button event parameters (Event::MouseButtonPressed, Event::MouseButtonReleased)
		MouseWheelEvent      mouseWheel;      ///< Mouse wheel event parameters (Event::MouseWheelMoved)
		FileDropEvent		 fileDrop;
	};
};