#include "stdafx.h"

#include "SceneLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "System/Scene/Scene.h"

#include "System/Serializer/SceneSerializer.h"
#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	SceneLibrary::SceneLibrary() :
		ILibrary(), _caches()
	{
	}

	void SceneLibrary::Initialize()
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
					if (IsSceneFile(path))
					{
						const auto scene = std::make_shared<Scene>();
						scene->Load(entryPath);

						Add(scene);
					}
				}
			}
		};
		contentSerializer(contentSerializer, sceneFolderPath);
	}

	void SceneLibrary::Update()
	{
	}

	void SceneLibrary::OnGui()
	{
	}

	void SceneLibrary::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void SceneLibrary::Finalize()
	{
	}

	void SceneLibrary::OnSerialized() const
	{
	}

	void SceneLibrary::OnDeserialized()
	{
	}

	void SceneLibrary::Add(Ref<Scene> scene)
	{
		const FilePath& filePath = scene->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in SceneLibrary::Add)");
		}

		_caches.try_emplace(filePath, scene);
	}

	void SceneLibrary::Replace(const FilePath& filePath, Ref<Scene> scene)
	{
		Remove(filePath);

		Add(scene);
	}

	void SceneLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR

			DENTY_INFO_LOG << "Succeed to remove " << filePath << " scene from library!";

			_caches.erase(it);

			return;

		}

		DENTY_SET_ERR_COLOR
		DENTY_INFO_LOG << "Failed to remove " << filePath << " scene from library!";
	}

	std::optional<Ref<Scene>> SceneLibrary::Find(SceneID id) const
	{
		for (auto&& scene : _caches | std::views::values)
		{
			if (scene->GetSceneID() != id)
				continue;

			return scene;
		}

		return std::nullopt;
	}

	std::optional<Ref<Scene>> SceneLibrary::Find(const FilePath& filePath) const
	{
		auto&& it = _caches.find(filePath);
		if (it != _caches.end())
		{
			return it->second;
		}

		return std::nullopt;
	}

	std::optional<Ref<Scene>> SceneLibrary::FindByName(std::string_view name) const
	{
		for (auto&& scene : _caches | std::views::values)
		{
			if (scene->GetName() != name)
				continue;

			return scene;
		}

		return std::nullopt;
	}

	bool SceneLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	bool SceneLibrary::IsSceneFile(const String& path) const
	{
		FilePath p = path.CStr();

		return (p.Extension() == SCENE_FILE_EXTENSION);
	}

    bool SceneLibrary::Exists(const FilePath& filePath) const
	{
		for (const auto& scene : _caches)
		{
			if (scene.first != filePath)
				continue;

			return true;
		}

		return false;
	}

    bool SceneLibrary::IsDuplicateID(SceneID id) const
    {
		for (auto&& scene : _caches | std::views::values)
		{
		    if (scene->GetSceneID() != id)
				continue;

			return true;
		}

		return false;
    }
}
