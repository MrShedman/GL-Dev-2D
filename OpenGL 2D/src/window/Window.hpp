#pragma once

#include "ContextSettings.hpp"
#include "VideoMode.hpp"
#include "WindowStyle.hpp"
#include "Math\Vector.h"
#include "Clock.h"
#include "NonCopyable.hpp"
#include "Config.h"
#include "OpenGL.hpp"
#include "..\rendering\Color.h"

namespace priv
{
	class WindowImplWin32;
}

class GlContext;
class Event;

class Window : NonCopyable
{
public:

    Window();

    Window(VideoMode mode, const std::string& title, Uint32 style = Style::Default, const ContextSettings& settings = ContextSettings());

    ~Window();

    void create(VideoMode mode, const std::string& title, Uint32 style = Style::Default, const ContextSettings& settings = ContextSettings());

    void close();

    bool isOpen() const;

    const ContextSettings& getSettings() const;

    bool pollEvent(Event& event);

    bool waitEvent(Event& event);

    Vector2i getPosition() const;

    void setPosition(const Vector2i& position);

    Vector2i getSize() const;

	Vector2i getCentre() const;

	float getAspect() const;

    void setSize(const Vector2i& size);

    void setTitle(const std::string& title);

    void setIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    void setVisible(bool visible);

    void setVerticalSyncEnabled(bool enabled);

    void setMouseCursorVisible(bool visible);

    void setKeyRepeatEnabled(bool enabled);

    void setFramerateLimit(unsigned int limit);

	void clear(Color color = Color::Black);

    void display();

	bool switchToFullscreen(const VideoMode& mode);

	bool switchToWindowed(const VideoMode& mode, Uint32 style, bool useLastSizeAndPosition);

	bool isFullscreen() const;

	void setRawMouseInput(bool flag);

	void setClippedCursor(bool clipped);

	void setDragDropAllowed(bool flag);

	bool isDragDropAllowed() const;

    WindowHandle getSystemHandle() const;

private:

    bool filterEvent(const Event& event);

    void initialize();

	priv::WindowImplWin32*	m_impl;           ///< Platform-specific implementation of the window
	GlContext*				m_context;        ///< Platform-specific implementation of the OpenGL context
    Clock					m_clock;          ///< Clock for measuring the elapsed time between frames
    Time					m_frameTimeLimit; ///< Current framerate limit
    Vector2i				m_size;           ///< Current size of the window
};