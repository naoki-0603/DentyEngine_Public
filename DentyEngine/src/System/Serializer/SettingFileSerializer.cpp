#include "stdafx.h"

#include "SettingFileSerializer.h"

#include "SerializeExtensions.h"

#include "System/Core/Setting/CollisionFilterSetting.h"
#include "System/Core/Setting/SceneSetting.h"

namespace DentyEngine
{
	const std::string SettingFileSerializer::OUTPUT_FILE_PATH = "Assets\\Settings\\";

	SettingFileSerializer::SettingFileSerializer()
	{
	}

    bool SettingFileSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const SceneSetting>& setting)
    {
        std::filesystem::path path = filePath;

        // Replace extension to .Json
        {
            path = path.replace_extension(SCENE_SETTING_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                //oArchive(setting);
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

    bool SettingFileSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const CollisionFilterSetting>& setting)
    {
        std::filesystem::path path = filePath;

        // Replace extension to .
        {
            path = path.replace_extension(SCENE_SETTING_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                //oArchive(setting);
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

    bool SettingFileSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<SceneSetting>& setting)
    {
        std::filesystem::path path = filePath;

        // Change extension to .Json
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
            //inputArchive(setting);

            ifs.close();
        }

        return true;
    }

    bool SettingFileSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<CollisionFilterSetting>& setting)
    {
        std::filesystem::path path = filePath;

        // Change extension to .Json
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
            //inputArchive(setting);

            ifs.close();
        }

        return true;
    }
}
