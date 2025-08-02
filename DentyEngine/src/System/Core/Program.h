#pragma once

#include "LayerManager.h"

namespace DentyEngine
{
	class EventWindowClosed;
	class EventWindowResized;

	class Program final
	{
	public:
		~Program() = default;
	    DELETE_COPY_MOVE_CONSTRUCTORS(Program)

		static Program* Create();

		void Launch(
			std::string_view applicationName,
			HINSTANCE hInstance,
			const Vector2& size
		);
		void Run();
		void Terminate();

		void OnEvent(Event* e);

		void OnWindowClosed(EventWindowClosed* e);
		void OnWindowResized(EventWindowResized* e);
	private:
		Program();

		Scope<LayerManager> _layerManager;

		std::string _name;
		bool _running;

		static bool _created;
	};
};