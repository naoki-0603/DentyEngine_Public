#include "stdafx.h"

#include "EditorAssetSetter.h"

#include "Graphics/Renderer/Material.h"
#include "System/Resource/Asset.h"

#include "System/Core/FileDialogs.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

namespace DentyEngine
{
	EditorAssetSetter::EditorAssetSetter()
	{
	}

	bool EditorAssetSetter::SetTextureAssetFromDirectory(
		const Ref<Material>& material, uint8_t textureUsed
	)
	{
		const std::string path = FileDialogs::OpenFile(FileDialogs::TEXTURE_FILTER);
		if (path.empty())
			return false;

		const FilePath relativeTextureFilePath = path.substr(path.find_last_of("A"));
		if (!relativeTextureFilePath.IsValid())
			return false;

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const std::optional<Ref<Texture2D>> texture2D = textureLibrary->Find<Texture2D>(relativeTextureFilePath);
		DENTY_CHECK_CONDITION(texture2D.has_value());

		// TODO Support other maps.
		if (!IsSupportedTexture(textureUsed))
			return false;

		/*if (textureUsed == Utils::TEXTURE_ALBEDO)
		{
			material->AddTextureAsAlbedo(texture2D.value());
		}*/

		return true;
	}

	bool EditorAssetSetter::IsSupportedTexture(uint8_t textureUsed)
	{
		return (
			textureUsed == Utils::TEXTURE_ALBEDO
		);
	}
}
