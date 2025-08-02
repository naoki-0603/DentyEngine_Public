#include "stdafx.h"

#include "CollisionMeshSerializer.h"

#include "Physics/CollisionMesh.h"
#include "Physics/ApproximationCollisionMesh.h"

#include "SerializeExtensions.h"

namespace DentyEngine
{
	CollisionMeshSerializer::CollisionMeshSerializer()
	{
	}

	bool CollisionMeshSerializer::SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const CollisionMesh>& collisionMesh)
	{
        auto path = filePath;

        // Replace extension to .Mesh
        {
            path = path.replace_extension(COLLISION_MESH_FILE_EXTENSION);
        }

        // Save
        std::stringstream ss(path.string().c_str(), std::ios_base::binary | std::ios::out | std::ios::in);
        {
            cereal::BinaryOutputArchive outputArchive(ss);
            outputArchive(collisionMesh);
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

	bool CollisionMeshSerializer::SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const ApproximationCollisionMesh>& approximationCollisionMesh)
	{
        auto path = filePath;

        // Replace extension to .Mesh
        {
            path = path.replace_extension(APPROXIMATION_COLLISION_MESH_FILE_EXTENSION);
        }

        // Save
        std::stringstream ss(path.string().c_str(), std::ios_base::binary | std::ios::out | std::ios::in);
        {
            cereal::BinaryOutputArchive outputArchive(ss);
            outputArchive(approximationCollisionMesh);
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

	bool CollisionMeshSerializer::DeserializeFromBinary(const std::filesystem::path& filePath, Ref<CollisionMesh>& collisionMesh)
	{
        auto path = filePath;

        // Replace extension to .Mesh
        {
            path = path.replace_extension(COLLISION_MESH_FILE_EXTENSION);
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
            inputArchive(collisionMesh);

            ifs.close();
        }

        return true;
	}

	bool CollisionMeshSerializer::DeserializeFromBinary(const std::filesystem::path& filePath, Ref<ApproximationCollisionMesh>& approximationCollisionMesh)
	{
        auto path = filePath;

        // Replace extension to .Mesh
        {
            path = path.replace_extension(APPROXIMATION_COLLISION_MESH_FILE_EXTENSION);
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
            inputArchive(approximationCollisionMesh);

            ifs.close();
        }

        return true;
	}
}
