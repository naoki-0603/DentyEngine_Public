#include "stdafx.h"

#include "MaterialLibrary.h"

#include "LibraryManager.h"
#include "TextureLibrary.h"

#include "System/Serializer/SerializeExtensions.h"

#include "Graphics/Renderer/Material.h"
#include "Graphics/Renderer/CubeMaterial.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "Graphics/Mesh/SkinnedMesh.h"
#include "Graphics/Mesh/StaticMesh.h"

#include "System/Events/EventEditor.h"

namespace DentyEngine
{
	MaterialLibrary::MaterialLibrary() :
		ILibrary(), _caches()
	{
	}

	void MaterialLibrary::Initialize()
	{
		auto materialFolderPath = EditorContentBrowser::ENTRY_FILE_PATH;
		auto contentSerializer = [this](auto self, FilePath& folderPath) -> void
		{
			for (auto& content : std::filesystem::directory_iterator(folderPath.Get()))
			{
				const auto& entryPath = content.path();
				const std::string entry = entryPath.filename().string();

				if (content.is_directory())
				{
					self(self, folderPath /= entry);
					folderPath = folderPath.ParentPath();
				}
				else
				{
					String path = entryPath.string();
					if (IsMaterialFile(path))
					{
						const auto material = std::make_shared<Material>();
						material->Load(entryPath);

						Add(material);
					}
					else if (IsCubeMaterialFile(path.CStr()))
					{
						const auto cubeMaterial = std::make_shared<CubeMaterial>();
						cubeMaterial->Load(entryPath);

						Add(cubeMaterial);
					}
				}
			}
		};
		contentSerializer(contentSerializer, materialFolderPath);
	}

	void MaterialLibrary::Finalize()
	{
	}

	void MaterialLibrary::Update()
	{
	}

	void MaterialLibrary::OnGui()
	{
	}

	void MaterialLibrary::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<EventOnMaterialAssetUpdated>(BIND_EVENT_FUNCTION(OnMaterialAssetUpdated));
	}

	void MaterialLibrary::OnSerialized() const
	{
	}

	void MaterialLibrary::OnDeserialized()
	{
	}

	void MaterialLibrary::Add(Ref<Material> material)
	{
		if (material->IsDefault()) return;

		const FilePath& filePath = material->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in MaterialLibrary::Add)");
		}

		_caches.try_emplace(filePath, material);
	}

	void MaterialLibrary::Add(Ref<CubeMaterial> cubeMaterial)
	{
		const FilePath& filePath = cubeMaterial->GetFilePathToSerialized();

		if (filePath.IsEmpty() || !filePath.IsValid())
		{
			DENTY_ASSERT(false, "File path is empty or invalid! (in MaterialLibrary::Add)");
		}

		_caches.try_emplace(filePath, cubeMaterial);
	}


	void MaterialLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR

			DENTY_INFO_LOG << "Succeed to remove " << filePath << " material from library!";

			_caches.erase(it);

			return;

		}

		DENTY_SET_ERR_COLOR

		DENTY_INFO_LOG << "Failed to remove " << filePath << " material from library!";
	}

	std::optional<Ref<Material>> MaterialLibrary::Find(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			if (it->second->GetAssetType() != Asset::AssetType::Material)
			{
				return std::nullopt;
			}

			return std::static_pointer_cast<Material>(it->second);
		}

		return std::nullopt;
	}

	std::optional<Ref<CubeMaterial>> MaterialLibrary::FindCube(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			if (it->second->GetAssetType() != Asset::AssetType::CubeMaterial)
			{
				return std::nullopt;
			}

			return std::static_pointer_cast<CubeMaterial>(it->second);
		}

		return std::nullopt;
	}

	bool MaterialLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)
		
		return false;
	}

	void MaterialLibrary::OnMaterialAssetUpdated([[maybe_unused]] EventOnMaterialAssetUpdated* e)
	{
		
	}

	bool MaterialLibrary::IsMaterialFile(const String& path) const
	{
		return (FilePath(path.CStr()).Extension() == MATERIAL_FILE_EXTENSION);
	}

	bool MaterialLibrary::IsCubeMaterialFile(const FilePath& filePath) const noexcept
	{
		return (filePath.Extension().GetAsString() == CUBE_MATERIAL_FILE_EXTENSION);
	}
}
