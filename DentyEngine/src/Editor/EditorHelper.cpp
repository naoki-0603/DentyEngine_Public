#include "stdafx.h"

#include "EditorHelper.h"

#include "Graphics/Renderer/Texture.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

namespace DentyEngine
{
	const Vector2 EditorHelper::TEXTURE_THUMBNAIL_SIZE = { 64, 64 };

	std::optional<Ref<Texture2D>> EditorHelper::SelectTexture()
	{
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		if (ImGui::Begin("SelectTexture##EditorHelper", nullptr, ImGuiWindowFlags_NoDocking))
		{
			uint32_t counter = 0u;
			for (const auto& texture : textureLibrary->GetCaches() | std::views::values)
			{
				if (texture->GetType() == Texture::Type::CubeMap)
					continue;

				ImGui::Image(texture->GetResourceAsComPtr().Get(), TEXTURE_THUMBNAIL_SIZE.ToImVec2());
				ImGui::SameLine();

				const float contentX =  ImGui::GetContentRegionAvail().x;

				const std::string label = texture->GetName() + "##" + std::to_string(counter++) + texture->GetName();
				ImVec2 size = ImGui::CalcTextSize(label.c_str());
				size.y = TEXTURE_THUMBNAIL_SIZE.y;
				if (size.x < contentX)
				{
					size.x = contentX;
				}

				bool selected = false;
				if (ImGui::Selectable(label.c_str(), false, 0, size))
				{
					selected = true;
				}

				if (selected)
				{
					ImGui::End();

					return std::reinterpret_pointer_cast<Texture2D>(texture);
				}
			}
		}

		ImGui::End();

		return std::nullopt;
	}

}
