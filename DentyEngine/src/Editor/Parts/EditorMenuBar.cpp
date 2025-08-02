#include "stdafx.h"

#include "EditorMenuBar.h"
#include "EditorContentBrowser.h"

#include "System/Core/FileDialogs.h"

#include "System/Events/EventEditor.h"

#include "System/Scene/Scene.h"

#include "System/Editor/EditorSceneManager.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/SceneLibrary.h"

#include "System/Serializer/SerializeExtensions.h"

#include "Utilities/StringUtilities.h"

namespace DentyEngine
{
	EditorMenuBar::EditorMenuBar() :
		IEditor()
	{
	}

	void EditorMenuBar::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void EditorMenuBar::OnGui(Ref<Scene> activeScene)
	{
        if (ImGui::BeginMenuBar())
        {
            FileMenuBar(activeScene);
            WindowMenuBar(activeScene);
            SettingsMenuBar(activeScene);

            ImGui::EndMenuBar();
        }

        if (activeScene)
        {
            Shortcuts(activeScene);
        }
	}

	void EditorMenuBar::FileMenuBar(Ref<Scene> activeScene)
	{
        if (ImGui::BeginMenu("File##EditorMenuBar"))
        {
            // Create new scene
            if (ImGui::MenuItem("New Scene"))
            {
                CreateNewScene(activeScene);
            }

            // Load scene
            if (ImGui::MenuItem("Load Scene..."))
            {
                LoadScene(activeScene);
            }

            // If active scene exists.
            if (activeScene)
            {
                // Save scene
                if (ImGui::MenuItem("Save"))
                {
                    Save(activeScene);
                }

                // Save as
                if (ImGui::MenuItem("Save As..."))
                {
                    SaveAs(activeScene);
                }

                // Clone As...
                if (ImGui::MenuItem("Clone As..."))
                {
                    CloneAs(activeScene);
                }
            }
            
            ImGui::EndMenu();
        }

        Shortcuts(activeScene);
	}

    void EditorMenuBar::CreateNewScene(Ref<Scene> activeScene)
    {
        // Create unique scene name.
        const auto sceneName = EditorContentBrowser::MakeUniqueName(
            std::string("New_"),
            "Assets\\Scenes",
            ".Scene"
        );

        // Set new path.
        if (sceneName.has_value())
        {
            const auto newScene = std::make_shared<Scene>(sceneName.value());
            newScene->SetFilePath("Assets\\Scenes\\" + sceneName.value() + SCENE_FILE_EXTENSION);
            newScene->SetFilePathToSerialized("Assets\\Scenes\\" + sceneName.value() + SCENE_FILE_EXTENSION);

            SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();
            sceneLibrary->Add(newScene);

            EditorSceneManager::SetReservedSceneFilePath(newScene->GetFilePathToSerialized(), true, false, false);
        }
    }

    void EditorMenuBar::LoadScene(Ref<Scene> activeScene)
    {
        const String scenePath = FileDialogs::OpenFile(FileDialogs::SCENE_FILTER);

        if (!scenePath.IsEmpty())
        {
            const String assetDirectory = "A";

            const uint32_t index = scenePath.FindLastOf(assetDirectory);
            if (index == UINT32_MAX)
            {
                DENTY_ERR_CONSOLE_LOG("Invalid scene file path!");
                return;
            }

            const FilePath relativeSceneFilePath = scenePath.SubStr(index).Get();
            const String sceneName = relativeSceneFilePath.Filename().RemoveExtension().GetAsString();

            // If same scene, return
            if (activeScene && activeScene->GetName() == sceneName.Get())
            {
                DENTY_WARN_CONSOLE_LOG("Can not be load same scene!");
                return;
            }

        	EditorSceneManager::SetReservedSceneFilePath(relativeSceneFilePath, false, false, false);
        }
    }

