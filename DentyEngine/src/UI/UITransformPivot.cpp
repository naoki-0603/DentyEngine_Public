#include "stdafx.h"

#include "UITransformPivot.h"

#include "System/UI/UserInterfaceItem.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	const std::array<std::string, 5u> UITransformPivot::PIVOT_TYPES =
	{
		"TopLeft", "TopRight", "BottomLeft", "BottomRight", "Center"
	};

	UITransformPivot::UITransformPivot(const UserInterfaceItem* item) :
		_itemRef(item), _pivot(Pivot::TopLeft), _pivotString(std::string(NAMEOF_ENUM(_pivot)))
	{
	}

	void UITransformPivot::OnGui()
	{
		ImGui::TextWrapped("Pivot");
		ImGui::SameLine();

		if (not GuiUtils::ComboBoxGui("##PivotUITransformPivot", PIVOT_TYPES.data(), static_cast<int32_t>(PIVOT_TYPES.size()), _pivotString))
		{
			if (_pivotString == std::string("TopLeft"))
			{
				_pivot = Pivot::TopLeft;
			}
			else if (_pivotString == std::string("TopRight"))
			{
				_pivot = Pivot::TopRight;
			}
			else if (_pivotString == std::string("BottomLeft"))
			{
				_pivot = Pivot::BottomLeft;
			}
			else if (_pivotString == std::string("BottomRight"))
			{
				_pivot = Pivot::BottomRight;
			}
			else if (_pivotString == std::string("Center"))
			{
				_pivot = Pivot::Center;
			}
		}
	}

	void UITransformPivot::SetPivot(Pivot pivot)
	{
		_pivot = pivot;
	}

	Vector2 UITransformPivot::GetPivotOffset() const
	{
		const Vector2 size = _itemRef->GetSize();

		switch (_pivot)
		{
		case Pivot::TopLeft: return Vector2(0.0f, 0.0f);
		case Pivot::TopRight: return Vector2(size.x, 0.0f);
		case Pivot::BottomLeft: return Vector2(0.0f, size.y);
		case Pivot::BottomRight: return size;
		case Pivot::Center: return Vector2(size.x * 0.5f, size.y * 0.5f);
		}

		DENTY_ERR_CONSOLE_LOG("Unknown pivot type! (in UITransformPivot::GetPivotOffset)");

		return Vector2(0.0f, 0.0f);
	}
}
