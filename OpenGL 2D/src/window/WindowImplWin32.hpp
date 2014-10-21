#pragma once

#include "Event.h"
#include <windows.h>
#include <queue>

namespace priv
{

class WindowImplWin32 : NonCopyable
{
public :

    WindowImplWin32(WindowHandle handle);
  
    WindowImplWin32(VideoMode mode, const std::string& title, Uint32 style, const ContextSettings& settings);

	static WindowImplWin32* create(VideoMode mode, const std::string& title, Uint32 style, const ContextSettings& settings);

	static WindowImplWin32* create(WindowHandle handle);

    ~WindowImplWin32();

    virtual WindowHandle getSystemHandle() const;

    virtual Vector2i getPosition() const;

    virtual void setPosition(const Vector2i& position);

    virtual Vector2i getSize() const;

    virtual void setSize(const Vector2i& size);

    virtual void setTitle(const std::string& title);

    virtual void setIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    virtual void setVisible(bool visible);

    virtual void setMouseCursorVisible(bool visible);

    virtual void setKeyRepeatEnabled(bool enabled);

	bool popEvent(Event& event, bool block);

	bool switchToFullscreen(const VideoMode& mode);

	bool switchToWindowed(const VideoMode& mode, Uint32 style, bool useLastSizeAndPosition);

	bool isFullscreen() const;

	void setRawMouseInput(bool flag);

	void setClippedCursor(bool clipped);

	void setDragDropAllowed(bool flag);

	bool isDragDropAllowed() const;

protected:

	void pushEvent(const Event& event);

    virtual void processEvents();

private:

    void registerWindowClass();

    void cleanup();

    void processEvent(UINT message, WPARAM wParam, LPARAM lParam);

    void setTracking(bool track);

	static Keyboard::Key WindowImplWin32::virtualKeyCodeToSF(WPARAM key, LPARAM flags);

    static LRESULT CALLBACK globalOnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:

	std::queue<Event> m_events;

    HWND     m_handle;           ///< Win32 handle of the window
    LONG_PTR m_callback;         ///< Stores the original event callback function of the control
    HCURSOR  m_cursor;           ///< The system cursor to display into the window
    HICON    m_icon;             ///< Custom icon assigned to the window
    bool     m_keyRepeatEnabled; ///< Automatic key-repeat state for keydown events
	Vector2i m_lastDesktopSize;
	Vector2i m_lastPosition;
	Vector2i m_lastSize;		 ///< The last handled size of the window
    bool     m_resizing;         ///< Is the window being resized?
    Uint16   m_surrogate;        ///< First half of the surrogate pair, in case we're receiving a Unicode character in two events
    bool     m_mouseInside;      ///< Mouse is inside the window?
	bool	 m_usingRawInput;
	bool	 m_cursorTrapped; 
	bool	 m_dragDropAllowed;
	RECT	 m_noClip;
};

}