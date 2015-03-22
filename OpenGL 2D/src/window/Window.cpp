
#include "Window.hpp"
#include "GlContext.hpp"
#include "WindowImplWin32.hpp"
#include "..\System\Utilities.h"

namespace
{
    const Window* fullscreenWindow = NULL;
}

Window::Window() :
m_impl          (NULL),
m_context       (NULL),
m_frameTimeLimit(Time::Zero),
m_size          (0, 0)
{

}

Window::Window(VideoMode mode, const std::string& title, Uint32 style, const ContextSettings& settings) :
m_impl          (NULL),
m_context       (NULL),
m_frameTimeLimit(Time::Zero),
m_size          (0, 0)
{
    create(mode, title, style, settings);
}

Window::~Window()
{
    close();

	delete m_context;
	m_context = NULL;
}

void Window::create(VideoMode mode, const std::string& title, Uint32 style, const ContextSettings& settings)
{
    // Destroy the previous window implementation
    close();

    // Fullscreen style requires some tests
    if (style & Style::Fullscreen)
    {
        // Make sure there's not already a fullscreen window (only one is allowed)
        if (fullscreenWindow)
        {
            style &= ~Style::Fullscreen;
        }
        else
        {
            // Make sure that the chosen video mode is compatible
            if (!mode.isValid())
            {
                mode = VideoMode::getFullscreenModes()[0];
            }

            // Update the fullscreen window
            fullscreenWindow = this;
        }
    }

    // Check validity of style
    if ((style & Style::Close) || (style & Style::Resize))
        style |= Style::Titlebar;

    // Recreate the window implementation
	m_impl = priv::WindowImplWin32::create(mode, title, style, settings);

    // Recreate the context
    m_context = new GlContext(settings, m_impl, mode.bitsPerPixel);

    // Perform common initializations
    initialize();
}

void Window::close()
{
    if (m_context)
    {
        // Delete the context
        //delete m_context;
       // m_context = NULL;
    }

    if (m_impl)
    {
        // Delete the window implementation
        delete m_impl;
        m_impl = NULL;
    }

    // Update the fullscreen window
    if (this == fullscreenWindow)
        fullscreenWindow = NULL;
}

bool Window::isOpen() const
{
    return m_impl != NULL;
}

const ContextSettings& Window::getSettings() const
{
    static const ContextSettings empty(0, 0, 0);

    return m_context ? m_context->getSettings() : empty;
}

bool Window::pollEvent(Event& event)
{
    if (m_impl && m_impl->popEvent(event, false))
    {
        return filterEvent(event);
    }
    else
    {
        return false;
    }
}

bool Window::waitEvent(Event& event)
{
    if (m_impl && m_impl->popEvent(event, true))
    {
        return filterEvent(event);
    }
    else
    {
        return false;
    }
}

Vector2i Window::getPosition() const
{
    return m_impl ? m_impl->getPosition() : Vector2i();
}

void Window::setPosition(const Vector2i& position)
{
    if (m_impl)
        m_impl->setPosition(position);
}

Vector2i Window::getSize() const
{
	if (m_impl)
		return m_impl->getSize();

	return m_size;  
}

Vector2i Window::getCentre() const
{
	return m_size / 2;
}

float Window::getAspect() const
{
	return static_cast<float>(m_size.x) / static_cast<float>(m_size.y);
}

void Window::setSize(const Vector2i& size)
{
    if (m_impl)
    {
        m_impl->setSize(size);

        // Cache the new size
		m_size = size;
    }
}

void Window::setTitle(const std::string& title)
{
    if (m_impl)
        m_impl->setTitle(title);
}

void Window::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    if (m_impl)
        m_impl->setIcon(width, height, pixels);
}

void Window::setVisible(bool visible)
{
    if (m_impl)
        m_impl->setVisible(visible);
}

void Window::setVerticalSyncEnabled(bool enabled)
{
	m_context->setVerticalSyncEnabled(enabled);
}

void Window::setMouseCursorVisible(bool visible)
{
    if (m_impl)
        m_impl->setMouseCursorVisible(visible);
}

void Window::setKeyRepeatEnabled(bool enabled)
{
    if (m_impl)
        m_impl->setKeyRepeatEnabled(enabled);
}

void Window::setFramerateLimit(unsigned int limit)
{
    if (limit > 0)
        m_frameTimeLimit = seconds(1.f / limit);
    else
        m_frameTimeLimit = Time::Zero;
}

void Window::setRawMouseInput(bool flag)
{
	if (m_impl)
		m_impl->setRawMouseInput(flag);
}

void Window::setClippedCursor(bool clipped)
{
	if (m_impl)
		m_impl->setClippedCursor(clipped);
}

void Window::setDragDropAllowed(bool flag)
{
	if (m_impl)
		m_impl->setDragDropAllowed(flag);
}

bool Window::isDragDropAllowed() const
{
	if (m_impl)
		return m_impl->isDragDropAllowed();
	else
		return false;
}


void Window::clear(Color color)
{
	float r = static_cast<float>(color.r) / 255.0f;
	float g = static_cast<float>(color.g) / 255.0f;
	float b = static_cast<float>(color.b) / 255.0f;
	float a = static_cast<float>(color.a) / 255.0f;

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display()
{
    // Display the backbuffer on screen
	m_context->display();

    // Limit the framerate if needed
    if (m_frameTimeLimit != Time::Zero)
    {
		Util::sleep(m_frameTimeLimit - m_clock.getElapsedTime());
		m_clock.restart();
    }
}

bool Window::switchToFullscreen(const VideoMode& mode)
{
	if (m_impl)
		return m_impl->switchToFullscreen(mode);
	else
		return false;
}

bool Window::switchToWindowed(const VideoMode& mode, Uint32 style, bool useLastSizeAndPosition)
{
	if (m_impl)
		return m_impl->switchToWindowed(mode, style, useLastSizeAndPosition);
	else
		return false;
}

bool Window::isFullscreen() const
{
	if (m_impl)
		return m_impl->isFullscreen();
	else
		return false;
}

WindowHandle Window::getSystemHandle() const
{
    return m_impl ? m_impl->getSystemHandle() : 0;
}

HGLRC Window::getGLDeviceContext() const
{
	return m_context ? m_context->getGLContext() : 0;
}

bool Window::filterEvent(const Event& event)
{
    // Notify resize events to the derived class
    if (event.type == Event::Resized)
    {
        // Cache the new size
        m_size.x = event.size.width;
        m_size.y = event.size.height;
    }

    return true;
}

void Window::initialize()
{
    // Setup default behaviours (to get a consistent behaviour across different implementations)
    setVisible(true);
    setMouseCursorVisible(true);
    setVerticalSyncEnabled(false);
    setKeyRepeatEnabled(true);

    // Get and cache the initial size of the window
    m_size = m_impl->getSize();

    // Reset frame time
    m_clock.restart();
}