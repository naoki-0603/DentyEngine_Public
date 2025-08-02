#pragma once

#include "Event.h"

namespace DentyEngine
{
	class EventKeyboard : public Event
	{
	public:
		EventKeyboard(const std::byte key) : Event(false), _keyInput(key) {}
		~EventKeyboard() override = default;

		virtual const char* GetName() const override {}
		virtual EventType GetEventType() const override {}

		std::byte GetInputKey() const { return _keyInput; }
	protected:
		std::byte _keyInput;
	};

	class EventMouseButton : public Event
	{
	public:
		EventMouseButton(const std::byte key) : Event(false), _keyInput(key) {}
		~EventMouseButton() override = default;

		virtual const char* GetName() const override { return "EventMouseButton"; }
		virtual EventType GetEventType() const override { return static_cast<EventType>(0); }

		std::byte GetInputKey() const { return _keyInput; }
	protected:
		std::byte _keyInput;
	};

	class EventGamePadButton : public Event
	{
	public:
		EventGamePadButton(const uint16_t keyInput) : Event(false), _keyInput(keyInput) {}
		~EventGamePadButton() override = default;

		virtual const char* GetName() const override {}
		virtual EventType GetEventType() const override {}

		uint16_t GetInputKey() const { return _keyInput; }
	protected:
		uint16_t _keyInput;
	};
};