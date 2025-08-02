#pragma once

namespace DentyEngine
{
	class Scene;

	class Event;
	class DENTY_NOVTABLE IEditor
	{
	public:
		IEditor() = default;
		virtual ~IEditor() = default;

		virtual void OnEvent(Event* e) = 0;
		virtual void OnGui(Ref<Scene> activeScene) = 0;
	};
};