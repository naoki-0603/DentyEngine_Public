#include "stdafx.h"

#include "ProjectSettingSerializer.h"

#include "System/Core/Setting/ProjectSetting.h"

#include "SerializeExtensions.h"

namespace DentyEngine
{
	ProjectSettingSerializer::ProjectSettingSerializer()
	{
	}

	bool ProjectSettingSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const ProjectSetting>& projectSetting)
	{
        auto path = filePath;

        // Replace extension to .DentyProjectSetting
        {
            path = path.replace_extension(PROJECT_SETTING_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(projectSetting);
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

	bool ProjectSettingSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<ProjectSetting>& projectSetting)
	{
        auto path = filePath;

        // Change extension to .DentyProjectSetting
        {
            path = path.replace_extension(PROJECT_SETTING_FILE_EXTENSION);
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
            inputArchive(projectSetting);

            ifs.close();
        }

        return true;
	}
}
