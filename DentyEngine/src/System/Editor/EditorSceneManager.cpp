#include "stdafx.h"

#include "EditorSceneManager.h"

#include "System/Scene/Scene.h"

#include "System/Events/EventEditor.h"
#include "System/Events/EventSystem.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/SceneLibrary.h"

#include "System/Core/UndoRedo/CommandRecorder.h"

namespace DentyEngine
{
	EditorSceneChangeParameters::EditorSceneChangeParameters() :
		isChangeToNewScene()
	{
	}

	void EditorSceneChangeParameters::Reset()
	{
		isChangeToNewScene = false;
	}

	void RuntimeSceneChangeParameters::Reset()
	{
		beginPlaySceneFilePath = FilePath();

		runtimeSceneChange = false;
	}

	RuntimeSceneChangeParameters::RuntimeSceneChangeParameters() :
		beginPlaySceneFilePath(), runtimeSceneChange(), reloadScene()
	{
	}

	CommonSceneChangeParameters::CommonSceneChangeParameters() :
		currentScene(), reservedLoadSceneFilePath()
	{
	}

	EditorSceneChangeParameters EditorSceneManager::_editorSceneChangeParameters = { };
	RuntimeSceneChangeParameters EditorSceneManager::_runtimeSceneChangeParameters = { };
	CommonSceneChangeParameters EditorSceneManager::_commonSceneChangeParameters = { };

	EditorSceneManager::EditorSceneManager()
	{
	}

	void EditorSceneManager::OnGui()
	{
		if (ImGui::CollapsingHeader("EditorSceneManager##CoreLayer"))
		{
		    ImGui::Text(
            "%s", 
                _runtimeSceneChangeParameters.beginPlaySceneFilePath.GetAsString().c_str()
			);
		}
	}

	void EditorSceneManager::OnEvent(Event* e)
	{
		if (e->GetEventType() == EventType::RuntimeSceneChange)
		{
			OnRuntimeSceneChange(static_cast<EventRuntimeSceneChange*>(e));
		}
	}

	void EditorSceneManager::ChangeScene(const FilePath& filePath)
	{
		{
			if (HasActiveScene())
			{
				// Fire scene deactivated event.
				{
					EventOnSceneDeActivated onSceneDeActivated;
					NOTIFY_EVENT(onSceneDeActivated);
				}

				// Clear command manager.
				{
					CommandRecorder::GetInstance().Clear();
				}
			}
		}

		// Find scene.
		{
			const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

			const std::optional<Ref<Scene>> scene = sceneLibrary->Find(filePath);
			if (not scene.has_value())
			{
				DENTY_SET_ERR_COLOR
				DENTY_ERR_LOG << "Scene filePath " << filePath;

				DENTY_ASSERT(false, "Scene not found! (in EditorSceneManager::ChangeScene)");
			}

			_commonSceneChangeParameters.currentScene = scene.value();
		}

		// Remove scene from library, if new scene.
		{
			if (_editorSceneChangeParameters.isChangeToNewScene)
			{
				SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();
				sceneLibrary->Remove(_commonSceneChangeParameters.currentScene->GetFilePathToSerialized());
			}
		}

		// Runtime
		{
			if (_runtimeSceneChangeParameters.runtimeSceneChange)
			{
				if (_runtimeSceneChangeParameters.reloadScene)
				{
					const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

					const std::optional<Ref<Scene>> scene = sceneLibrary->Find(filePath);
					if (scene.has_value())
					{
						const auto clone = std::make_shared<Scene>();
						scene.value()->Clone(clone);

						_commonSceneChangeParameters.currentScene = clone;
					}
				}
			}
		}

		// Fire scene activated event.
		{
			EventOnSceneActivated eventOnSceneActivated(_commonSceneChangeParameters.currentScene);
			NOTIFY_EVENT(eventOnSceneActivated);
		}

		// Runtime 
		{
			if (_runtimeSceneChangeParameters.runtimeSceneChange)
			{
				EventRuntimeSceneChanged eventRuntimeSceneChanged(_commonSceneChangeParameters.currentScene->GetName());
				NOTIFY_EVENT(eventRuntimeSceneChanged);
			}

		}

		// Reset
		{
			if (not _runtimeSceneChangeParameters.runtimeSceneChange)
			{
			    _runtimeSceneChangeParameters.Reset();
			}
			_editorSceneChangeParameters.Reset();
			_commonSceneChangeParameters.reservedLoadSceneFilePath = FilePath();
		}
	}

	void EditorSceneManager::SetReservedSceneFilePath(const FilePath& filePath, bool isNewScene, bool runtimeSceneChange, bool runtimeReloadScene)
	{
		// Common
		{
			_commonSceneChangeParameters.reservedLoadSceneFilePath = filePath;
		}

		// Editor
		{
			_editorSceneChangeParameters.isChangeToNewScene = isNewScene;
		}

		// Runtime
		{
			_runtimeSceneChangeParameters.runtimeSceneChange = runtimeSceneChange;
			_runtimeSceneChangeParameters.reloadScene = runtimeReloadScene;
		}
	}

	void EditorSceneManager::SetCurrentPlayingScene([[maybe_unused]] Ref<Scene> playingScene)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		//_specification.runtimeSpecification.currentPlayingScene = playingScene;
	}

	void EditorSceneManager::SetBeginPlaySceneFilePath(const FilePath& filePath)
	{
		_runtimeSceneChangeParameters.beginPlaySceneFilePath = filePath;
	}

	Ref<Scene> EditorSceneManager::GetCurrentPlayingScene()
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return nullptr;
		//return _specification.runtimeSpecification.currentPlayingScene;
	}

	const FilePath& EditorSceneManager::GetBeginPlaySceneFilePath()
	{
		return _runtimeSceneChangeParameters.beginPlaySceneFilePath;
	}

	Ref<Scene> EditorSceneManager::GetActiveScene()
	{
		return _commonSceneChangeParameters.currentScene;
	}

	const FilePath& EditorSceneManager::GetReservedSceneFilePath()
	{
		return _commonSceneChangeParameters.reservedLoadSceneFilePath;
	}

	bool EditorSceneManager::HasReservedSceneFilePath()
	{
		return not (_commonSceneChangeParameters.reservedLoadSceneFilePath.IsEmpty());
	}

	bool EditorSceneManager::HasActiveScene()
	{
		return (_commonSceneChangeParameters.currentScene != nullptr);
	}

	void EditorSceneManager::OnRuntimeSceneChange(EventRuntimeSceneChange* e)
	{
		SetReservedSceneFilePath(e->GetFilePath(), false, true, e->IsReloadScene());
	}
}
