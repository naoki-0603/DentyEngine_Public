#include "stdafx.h"

#include "AnimationClipLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "System/Resource/Animations/AnimationClip.h"

#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	AnimationClipLibrary::AnimationClipLibrary() :
		ILibrary(), _caches()
	{
	}

	void AnimationClipLibrary::Initialize()
	{
		auto animationClipFolderPath = EditorContentBrowser::ENTRY_FILE_PATH;
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
					if (path.Contains(ANIMATION_FILE_EXTENSION))
					{
						const auto clip = std::make_shared<MeshResource::AnimationClip>();
						clip->Load(entryPath);

						Add(clip);
					}
				}
			}
		};
		contentSerializer(contentSerializer, animationClipFolderPath);
	}

	void AnimationClipLibrary::Finalize()
	{
	}

	void AnimationClipLibrary::Update()
	{
	}

	void AnimationClipLibrary::OnGui()
	{
	}

	void AnimationClipLibrary::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void AnimationClipLibrary::OnSerialized() const
	{
	}

	void AnimationClipLibrary::OnDeserialized()
	{
	}

	void AnimationClipLibrary::Add(const Ref<MeshResource::AnimationClip>& clip)
	{
		const auto& filePath = clip->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in AnimationClipLibrary::Add)");
		}

		_caches.try_emplace(filePath, clip);
	}

	void AnimationClipLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Succeed to remove " << filePath << " animation clip from library!";

			_caches.erase(it);

			return;
		}

		DENTY_SET_ERR_COLOR
		DENTY_ERR_LOG << "Failed to remove " << filePath << " animation clip from library!";
	}

	std::optional<Ref<MeshResource::AnimationClip>> AnimationClipLibrary::Find(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return it->second;
		}

		return std::nullopt;
	}


	bool AnimationClipLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	void AnimationClipLibrary::Reload([[maybe_unused]] AnimationClipID id)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)
	}
}
