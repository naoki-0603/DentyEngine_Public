#include "stdafx.h"

#include "SkeletonSerializer.h"
#include "SerializeExtensions.h"

#include "Graphics/Mesh/Skeleton.h"

namespace DentyEngine
{
	SkeletonSerializer::SkeletonSerializer()
	{
	}

	bool SkeletonSerializer::SerializeAsJson(const std::filesystem::path& filePath, const Ref<const MeshResource::Skeleton>& skeleton)
	{
        auto path = filePath;

        // Replace extension to .DentyAvatar
        {
            path = path.replace_extension(AVATAR_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string().c_str(), std::ios::out | std::ios::in);
            {
                cereal::JSONOutputArchive oArchive(ss);

                oArchive(skeleton);
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

	bool SkeletonSerializer::DeserializeFromJson(const std::filesystem::path& filePath, Ref<MeshResource::Skeleton>& skeleton)
	{
        auto path = filePath;

        // Change extension to .DentyAvatar
        {
            path = path.replace_extension(AVATAR_FILE_EXTENSION);
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
            inputArchive(skeleton);

            ifs.close();
        }

        return true;
	}
}
