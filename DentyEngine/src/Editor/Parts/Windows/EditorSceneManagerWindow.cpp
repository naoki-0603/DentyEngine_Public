#include "stdafx.h"

#include "EditorSceneManagerWindow.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Events/EventEditor.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/SceneLibrary.h"

#include "System/Scene/Scene.h"

namespace DentyEngine
{
	EditorSceneManagerWindow::EditorSceneManagerWindow() :
		EditorWindow(), IEditor(), _scenes(), _reservedRemoveScenes()
	{
	}

	void EditorSceneManagerWindow::OnGui(Ref<Scene> activeScene)
	{
		// If not opened, return.
		if (not _open) return;

		if (ImGui::Begin("SceneManager##EditorSettingsWindow", &_open, IMGUI_DEFAULT_WINDOW_FLAGS))
		{
			ImGui::Text("Scene");

			// Drag and drop.
			{
				char buf[] = "Drag and drop here!";
				ImGui::InputText("##SkinnedMeshRenderer", buf, sizeof(buf), ImGuiInputTextFlags_ReadOnly);

				const DragAndDropSpecification specification =
				{
					"SceneItem", sizeof(FilePath),
					nullptr, DragAndDropDataType::Scene,
					DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(SceneAssetDragAndDropTargetCallback),
					DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
				};
				DragAndDropTarget dragAndDropTarget(specification);
			}

			ImGui::Separator();

			// Scene list.
			if (ImGui::BeginListBox("Scenes##EditorSceneManagerWindow"))
			{
				for (const auto& scene : _scenes)
				{
					ImGui::Text("ID: %u, Name: %s", scene.second->GetSceneID(), scene.second->GetName().c_str());
					ImGui::SameLine();

					std::string label = "Remove##" + scene.first.GetAsString();
					if (ImGui::Button(label.c_str()))
					{
						_reservedRemoveScenes.emplace(scene.first, scene.second);

						//GameSceneManager::GetInstance().Remove(scene.first);
					}
				}
				ImGui::EndListBox();
			}

			// Remove
			for (const auto& scene : _reservedRemoveScenes)
			{
				_scenes.erase(scene.first);
			}
			_reservedRemoveScenes.clear();


		}
		ImGui::End();


		// Check is window closed.
		if (not _open)
		{
			EventOnSceneManagerWindowClosed eventOnSceneManagerWindowClosed;
			NOTIFY_EVENT(eventOnSceneManagerWindowClosed);
		}
	}

	void EditorSceneManagerWindow::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnSceneManagerWindowOpened>(BIND_EVENT_FUNCTION(OnOpened));
		dispatcher.Dispatch<EventOnSceneManagerWindowClosed>(BIND_EVENT_FUNCTION(OnClosed));
	}

	void EditorSceneManagerWindow::OnOpened([[maybe_unused]] Event* e)
	{
		_open = true;

		// Set window size.
		ImGui::SetNextWindowSize(ImVec2(
			IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y
		));
	}

	void EditorSceneManagerWindow::OnClosed([[maybe_unused]] Event* e)
	{
		_open = false;
	}

	void EditorSceneManagerWindow::SceneAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::Scene, "Data type must be Scene!");

		const auto sceneFilePath = *static_cast<const FilePath*>(data);

		const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();
		const auto scene = sceneLibrary->Find(sceneFilePath);
		if (!scene.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Scene " << sceneFilePath << " not found!";

			return;
		}

		_scenes.try_emplace(sceneFilePath, scene.value());
	}
}
