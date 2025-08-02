#include "stdafx.h"

#include "Input.h"

#include "System/Core/DString.h"

namespace DentyEngine
{
	//
	// Static variables.
	//
	Mouse Input::_mouse = { };
	Keyboard Input::_keyboard = { };
	std::array<GamePad, Input::NUM_GAME_PAD> Input::_gamePads = { };

	//
	// Static functions.
	//
	void Input::Update()
	{
		_mouse.Update();
		_keyboard.Update();

		for (auto&& gamePad : _gamePads)
		{
			gamePad.Update();
		}
	}

	void Input::OnGui()
	{
		if (ImGui::CollapsingHeader("Input##CoreLayer"))
		{
			if (ImGui::TreeNodeEx("Mouse##Input"))
			{
				_mouse.OnGui();

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Keyboard##Input"))
			{
				_keyboard.OnGui();

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("GamePad##Input"))
			{
				_gamePads.at(0).OnGui();

				ImGui::TreePop();
			}
		}
	}

	void Input::SetMousePositionX(int32_t x)
	{
		_mouse.SetMousePositionX(x);
	}

	void Input::SetMousePositionY(int32_t y)
	{
		_mouse.SetMousePositionY(y);
	}

	void Input::SetMousePosition(const Vector2I& position)
	{
		_mouse.SetMousePosition(position);
	}

	void Input::SetMouseWheelSpeed(float wheelSpeed)
	{
		_mouse.SetWheelSpeed(wheelSpeed);
	}

	void Input::ResetMouseWheelSpeed()
	{
		_mouse.ResetWheelSpeed();
	}

	float Input::GetMouseWheelSpeed()
	{
		return _mouse.GetWheelSpeed();
	}

	float Input::GetMousePanX()
	{
		return _mouse.GetPanX();
	}

	float Input::GetMousePanY()
	{
		return _mouse.GetPanY();
	}

	Vector2I Input::GetMousePosition()
	{
		return _mouse.GetMousePosition();
	}

	bool Input::IsMousePressed(MouseButton button)
	{
		return _mouse.IsPressed(button);
	}

	bool Input::IsMouseTriggered(MouseButton button)
	{
		return _mouse.IsTrigger(button);
	}

	bool Input::IsMouseReleased(MouseButton button)
	{
		return _mouse.IsReleased(button);
	}

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		return _keyboard.IsPressed(keyCode);
	}

	bool Input::IsKeyTriggered(KeyCode keyCode)
	{
		return _keyboard.IsTrigger(keyCode);
	}

	bool Input::IsKeyReleased(KeyCode keyCode) 
	{
		return _keyboard.IsReleased(keyCode);
	}

	uint8_t Input::GetLeftTrigger(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetLeftTrigger();
	}

	uint8_t Input::GetRightTrigger(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetRightTrigger();
	}

	int16_t Input::GetThumbLX(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetThumbLX();
	}

	int16_t Input::GetThumbLY(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetThumbLY();
	}

	int16_t Input::GetThumbRX(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetThumbRX();
	}

	int16_t Input::GetThumbRY(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetThumbRY();
	}

	float Input::GetNormalizedThumbLX(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetNormalizedThumbLX();
	}

	float Input::GetNormalizedThumbLY(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetNormalizedThumbLY();
	}

	float Input::GetNormalizedThumbRX(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetNormalizedThumbRX();
	}

	float Input::GetNormalizedThumbRY(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).GetNormalizedThumbRY();
	}

	bool Input::IsPadPressed(PadKeyCode keyCode, uint32_t padNum) 
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).IsPressed(keyCode);
	}

	bool Input::IsPadTriggered(PadKeyCode keyCode, uint32_t padNum) 
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).IsTrigger(keyCode);
	}

	bool Input::IsPadReleased(PadKeyCode keyCode, uint32_t padNum) 
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).IsReleased(keyCode);
	}

	bool Input::IsPadLeftTriggerTriggered(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).IsLeftTriggerTriggered();
	}

	bool Input::IsPadRightTriggerTriggered(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).IsRightTriggerTriggered();
	}

	bool Input::IsPadConnectedAt(uint32_t padNum)
	{
		DENTY_CHECK_IS_OUT_OF_RANGE_STL(padNum, _gamePads, __func__)

		return _gamePads.at(padNum).Connected();
	}
}
