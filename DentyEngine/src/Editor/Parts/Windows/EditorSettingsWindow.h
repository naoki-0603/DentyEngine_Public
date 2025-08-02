#pragma once

#include "EditorWindow.h"
#include "Editor/Parts/IEditor.h"

namespace DentyEngine
{
    class EditorSettingsWindow final : public EditorWindow, public IEditor
    {
    public:
        EditorSettingsWindow();
        ~EditorSettingsWindow() override = default;

        void OnGui(Ref<Scene> activeScene) override;
        void OnEvent(Event* e) override;

        // Events
    private:
        // Called from when window is opened.
        void OnOpened(Event* e) override;

        // Called from when window is closed.
        void OnClosed(Event* e) override;
    };
}
