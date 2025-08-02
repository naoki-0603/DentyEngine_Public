#pragma once

namespace DentyEngine
{
	class Event;
	class IScene
	{
	public:
		IScene() = default;
		virtual ~IScene() = default;

		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnLateUpdate(float deltaTime) = 0;
		virtual void OnFixedUpdate() = 0;
		virtual void OnEvent(Event* e) = 0;
		virtual void OnSerialized() const = 0;
		virtual void OnDeserialized() = 0;
	};
}

