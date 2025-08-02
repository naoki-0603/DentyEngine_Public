#include "stdafx.h"

#include "SceneSerializer.h"

#include "System/Scene/Scene.h"

#include "SerializeExtensions.h"

namespace DentyEngine
{
	const std::string SceneSerializer::SCENE_FILE_OUTPUT_PATH = "Assets\\Scenes\\";

	SceneSerializer::SceneSerializer()
	{
	}

	bool SceneSerializer::SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const Scene>& scene)
	{
		auto path = filePath;

		// Change extension to .DentyScene
		{
			path = path.replace_extension(SCENE_FILE_EXTENSION);
		}

		// Save
		{
			std::stringstream ss(path.string(), std::ios_base::binary | std::ios::out | std::ios::in);
			{
				cereal::BinaryOutputArchive oArchive(ss);

				oArchive(scene);
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

	bool SceneSerializer::DeserializeFromBinary(const std::filesystem::path& filePath, Ref<Scene>& scene)
	{
		auto path = filePath;

		// Change extension to .DentyScene
		{
			path = path.replace_extension(SCENE_FILE_EXTENSION);
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

			iArchive(scene);

			ifs.close();
		}

		return true;
	}
}
