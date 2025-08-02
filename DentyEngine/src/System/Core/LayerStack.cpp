#include "stdafx.h"

#include "LayerStack.h"

#include "System/Layer/Layer.h"

#include "System/Events/EventSystem.h"

namespace DentyEngine
{
	void LayerStack::OnAttach()
	{
		for (auto&& layer : _layers)
		{
			if (!layer->Enabled()) continue;

			layer->OnAttach();
		}

		EventLayerInitialized eventLayerInitialized;
		NOTIFY_EVENT(eventLayerInitialized);
	}

	void LayerStack::OnUpdate(const float deltaTime)
	{
		for (auto rit = _layers.rbegin(); rit != _layers.rend(); ++rit)
		{
			if (!(*rit)->Enabled()) continue;

			(*rit)->OnUpdate(deltaTime);
		}
	}

	void LayerStack::OnLateUpdate(float deltaTime)
	{
		for (auto rit = _layers.rbegin(); rit != _layers.rend(); ++rit)
		{
			if (!(*rit)->Enabled())
				continue;

			(*rit)->OnLateUpdate(deltaTime);
		}
	}

	void LayerStack::OnFixedUpdate()
	{
		for (auto rit = _layers.rbegin(); rit != _layers.rend(); ++rit)
		{
			if (!(*rit)->Enabled()) continue;

			(*rit)->OnFixedUpdate();
		}
	}

	void LayerStack::OnGui()
    {
		for (auto rit = _layers.rbegin(); rit != _layers.rend(); ++rit)
		{
		    if (!(*rit)->Enabled()) continue;

			(*rit)->OnGui();
		}
    }

    void LayerStack::OnDetach()
	{
		for (auto rit = _layers.rbegin(); rit != _layers.rend(); ++rit)
		{
			if ((*rit)->Enabled())
			{
				(*rit)->OnDetach();
			}

			delete (*rit);
		}
	}

	void LayerStack::OnEvent(Event* e)
	{
		for (auto rit = _layers.rbegin(); rit != _layers.rend(); ++rit)
		{
			if (!(*rit)->Enabled()) continue;

			(*rit)->OnEvent(e);
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		_layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto it = std::ranges::find(std::as_const(_layers), layer);

		if (it != _layers.end())
		{
			(*it)->OnDetach();
			_layers.erase(it);
		}
	}

	const std::vector<Layer*>::iterator LayerStack::Begin()
	{
		return _layers.begin();
	}

	const std::vector<Layer*>::iterator LayerStack::End()
	{
		return _layers.end();
	}
};