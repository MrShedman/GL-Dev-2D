
#include "GlContext.hpp"
#include "GL\glew.h"
#include <iostream>

namespace
{
	int evaluateFormat(unsigned int bitsPerPixel, const ContextSettings& settings, int colorBits, int depthBits, int stencilBits, int antialiasing)
	{
		return std::abs(static_cast<int>(bitsPerPixel - colorBits)) +
			std::abs(static_cast<int>(settings.depthBits - depthBits)) +
			std::abs(static_cast<int>(settings.stencilBits - stencilBits));
	}

	bool initGlew()
	{
		glewExperimental = GL_TRUE;

		GLenum res = glewInit();
		if (res != GLEW_OK)
		{
			std::cout << glewGetErrorString(res) << std::endl;
			return false;
		}

		return true;
	}

	static GlContext dummy;
}

GlContext::GlContext()
:
m_window(NULL),
m_deviceContext(NULL),
m_context(NULL),
m_ownsWindow(true)
{
	m_window = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(m_window, SW_HIDE);
	m_deviceContext = GetDC(m_window);

	if (m_deviceContext)
	{
		createContext(VideoMode::getDesktopMode().bitsPerPixel, ContextSettings());
	}

	makeCurrent();

	//now that we have a dummy context we can initialize glew
	initGlew();
}

GlContext::GlContext(const ContextSettings& settings, const priv::WindowImplWin32* owner, unsigned int bitsPerPixel)
:
m_window(NULL),
m_deviceContext(NULL),
m_context(NULL),
m_ownsWindow(false)
{
	// Get the owner window and its device context
	m_window = owner->getSystemHandle();
	m_deviceContext = GetDC(m_window);

	// Create the context
	if (m_deviceContext)
	{
		createContext(bitsPerPixel, settings);
	}

	makeCurrent();
}

GlContext::~GlContext()
{
	// Destroy the OpenGL context
	if (m_context)
	{
		if (wglGetCurrentContext() == m_context)
		{
			wglMakeCurrent(NULL, NULL);
		}

		std::cout << "Destroying Context" << std::endl;
		wglDeleteContext(m_context);
	}

	// Destroy the device context
	if (m_deviceContext)
	{
		ReleaseDC(m_window, m_deviceContext);
	}

	// Destroy the window if we own it
	if (m_window && m_ownsWindow)
	{
		DestroyWindow(m_window);
	}
}

bool GlContext::makeCurrent()
{
	return m_deviceContext && m_context && wglMakeCurrent(m_deviceContext, m_context);
}

void GlContext::display()
{
	if (m_deviceContext && m_context)
	{
		SwapBuffers(m_deviceContext);
	}
}

HGLRC GlContext::getGLContext() const
{
	return m_context;
}

const ContextSettings& GlContext::getSettings() const
{
	return m_settings;
}

void GlContext::setVerticalSyncEnabled(bool enabled)
{
	wglSwapIntervalEXT(enabled ? 1 : 0);
}

void GlContext::createContext(unsigned int bitsPerPixel, const ContextSettings& settings)
{
	// Save the creation settings
	m_settings = settings;

	// Setup a pixel format descriptor from the rendering settings
	PIXELFORMATDESCRIPTOR descriptor;
	ZeroMemory(&descriptor, sizeof(descriptor));
	descriptor.nSize = sizeof(descriptor);
	descriptor.nVersion = 1;
	descriptor.iLayerType = PFD_MAIN_PLANE;
	descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	descriptor.iPixelType = PFD_TYPE_RGBA;
	descriptor.cColorBits = static_cast<BYTE>(bitsPerPixel);
	descriptor.cDepthBits = static_cast<BYTE>(m_settings.depthBits);
	descriptor.cStencilBits = static_cast<BYTE>(m_settings.stencilBits);
	descriptor.cAlphaBits = bitsPerPixel == 32 ? 8 : 0;

	// Get the pixel format that best matches our requirements
	int bestFormat = ChoosePixelFormat(m_deviceContext, &descriptor);
	if (bestFormat == 0)
	{
		return;
	}	

	// Extract the depth and stencil bits from the chosen format
	PIXELFORMATDESCRIPTOR actualFormat;
	actualFormat.nSize = sizeof(actualFormat);
	actualFormat.nVersion = 1;
	DescribePixelFormat(m_deviceContext, bestFormat, sizeof(actualFormat), &actualFormat);
	m_settings.depthBits = actualFormat.cDepthBits;
	m_settings.stencilBits = actualFormat.cStencilBits;

	// Set the chosen pixel format
	if (!SetPixelFormat(m_deviceContext, bestFormat, &actualFormat))
	{
		return;
	}

	// Create the OpenGL context -- first try context versions >= 3.0 if it is requested (they require special code)
	while (!m_context && (m_settings.majorVersion >= 3))
	{
		int profile = settings.core == true ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;

		int attributes[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>(m_settings.majorVersion),
			WGL_CONTEXT_MINOR_VERSION_ARB, static_cast<int>(m_settings.minorVersion),
			WGL_CONTEXT_PROFILE_MASK_ARB, profile,
			0, 0
		};

		m_context = wglCreateContextAttribsARB(m_deviceContext, NULL, attributes);

		// If we couldn't create the context, lower the version number and try again -- stop at 3.0
		// Invalid version numbers will be generated by this algorithm (like 3.9), but we really don't care
		if (!m_context)
		{
			if (m_settings.minorVersion > 0)
			{
				// If the minor version is not 0, we decrease it and try again
				m_settings.minorVersion--;
			}
			else
			{
				// If the minor version is 0, we decrease the major version
				m_settings.majorVersion--;
				m_settings.minorVersion = 9;
			}
		}
	}

	// If the OpenGL >= 3.0 context failed or if we don't want one, create a regular OpenGL 1.x/2.x context
	if (!m_context)
	{
		// set the context version to 2.0 (arbitrary)
		m_settings.majorVersion = 2;
		m_settings.minorVersion = 0;

		m_context = wglCreateContext(m_deviceContext);
		if (!m_context)
		{
			std::cout << "Failed to create suitable OpenGL Context!" << std::endl;
			return;
		}
	}
}
