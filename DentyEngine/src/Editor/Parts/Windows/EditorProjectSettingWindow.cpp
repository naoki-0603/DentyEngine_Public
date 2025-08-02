#include "stdafx.h"

#include "EditorProjectSettingWindow.h"

#include "System/Core/Project.h"
#include "System/Core/RuntimeSceneManager.h"
#include "System/Core/Setting/ProjectSetting.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"
#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Events/Event.h"
#include "System/Events/EventEditor.h"
#include "System/Resource/Library/LibraryManager.h"

#include "System/Resource/Library/SceneLibrary.h"
#include "System/Scene/Scene.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	EditorProjectSettingWindow::EditorProjectSettingWindow() :
		EditorWindow(), IEditor(), _scenes(), _reservedRemoveScene()
	{
	}

	void EditorProjectSettingWindow::OnGui(Ref<Scene> activeScene)
	{
		// If not opened, return.
		if (not _open) return;

		if (ImGui::Begin("ProjectSettingWindow##EditorProjectSettingWindow", &_open))
		{
			if (ImGui::CollapsingHeader("Scene##ProjectSettingWindow"))
			{
				SceneSetting();
			}

			if (GuiUtils::ButtonCentered("SaveSetting##EditorProjectSettingWindow", Vector2(300.0f, 32.0f)))
			{
				SaveSetting();
			}

		}
		ImGui::End();

		// Check is window closed.
		if (not _open)
		{
			EventOnSettingsWindowClosed eventOnSettingsWindowClosed;
			NOTIFY_EVENT(eventOnSettingsWindowClosed);
		}
	}

	void EditorProjectSettingWindow::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventProjectSettingWindowOpen>(BIND_EVENT_FUNCTION(OnOpened));
		dispatcher.Dispatch<EventProjectSettingWindowClose>(BIND_EVENT_FUNCTION(OnClosed));
	}

	void EditorProjectSettingWindow::OnOpened([[maybe_unused]] Event* e)
	{
		_open = true;

		// Set window size.
		ImGui::SetNextWindowSize(ImVec2(
			IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y
		));

		if (_scenes.empty())
		{
			const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

			const Ref<ProjectSetting> setting = Project::GetProjectSetting();
			for (const auto& sceneInfo : setting->runtimeSceneChange.scenes | std::views::keys)
			{
				const std::optional<Ref<Scene>> scene = sceneLibrary->FindByName(sceneInfo);
				DENTY_CHECK_CONDITION(scene)

				_scenes.try_emplace(scene.value()->GetSceneID(), scene.value());
			}
		}
	}

	void EditorProjectSettingWindow::OnClosed([[maybe_unused]] Event* e)
	{
		_open = false;
	}

	void EditorProjectSettingWindow::SceneSetting()
	{
		// Drag and drop.
		{
			char buf[] = "Drag and drop here!";
			ImGui::InputText("##SkinnedMeshRenderer", buf, sizeof(buf), ImGuiInputTextFlags_ReadOnly);

			const DragAndDropSpecification specification =
			{
				"SceneItem", sizeof(SceneID),
				nullptr, DragAndDropDataType::Scene,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(SceneAssetDragAndDropTargetCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropTarget dragAndDropTarget(specification);
		}

		// Display scenes.
		{
			if (ImGui::BeginListBox("Scenes##EditorProjectSettingWindow"))
			{
				for (auto&& scene : _scenes | std::views::values)
				{
					ImGui::Text("ID: %u, Name: %s", scene->GetSceneID(), scene->GetName().c_str());
					ImGui::SameLine();

					std::string label = "Remove##" + scene->GetName();
					if (ImGui::Button(label.c_str()))
					{
						_reservedRemoveScene = scene;
					}
				}
				ImGui::EndListBox();
			}

			// Remove
			if (_reservedRemoveScene)
			{
				_scenes.erase(_reservedRemoveScene->GetSceneID());

				RuntimeSceneManager::Remove(_reservedRemoveScene);
			}
			_reservedRemoveScene = nullptr;
		}
	}

	void EditorProjectSettingWindow::SaveSetting()
	{
		const Ref<ProjectSetting> setting = Project::GetProjectSetting();

		setting->runtimeSceneChange.Reset();
		for (const auto& scene : _scenes | std::views::values)
		{
			setting->runtimeSceneChange.scenes.try_emplace(
				scene->GetName(), scene->GetFilePathToSerialized()
			);
		}

		setting->Save(true);
	}

	void EditorProjectSettingWindow::SceneAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_CHECK_CONDITION(dataType == DragAndDropDataType::Scene)

		const auto id = (*static_cast<const SceneID*>(data));

		const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

		const std::optional<Ref<Scene>> scene = sceneLibrary->Find(id);
		if (not scene.has_value())
		{
			DENTY_ASSERT(false, "Scene not found! (in EditorProjectSettingWindow::SceneAssetDragAndDropTargetCallback)");
		}

		_scenes.try_emplace(scene.value()->GetSceneID(), scene.value());

		RuntimeSceneManager::Add(scene.value());
	}
}
