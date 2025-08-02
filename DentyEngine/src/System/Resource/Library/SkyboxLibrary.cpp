#include "stdafx.h"

#include "SkyboxLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"
#include "Graphics/Skybox.h"
#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	SkyboxLibrary::SkyboxLibrary() :
		ILibrary(), _caches()
	{
	}

	void SkyboxLibrary::Initialize()
	{
		auto skyboxFilePath = EditorContentBrowser::ENTRY_FILE_PATH;
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
					FilePath path = entryPath.string();
					if (path.Extension() == ".dds")
					{
						if (String(path.GetAsString()).Contains("_CM"))
						{
							String skyboxPath = path.GetAsString();
							skyboxPath.RemoveExtension();
							skyboxPath.Append(SKYBOX_FILE_EXTENSION);

							// Already exists?
							if (Utils::FileExists(skyboxPath.Get()))
							{
								continue;
							}

							// Otherwise create skybox.
							{
								FilePath originalTexturePath = path;
								originalTexturePath.ReplaceExtension(TEXTURE_FILE_EXTENSION);

								auto skybox = std::make_shared<Skybox>();
								skybox->Create(originalTexturePath);
								skybox->Save();

								Add(skybox);
							}
						}
					}
					else if (IsSkyboxFile(path.GetAsString()))
					{
						auto skybox = std::make_shared<Skybox>();
						skybox->Load(entryPath);

						Add(skybox);
					}
				}
			}
		};
		contentSerializer(contentSerializer, skyboxFilePath);
	}

	void SkyboxLibrary::Update()
	{
	}

	void SkyboxLibrary::OnGui()
	{
	}

	void SkyboxLibrary::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void SkyboxLibrary::Finalize()
	{
	}

	void SkyboxLibrary::OnSerialized() const
	{
	}

	void SkyboxLibrary::OnDeserialized()
	{
	}

	void SkyboxLibrary::Add(const Ref<Skybox>& skybox)
	{
		const FilePath& filePath = skybox->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in SkyboxLibrary::Add)");
		}

		_caches.try_emplace(filePath, skybox);
	}

	void SkyboxLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR

			DENTY_INFO_LOG << "Succeed to remove " << filePath << " skybox from library!";

			_caches.erase(it);

			return;

		}

		DENTY_SET_ERR_COLOR
		DENTY_INFO_LOG << "Failed to remove " << filePath << " skybox from library!";
	}

	std::optional<Ref<Skybox>> SkyboxLibrary::Find(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return it->second;
		}

		return std::nullopt;
	}

	bool SkyboxLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	bool SkyboxLibrary::IsSkyboxFile(const String& filePath) const
	{
		return filePath.Contains(SKYBOX_FILE_EXTENSION);
	}
}
