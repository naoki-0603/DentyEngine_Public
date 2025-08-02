#include "stdafx.h"

#include "ScriptSerializer.h"

#include "SerializeExtensions.h"

#include "System/Resource/Script.h"

namespace DentyEngine
{
	ScriptSerializer::ScriptSerializer()
	{
	}

	bool ScriptSerializer::SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const Script>& script)
	{
        auto path = filePath;

        // Replace extension to .Json
        {
            path = path.replace_extension(SCRIPT_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(script);
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

	bool ScriptSerializer::DeserializeFromBinary(const std::filesystem::path& filePath, Ref<Script>& script)
	{
        auto path = filePath;

        // Change extension to .Json
        {
            path = path.replace_extension(SCRIPT_FILE_EXTENSION);
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
            inputArchive(script);

            ifs.close();
        }

        return true;
	}
}
