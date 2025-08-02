#include "stdafx.h"

#include "SceneTreeLibrary.h"

#include "System/Core/DString.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	SceneTreeLibrary::SceneTreeLibrary() :
		ILibrary(), _caches()
	{
	}

	void SceneTreeLibrary::Initialize()
	{
		auto shaderFolderPath = EditorContentBrowser::ENTRY_FILE_PATH;

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

				{
					const String path = entryPath.string();
					if (IsSceneTreeFile(path))
					{
						const auto sceneTree = Utils::CreateRef<MeshResource::SceneTree>();
						sceneTree->Load(entryPath);

						Add(sceneTree);
					}
				}

			}
		};
		contentSerializer(contentSerializer, shaderFolderPath);
	}

	void SceneTreeLibrary::Finalize()
	{
	}

	void SceneTreeLibrary::Update()
	{
	}

	void SceneTreeLibrary::OnGui()
	{
	}

	void SceneTreeLibrary::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void SceneTreeLibrary::OnSerialized() const
	{
	}

	void SceneTreeLibrary::OnDeserialized()
	{
	}

    void SceneTreeLibrary::GenerateSceneTreesMeshes()
    {
		for (auto&& sceneTree : _caches | std::views::values)
		{
		    sceneTree->AttachMeshes();
		}
    }

    void SceneTreeLibrary::Add(Ref<MeshResource::SceneTree> sceneTree)
	{
		const auto& filePath = sceneTree->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in SceneTreeLibrary::Add)");
		}

		_caches.try_emplace(filePath, sceneTree);
	}

	void SceneTreeLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Succeed to remove " << filePath << " scene tree from library!";

			_caches.erase(it);

			return;
		}

		DENTY_SET_ERR_COLOR
		DENTY_INFO_LOG << "Failed to remove " << filePath << " scene tree from library!";
	}

	std::optional<Ref<MeshResource::SceneTree>> SceneTreeLibrary::Find(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return it->second;
		}

		return std::nullopt;
	}

	bool SceneTreeLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	bool SceneTreeLibrary::IsSceneTreeFile(const String& path) const
	{
		return path.Contains(SCENE_TREE_FILE_EXTENSION);
	}
}
