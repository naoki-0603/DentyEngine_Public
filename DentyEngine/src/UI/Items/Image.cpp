#include "stdafx.h"

#include "Image.h"

#include "Graphics/Renderer/Texture2D.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"
#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

#include <directxtk/include/directxtk/SpriteBatch.h>

namespace DentyEngine
{
	Rect::Rect() :
		left(), top(), right(), bottom()
	{
	}

	Rect::Rect(int32_t left, int32_t top, int32_t right, int32_t bottom) :
		left(left), top(top), right(right), bottom(bottom)
	{
	}

	UIImageData::UIImageData() :
		clipRect(), textureFilePathToSerialized(), blendType(BlendState::Type::Alpha), useClipRect()
	{
	}

	UIImage::UIImage() :
		UserInterfaceItem(), Object(), _spriteBatch(), _data()
	{
		_name = "UIImage";
	}

	void UIImage::Create()
	{
		Initialize();
	}

	void UIImage::Render()
	{
		if (not _texture)
			return;

		// Get blend state.
		ID3D11BlendState* blendState = BlendState::GetBuildInBlendStatesAt(_data.blendType);

		_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, blendState);

		if (_data.useClipRect)
		{
			const RECT clipRect = { _data.clipRect.left, _data.clipRect.top, _data.clipRect.right, _data.clipRect.bottom };

			_spriteBatch->Draw(
				_texture->GetResourceAsComPtr().Get(),
				transform.position, &clipRect,
				DirectX::Colors::White, transform.rotation,
				transform.pivot.GetPivotOffset(),
				Vector2(transform.scale, transform.scale)
			);
		}
		else
		{
			_spriteBatch->Draw(
				_texture->GetResourceAsComPtr().Get(),
				transform.position, nullptr,
				DirectX::Colors::White, transform.rotation,
				transform.pivot.GetPivotOffset(),
				Vector2(transform.scale, transform.scale)
			);
		}
		
		_spriteBatch->End();
	}

	void UIImage::OnGui()
	{
		// Drag and drop area.
		{
			auto temp = std::string();
			ImGui::TextWrapped("DragAndDropHere!");
			ImGui::InputText("##DragAndDropAreaUIImage", temp.data(), temp.size(), ImGuiInputTextFlags_ReadOnly);

			const DragAndDropSpecification specification =
			{
				"TextureItem", sizeof(TextureID),
				nullptr, DragAndDropDataType::Texture,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(TextureAssetDragAndDropTargetCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropTarget dragAndDropTarget(specification);
		}

		ImGui::Separator();

		ImGui::TextWrapped("UseClipRect");
		ImGui::SameLine();
		ImGui::Checkbox("##UseClipRectUIImage", &_data.useClipRect);

		// Clip position.
		{
			if (_texture && _data.useClipRect)
			{
				const Vector2 textureSize = _texture->GetSize();
				{
					ImGui::Text("TextureSize: %f, %f", textureSize.x, textureSize.y);
				}

				ImGui::TextWrapped("ClipRectLeft");
				ImGui::SameLine();
				ImGui::InputInt("##clipRectUIImage", &_data.clipRect.left);

				ImGui::TextWrapped("ClipRectTop");
				ImGui::SameLine();
				ImGui::InputInt("##ClipRectTopUIImage", &_data.clipRect.top);

				ImGui::TextWrapped("ClipRectRight");
				ImGui::SameLine();
				ImGui::InputInt("##ClipRectRightUIImage", &_data.clipRect.right);

				ImGui::TextWrapped("ClipRectBottom");
				ImGui::SameLine();
				ImGui::InputInt("##ClipRectBottomUIImage", &_data.clipRect.bottom);

				ImGui::Separator();
			}
		}


		UserInterfaceItem::OnGui();
	}

	void UIImage::OnUpdate()
	{
	}

	void UIImage::OnSerialized() const
	{
		Object::OnSerialized();
	}

	void UIImage::OnDeserialized()
	{
		Object::OnDeserialized();
	}

	void UIImage::OnSceneActivated()
	{
		Initialize();
	}

	void UIImage::OnSceneDeActivated()
	{
	}

	void UIImage::OnRuntimeSceneChanged()
	{
	}

	void UIImage::SetTexture(Ref<Texture2D> texture)
	{
		_texture = texture;
		_data.textureFilePathToSerialized = texture->GetFilePathToSerialized();
	}

	void UIImage::SetClipPosition(int32_t left, int32_t top, int32_t right, int32_t bottom)
	{
		_data.clipRect = Rect(left, top, right, bottom);
	}

	const char* UIImage::GetItemType() const
	{
		return "Image";
	}

	Vector2 UIImage::GetSize() const
	{
		if (not _texture)
			return Vector2();

		return _texture->GetSize();
	}

	Vector4I UIImage::GetClipPosition() const
	{
		return Vector4I(
			_data.clipRect.left, _data.clipRect.top, _data.clipRect.right, _data.clipRect.bottom
		);
	}

	Vector2 UIImage::GetCenterOfImage() const
	{
		const Vector2 textureSize = GetSize();
		if (textureSize.IsZero())
		{
			return textureSize;
		}

		return Vector2(
			transform.position.x + (textureSize.x * 0.5f),
			transform.position.y + (textureSize.y * 0.5f)
		);
	}

	const FilePath& UIImage::GetTextureFilePath() const
	{
		return _data.textureFilePathToSerialized;
	}

	void UIImage::Initialize()
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		if (_spriteBatch)
			_spriteBatch.reset();

		_spriteBatch = std::make_shared<DirectX::DX11::SpriteBatch>(context);

		if (_data.textureFilePathToSerialized.IsValid())
		{
			const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

			const std::optional<Ref<Texture2D>> texture2d = textureLibrary->Find<Texture2D>(_data.textureFilePathToSerialized);
			if (texture2d.has_value())
			{
				_texture = texture2d.value();
			}
		}
	}

	void UIImage::TextureAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_CHECK_CONDITION(dataType == DragAndDropDataType::Texture)

		const auto id = *static_cast<TextureID*>(data);

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const std::optional<Ref<Texture2D>> texture2d = textureLibrary->Find<Texture2D>(id);
		if (texture2d.has_value())
		{
			_texture = texture2d.value();
			_data.textureFilePathToSerialized = _texture->GetFilePathToSerialized();
		}
		else
		{
		}
	}
}
