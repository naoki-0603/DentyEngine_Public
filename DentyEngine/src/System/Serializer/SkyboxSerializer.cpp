#include "stdafx.h"

#include "SkyboxSerializer.h"

#include "SerializeExtensions.h"

#include "Graphics/Skybox.h"

namespace DentyEngine
{
	SkyboxSerializer::SkyboxSerializer()
	{
	}

	bool SkyboxSerializer::SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const Skybox>& skybox)
	{
        auto path = filePath;

        // Replace extension to .Skybox
        {
            path = path.replace_extension(SKYBOX_FILE_EXTENSION);
        }

        // Save
        std::stringstream ss(path.string().c_str(), std::ios_base::binary | std::ios::out | std::ios::in);
        {
            cereal::BinaryOutputArchive outputArchive(ss);
            outputArchive(skybox);
        }

        {
            std::ofstream ofs(
                path, std::ios_base::binary | std::ios::out | std::ios::in | std::ios_base::trunc
            );

            if (!ofs.is_open())
            {
                ofs.close();

                return false;
            }

            ofs << ss.rdbuf();
        }

        return true;
	}

	bool SkyboxSerializer::DeserializeFromBinary(const std::filesystem::path& filePath, Ref<Skybox>& skybox)
	{
        auto path = filePath;

        // Replace extension to .Skybox
        {
            path = path.replace_extension(SKYBOX_FILE_EXTENSION);
        }

        // Load
        {
            std::ifstream ifs(path.c_str(), std::ios::binary | std::ios::in | std::ios::out);

            if (!ifs.is_open())
            {
                ifs.close();

                return false;
            }

            cereal::BinaryInputArchive inputArchive(ifs);
            inputArchive(skybox);

            ifs.close();
        }

        return true;
    }
}