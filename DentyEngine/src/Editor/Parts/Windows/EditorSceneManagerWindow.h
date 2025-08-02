#pragma once

#include "EditorWindow.h"
#include "Editor/Parts/IEditor.h"

#include "System/Core/DFilePath.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
    class EditorSceneManagerWindow final : public EditorWindow, public IEditor
    {
    public:
        EditorSceneManagerWindow();
        ~EditorSceneManagerWindow() override = default;

        void OnGui(Ref<Scene> activeScene) override;
        void OnEvent(Event* e) override;

        // Events
    private:
        // Called from when window is opened.
        void OnOpened(Event* e) override;

        // Called from when window is closed.
        void OnClosed(Event* e) override;

    private:
        void SceneAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);

    private:
        std::unordered_map<FilePath, Ref<Scene>, FilePath::Hash> _scenes;
        std::unordered_map<FilePath, Ref<Scene>, FilePath::Hash> _reservedRemoveScenes;
    };
}
