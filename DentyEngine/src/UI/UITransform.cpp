#include "stdafx.h"

#include "UITransform.h"

#include "System/UI/UISystem.h"

namespace DentyEngine
{
	UITransform::UITransform(const UserInterfaceItem* item) :
		Object(), pivot(item), position(), scale(1.0f), rotation()
	{
		_name = "UITransform";
	}

	void UITransform::OnGui()
	{
		pivot.OnGui();

		const Vector2 canvasSize = UISystem::GetCanvasSize();
		
		ImGui::SliderFloat("PositionX##UITransform", &position.x, -64.0f, canvasSize.x);
		ImGui::SliderFloat("PositionY##UITransform", &position.y, -64.0f, canvasSize.y);
		ImGui::SliderFloat("Scale##UITransform", &scale, -10.0f, +10.0f);
		ImGui::SliderAngle("Rotation##UITransform", &rotation);

		ImGui::Separator();

		ImGui::TextWrapped("CanvasSize: %f, %f", canvasSize.x, canvasSize.y);
		ImGui::TextWrapped("PositionRate: %f, %f", _positionRate.x, _positionRate.y);

		_positionRate = { position.x / canvasSize.x, position.y / canvasSize.y };
	}

	void UITransform::OnSerialized() const
	{
		Object::OnSerialized();
	}

	void UITransform::OnDeserialized()
	{
		Object::OnDeserialized();
	}

	void UITransform::OnResize(const Vector2& size)
	{
		const Vector2 fixedPosition = size * _positionRate;

		position = fixedPosition;
	}
}
