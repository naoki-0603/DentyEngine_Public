#pragma once

#include "Mouse.h"
#include "Keyboard.h"
#include "GamePad.h"

namespace DentyEngine
{
	class Event;

	class Input final
	{
	public:
		Input() = default;
		~Input() = default;

		static void Update();

		static void OnGui();

		//
		// Setter
		//

		//
		// Mouse
		//
		static void SetMousePositionX(int32_t x);
		static void SetMousePositionY(int32_t y);
		static void SetMousePosition(const Vector2I& position);

		static void SetMouseWheelSpeed(float wheelSpeed);

		static void ResetMouseWheelSpeed();

		//
		// Getter
		//

		//
		// Mouse
		//
		[[nodiscard]]
		static float GetMouseWheelSpeed();

		[[nodiscard]]
		static float GetMousePanX();

		[[nodiscard]]
		static float GetMousePanY();

		[[nodiscard]]
		static Vector2I GetMousePosition();

		[[nodiscard]]
		static bool IsMousePressed(MouseButton button);

		[[nodiscard]]
		static bool IsMouseTriggered(MouseButton button);

		[[nodiscard]]
		static bool IsMouseReleased(MouseButton button);


		//
		// Keyboard
		//
		[[nodiscard]]
		static bool IsKeyPressed(KeyCode keyCode) ;

		[[nodiscard]]
		static bool IsKeyTriggered(KeyCode keyCode) ;

		[[nodiscard]]
		static bool IsKeyReleased(KeyCode keyCode) ;

		//
		// Game pad
		//

		// Return 0 ~ 255
		// Pad num is default is zero
		[[nodiscard]]
		static uint8_t GetLeftTrigger(uint32_t padNum = 0);

		// Return 0 ~ 255
		// Pad num is default is zero
		[[nodiscard]]
		static uint8_t GetRightTrigger(uint32_t padNum = 0);

		// Return -32768 ~ +32767
		// Pad num is default is zero
		[[nodiscard]]
		static int16_t GetThumbLX(uint32_t padNum = 0);

		// Return -32768 ~ +32767
		// Pad num is default is zero
		[[nodiscard]]
		static int16_t GetThumbLY(uint32_t padNum = 0);

		// Return -32768 ~ +32767
		// Pad num is default is zero
		[[nodiscard]]
		static int16_t GetThumbRX(uint32_t padNum = 0);

		// Return -32768 ~ +32767
		// Pad num is default is zero
		[[nodiscard]]
		static int16_t GetThumbRY(uint32_t padNum = 0);

		// Return normalized thumb lx
		// Pad num is default is zero
		[[nodiscard]]
		static float GetNormalizedThumbLX(uint32_t padNum = 0);

		// Return normalized thumb ly
		// Pad num is default is zero
		[[nodiscard]]
		static float GetNormalizedThumbLY(uint32_t padNum = 0);

		// Return normalized thumb rx
		// Pad num is default is zero
		[[nodiscard]]
		static float GetNormalizedThumbRX(uint32_t padNum = 0);

		// Return normalized thumb ry
		// Pad num is default is zero
		[[nodiscard]]
		static float GetNormalizedThumbRY(uint32_t padNum = 0);

		// Pad num is default is zero
		[[nodiscard]]
		static bool IsPadPressed(PadKeyCode keyCode, uint32_t padNum = 0u) ;

		// Pad num is default is zero
		[[nodiscard]]
		static bool IsPadTriggered(PadKeyCode keyCode, uint32_t padNum = 0u) ;

		// Pad num is default is zero
		[[nodiscard]]
		static bool IsPadReleased(PadKeyCode keyCode, uint32_t padNum = 0u) ;

		[[nodiscard]]
		static bool IsPadLeftTriggerTriggered(uint32_t padNum = 0u);

		[[nodiscard]]
		static bool IsPadRightTriggerTriggered(uint32_t padNum = 0u);

		// Pad num is default is zero
		[[nodiscard]]
		static bool IsPadConnectedAt(uint32_t padNum = 0);
	private:
		static constexpr uint32_t NUM_GAME_PAD = 4u;
	private:
		static Mouse _mouse;
		static Keyboard _keyboard;
		static std::array<GamePad, NUM_GAME_PAD> _gamePads;
	};
}