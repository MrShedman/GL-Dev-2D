#pragma once

#include "WindowImplWin32.hpp"
#include "ContextSettings.hpp"
#include "NonCopyable.hpp"

class GlContext : private NonCopyable
{
public:

	GlContext();

	GlContext(const ContextSettings& settings, const priv::WindowImplWin32* owner, unsigned int bitsPerPixel);

	~GlContext();

	bool makeCurrent();

	void display();

	void setVerticalSyncEnabled(bool enabled);

	const ContextSettings& getSettings() const;

private:

	void createContext(unsigned int bitsPerPixel, const ContextSettings& settings);

	ContextSettings m_settings;

	HWND  m_window;        ///< Window to which the context is attached
	HDC   m_deviceContext; ///< Device context associated to the context
	HGLRC m_context;       ///< OpenGL context
	bool  m_ownsWindow;    ///< Do we own the target window?
};