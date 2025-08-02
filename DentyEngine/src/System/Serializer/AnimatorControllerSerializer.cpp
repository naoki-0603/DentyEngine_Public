#include "stdafx.h"

#include "AnimatorControllerSerializer.h"

#include "SerializeExtensions.h"

#include "System/Resource/Animations/AnimatorController.h"

namespace DentyEngine
{
	AnimatorControllerSerializer::AnimatorControllerSerializer()
	{
	}

	bool AnimatorControllerSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const AnimatorController>& controller)
	{
		auto path = filePath;

		// Replace extension to .DentyAnimatorController
		{
			path = path.replace_extension(ANIMATOR_CONTROLLER_FILE_EXTENSION);
		}

		// Save
		{
			std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
			{
				cereal::JSONOutputArchive oArchive(ss);

				oArchive(controller);
			}

			std::ofstream ofs(
				path, std::ios::out | std::ios::in | std::ios_base::trunc
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

	bool AnimatorControllerSerializer::DeserializeFromJson(const std::filesystem::path& filePath , Ref<AnimatorController>& controller)
	{
		auto path = filePath;

		// Change extension to .DentyAnimatorController
		{
			path = path.replace_extension(ANIMATOR_CONTROLLER_FILE_EXTENSION);
		}

		// Load
		{
			std::ifstream ifs(path.c_str(), std::ios::in | std::ios::out);

			if (!ifs.is_open())
			{
				ifs.close();

				return false;
			}

			cereal::JSONInputArchive inputArchive(ifs);
			inputArchive(controller);

			ifs.close();
		}

		return true;
	}
}
