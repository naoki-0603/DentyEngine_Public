#include "stdafx.h"

#include "UserInterfaceItem.h"

#include "UISystem.h"
#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	const std::array<std::string, 3u> UserInterfaceItem::RENDERING_ORDER_TYPES =
	{
		"DistanceView", "MiddleDistance", "CloseView"
	};

	UserInterfaceItem::UserInterfaceItem() :
		transform(this), _renderingOrder(UserInterfaceItemRenderingOrder::DistanceView),
		_render(true), _renderingOrderString(std::string(NAMEOF_ENUM(_renderingOrder)))
	{

	}

	void UserInterfaceItem::OnGui()
	{
		// Is render?
		{
			ImGui::TextWrapped("Render");
			ImGui::SameLine();
			ImGui::Checkbox("##RenderUserInterfaceItem", &_render);
		}

		// Rendering order.
		{
			ImGui::TextWrapped("RenderingOrder");
			ImGui::SameLine();

			if (not GuiUtils::ComboBoxGui("##RenderingOrderUserInterfaceItem",
				RENDERING_ORDER_TYPES.data(), static_cast<int32_t>(RENDERING_ORDER_TYPES.size()), _renderingOrderString
			))
			{
				bool isChanged = false;
				if (_renderingOrderString == std::string("DistanceView"))
				{
					if (_renderingOrderString != std::string(NAMEOF_ENUM(_renderingOrder)))
					{
						isChanged = true;
						UISystem::RemoveItem(shared_from_this());
					}

					_renderingOrder = UserInterfaceItemRenderingOrder::DistanceView;

					if (isChanged)
					{
						UISystem::AddItem(shared_from_this());
					}
				}
				else if (_renderingOrderString == std::string("MiddleDistance"))
				{
					if (_renderingOrderString != std::string(NAMEOF_ENUM(_renderingOrder)))
					{
						isChanged = true;

						UISystem::RemoveItem(shared_from_this());
					}

					_renderingOrder = UserInterfaceItemRenderingOrder::MiddleDistance;

					if (isChanged)
					{
						UISystem::AddItem(shared_from_this());
					}
				}
				else if (_renderingOrderString == std::string("CloseView"))
				{
					if (_renderingOrderString != std::string(NAMEOF_ENUM(_renderingOrder)))
					{
						isChanged = true;

						UISystem::RemoveItem(shared_from_this());
					}

					_renderingOrder = UserInterfaceItemRenderingOrder::CloseView;

					if (isChanged)
					{
						UISystem::AddItem(shared_from_this());
					}
				}
			}
		}

		// Transform
		if (ImGui::CollapsingHeader("Status##UserInterfaceItem", ImGuiTreeNodeFlags_DefaultOpen))
		{
			transform.OnGui();

			const Vector2 size = GetSize();
			ImGui::TextWrapped("ItemSize: %f, %f", size.x, size.y);
		}
	}
}
