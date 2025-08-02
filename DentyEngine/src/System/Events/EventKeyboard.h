#pragma once

#include "EventInput.h"

namespace DentyEngine
{
	class EventKeyPressed final : public EventKeyboard
	{
	public:
		EventKeyPressed(const std::byte key) :
			EventKeyboard(key) {}
		~EventKeyPressed() override = default;

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class EventKeyReleased final : public EventKeyboard
	{
	public:
		EventKeyReleased(const std::byte key) :
			EventKeyboard(key) {}
		~EventKeyReleased() override = default;

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class EventKeyTriggered final : public EventKeyboard
	{
	public:
		EventKeyTriggered(const std::byte key) :
			EventKeyboard(key) {}
		~EventKeyTriggered() override = default;

		EVENT_CLASS_TYPE(KeyTriggered)
	};
};