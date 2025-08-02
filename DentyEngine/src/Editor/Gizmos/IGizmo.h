#pragma once

namespace DentyEngine
{
	class Event;

	class IGizmo
	{
	public:
		IGizmo() = default;
		virtual ~IGizmo() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(IGizmo)

		virtual void Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void OnEvent(Event* e) = 0;
	};
}