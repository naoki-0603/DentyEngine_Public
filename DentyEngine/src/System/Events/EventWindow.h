#pragma once

#include "Event.h"

namespace DentyEngine
{
	class EventWindowClosed final : public Event
	{
	public:
		EventWindowClosed() : Event(true) {}
		~EventWindowClosed() override = default;

		EVENT_CLASS_TYPE(WindowClosed)
	};

	class EventWindowResized final : public Event
	{
	public:
		EventWindowResized(const Vector2& size) :
			Event(true), _size(size) {}
		~EventWindowResized() override = default;

		EVENT_CLASS_TYPE(WindowResized)

		const Vector2& GetSize() const { return _size; }
	private:
		Vector2 _size;
	};
};