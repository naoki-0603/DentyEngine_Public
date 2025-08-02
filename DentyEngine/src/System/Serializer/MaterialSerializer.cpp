#include "stdafx.h"

#include "MaterialSerializer.h"

#include "SerializeExtensions.h"

#include "Graphics/Renderer/Material.h"
#include "Graphics/Renderer/CubeMaterial.h"

namespace DentyEngine
{
    MaterialSerializer::MaterialSerializer()
    {
    }

    bool MaterialSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const Material>& material)
    {
        auto path = filePath;

        // Replace extension to .Material
        {
            path = path.replace_extension(MATERIAL_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(material);
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

    bool MaterialSerializer::CubeSerializeAsJson(const std::filesystem::path& filePath, const Ref<const CubeMaterial>& cubeMaterial)
    {
        auto path = filePath;

        // Replace extension to .DentyCubeMaterial
        {
            path = path.replace_extension(CUBE_MATERIAL_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(cubeMaterial);
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

    bool MaterialSerializer::CubeDeserializeFromJson(const std::filesystem::path& filePath, Ref<CubeMaterial>& cubeMaterial)
    {
        auto path = filePath;

        // Change extension to .DentyCubeMaterial
        {
            path = path.replace_extension(CUBE_MATERIAL_FILE_EXTENSION);
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
            inputArchive(cubeMaterial);

            ifs.close();
        }

        return true;
    }

    bool MaterialSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<Material>& material)
    {
        auto path = filePath;

        // Change extension to .DentyMaterial
        {
		    path = path.replace_extension(MATERIAL_FILE_EXTENSION);
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
            inputArchive(material);

            ifs.close();
        }

        return true;
    }
}
