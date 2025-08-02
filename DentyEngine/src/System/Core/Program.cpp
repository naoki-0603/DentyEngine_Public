#include "stdafx.h"

#include "Program.h"

#include "Project.h"
#include "RuntimeSceneManager.h"
#include "Timer.h"
#include "Window.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Events/EventWindow.h"
#include "System/Events/EventRender.h"

#include "System/Layer/ApplicationLayer.h"

#include "System/Layer/EditorLayer.h"

#include "System/Profiler/FrameState.h"
#include "System/Profiler/MeasuredData.h"

#include "System/Core/System.h"

#include "System/Editor/EditorSceneManager.h"

namespace DentyEngine
{
	bool Program::_created = false;

	Program::Program() :
		_layerManager(std::make_unique<LayerManager>()),
		_name(), _running()
	{
	}

	Program* Program::Create()
	{
		DENTY_ASSERT(not _created, "Program already created.");

		_created = true;

		return new Program;
	}

	void Program::Launch(
		std::string_view applicationName,
		HINSTANCE hInstance,
		const Vector2& size)
	{
		[[maybe_unused]] NotifyEvent notifyEvent(BIND_EVENT_FUNCTION(Program::OnEvent));

		System::OnLaunch();

		ConsoleLogger::GetInstance().OnLaunch();

		Window::Create(applicationName, hInstance, size);

		Project::OnLaunch();

		Timer::Initialize();

		_layerManager->OnLaunch();

		RuntimeSceneManager::OnLaunch();

	    _running = true;
	}

	void Program::Run()
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();
		IDXGISwapChain1* swapChain = GraphicsManager::GetInstance().GetSwapChain();
		ID3D11RenderTargetView* mainRtv = GraphicsManager::GetInstance().GetRenderTargetView();

		const D3D11_VIEWPORT& mainViewport = GraphicsManager::GetInstance().GetViewport();

		// Main loop.
		while (_running)
		{
			FrameStateSpecification specification =
			{
				"Program::Run"
			};
			FrameState frameState(specification);

			// Begin Frame
			{
				Timer::BeginFrame();
			}

			// Editor scene manager change scene.
			{
				if (EditorSceneManager::HasReservedSceneFilePath())
				{
					EditorSceneManager::ChangeScene(EditorSceneManager::GetReservedSceneFilePath());
				}
			}

			// Begin Fixed update
			{
				if (ApplicationLayer::IsPlay())
				{
					
					_layerManager->FixedUpdate();
				}
			}

			// Begin Update
			{
				_layerManager->Update();
			}

			// Late update
			{
				_layerManager->LateUpdate();
			}

			// Rendering process.
			{
				// For editor view.
				{
					EventSceneRendering sceneViewRendering(context, mainRtv, mainViewport, EventSceneRendering::ViewType::Scene);
					NOTIFY_EVENT(sceneViewRendering);
				}

				// For game view.
				{
					EventSceneRendering sceneGameRendering(context, mainRtv, mainViewport, EventSceneRendering::ViewType::Game);
					NOTIFY_EVENT(sceneGameRendering);
				}
			}

			// Editor GUI
			{
				EditorLayer::Begin();

				Timer::Gui();

				_layerManager->Gui();

				EditorLayer::End();
			}

			// Present
			{
				EventPresent present(swapChain);
				NOTIFY_EVENT(present);
			}

			{
				Input::ResetMouseWheelSpeed();
			}

			// Update window
			{
				Window::OnUpdate();
			}

			// End Frame
			{
				Timer::EndFrame();

				// Calculate delta time.
				Timer::Update();

				// Clear measured data.
				MeasuredData::GetInstance().Clear();
			}
		}
	}

	void Program::Terminate()
	{
		_layerManager->OnTerminate();

		ConsoleLogger::GetInstance().OnTerminate();
	}

	void Program::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventWindowClosed>(BIND_EVENT_FUNCTION(Program::OnWindowClosed));
		dispatcher.Dispatch<EventWindowResized>(BIND_EVENT_FUNCTION(Program::OnWindowResized));

		if (not e->handled || e->listenerMultipleExists)
		{
			_layerManager->OnEvent(e);
		}
	}

	void Program::OnWindowClosed([[maybe_unused]] EventWindowClosed* e)
	{
		_running = false;
	}

	void Program::OnWindowResized([[maybe_unused]] EventWindowResized* e)
	{

	}
};