    void EditorMenuBar::WindowMenuBar(Ref<Scene> activeScene)
	{
        if (ImGui::BeginMenu("Window##EditorMenuBar"))
        {
            // Animator window.
            if (ImGui::MenuItem("Animator##EditorMenuBar"))
            {
                // Create event.
                EventOnAnimatorWindowOpened eventOnAnimatorWindowOpened(nullptr);
                NOTIFY_EVENT(eventOnAnimatorWindowOpened);
            }

            // Graphics window.
            if (ImGui::MenuItem("Graphics##EditorMenuBar"))
            {
	            // Create event.
                EventOnGraphicsWindowOpened eventOnGraphicsWindowOpened;
                NOTIFY_EVENT(eventOnGraphicsWindowOpened);
            }

            // Settings window.
            if (ImGui::MenuItem("Settings##EditorMenuBar"))
            {
                // Create event.
                EventOnSettingsWindowOpened eventOnSettingsWindowOpened;
                NOTIFY_EVENT(eventOnSettingsWindowOpened);
            }

            // Scene manager window.
            if (ImGui::MenuItem("SceneManager##EditorMenuBar"))
            {
                // Create event.
                EventOnSceneManagerWindowOpened eventOnSceneManagerWindowOpened;
                NOTIFY_EVENT(eventOnSceneManagerWindowOpened);
            }

            // Post effect window.
            if (ImGui::MenuItem("PostEffect##EditorMenuBar"))
            {
                EventOnPostEffectWindowOpened eventOnPostEffectWindowOpened;
                NOTIFY_EVENT(eventOnPostEffectWindowOpened);
            }

            // Profiler window.
            if (ImGui::MenuItem("Profiler##EditorMenuBar"))
            {
                EventOnProfilerWindowOpened eventOnProfilerWindowOpened;
                NOTIFY_EVENT(eventOnProfilerWindowOpened);
            }

            ImGui::EndMenu();
        }
	}

    void EditorMenuBar::SettingsMenuBar(Ref<Scene> activeScene)
    {
        if (ImGui::BeginMenu("Settings##EditorMenuBar"))
        {
            if (ImGui::MenuItem("Project##EditorMenuBar"))
            {
                ProjectSettings(activeScene);
            }

            ImGui::EndMenu();
        }
    }

    void EditorMenuBar::ProjectSettings([[maybe_unused]] Ref<Scene> activeScene)
    {
        EventProjectSettingWindowOpen eventProjectSettingWindowOpen;
        NOTIFY_EVENT(eventProjectSettingWindowOpen);
    }

    void EditorMenuBar::Save(Ref<Scene> activeScene)
	{
        activeScene->BeforeSave();
        activeScene->Save(true);

        // Add scene if doesn't exists in library.
        {
            SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();
            if (!sceneLibrary->Exists(activeScene->GetFilePathToSerialized()))
            {
                sceneLibrary->Add(activeScene);
            }
        }
	}

	void EditorMenuBar::SaveAs(Ref<Scene> activeScene)
	{
        const std::string scenePath = FileDialogs::SaveFile(FileDialogs::SCENE_FILTER);

        if (!scenePath.empty())
        {
            const FilePath relativeSceneFilePath = scenePath.substr(scenePath.find_last_of("A"));

            // Already exists.
        	if (relativeSceneFilePath.IsValid())
            {
                // Remove file
                relativeSceneFilePath.RemoveFile();

                // Remove from scene library.
                SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();
                sceneLibrary->Remove(relativeSceneFilePath);
            }

            std::string sceneName = scenePath.substr(scenePath.find_last_of("\\") + 1);
            if (StringUtils::StringContains(sceneName, SCENE_FILE_EXTENSION))
            {
                sceneName = StringUtils::RemoveExtension(sceneName, ".");
            }

            activeScene->BeforeSave();
            activeScene->SaveAs(sceneName);
        }
	}

    void EditorMenuBar::CloneAs(Ref<Scene> activeScene)
    {
        const std::string scenePath = FileDialogs::SaveFile(FileDialogs::SCENE_FILTER);

        if (not scenePath.empty())
        {
            const FilePath relativeSceneFilePath = scenePath.substr(scenePath.find_last_of("A"));

            // Already exists.
        	if (relativeSceneFilePath.IsValid())
            {
                // Remove file
                relativeSceneFilePath.RemoveFile();

                // Remove from scene library.
                SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();
                sceneLibrary->Remove(relativeSceneFilePath);
            }

            std::string sceneName = scenePath.substr(scenePath.find_last_of("\\") + 1);
            if (StringUtils::StringContains(sceneName, SCENE_FILE_EXTENSION))
            {
                sceneName = StringUtils::RemoveExtension(sceneName, ".");
            }

            const auto cloneScene = std::make_shared<Scene>();
            activeScene->Clone(cloneScene);

            cloneScene->BeforeSave();
            cloneScene->SaveAs(sceneName);
        }
    }

    void EditorMenuBar::Shortcuts(Ref<Scene> activeScene)
	{
		const bool shift = Input::IsKeyPressed(KeyCode::LShift);
		const bool ctrl = Input::IsKeyPressed(KeyCode::LCtrl);

        if (ctrl && shift)
        {
            // SaveAs...
            if (Input::IsKeyTriggered(KeyCode::S))
            {
                SaveAs(activeScene);
            }
        }

        if (ctrl)
        {
	        // Save
            if (Input::IsKeyTriggered(KeyCode::S))
            {
                Save(activeScene);
            }
        }
	}
}
