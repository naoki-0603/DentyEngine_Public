#include "stdafx.h"

#include "SerializeExtensions.h"
#include "ShaderSerializer.h"

#include "Graphics/Renderer/Shader.h"

namespace DentyEngine
{
	const std::string ShaderSerializer::SHADER_FILE_OUTPUT_PATH = "Assets\\Shaders\\";
	const std::string ShaderSerializer::BUILD_IN_SHADER_OUTPUT_PATH = "Assets\\Shaders\\BuildIn\\";

	ShaderSerializer::ShaderSerializer()
	{
	}

	bool ShaderSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const Shader>& shader)
	{
        auto path = filePath;

        // Replace extension to .DentyShader
        {
            path = path.replace_extension(SHADER_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(shader);
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

	bool ShaderSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<Shader>& shader)
	{
        auto path = filePath;

        // Change extension to .DentyShader
        {
            path = path.replace_extension(SHADER_FILE_EXTENSION);
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
            inputArchive(shader);

            ifs.close();
        }

        return true;
    }
}
