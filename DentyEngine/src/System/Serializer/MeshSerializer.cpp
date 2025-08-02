#include "stdafx.h"

#include "MeshSerializer.h"

#include "SerializeExtensions.h"

#include "Graphics/Mesh/StaticMesh.h"
#include "Graphics/Mesh/SkinnedMesh.h"

namespace DentyEngine
{
	MeshSerializer::MeshSerializer()
	{
	}

    bool MeshSerializer::SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const StaticMesh>& staticMesh)
    {
        auto path = filePath;

	    // Replace extension to .Mesh
        {
            path = path.replace_extension(MESH_FILE_EXTENSION);
        }

        // Save
        std::stringstream ss(path.string().c_str(), std::ios_base::binary | std::ios::out | std::ios::in);
        {
            cereal::BinaryOutputArchive outputArchive(ss);
            outputArchive(staticMesh);
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

    bool MeshSerializer::SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const SkinnedMesh>& skinnedMesh)
    {
        auto path = filePath;

	    // Replace extension to .Mesh
        {
            path = path.replace_extension(MESH_FILE_EXTENSION);
        }

        // Save
        std::stringstream ss(path.string().c_str(), std::ios_base::binary | std::ios::out | std::ios::in);
        {
            cereal::BinaryOutputArchive outputArchive(ss);
            outputArchive(skinnedMesh);
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

    bool MeshSerializer::DeserializeFromBinary(const std::filesystem::path& filePath, Ref<StaticMesh>& staticMesh)
    {
        auto path = filePath;

        // Replace extension to .Mesh
        {
            path = path.replace_extension(MESH_FILE_EXTENSION);
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
            inputArchive(staticMesh);

            ifs.close();
        }

        return true;
    }

    bool MeshSerializer::DeserializeFromBinary(const std::filesystem::path& filePath, Ref<SkinnedMesh>& skinnedMesh)
    {
        auto path = filePath;

        // Replace extension to .Mesh
        {
            path = path.replace_extension(MESH_FILE_EXTENSION);
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
            inputArchive(skinnedMesh);

            ifs.close();
        }

        return true;
    }
}
