#include "stdafx.h"

#include "SceneSettingSerializer.h"

#include "SerializeExtensions.h"

#include "System/Core/Setting/SceneSetting.h"

namespace DentyEngine
{
	const std::string SceneSettingSerializer::SCENE_SETTING_FILE_OUTPUT_PATH = "Assets\\Scenes\\Settings\\";

	SceneSettingSerializer::SceneSettingSerializer()
	{
	}

	bool SceneSettingSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const SceneSetting>& setting)
	{
        auto path = filePath;

        // Replace extension to .DentySceneSetting
        {
            path = path.replace_extension(SCENE_SETTING_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(setting);
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

	bool SceneSettingSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<SceneSetting>& setting)
	{
        auto path = filePath;

        // Change extension to .DentySceneSetting
        {
            path = path.replace_extension(SCENE_SETTING_FILE_EXTENSION);
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
            inputArchive(setting);

            ifs.close();
        }

        return true;
	}
}
