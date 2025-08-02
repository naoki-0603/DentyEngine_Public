#include "stdafx.h"

#include "AnimationClipSerializer.h"

#include "SerializeExtensions.h"
#include "System/Resource/Animations/AnimationClip.h"

namespace DentyEngine
{
	AnimationClipSerializer::AnimationClipSerializer()
	{
	}

	bool AnimationClipSerializer::SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const MeshResource::AnimationClip>& clip)
	{
		auto path = filePath;

		// Change extension to .Animation
		{
			path = path.replace_extension(ANIMATION_FILE_EXTENSION);
		}

		// Save
		{
			std::stringstream ss(path.string(), std::ios_base::binary | std::ios::out | std::ios::in);
			{
				cereal::BinaryOutputArchive oArchive(ss);

				oArchive(clip);
			}

			std::ofstream ofs(
				path, std::ios_base::binary | std::ios::out | std::ios::in | std::ios_base::trunc
			);

			if (!ofs.is_open())
			{
				ofs.close();

				return false;
			}

			ofs << ss.rdbuf();

			ofs.close();
		}
		
		return true;
	}

	bool AnimationClipSerializer::DeserializeFromBinary(const std::filesystem::path& filePath, Ref<MeshResource::AnimationClip>& clip)
	{
		auto path = filePath;

		// Change extension to .Animation
		{
			path = path.replace_extension(ANIMATION_FILE_EXTENSION);
		}

		// Load
		{
			std::ifstream ifs(path.string(), std::ios::binary | std::ios::in | std::ios::out);

			if (!ifs.is_open())
			{
				ifs.close();

				return false;
			}

			cereal::BinaryInputArchive iArchive(ifs);

			iArchive(clip);

			ifs.close();
		}

		return true;
	}
}
