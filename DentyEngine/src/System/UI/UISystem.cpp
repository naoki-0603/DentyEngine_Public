
#include "stdafx.h"

#include "UISystem.h"

#include "UserInterfaceItem.h"
#include "UI/Items/Text.h"

#include "System/Core/Window.h"

#include "System/Events/Event.h"
#include "System/Events/EventScene.h"
#include "System/Events/EventSystem.h"
#include "System/Events/EventRender.h"
#include "UI/Items/Image.h"

namespace DentyEngine
{
	UISystemSpecification::UISystemSpecification() :
		items(), canvasSize()
	{
	}

	UISystemSpecification UISystem::_specification = { };

	UISystem::UISystem()
	{
	}

	void UISystem::Initialize()
	{
		_specification.canvasSize = Window::GetInfo().GetSize<Vector2>();

		// Resize
		{
			_specification.items.try_emplace(UserInterfaceItemRenderingOrder::DistanceView, std::vector<Ref<UserInterfaceItem>>());
			_specification.items.try_emplace(UserInterfaceItemRenderingOrder::MiddleDistance, std::vector<Ref<UserInterfaceItem>>());
			_specification.items.try_emplace(UserInterfaceItemRenderingOrder::CloseView, std::vector<Ref<UserInterfaceItem>>());
		}
	}

	void UISystem::OnEvent(Event* e)
	{
		switch (e->GetEventType())
		{
		case EventType::SceneActivated:
			OnSceneActivated(static_cast<EventOnSceneActivated*>(e));
			break;
		case EventType::SceneDeActivated:
			OnSceneDeActivated(static_cast<EventOnSceneDeActivated*>(e));
			break;
		case EventType::RuntimeSceneChanged:
			OnRuntimeSceneChanged(static_cast<EventRuntimeSceneChanged*>(e));
			break;
		case EventType::GameViewSizeChanged:
			OnGameViewSizeChanged(static_cast<EventOnGameViewSizeChanged*>(e));
			break;
		}
	}

	void UISystem::AddItem(Ref<UserInterfaceItem> item)
	{
		_specification.items.at(item->GetRenderingOrder()).emplace_back(item);
	}

	void UISystem::RemoveItem(const Ref<UserInterfaceItem>& item)
	{
		auto&& it = _specification.items.find(item->GetRenderingOrder());
		if (it != _specification.items.end())
		{
			it->second.erase(std::ranges::find(it->second, item));
		}
	}

	void UISystem::Clear()
	{
		for (auto&& items : _specification.items | std::views::values)
		{
			items.clear();
		}
	}

	void UISystem::OnSceneRender()
	{
		for (uint32_t renderingOrderIndex = 0; renderingOrderIndex < static_cast<uint32_t>(UserInterfaceItemRenderingOrder::Max); ++renderingOrderIndex)
		{
			auto&& items = _specification.items.at(static_cast<UserInterfaceItemRenderingOrder>(renderingOrderIndex));

			for (auto&& item : items)
			{
				if (not item->IsRender())
					continue;

				if (item->GetItemType() == std::string("Text"))
				{
					RenderText(std::reinterpret_pointer_cast<UIText>(item));
				}
				else if (item->GetItemType() == std::string("Image"))
				{
					RenderImage(std::reinterpret_pointer_cast<UIImage>(item));
				}
			}
		}
	}

	const Vector2& UISystem::GetCanvasSize()
	{
		return _specification.canvasSize;
	}

	void UISystem::OnSceneActivated([[maybe_unused]] EventOnSceneActivated* e)
	{
	}

	void UISystem::OnSceneDeActivated([[maybe_unused]] EventOnSceneDeActivated* e)
	{
		Clear();
	}

	void UISystem::OnRuntimeSceneChanged([[maybe_unused]] EventRuntimeSceneChanged* e)
	{
	}

	void UISystem::OnGameViewSizeChanged(EventOnGameViewSizeChanged* e)
	{
		_specification.canvasSize = e->GetSize();

		Resize();
	}

	void UISystem::Resize()
	{
		for (uint32_t renderingOrderIndex = 0; renderingOrderIndex < static_cast<uint32_t>(UserInterfaceItemRenderingOrder::Max); ++renderingOrderIndex)
		{
			auto&& items = _specification.items.at(static_cast<UserInterfaceItemRenderingOrder>(renderingOrderIndex));

			for (auto&& item : items)
			{
				item->transform.OnResize(_specification.canvasSize);
			}
		}
	}

	void UISystem::RenderText(const Ref<UIText>& text)
	{
		text->Render();
	}

	void UISystem::RenderImage(const Ref<UIImage>& image)
	{
		image->Render();
	}
}
