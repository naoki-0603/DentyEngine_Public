#include "stdafx.h"

#include "LayerManager.h"

#include "Timer.h"

#include "../Layer/ApplicationLayer.h"
#include "../Layer/EditorLayer.h"
#include "../Layer/CoreLayer.h"

namespace DentyEngine
{
	void LayerManager::Initialize()
	{
		// This order is valid only at initialization.
		const auto coreLayer = new CoreLayer(true);
		const auto editorLayer = new EditorLayer(true);
		const auto applicationLayer = new ApplicationLayer(true);

		_layerStack->PushLayer(coreLayer);
		_layerStack->PushLayer(editorLayer);
		_layerStack->PushLayer(applicationLayer);

		_layerStack->OnAttach();
	}

	void LayerManager::OnLaunch()
	{
		Initialize();
	}

	void LayerManager::OnTerminate()
	{
		Finalize();
	}

	void LayerManager::Gui()
	{
		OnGui();
	}

	void LayerManager::Update()
	{
		_layerStack->OnUpdate(Timer::GetDeltaTime());
	}

	void LayerManager::LateUpdate()
	{
		_layerStack->OnLateUpdate(Timer::GetDeltaTime());
	}

	void LayerManager::FixedUpdate()
	{
		_layerStack->OnFixedUpdate();
	}

	void LayerManager::OnGui()
    {
		_layerStack->OnGui();
    }

    void LayerManager::Finalize()
	{
		_layerStack->OnDetach();
	}

    void LayerManager::OnEvent(Event* e)
	{
		_layerStack->OnEvent(e);
	}
};