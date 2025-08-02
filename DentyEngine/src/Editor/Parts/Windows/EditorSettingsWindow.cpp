#include "stdafx.h"

#include "EditorSettingsWindow.h"

#include "System/Events/EventEditor.h"

#include "System/Core/Setting/GlobalSettingsManager.h"

namespace DentyEngine
{
    EditorSettingsWindow::EditorSettingsWindow() :
        EditorWindow(), IEditor()
    {
    }

    void EditorSettingsWindow::OnGui(Ref<Scene> activeScene)
    {
        // If not opened, return.
        if (not _open) return;

        if (ImGui::Begin("Settings##EditorSettingsWindow", &_open))
        {
            GlobalSettingsManager::OnGui();
        }
        ImGui::End();

        // Check is window closed.
        if (not _open)
        {
            EventOnSettingsWindowClosed eventOnSettingsWindowClosed;
            NOTIFY_EVENT(eventOnSettingsWindowClosed);
        }
    }

    void EditorSettingsWindow::OnEvent(Event* e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<EventOnSettingsWindowOpened>(BIND_EVENT_FUNCTION(OnOpened));
        dispatcher.Dispatch<EventOnSettingsWindowClosed>(BIND_EVENT_FUNCTION(OnClosed));
    }

    void EditorSettingsWindow::OnOpened([[maybe_unused]] Event* e)
    {
        _open = true;

        // Set window size.
        ImGui::SetNextWindowSize(ImVec2(
            IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y
        ));

        // Load from .GlobalSettings file.
        GlobalSettingsManager::OnEditorSettingsWindowOpened();
    }

    void EditorSettingsWindow::OnClosed([[maybe_unused]] Event* e)
    {
        _open = false;

        // Save As .GlobalSettings file.
        GlobalSettingsManager::OnEditorSettingsWindowClosed();
    }
}
