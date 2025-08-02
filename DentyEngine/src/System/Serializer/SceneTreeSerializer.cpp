#include "stdafx.h"

#include "Graphics/Mesh/SceneTree.h"

#include "SceneTreeSerializer.h"

#include "SerializeExtensions.h"

namespace DentyEngine
{
	bool SceneTreeSerializer::SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const MeshResource::SceneTree>& sceneTree)
	{
        auto path = filePath;

        // Replace extension to .DentySceneTree
        {
            path = path.replace_extension(SCENE_TREE_FILE_EXTENSION);
        }

        // Save
        {
            std::stringstream ss(path.string(), std::ios_base::binary | std::ios::out | std::ios::in);
            {
                cereal::BinaryOutputArchive oArchive(ss);

                oArchive(sceneTree);
            }

            std::ofstream ofs(
                path, std::ios_base::binary | std::ios::out | std::ios::in | std::ios_base::trunc
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

	bool SceneTreeSerializer::DeserializeFromBinary(const std::filesystem::path& filePath, Ref<MeshResource::SceneTree>& sceneTree)
	{
        auto path = filePath;

        // Change extension to .DentySceneTree.
        {
            path = path.replace_extension(SCENE_TREE_FILE_EXTENSION);
        }

        // Load
        {
            std::ifstream ifs(path.string(), std::ios::binary | std::ios::in | std::ios::out);

            if (!ifs.is_open())
            {
                ifs.close();

                return false;
            }

            cereal::BinaryInputArchive iArchive(ifs);

            iArchive(sceneTree);

            ifs.close();
        }

        return true;
	}
}
