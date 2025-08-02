#pragma once

#include "KeyCode.h"

namespace DentyEngine
{
	class Keyboard final
	{
	public:
		Keyboard();
		~Keyboard() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(Keyboard)

		void Update();

		void OnGui();

		[[nodiscard]]
		bool IsPressed(KeyCode key) const;

		[[nodiscard]]
		bool IsTrigger(KeyCode key) const;

		[[nodiscard]]
		bool IsReleased(KeyCode key) const;
	private:
		[[nodiscard]]
		uint32_t GetVirtualKey(KeyCode key) const;

	private:
		std::array<uint8_t, 256> _keys;
		std::array<uint8_t, 256> _prevKeys;
	};
}
