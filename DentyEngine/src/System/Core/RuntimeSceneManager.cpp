#include "stdafx.h"

#include "RuntimeSceneManager.h"

#include "Project.h"

#include "Setting/ProjectSetting.h"

#include "System/Scene/Scene.h"
#include "System/Events/EventSystem.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/SceneLibrary.h"

namespace DentyEngine
{
	std::unordered_map<FilePath, Ref<Scene>, FilePath::Hash> RuntimeSceneManager::_scenes = {};

	RuntimeSceneManager::RuntimeSceneManager()
	{
	}

	void RuntimeSceneManager::OnLaunch()
	{
		const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

		const Ref<ProjectSetting> setting = Project::GetProjectSetting();

		for (const auto& sceneName : setting->runtimeSceneChange.scenes | std::views::keys)
		{
			const std::optional<Ref<Scene>> scene = sceneLibrary->FindByName(sceneName);
			DENTY_CHECK_CONDITION(scene.has_value())

			_scenes.try_emplace(scene.value()->GetFilePathToSerialized(), scene.value());
		}
	}

	void RuntimeSceneManager::OnGui()
	{
		if (ImGui::CollapsingHeader("RuntimeSceneManager##CoreLayer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto&& scene : _scenes | std::views::values)
			{
				ImGui::Text(scene->GetName().c_str());
			}
		}
	}

	void RuntimeSceneManager::Add(Ref<Scene> scene)
	{
		_scenes.try_emplace(scene->GetFilePathToSerialized(), scene);
	}

	void RuntimeSceneManager::Remove(Ref<Scene> scene)
	{
		_scenes.erase(scene->GetFilePathToSerialized());
	}

	void RuntimeSceneManager::ChangeSceneFromIndex(int32_t index, bool reloadScene)
	{
		if (index >= GetSceneCount() || index < 0)
		{
			DENTY_ERR_CONSOLE_LOG("Index out of range!");
			return;
		}

		int32_t counter = 0;
		for (auto&& scene : _scenes | std::views::values)
		{
			if (index == counter)
			{
				EventRuntimeSceneChange eventRuntimeSceneChange(scene->GetFilePathToSerialized().GetAsString(), reloadScene);
				NOTIFY_EVENT(eventRuntimeSceneChange);

				break;
			}

			++counter;
		}
	}

	void RuntimeSceneManager::ChangeSceneFromName(std::string_view name, bool reloadScene)
	{
		for (auto&& scene : _scenes | std::views::values)
		{
			if (scene->GetName() == name)
			{
				EventRuntimeSceneChange eventRuntimeSceneChange(scene->GetFilePathToSerialized().GetAsString(), reloadScene);
				NOTIFY_EVENT(eventRuntimeSceneChange);

				return;
			}
		}

		DENTY_ERR_CONSOLE_LOG("Scene not found! (name: %s)", std::string(name).c_str());
	}

	int32_t RuntimeSceneManager::GetSceneCount()
	{
		return static_cast<int32_t>(_scenes.size());
	}

}
