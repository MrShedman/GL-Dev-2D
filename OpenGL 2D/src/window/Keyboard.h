#pragma once

#include <windows.h>

class Keyboard
{
public:

	enum Key
	{
		Unknown = -1, ///< Unhandled key
		A = 0,        ///< The A key
		B,            ///< The B key
		C,            ///< The C key
		D,            ///< The D key
		E,            ///< The E key
		F,            ///< The F key
		G,            ///< The G key
		H,            ///< The H key
		I,            ///< The I key
		J,            ///< The J key
		K,            ///< The K key
		L,            ///< The L key
		M,            ///< The M key
		N,            ///< The N key
		O,            ///< The O key
		P,            ///< The P key
		Q,            ///< The Q key
		R,            ///< The R key
		S,            ///< The S key
		T,            ///< The T key
		U,            ///< The U key
		V,            ///< The V key
		W,            ///< The W key
		X,            ///< The X key
		Y,            ///< The Y key
		Z,            ///< The Z key
		Num0,         ///< The 0 key
		Num1,         ///< The 1 key
		Num2,         ///< The 2 key
		Num3,         ///< The 3 key
		Num4,         ///< The 4 key
		Num5,         ///< The 5 key
		Num6,         ///< The 6 key
		Num7,         ///< The 7 key
		Num8,         ///< The 8 key
		Num9,         ///< The 9 key
		Escape,       ///< The Escape key
		LControl,     ///< The left Control key
		LShift,       ///< The left Shift key
		LAlt,         ///< The left Alt key
		LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
		RControl,     ///< The right Control key
		RShift,       ///< The right Shift key
		RAlt,         ///< The right Alt key
		RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
		Menu,         ///< The Menu key
		LBracket,     ///< The [ key
		RBracket,     ///< The ] key
		SemiColon,    ///< The ; key
		Comma,        ///< The , key
		Period,       ///< The . key
		Quote,        ///< The ' key
		Slash,        ///< The / key
		BackSlash,    ///< The \ key
		Tilde,        ///< The ~ key
		Equal,        ///< The = key
		Dash,         ///< The - key
		Space,        ///< The Space key
		Return,       ///< The Return key
		BackSpace,    ///< The Backspace key
		Tab,          ///< The Tabulation key
		PageUp,       ///< The Page up key
		PageDown,     ///< The Page down key
		End,          ///< The End key
		Home,         ///< The Home key
		Insert,       ///< The Insert key
		Delete,       ///< The Delete key
		Add,          ///< The + key
		Subtract,     ///< The - key
		Multiply,     ///< The * key
		Divide,       ///< The / key
		Left,         ///< Left arrow
		Right,        ///< Right arrow
		Up,           ///< Up arrow
		Down,         ///< Down arrow
		Numpad0,      ///< The numpad 0 key
		Numpad1,      ///< The numpad 1 key
		Numpad2,      ///< The numpad 2 key
		Numpad3,      ///< The numpad 3 key
		Numpad4,      ///< The numpad 4 key
		Numpad5,      ///< The numpad 5 key
		Numpad6,      ///< The numpad 6 key
		Numpad7,      ///< The numpad 7 key
		Numpad8,      ///< The numpad 8 key
		Numpad9,      ///< The numpad 9 key
		F1,           ///< The F1 key
		F2,           ///< The F2 key
		F3,           ///< The F3 key
		F4,           ///< The F4 key
		F5,           ///< The F5 key
		F6,           ///< The F6 key
		F7,           ///< The F7 key
		F8,           ///< The F8 key
		F9,           ///< The F9 key
		F10,          ///< The F10 key
		F11,          ///< The F11 key
		F12,          ///< The F12 key
		F13,          ///< The F13 key
		F14,          ///< The F14 key
		F15,          ///< The F15 key
		Pause,        ///< The Pause key

		KeyCount      ///< Keep last -- the total number of keyboard keys
	};

