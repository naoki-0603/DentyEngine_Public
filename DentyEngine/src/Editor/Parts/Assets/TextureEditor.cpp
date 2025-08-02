#include "stdafx.h"

#include "TextureEditor.h"

#include "Graphics/Renderer/Texture.h"
#include "Graphics/Renderer/Texture2D.h"

#include "System/Events/EventEditor.h"

namespace DentyEngine
{
	TextureEditor::TextureEditor() :
		IEditor(), AssetEditor()
	{
	}

	void TextureEditor::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnTextureEditorOpened>(BIND_EVENT_FUNCTION(OnOpened));
		dispatcher.Dispatch<EventOnTextureEditorClosed>(BIND_EVENT_FUNCTION(OnClosed));
	}

	void TextureEditor::OnGui(Ref<Scene> activeScene)
	{
		if (not _open)
			return;

		if (ImGui::Begin("TextureEditor##AssetEditor", &_open, IMGUI_DEFAULT_WINDOW_FLAGS))
		{
			// TextureID
			ImGui::Text("TextureID: %u", _texture->GetTextureID());

			const std::string filePath = _texture->GetFilePath().GetAsString();
			ImGui::Text("FilePath: %s", filePath.c_str());

			const std::string filePathToSerialized = _texture->GetFilePathToSerialized().GetAsString();
			ImGui::Text("FilePathToSerialized: %s", filePathToSerialized.c_str());

			ImGui::Separator();

			// Display image.
			switch (_texture->GetType())
			{
			case Texture::Type::Texture2D:
				{
					const auto& texture2d = std::static_pointer_cast<Texture2D>(_texture);
					texture2d->ExpandedImageGui(Vector2(256, 256));
				}
				break;
			case Texture::Type::CubeMap:
				{
					
				}
				break;
			}
		}

		ImGui::End();

		// If closed.
		if (not _open)
		{
			EventOnTextureEditorClosed eventOnTextureEditorClosed;
			NOTIFY_EVENT(eventOnTextureEditorClosed);
		}
	}

	void TextureEditor::OnOpened(EventOnTextureEditorOpened* e)
	{
		_open = true;
		_texture = e->GetTexture();

		ImGui::SetNextWindowSize(ImVec2(IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y));
	}

	void TextureEditor::OnClosed([[maybe_unused]] EventOnTextureEditorClosed* e)
	{
		_open = false;
		_texture = nullptr;
	}
}
