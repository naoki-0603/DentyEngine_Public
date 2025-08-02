#pragma once

#include "EventConstants.h"

namespace DentyEngine
{
	class Event
	{
	public:
		Event() = delete;
		Event(bool listenerMultipleExists) : handled(),
		listenerMultipleExists(listenerMultipleExists) {}
		virtual ~Event() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(Event)

		virtual const char* GetName() const = 0;
		virtual EventInheritedType GetInheritedType() const { return EventInheritedType::Event; }
		virtual EventType GetEventType() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool handled;
		bool listenerMultipleExists;
	};

	class NotifyEvent final
	{
	public:
		using EventCallback = std::function<void(Event* e)>;
	public:
		NotifyEvent() = delete;
		NotifyEvent(const EventCallback& callback);
		~NotifyEvent() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(NotifyEvent)

		static void Notify(Event* e);
	private:
		static EventCallback _callback;
	};

	class EventDispatcher final
	{
	public:
		EventDispatcher(Event* e) : _event(e) {}
		~EventDispatcher() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(EventDispatcher)

		template <class T, class U>
		void Dispatch(const U& function)
		{
			if (_event->GetEventType() == T::GetEventTypeStatic())
			{
				function(static_cast<T*>(_event));
				_event->handled = true;
			}
		}
	private:
		Event* _event;
	};

#define EVENT_CLASS_TYPE(type) static EventType GetEventTypeStatic() { return EventType::type; } \
	const char* GetName() const override { return #type; } \
	EventType GetEventType() const { return EventType::type; }

#define BIND_EVENT_FUNCTION(function) [this](auto&&... args) -> decltype(auto) { return this->function(std::forward<decltype(args)>(args)...); }

#define NOTIFY_EVENT(event) NotifyEvent::Notify(&(event))
};