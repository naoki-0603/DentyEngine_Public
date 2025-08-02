#include "stdafx.h"

#include "ApplicationLayer.h"

#include "System/Editor/EditorSceneManager.h"

#include "System/Events/EventEditor.h"
#include "System/Events/EventSystem.h"
#include "System/Events/EventScene.h"
#include "System/Graphics/Renderer/CPUParticleSystem.h"

#include "System/Graphics/Renderer/RenderManager.h"

#include "System/Scene/Scene.h"

#include "System/Scripting/ScriptEngine.h"

namespace DentyEngine
{
	ApplicationLayer::State ApplicationLayer::_state = State::Edit;

	void ApplicationLayer::OnAttach()
	{
		_initialized = true;
	}

	void ApplicationLayer::OnDetach()
	{
	}

	void ApplicationLayer::OnUpdate(const float deltaTime)
	{
		if (_activeScene)
		{
			if (IsPause())
				return;

			_activeScene->OnUpdate(deltaTime);
		}
	}

	void ApplicationLayer::OnLateUpdate(float deltaTime)
	{
		if (_activeScene)
		{
			if (IsPause())
				return;

			_activeScene->OnLateUpdate(deltaTime);
		}
	}

	void ApplicationLayer::OnFixedUpdate()
	{
		if (_activeScene)
		{
			if (IsPause())
				return;

			if (IsPlay())
			{
				_activeScene->OnFixedUpdate();
			}
		}
	}

	void ApplicationLayer::OnGui()
    {
		if (ImGui::Begin("ApplicationLayer##Layer", &_open))
		{
			ImGui::Text("State: %s", std::string(NAMEOF_ENUM(_state)).c_str());

			ImGui::Text("CurrentPlayingSceneName: %s", _currentPlayingSceneName.c_str());
		}

		ImGui::End();

    }

	void ApplicationLayer::OnEvent(Event* e)
	{
		Layer::OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnSceneActivated>(BIND_EVENT_FUNCTION(OnSceneActivated));

		dispatcher.Dispatch<EventOnScenePlayButtonPressed>(BIND_EVENT_FUNCTION(OnScenePlayButtonPressed));
		dispatcher.Dispatch<EventOnSceneResumeButtonPressed>(BIND_EVENT_FUNCTION(OnSceneResumeButtonPressed));
		dispatcher.Dispatch<EventOnScenePauseButtonPressed>(BIND_EVENT_FUNCTION(OnScenePauseButtonPressed));

		if ((!e->handled || e->listenerMultipleExists))
		{
			if (_activeScene)
			{
			    _activeScene->OnEvent(e);
			}
		}

		dispatcher.Dispatch<EventOnSceneDeActivated>(BIND_EVENT_FUNCTION(OnSceneDeactivated));
	}

	void ApplicationLayer::OnSceneActivated(EventOnSceneActivated* e)
	{
		e->SetScene(_activeScene);

		_currentPlayingSceneName = _activeScene->GetName();
	}

	void ApplicationLayer::OnSceneDeactivated([[maybe_unused]] EventOnSceneDeActivated* e)
	{
		_activeScene = nullptr;
	}

	void ApplicationLayer::OnScenePlayButtonPressed([[maybe_unused]] EventOnScenePlayButtonPressed* e)
	{
		// Play
		if (_state == State::Play)
		{
			// Replace scene library scene.
			//SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();
			//sceneLibrary->Replace(_currentPlayingScene->GetFilePathToSerialized(), _currentPlayingScene);

			// Clear physics, light and render manager.
			// CommandRecorder::GetInstance().Clear();
			// PhysicsEngine::GetInstance()->ClearAll();
			// LightManager::GetInstance().Reset();
			// RenderManager::GetInstance().Reset();

			//// Runtime scene changed.
			//if (EditorSceneManager::HasReservedSceneFilePath())
			//{
			//	EditorSceneManager::SetReservedSceneFilePath(EditorSceneManager::GetBeginPlaySceneFilePath(), false, false, false);
			//}
			//else

			// Change scene.
			{
				const FilePath& filePath = EditorSceneManager::GetBeginPlaySceneFilePath();

				EditorSceneManager::SetReservedSceneFilePath(filePath, false, false, false);
			}

			// Fire event
			{
				EventOnApplicationStateChangedToEdit eventOnApplicationStateChangedToEdit;
				NOTIFY_EVENT(eventOnApplicationStateChangedToEdit);
			}

			_state = State::Edit;
		}
		// Edit
		else
		{
			// Set current playing scene.
			EditorSceneManager::SetBeginPlaySceneFilePath(_activeScene->GetFilePathToSerialized());

			// ScriptEngine
			ScriptEngine::OnScenePlayButtonPressed();

			// ParticleSystems
			{
				CPUParticleSystem::GetInstance()->OnScenePlayButtonPressed();
			}

			// Fire event
			EventOnApplicationStateChangedToPlay eventOnApplicationStateChangedToPlay;
			NOTIFY_EVENT(eventOnApplicationStateChangedToPlay);

			_state = State::Play;
		}
	}

	void ApplicationLayer::OnSceneResumeButtonPressed([[maybe_unused]] EventOnSceneResumeButtonPressed* e)
	{
		_state = State::Play;
	}

	void ApplicationLayer::OnScenePauseButtonPressed([[maybe_unused]] EventOnScenePauseButtonPressed* e)
	{
		_state = State::Pause;
	}
};