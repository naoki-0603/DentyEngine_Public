#include "stdafx.h"

#include "AnimatorControllerLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "System/Resource/Animations/AnimatorController.h"

#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	AnimatorControllerLibrary::AnimatorControllerLibrary() :
		ILibrary(), _caches()
	{
	}

	void AnimatorControllerLibrary::Initialize()
	{
		auto animatorControllerFilePath = EditorContentBrowser::ENTRY_FILE_PATH;
		auto contentSerializer = [this](auto self, FilePath& folderPath) -> void
		{
			for (auto& content : std::filesystem::directory_iterator(folderPath.Get()))
			{
				const auto& entryPath = content.path();
				std::string entry = entryPath.filename().string();

				if (content.is_directory())
				{
					self(self, folderPath /= entry);
					folderPath = folderPath.ParentPath();
				}
				else
				{
					String path = entryPath.string();
					if (IsAnimatorControllerFile(path))
					{
						auto controller = std::make_shared<AnimatorController>();

						controller->Load(String::ToPath(path).Get());

						Add(controller);

						controller->OnLoaded();
					}
				}
			}
		};
		contentSerializer(contentSerializer, animatorControllerFilePath);
	}

	void AnimatorControllerLibrary::Finalize()
	{
	}

	void AnimatorControllerLibrary::Update()
	{
	}

	void AnimatorControllerLibrary::OnGui()
	{
	}

	void AnimatorControllerLibrary::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void AnimatorControllerLibrary::OnSerialized() const
	{
	}

	void AnimatorControllerLibrary::OnDeserialized()
	{
	}

	void AnimatorControllerLibrary::Add(const Ref<AnimatorController>& controller)
	{
		const auto& filePath = controller->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in AnimatorControllerLibrary::Add)");
		}

		_caches.try_emplace(filePath, controller);
	}

	void AnimatorControllerLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			_caches.erase(it);

			DENTY_SET_INFO_COLOR

			DENTY_INFO_LOG << "Succeed to remove " << filePath << " animator controller from library!";

			return;

		}

		DENTY_SET_ERR_COLOR

		DENTY_ERR_LOG << "Failed to remove " << filePath << " animator controller from library!";
	}

	std::optional<Ref<AnimatorController>> AnimatorControllerLibrary::Find(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return it->second;
		}

		return std::nullopt;
	}

	bool AnimatorControllerLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	void AnimatorControllerLibrary::Reload([[maybe_unused]] AnimatorControllerID id)
	{
	}

	bool AnimatorControllerLibrary::IsAnimatorControllerFile(const String& path) const
	{
		return (path.Contains(ANIMATOR_CONTROLLER_FILE_EXTENSION));
	}

}
