#include "stdafx.h"

#include "Keyboard.h"

namespace DentyEngine
{
	Keyboard::Keyboard() :
		_keys(), _prevKeys()
	{
	}

	void Keyboard::Update()
	{
		_prevKeys = _keys;

		if (GetKeyboardState(_keys.data()) == 0)
		{
			// Failed
			const DWORD errorCode = GetLastError();

			DENTY_SET_FATAL_COLOR
			DENTY_FATAL_LOG << "ErrorCode: " << errorCode;

			DENTY_ASSERT(false, "Failed to get keyboard state! (in Keyboard::Update)");
		}
	}

	void Keyboard::OnGui()
	{
		ImGui::Text("PressedAKey: %s", IsPressed(KeyCode::A) ? "true" : "false");
		ImGui::Text("TriggeredAKey: %s", IsTrigger(KeyCode::A) ? "true" : "false");
		ImGui::Text("ReleasedAKey: %s", IsReleased(KeyCode::A) ? "true" : "false");
	}

	bool Keyboard::IsPressed(KeyCode button) const
	{
		const uint32_t virtualKey = GetVirtualKey(button);

		if (virtualKey == UINT_MAX) return false;

		return _keys[virtualKey] & 0x80u;
	}

	bool Keyboard::IsTrigger(KeyCode button) const
	{
		const uint32_t virtualKey = GetVirtualKey(button);

		if (virtualKey == UINT_MAX) return false;

		return (~(_prevKeys[virtualKey]) & (_keys[virtualKey]));
	}

	bool Keyboard::IsReleased(KeyCode button) const
	{
		const uint32_t virtualKey = GetVirtualKey(button);

		if (virtualKey == UINT_MAX) return false;

		return ((_prevKeys[virtualKey] & 0x80u) && !(_keys[virtualKey] & 0x80u));
	}

	uint32_t Keyboard::GetVirtualKey(KeyCode key) const
	{
		switch (key)
		{
			case KeyCode::A: return 'A';
			case KeyCode::B: return 'B';
			case KeyCode::C: return 'C';
			case KeyCode::D: return 'D';
			case KeyCode::E: return 'E';
			case KeyCode::F: return 'F';
			case KeyCode::G: return 'G';
			case KeyCode::H: return 'H';
			case KeyCode::I: return 'I';
			case KeyCode::J: return 'J';
			case KeyCode::K: return 'K';
			case KeyCode::L: return 'L';
			case KeyCode::N: return 'N';
			case KeyCode::M: return 'M';
			case KeyCode::O: return 'O';
			case KeyCode::P: return 'P';
			case KeyCode::Q: return 'Q';
			case KeyCode::R: return 'R';
			case KeyCode::S: return 'S';
			case KeyCode::T: return 'T';
			case KeyCode::U: return 'U';
			case KeyCode::V: return 'V';
			case KeyCode::W: return 'W';
			case KeyCode::X: return 'X';
			case KeyCode::Y: return 'Y';
			case KeyCode::Z: return 'Z';
			case KeyCode::ArrowLeft: return VK_LEFT;
			case KeyCode::ArrowUp: return VK_UP;
			case KeyCode::ArrowDown: return VK_DOWN;
			case KeyCode::ArrowRight: return VK_RIGHT;
			case KeyCode::F1: return VK_F1;
			case KeyCode::F2: return VK_F2;
			case KeyCode::F3: return VK_F3;
			case KeyCode::F4: return VK_F4;
			case KeyCode::F5: return VK_F5;
			case KeyCode::F6: return VK_F6;
			case KeyCode::F7: return VK_F7;
			case KeyCode::F8: return VK_F8;
			case KeyCode::F9: return VK_F9;
			case KeyCode::F10: return VK_F10;
			case KeyCode::F11: return VK_F11;
			case KeyCode::F12: return VK_F12;
			case KeyCode::Tab: return VK_TAB;
			case KeyCode::Space: return VK_SPACE;
			case KeyCode::LCtrl: return VK_LCONTROL;
			case KeyCode::RCtrl: return VK_RCONTROL;
			case KeyCode::LShift: return VK_SHIFT;
			case KeyCode::RShift: return VK_RSHIFT;
			case KeyCode::Escape: return VK_ESCAPE;
			case KeyCode::BackSpace: return VK_BACK;
			case KeyCode::Return: return VK_RETURN;
			case KeyCode::Alt: return VK_MENU;
		}

		return UINT32_MAX;
	}
}
