#pragma once

#include "EditorWindow.h"
#include "Editor/Parts/IEditor.h"

#include "System/Core/ID/SceneID.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
	class EditorProjectSettingWindow final : public EditorWindow, public IEditor
	{
	public:
		EditorProjectSettingWindow();
		~EditorProjectSettingWindow() override = default;

        //
        // public functions.
        //
        void OnGui(Ref<Scene> activeScene) override;
        void OnEvent(Event* e) override;

        //
        // private functions.
        //
    private:
        void OnOpened(Event* e) override;
        void OnClosed(Event* e) override;

        void SceneSetting();

        void SaveSetting();

        // DragAndDropTargetCallback
        void SceneAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData);

	private:
        std::unordered_map<SceneID, Ref<Scene>> _scenes;
        Ref<Scene> _reservedRemoveScene;
	};
}
