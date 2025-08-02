#pragma once

#include "EventInput.h"

namespace DentyEngine
{
	class EventGamePadButtonPressed final : public EventGamePadButton
	{
	public:
		EventGamePadButtonPressed(const uint16_t key) :
			EventGamePadButton(key) {}
		~EventGamePadButtonPressed() override = default;

		EVENT_CLASS_TYPE(GamePadButtonPressed)
	};

	class EventGamePadButtonReleased final : public EventGamePadButton
	{
	public:
		EventGamePadButtonReleased(const uint16_t key) :
			EventGamePadButton(key) {}
		~EventGamePadButtonReleased() override = default;

		EVENT_CLASS_TYPE(GamePadButtonReleased)
	};

	class EventGamePadButtonTriggered final : public EventGamePadButton
	{
	public:
		EventGamePadButtonTriggered(const uint16_t key) :
			EventGamePadButton(key) {}
		~EventGamePadButtonTriggered() override = default;

		EVENT_CLASS_TYPE(GamePadButtonTriggered)
	};
};