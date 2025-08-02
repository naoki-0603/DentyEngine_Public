#pragma once

namespace DentyEngine
{
	class Event;

	class Layer
	{
	public:
		Layer(std::string_view name, const bool enabled) : _name(name), _enabled(enabled), _initialized() {}
		virtual ~Layer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(Layer)

		virtual void OnAttach() = 0;
		virtual void OnUpdate(const float deltaTime) = 0;
		virtual void OnLateUpdate(float deltaTime) = 0;
		virtual void OnFixedUpdate() = 0;
		virtual void OnDetach() = 0;
		virtual void OnGui() = 0;
		virtual void OnEvent(Event* e);

		[[nodiscard]]
		const std::string& GetName() const { return _name; }

		[[nodiscard]]
		bool Enabled() const { return _enabled; }
	protected:
		std::string _name;

		bool _enabled;
		bool _initialized;
	};
};