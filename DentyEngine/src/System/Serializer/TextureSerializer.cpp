#include "stdafx.h"

#include "TextureSerializer.h"

#include "SerializeExtensions.h"

#include "Graphics/Renderer/Texture2D.h"
#include "Graphics/Renderer/CubeMap.h"

namespace DentyEngine
{
	TextureSerializer::TextureSerializer()
	{
	}

    bool TextureSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const Texture2D>& texture2D)
    {
        auto path = filePath;

        // Replace extension to .DentyTexture.
        {
            path = path.replace_extension(TEXTURE_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(texture2D);
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

    bool TextureSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const CubeMap>& cubeMap)
    {
        auto path = filePath;

        // Replace extension to .DentyTexture.
        {
            path = path.replace_extension(TEXTURE_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(cubeMap);
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

    bool TextureSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<Texture>& texture)
    {
        auto path = filePath;

        // Change extension to .DentyTexture
        {
            path = path.replace_extension(TEXTURE_FILE_EXTENSION);
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
            inputArchive(texture);

            ifs.close();
        }

        return true;
    }

    bool TextureSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<Texture2D>& texture2D)
    {
        auto path = filePath;

        // Change extension to .DentyTexture
        {
            path = path.replace_extension(TEXTURE_FILE_EXTENSION);
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
            inputArchive(texture2D);

            ifs.close();
        }

        return true;
    }

    bool TextureSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<CubeMap>& cubeMap)
    {
        auto path = filePath;

        // Change extension to .DentyTexture.
        {
            path = path.replace_extension(TEXTURE_FILE_EXTENSION);
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
            inputArchive(cubeMap);

            ifs.close();
        }

        return true;
    }
}
