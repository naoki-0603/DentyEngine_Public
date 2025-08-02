#include "stdafx.h"

#include "GamePad.h"

namespace DentyEngine
{
	// Ref https://github.com/walbourn/directx-sdk-samples/blob/main/XInput/SimpleController/SimpleController.cpp
	GamePad::GamePad() :
		_currentState(), _prevState(), _padNum(), _connected()
	{
	}

	void GamePad::Update()
	{
		_prevState = _currentState;

		{
			const DWORD result = XInputGetState(static_cast<DWORD>(_padNum), &_currentState);

			if (result == ERROR_SUCCESS)
			{
				// Connected.
				_connected = true;
			}
			else
			{
				// Not connected.
				_connected = false;
			}

			// Zero value if thumb sticks are within the dead zone. (copy from above link)
			// Left stick.
			if (_currentState.Gamepad.sThumbLX < +static_cast<SHORT>(INPUT_DEAD_ZONE) &&
				_currentState.Gamepad.sThumbLX > -static_cast<SHORT>(INPUT_DEAD_ZONE) &&
				_currentState.Gamepad.sThumbLY < +static_cast<SHORT>(INPUT_DEAD_ZONE) &&
				_currentState.Gamepad.sThumbLY > -static_cast<SHORT>(INPUT_DEAD_ZONE))
			{
				_currentState.Gamepad.sThumbLX = 0;
				_currentState.Gamepad.sThumbLY = 0;
			}

			// Right stick.
			if (_currentState.Gamepad.sThumbRX < +static_cast<SHORT>(INPUT_DEAD_ZONE) &&
				_currentState.Gamepad.sThumbRX > -static_cast<SHORT>(INPUT_DEAD_ZONE) &&
				_currentState.Gamepad.sThumbRY < +static_cast<SHORT>(INPUT_DEAD_ZONE) &&
				_currentState.Gamepad.sThumbRY > -static_cast<SHORT>(INPUT_DEAD_ZONE))
			{
				_currentState.Gamepad.sThumbRX = 0;
				_currentState.Gamepad.sThumbRY = 0;
			}
		}
	}

	void GamePad::OnGui()
	{
		ImGui::Text("Pressed A Button: %s", IsPressed(PadKeyCode::A) ? "true" : "false");
		ImGui::Text("Triggered A Button: %s", IsTrigger(PadKeyCode::A) ? "true" : "false");
		ImGui::Text("Released A Button: %s", IsReleased(PadKeyCode::A) ? "true" : "false");

		ImGui::Separator();

		ImGui::Text("NormalizedThumbLX : %f", GetNormalizedThumbLX());
		ImGui::Text("NormalizedThumbLY : %f", GetNormalizedThumbLY());
		ImGui::Text("NormalizedThumbRX : %f", GetNormalizedThumbRX());
		ImGui::Text("NormalizedThumbRY : %f", GetNormalizedThumbRY());
	}

	bool GamePad::IsPressed(PadKeyCode key) const
	{
		const uint32_t keyCode = GetBitMask(key);

		if (keyCode == UINT32_MAX) return false;

		return _currentState.Gamepad.wButtons & keyCode;
	}

	bool GamePad::IsTrigger(PadKeyCode key) const
	{
		const uint32_t keyCode = GetBitMask(key);

		if (keyCode == UINT32_MAX) return false;

		return (!(_prevState.Gamepad.wButtons & keyCode) && (_currentState.Gamepad.wButtons & keyCode));

	}

	bool GamePad::IsReleased(PadKeyCode key) const
	{
		const uint32_t keyCode = GetBitMask(key);

		if (keyCode == UINT32_MAX) return false;

		return ((_prevState.Gamepad.wButtons & keyCode) && !(_currentState.Gamepad.wButtons & keyCode));
	}

	bool GamePad::IsLeftTriggerTriggered() const
	{
		return ((_prevState.Gamepad.bLeftTrigger == 0) && (_currentState.Gamepad.bLeftTrigger > 0));
	}

	bool GamePad::IsRightTriggerTriggered() const
	{
		return ((_prevState.Gamepad.bRightTrigger == 0) && (_currentState.Gamepad.bRightTrigger > 0));
	}

	uint32_t GamePad::GetBitMask(PadKeyCode keyCode) const
	{
		switch (keyCode)
		{
			case PadKeyCode::DPadUp: return XINPUT_GAMEPAD_DPAD_UP;
			case PadKeyCode::DPadDown: return XINPUT_GAMEPAD_DPAD_DOWN;
			case PadKeyCode::DPadLeft: return XINPUT_GAMEPAD_DPAD_LEFT;
			case PadKeyCode::DPadRight: return XINPUT_GAMEPAD_DPAD_RIGHT;
			case PadKeyCode::Start: return XINPUT_GAMEPAD_START;
			case PadKeyCode::Back: return XINPUT_GAMEPAD_BACK;
			case PadKeyCode::LeftThumb: return XINPUT_GAMEPAD_LEFT_THUMB;
			case PadKeyCode::RightThumb: return XINPUT_GAMEPAD_RIGHT_THUMB;
			case PadKeyCode::LeftShoulder: return XINPUT_GAMEPAD_LEFT_SHOULDER;
			case PadKeyCode::RightShoulder: return XINPUT_GAMEPAD_RIGHT_SHOULDER;
			case PadKeyCode::A: return XINPUT_GAMEPAD_A;
			case PadKeyCode::B: return XINPUT_GAMEPAD_B;
			case PadKeyCode::X: return XINPUT_GAMEPAD_X;
			case PadKeyCode::Y: return XINPUT_GAMEPAD_Y;
		}

		return UINT32_MAX;
	}
}
