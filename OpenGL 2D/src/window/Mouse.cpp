#include "Mouse.h"

bool Mouse::isButtonPressed(Button button)
{
	int vkey = 0;
	switch (button)
	{
	case Mouse::Left:     vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON; break;
	case Mouse::Right:    vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON; break;
	case Mouse::Middle:   vkey = VK_MBUTTON;  break;
	case Mouse::XButton1: vkey = VK_XBUTTON1; break;
	case Mouse::XButton2: vkey = VK_XBUTTON2; break;
	default:              vkey = 0;           break;
	}

	return (GetAsyncKeyState(vkey) & 0x8000) != 0;
}

Vector2i Mouse::getPosition()
{
	POINT point;
	GetCursorPos(&point);
	return Vector2i(point.x, point.y);
}

Vector2i Mouse::getPosition(const Window& relativeTo)
{
	WindowHandle handle = relativeTo.getSystemHandle();
	if (handle)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(handle, &point);
		return Vector2i(point.x, point.y);
	}
	else
	{
		return Vector2i();
	}
}

void Mouse::setPosition(const Vector2i& position)
{
	SetCursorPos(position.x, position.y);
}

void Mouse::setPosition(const Vector2i& position, const Window& relativeTo)
{
	WindowHandle handle = relativeTo.getSystemHandle();
	if (handle)
	{
		POINT point = { position.x, position.y };
		ClientToScreen(handle, &point);
		SetCursorPos(point.x, point.y);
	}
}