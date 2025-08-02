#include "stdafx.h"

#include "SceneSettingLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "System/Core/Setting/SceneSetting.h"
#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	SceneSettingLibrary::SceneSettingLibrary() :
		ILibrary(), _caches()
	{
	}

	void SceneSettingLibrary::Initialize()
	{
		auto sceneFolderPath = EditorContentBrowser::ENTRY_FILE_PATH;
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
					if (IsSceneSettingFile(path))
					{
						const auto setting = std::make_shared<SceneSetting>();
						setting->Load(entryPath);

						Add(setting);
					}
				}
			}
		};
		contentSerializer(contentSerializer, sceneFolderPath);
	}

	void SceneSettingLibrary::Update()
	{
	}

	void SceneSettingLibrary::OnGui()
	{
	}

	void SceneSettingLibrary::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void SceneSettingLibrary::Finalize()
	{
	}

	void SceneSettingLibrary::OnSerialized() const
	{
	}

	void SceneSettingLibrary::OnDeserialized()
	{
	}

	void SceneSettingLibrary::Add(Ref<SceneSetting> sceneSetting)
	{
		const FilePath& filePath = sceneSetting->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in SceneSettingLibrary::Add)");
		}

		_caches.try_emplace(filePath, sceneSetting);
	}

	void SceneSettingLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Succeed to remove " << filePath << " scene setting from library!";

			_caches.erase(it);

			return;

		}

		DENTY_SET_ERR_COLOR
		DENTY_INFO_LOG << "Failed to remove " << filePath << " scene setting from library!";
	}

	std::optional<Ref<SceneSetting>> SceneSettingLibrary::Find(const FilePath& filePath) const
	{
		for (auto&& setting : _caches | std::views::values)
		{
			if (setting->GetFilePathToSerialized() != filePath)
				continue;

			return setting;
		}

		return std::nullopt;
	}

	bool SceneSettingLibrary::Reload(const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	bool SceneSettingLibrary::IsSceneSettingFile(const String& path) const
	{
		return path.Contains(SCENE_FILE_EXTENSION);
	}

	bool SceneSettingLibrary::Exists(const FilePath& filePath) const
	{
		for (const auto& setting : _caches)
		{
			if (setting.first != filePath)
				continue;

			return true;
		}

		return false;
	}
}
