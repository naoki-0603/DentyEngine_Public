#pragma once

#include "IEditor.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
	class GameObject;

	class EventSelectedGameObject;
	class EventOnGameObjectDestroyed;

	class EventOnSceneDeActivated;

	class EditorInspector final : public IEditor
	{
	public:
		EditorInspector();
		~EditorInspector() override = default;

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

		void OnSelectedGameObject(EventSelectedGameObject* e);
		void OnGameObjectDestroyed(EventOnGameObjectDestroyed* e);

		void OnSceneDeActivated(EventOnSceneDeActivated* e);
	private:
		void AddComponentOnGui();

		void ScriptAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);

	private:
		GameObject* _selectedGameObject;

		bool _pressedAddComponentButton;
		bool _locked;
		bool _open;
	};
};