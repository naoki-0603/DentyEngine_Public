#pragma once

#include "EditorView.h"

namespace DentyEngine
{
    class EventOnApplicationStateChangedToEdit;
    class EventRuntimeSceneChanged;

	class EditorGameView final : public EditorView
	{
	public:
		EditorGameView();
		~EditorGameView() override = default;

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;
	private:
		void OnRuntimeSceneChanged(EventRuntimeSceneChanged* e);
		void OnApplicationStateChangedToEdit(EventOnApplicationStateChangedToEdit* e);
	};
};