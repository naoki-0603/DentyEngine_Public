#include "stdafx.h"

#include "CoreLayer.h"

#include "ApplicationLayer.h"

#include "System/Core/RuntimeSceneManager.h"
#include "System/Core/Timer.h"
#include "System/Core/Window.h"

#include "System/Core/Setting/GlobalSettingsManager.h"

#include "System/Core/UndoRedo/CommandRecorder.h"

#include "../Graphics/GraphicsManager.h"

#include "System/Editor/EditorSceneManager.h"

#include "System/Resource/Library/LibraryManager.h"

#include "System/Input/Input.h"

#include "System/Scripting/ScriptEngine.h"

#include "System/UI/UISystem.h"

#include "System/Graphics/Renderer/ParticleSystem.h"
#include "System/Graphics/Renderer/RenderManager.h"
#include "System/Graphics/Renderer/CPUParticleSystem.h"

#include "System/Physics/PhysicsEngine.h"

namespace DentyEngine
{
	void CoreLayer::OnAttach()
	{
		// Graphics manager
		{
			GraphicsManager::GetInstance().Initialize();
			GraphicsManager::GetInstance().GenerateBuildIn();
		}

		// GlobalSettings.
		{
		    GlobalSettingsManager::Initialize();
		}

		// ScriptEngine.
		{
		    ScriptEngine::Initialize();
		}

		// Library manager.
		{
			LibraryManager::GetInstance().Initialize();
		}

		// Physics engine.
		{
			PhysicsEngine::GetInstance()->Initialize();
		}

		// Rendering managers.
		{
			RenderManager::GetInstance().Initialize();
			LightManager::GetInstance().Initialize();
		}

		// CPU ParticleSystem
		{
			CPUParticleSystem::GetInstance()->Initialize();
		}

		// UI System
		{
			UISystem::Initialize();
		}

		// Particle system
		{
			ParticleSystem::Initialize();
		}

		_initialized = true;
	}

	void CoreLayer::OnUpdate(const float deltaTime)
	{
		// Input
		{
			Input::Update();
		}

		// Library manager.
		{
			LibraryManager::GetInstance().Update();
		}

		// Physics engine.
		{
			PhysicsEngine::GetInstance()->OnUpdate();

			if (ApplicationLayer::IsPlay())
			{
				float fixedTime = deltaTime;
				if (Timer::GetFPS() > (Timer::GetDisplayFrequency() + 5) || Timer::GetFPS() < (Timer::GetDisplayFrequency() - 5))
				{
				    fixedTime = 1.0f / static_cast<float>(Timer::GetDisplayFrequency());
				}
				PhysicsEngine::GetInstance()->StepSimulate(fixedTime);
			}
		}

		// Rendering managers.
		{
			RenderManager::GetInstance().Update();
			LightManager::GetInstance().Update();
		}

		// CPU ParticleSystem
		{
			CPUParticleSystem::GetInstance()->OnUpdate(deltaTime);
		}

		// Command recorder.
		{
			CommandRecorder::GetInstance().OnUpdate(deltaTime);
		}

		// Script engine.
		{
			ScriptEngine::OnUpdate();

			if (ScriptEngine::IsRequestedReload())
			{
				ScriptEngine::OnRequestedReload();
			}
		}

		// Global setting.
		{
		    GlobalSettingsManager::Update();
		}
	}

	void CoreLayer::OnLateUpdate([[maybe_unused]] float deltaTime)
	{
		// Script engine
		{
			ScriptEngine::OnLateUpdate();
		}
	}

	void CoreLayer::OnFixedUpdate()
	{
		// Script engine
		{
			ScriptEngine::OnFixedUpdate();
		}
	}

	void CoreLayer::OnGui()
    {
		if (ImGui::Begin("Managers##CoreLayer"))
		{
			// Library manager.
			{
				LibraryManager::GetInstance().OnGui();
			}

			// Graphics manager.
			{
				GraphicsManager::GetInstance().OnGui();
			}

			// Rendering managers.
			{
				ParticleSystem::OnGui();
				RenderManager::GetInstance().OnGui();
				LightManager::GetInstance().OnGui();
			}

			// CPU Particle system
			{
				CPUParticleSystem::GetInstance()->OnGui();
			}

			// Physics engine.
			{
				PhysicsEngine::GetInstance()->OnGui();
			}

			// Editor scene manager.
			{
				EditorSceneManager::OnGui();
			}

			// Command recorder.
			{
				CommandRecorder::GetInstance().OnGui();
			}

			// Input 
			{
				Input::OnGui();
			}

			// Window
			{
				Window::OnGui();
			}

			// Runtime
			{
				RuntimeSceneManager::OnGui();
			}

			// Script engine.
			{
				ScriptEngine::OnGui();
			}
		}

		ImGui::End();
    }

    void CoreLayer::OnDetach()
	{
		// Library manager.
		{
			LibraryManager::GetInstance().Finalize();
		}

		// Graphics manager.
		{
			GraphicsManager::GetInstance().Finalize();
		}

		// Rendering managers.
		{
			RenderManager::GetInstance().Finalize();
			LightManager::GetInstance().Finalize();
		}

		// Physics engine.
		{
			PhysicsEngine::GetInstance()->Finalize();
		}

		// ScriptEngine
		{
		    ScriptEngine::Finalize();
		}

		// GlobalSettings.
		{
		    GlobalSettingsManager::Finalize();
		}

		// Particle system.
		{
		    ParticleSystem::Finalize();
		}
	}

	void CoreLayer::OnEvent(Event* e)
	{
		// Library manager.
		{
			LibraryManager::GetInstance().OnEvent(e);
		}

		// Rendering managers.
		{
			RenderManager::GetInstance().OnEvent(e);
			LightManager::GetInstance().OnEvent(e);
		}

		// Physics engine.
		{
			PhysicsEngine::GetInstance()->OnEvent(e);
		}

		// ScriptEngine
		{
		    ScriptEngine::OnEvent(e);
		}

		// EditorSceneManager
		{
			EditorSceneManager::OnEvent(e);
		}


		// UISystem
		{
			UISystem::OnEvent(e);
		}
	}
};