#pragma once

#include "KeyCode.h"

namespace DentyEngine
{
	class GamePad final
	{
	public:
		GamePad();
		~GamePad() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(GamePad)

		void Update();;

		void OnGui();

		//
		// Setter
		//
		void SetPadNum(int padNum) { _padNum = padNum; }

		//
		// Getter
		//
		[[nodiscard]]
		bool IsPressed(PadKeyCode key) const;

		[[nodiscard]]
		bool IsTrigger(PadKeyCode key) const;

		[[nodiscard]]
		bool IsReleased(PadKeyCode key) const;

		[[nodiscard]]
		bool IsLeftTriggerTriggered() const;

		[[nodiscard]]
		bool IsRightTriggerTriggered() const;

		// Return 0 ~ 255
		[[nodiscard]]
		uint8_t GetLeftTrigger() const { return _currentState.Gamepad.bLeftTrigger; }

		// Return 0 ~ 255
		[[nodiscard]]
		uint8_t GetRightTrigger() const { return _currentState.Gamepad.bRightTrigger; }

		// Return -32768 ~ +32767
		[[nodiscard]]
		int16_t GetThumbLX() const { return _currentState.Gamepad.sThumbLX; }

		// Return -32768 ~ +32767
		[[nodiscard]]
		int16_t GetThumbLY() const { return _currentState.Gamepad.sThumbLY; }

		// Return -32768 ~ +32767
		[[nodiscard]]
		int16_t GetThumbRX() const { return _currentState.Gamepad.sThumbRX; }

		// Return -32768 ~ +32767
		[[nodiscard]]
		int16_t GetThumbRY() const { return _currentState.Gamepad.sThumbRY; }

		// Return normalized thumb lx
		[[nodiscard]]
		float GetNormalizedThumbLX() const { return static_cast<float>(_currentState.Gamepad.sThumbLX) / SHORT_MAX; }

		// Return normalized thumb ly
		[[nodiscard]]
		float GetNormalizedThumbLY() const { return static_cast<float>(_currentState.Gamepad.sThumbLY) / SHORT_MAX; }

		// Return normalized thumb rx
		[[nodiscard]]
		float GetNormalizedThumbRX() const { return static_cast<float>(_currentState.Gamepad.sThumbRX) / SHORT_MAX; }

		// Return normalized thumb ry
		[[nodiscard]]
		float GetNormalizedThumbRY() const { return static_cast<float>(_currentState.Gamepad.sThumbRY) / SHORT_MAX; }

		[[nodiscard]]
		bool Connected() const { return _connected; }
	private:
		[[nodiscard]]
		uint32_t GetBitMask(PadKeyCode keyCode) const;

	private:
		XINPUT_STATE _currentState;
		XINPUT_STATE _prevState;

		int32_t _padNum;

		bool _connected;
	private:
		static constexpr float INPUT_DEAD_ZONE = 0.24f * static_cast<float>(0x7FFF);
	};
}
