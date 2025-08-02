#include "stdafx.h"

#include "SkeletonLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "Graphics/Mesh/Skeleton.h"

#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	SkeletonLibrary::SkeletonLibrary() :
		ILibrary(), _caches()
	{
	}

	void SkeletonLibrary::Initialize()
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
					if (IsSkeletonFile(path))
					{
						const auto skeleton = std::make_shared<MeshResource::Skeleton>();
						skeleton->Load(entryPath);

						Add(skeleton);
					}
				}
			}
		};
		contentSerializer(contentSerializer, shaderFolderPath);
	}

	void SkeletonLibrary::Update()
	{
	}

	void SkeletonLibrary::OnGui()
	{
	}

	void SkeletonLibrary::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void SkeletonLibrary::Finalize()
	{
		_caches.clear();
	}

	void SkeletonLibrary::OnSerialized() const
	{
	}

	void SkeletonLibrary::OnDeserialized()
	{
	}

	void SkeletonLibrary::Add(const Ref<MeshResource::Skeleton>& skeleton)
	{
		const auto& filePath = skeleton->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in SkeletonLibrary::Add)");
		}

		_caches.try_emplace(filePath, skeleton);
	}

	void SkeletonLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Succeed to remove " << filePath << " skeleton from library!";

			_caches.erase(it);

			return;
		}

		DENTY_SET_ERR_COLOR
		DENTY_INFO_LOG << "Failed to remove " << filePath << " skeleton from library!";
	}

	std::optional<Ref<MeshResource::Skeleton>> SkeletonLibrary::Find(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return it->second;
		}

		return std::nullopt;
	}

	bool SkeletonLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	bool SkeletonLibrary::IsSkeletonFile(const String& filePath) const
	{
		return (filePath.Contains(AVATAR_FILE_EXTENSION));
	}
}
