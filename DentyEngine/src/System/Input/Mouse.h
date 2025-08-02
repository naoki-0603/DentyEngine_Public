#pragma once

#include "KeyCode.h"

namespace DentyEngine
{
	class Mouse final
	{
	public:
		Mouse();
		~Mouse() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(Mouse)

		void Update();

		void OnGui();

		[[nodiscard]]
		bool IsPressed(MouseButton button) const;

		[[nodiscard]]
		bool IsTrigger(MouseButton button) const;

		[[nodiscard]]
		bool IsReleased(MouseButton button) const;

		//
		// Setter
		//
		void SetMousePositionX(int32_t x);
		void SetMousePositionY(int32_t y);
		void SetMousePosition(const Vector2I& position);

		void SetWheelSpeed(float wheelSpeed);

		void ResetWheelSpeed();

		//
		// Getter
		//
		[[nodiscard]]
		int32_t GetMousePositionX() const;

		[[nodiscard]]
		int32_t GetMousePositionY() const;

		[[nodiscard]]
		Vector2I GetMousePosition() const;

		[[nodiscard]]
		float GetPanX() const;

		[[nodiscard]]
		float GetPanY() const;

		[[nodiscard]]
		float GetWheelSpeed() const;
	private:
		[[nodiscard]]
		uint32_t GetVirtualKey(MouseButton button) const;

	private:
		std::array<uint8_t, 256> _keys;
		std::array<uint8_t, 256> _prevKeys;

		Vector2I _prevPosition;
		Vector2I _curPosition;

		float _wheelSpeed;
	private:
		static constexpr float MAX_PAN_ABS = 30.0f;
	};

	
}
