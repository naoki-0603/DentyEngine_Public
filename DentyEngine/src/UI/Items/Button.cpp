#include "stdafx.h"

#include "Button.h"

#include "System/Core/ID/TextureID.h"
#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

namespace DentyEngine
{
	UIButtonData::UIButtonData() :
		textureFilePathToSerialized()
	{
	}

	UIButton::UIButton() :
		UserInterfaceItem(), ISelectableUserInterfaceItem(), Object(),
		_spriteBatch(), _texture(), _data()
	{
		_name = "UIButton";
	}

	void UIButton::Create()
	{
		Initialize();
	}

	void UIButton::Render()
	{
	}

	void UIButton::OnGui()
	{
		// Drag and drop area.
		{
			auto temp = std::string();
			ImGui::TextWrapped("DragAndDropHere!");
			ImGui::InputText("##DragAndDropAreaUIButton", temp.data(), temp.size(), ImGuiInputTextFlags_ReadOnly);

			const DragAndDropSpecification specification =
			{
				"TextureItem", sizeof(TextureID),
				nullptr, DragAndDropDataType::Texture,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(TextureAssetDragAndDropTargetCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropTarget dragAndDropTarget(specification);
		}

		UserInterfaceItem::OnGui();
	}

	void UIButton::OnUpdate()
	{
	}

	void UIButton::OnSerialized() const
	{
		Object::OnSerialized();
	}

	void UIButton::OnDeserialized()
	{
		Object::OnDeserialized();
	}

	void UIButton::OnSceneActivated()
	{
	}

	void UIButton::OnSceneDeActivated()
	{
	}

	void UIButton::OnRuntimeSceneChanged()
	{
	}

	void UIButton::OnSelected()
	{

	}

	const char* UIButton::GetItemType() const
	{
		return "Button";
	}

	Vector2 UIButton::GetSize() const
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return {};
	}

	void UIButton::Initialize()
	{

	}

	void UIButton::TextureAssetDragAndDropTargetCallback([[maybe_unused]] void* data, [[maybe_unused]] DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
	}
}
