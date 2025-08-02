#include "stdafx.h"

#include "PrefabSerializer.h"

#include "SerializeExtensions.h"

#include "../Resource/Prefab.h"

namespace DentyEngine
{
	bool PrefabSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const Prefab>& prefab)
	{
        auto path = filePath;

        // Replace extension to .DentyPrefab
        {
            path = path.replace_extension(PREFAB_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(prefab);
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

	bool PrefabSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<Prefab>& prefab)
	{
        auto path = filePath;

        // Change extension to .DentyPrefab
        {
            path = path.replace_extension(PREFAB_FILE_EXTENSION);
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
            inputArchive(prefab);

            ifs.close();
        }

        return true;
	}
}
