#include "stdafx.h"

#include "SceneTree.h"

#include "Graphics/Mesh/Mesh.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "System/Serializer/SceneTreeSerializer.h"

namespace DentyEngine
{
	namespace MeshResource
	{
		const String SceneTree::ADD_FILENAME_AT_END = "_DentySceneTree";

		Ref<Texture2D> SceneTree::_sceneTreeAssetTexture = nullptr;

		SceneTree::SceneTree() : Object(), Asset(),
		                         _resources()
		{
		}

		void SceneTree::Save(bool isUpdate) const
		{
			// If failed 
			if (!SceneTreeSerializer::SerializeAsBinary(_filePathToSerialized.Get(), shared_from_this()))
			{
				DENTY_SET_ERR_COLOR

				if (isUpdate)
				{
					DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " scene tree!";
				}
				else
				{
					DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " scene tree!";
				}

				return;
			}

			DENTY_SET_INFO_COLOR

			if (isUpdate)
			{
				DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " scene tree!";
			}
			else
			{
				DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " scene tree!";
			}
		}

		void SceneTree::Load(const std::filesystem::path& filePath)
		{
			// If failed
			auto sceneTree = Utils::CreateRef<SceneTree>();

			if (!SceneTreeSerializer::DeserializeFromBinary(filePath, sceneTree))
			{
				DENTY_SET_ERR_COLOR

				DENTY_ERR_LOG << "Failed to load " << filePath << " scene tree!";

				return;
			}

			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Succeed to load " << filePath << " scene tree!";

			sceneTree->Clone(shared_from_this());
		}

		void SceneTree::Rename([[maybe_unused]] std::string_view newName, [[maybe_unused]] bool deleteOldFile)
		{
		}

		void SceneTree::OnReload()
		{
		}

		void SceneTree::Clone(const Ref<Asset>& asset)
		{
			DENTY_ASSERT(asset->GetAssetType() == AssetType::SceneTree, "Asset type must be Shader. (in SceneTree::Clone)");

			auto&& target = std::static_pointer_cast<SceneTree>(asset);

			// Object class
			{
				target->_name = _name;
				target->_uuid = _uuid;
			}

			// Asset class
			{
				target->_previewImage = _previewImage;
				target->_previewImageFilePath = _previewImageFilePath;
				target->_filePath = _filePath;
				target->_filePathToSerialized = _filePathToSerialized;
			}

			// This class
			{
				target->_resources = _resources;
			}
		}

        void SceneTree::AttachMeshes()
        {
			if (!_resources.at(0).isSkinned)
				return;

			const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

			for (auto&& resource : _resources)
			{
			    if (resource.nodeType != NodeType::Mesh)
					continue;

				const auto mesh = meshLibrary->Find<Mesh>(resource.meshFilePath);
				if (!mesh.has_value())
				{
				    DENTY_SET_ERR_COLOR
					DENTY_ERR_LOG << "Mesh " << resource.meshFilePath << " not found!";

					DENTY_ASSERT(false, "");
				}

				resource.mesh = mesh.value()->FindSubMeshByName<Mesh>(resource.name);
			}
        }

        void SceneTree::BindMeshes(Ref<Mesh> mesh)
        {
			if (mesh->GetMeshType() == std::string("StaticMesh"))
			{
				_resources.at(0).isSkinned = false;

				return;
			}

			// Bind meshes.
            for (auto& resource : _resources)
            {
                if (resource.nodeType != NodeType::Mesh)
                    continue;

				resource.isSkinned = true;

                const auto subMesh = mesh->FindSubMeshByName<Mesh>(resource.name);

				const FilePath& filePath = subMesh->GetFilePathToSerialized();
				if (filePath.IsEmpty())
				{
				    DENTY_ASSERT(false, "File path is empty! (in SceneTree::BindMeshes)");
				}

				resource.mesh = subMesh;
				resource.meshFilePath = subMesh->GetFilePathToSerialized();
            }

			// Update
			Save(true);
        }

        std::optional<SceneTreeResource> SceneTree::Find(uint32_t index) const
		{
			const uint32_t size = static_cast<uint32_t>(_resources.size());
			if (index >= size)
			{
				DENTY_SET_ERR_COLOR
				DENTY_ERR_LOG << "Index out of range! (in SceneTree::Find)";

				return std::nullopt;
			}

			return _resources.at(index);
		}

		std::optional<SceneTreeResource> SceneTree::Find(std::string_view name) const
		{
			for (const auto& resource : _resources)
			{
				if (resource.name != name)
					continue;

				return resource;
			}

			return std::nullopt;
		}

		void SceneTree::LoadAssetTexture()
		{
			DENTY_ASSERT(not _sceneTreeAssetTexture, "Scene tree asset texture already loaded!");

			const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
			_sceneTreeAssetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::SCENE_TREE_DEFAULT_THUMBNAIL_FILE_PATH).value();

			DENTY_SET_INFO_COLOR
			DENTY_INFO_LOG << "Scene tree asset texture successfully loaded!";
		}

		Matrix SceneTree::ComputeTransform(const Matrix& matrix, const SceneTreeResource& treeResource)
		{
			const Matrix m = matrix * treeResource.transform;
			if (treeResource.parentIndex == -1)
			{
				return m;
			}

			const auto& parentTreeResource = _resources.at(treeResource.parentIndex);

			return ComputeTransform(m, parentTreeResource);
		}
	}
}