	static bool isKeyPressed(Keyboard::Key key)
	{
		int vkey = 0;
		switch (key)
		{
		default:                   vkey = 0;             break;
		case Keyboard::A:          vkey = 'A';           break;
		case Keyboard::B:          vkey = 'B';           break;
		case Keyboard::C:          vkey = 'C';           break;
		case Keyboard::D:          vkey = 'D';           break;
		case Keyboard::E:          vkey = 'E';           break;
		case Keyboard::F:          vkey = 'F';           break;
		case Keyboard::G:          vkey = 'G';           break;
		case Keyboard::H:          vkey = 'H';           break;
		case Keyboard::I:          vkey = 'I';           break;
		case Keyboard::J:          vkey = 'J';           break;
		case Keyboard::K:          vkey = 'K';           break;
		case Keyboard::L:          vkey = 'L';           break;
		case Keyboard::M:          vkey = 'M';           break;
		case Keyboard::N:          vkey = 'N';           break;
		case Keyboard::O:          vkey = 'O';           break;
		case Keyboard::P:          vkey = 'P';           break;
		case Keyboard::Q:          vkey = 'Q';           break;
		case Keyboard::R:          vkey = 'R';           break;
		case Keyboard::S:          vkey = 'S';           break;
		case Keyboard::T:          vkey = 'T';           break;
		case Keyboard::U:          vkey = 'U';           break;
		case Keyboard::V:          vkey = 'V';           break;
		case Keyboard::W:          vkey = 'W';           break;
		case Keyboard::X:          vkey = 'X';           break;
		case Keyboard::Y:          vkey = 'Y';           break;
		case Keyboard::Z:          vkey = 'Z';           break;
		case Keyboard::Num0:       vkey = '0';           break;
		case Keyboard::Num1:       vkey = '1';           break;
		case Keyboard::Num2:       vkey = '2';           break;
		case Keyboard::Num3:       vkey = '3';           break;
		case Keyboard::Num4:       vkey = '4';           break;
		case Keyboard::Num5:       vkey = '5';           break;
		case Keyboard::Num6:       vkey = '6';           break;
		case Keyboard::Num7:       vkey = '7';           break;
		case Keyboard::Num8:       vkey = '8';           break;
		case Keyboard::Num9:       vkey = '9';           break;
		case Keyboard::Escape:     vkey = VK_ESCAPE;     break;
		case Keyboard::LControl:   vkey = VK_LCONTROL;   break;
		case Keyboard::LShift:     vkey = VK_LSHIFT;     break;
		case Keyboard::LAlt:       vkey = VK_LMENU;      break;
		case Keyboard::LSystem:    vkey = VK_LWIN;       break;
		case Keyboard::RControl:   vkey = VK_RCONTROL;   break;
		case Keyboard::RShift:     vkey = VK_RSHIFT;     break;
		case Keyboard::RAlt:       vkey = VK_RMENU;      break;
		case Keyboard::RSystem:    vkey = VK_RWIN;       break;
		case Keyboard::Menu:       vkey = VK_APPS;       break;
		case Keyboard::LBracket:   vkey = VK_OEM_4;      break;
		case Keyboard::RBracket:   vkey = VK_OEM_6;      break;
		case Keyboard::SemiColon:  vkey = VK_OEM_1;      break;
		case Keyboard::Comma:      vkey = VK_OEM_COMMA;  break;
		case Keyboard::Period:     vkey = VK_OEM_PERIOD; break;
		case Keyboard::Quote:      vkey = VK_OEM_7;      break;
		case Keyboard::Slash:      vkey = VK_OEM_2;      break;
		case Keyboard::BackSlash:  vkey = VK_OEM_5;      break;
		case Keyboard::Tilde:      vkey = VK_OEM_3;      break;
		case Keyboard::Equal:      vkey = VK_OEM_PLUS;   break;
		case Keyboard::Dash:       vkey = VK_OEM_MINUS;  break;
		case Keyboard::Space:      vkey = VK_SPACE;      break;
		case Keyboard::Return:     vkey = VK_RETURN;     break;
		case Keyboard::BackSpace:  vkey = VK_BACK;       break;
		case Keyboard::Tab:        vkey = VK_TAB;        break;
		case Keyboard::PageUp:     vkey = VK_PRIOR;      break;
		case Keyboard::PageDown:   vkey = VK_NEXT;       break;
		case Keyboard::End:        vkey = VK_END;        break;
		case Keyboard::Home:       vkey = VK_HOME;       break;
		case Keyboard::Insert:     vkey = VK_INSERT;     break;
		case Keyboard::Delete:     vkey = VK_DELETE;     break;
		case Keyboard::Add:        vkey = VK_ADD;        break;
		case Keyboard::Subtract:   vkey = VK_SUBTRACT;   break;
		case Keyboard::Multiply:   vkey = VK_MULTIPLY;   break;
		case Keyboard::Divide:     vkey = VK_DIVIDE;     break;
		case Keyboard::Left:       vkey = VK_LEFT;       break;
		case Keyboard::Right:      vkey = VK_RIGHT;      break;
		case Keyboard::Up:         vkey = VK_UP;         break;
		case Keyboard::Down:       vkey = VK_DOWN;       break;
		case Keyboard::Numpad0:    vkey = VK_NUMPAD0;    break;
		case Keyboard::Numpad1:    vkey = VK_NUMPAD1;    break;
		case Keyboard::Numpad2:    vkey = VK_NUMPAD2;    break;
		case Keyboard::Numpad3:    vkey = VK_NUMPAD3;    break;
		case Keyboard::Numpad4:    vkey = VK_NUMPAD4;    break;
		case Keyboard::Numpad5:    vkey = VK_NUMPAD5;    break;
		case Keyboard::Numpad6:    vkey = VK_NUMPAD6;    break;
		case Keyboard::Numpad7:    vkey = VK_NUMPAD7;    break;
		case Keyboard::Numpad8:    vkey = VK_NUMPAD8;    break;
		case Keyboard::Numpad9:    vkey = VK_NUMPAD9;    break;
		case Keyboard::F1:         vkey = VK_F1;         break;
		case Keyboard::F2:         vkey = VK_F2;         break;
		case Keyboard::F3:         vkey = VK_F3;         break;
		case Keyboard::F4:         vkey = VK_F4;         break;
		case Keyboard::F5:         vkey = VK_F5;         break;
		case Keyboard::F6:         vkey = VK_F6;         break;
		case Keyboard::F7:         vkey = VK_F7;         break;
		case Keyboard::F8:         vkey = VK_F8;         break;
		case Keyboard::F9:         vkey = VK_F9;         break;
		case Keyboard::F10:        vkey = VK_F10;        break;
		case Keyboard::F11:        vkey = VK_F11;        break;
		case Keyboard::F12:        vkey = VK_F12;        break;
		case Keyboard::F13:        vkey = VK_F13;        break;
		case Keyboard::F14:        vkey = VK_F14;        break;
		case Keyboard::F15:        vkey = VK_F15;        break;
		case Keyboard::Pause:      vkey = VK_PAUSE;      break;
		}

		return (GetAsyncKeyState(vkey) & 0x8000) != 0;
	}
};