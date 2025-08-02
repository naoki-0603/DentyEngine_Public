#pragma once

#include "EventInput.h"

namespace DentyEngine
{
	 /*class EventMouseButtonPressed final : public EventMouseButton
	 {
	 public:
	 	EventMouseButtonPressed(const std::byte key) :
	 		EventMouseButton(key) {}
	 	~EventMouseButtonPressed() override = default;
	 
	 	EVENT_CLASS_TYPE(MouseButtonPressed)
	 };*/
	 
	 class EventMouseButtonReleased final : public EventMouseButton
	 {
	 public:
	 	EventMouseButtonReleased(const std::byte key) :
	 		EventMouseButton(key) {}
	 	~EventMouseButtonReleased() override = default;
	 
	 	EVENT_CLASS_TYPE(MouseButtonReleased)
	 };

	class EventMouseScrolled final : public Event
	{
	public:
		EventMouseScrolled(float speed) :
			_wheelSpeed(speed), Event(false) {}
		~EventMouseScrolled() override = default;

		float GetWheelSpeed() const { return _wheelSpeed; }

		EVENT_CLASS_TYPE(MouseScrolled)
	private:
		float _wheelSpeed;
	};

	// class EventMouseMoved final : public Event
	// {
	// public:
	// 	EventMouseMoved(const Vector2& position) :
	// 		_position(position), Event(false) {}
	// 	~EventMouseMoved() override = default;
	// 
	// 	const Vector2& GetPosition() const { return _position; }
	// 
	// 	EVENT_CLASS_TYPE(MouseMoved)
	// private:
	// 	Vector2 _position;
	// };
};