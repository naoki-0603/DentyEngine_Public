#include "stdafx.h"

#include "Mesh.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"

#include "Graphics/Renderer/Material.h"
#include "Graphics/Renderer/Texture2D.h"
#include "Physics/CollisionMesh.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MaterialLibrary.h"
#include "System/Resource/Library/TextureLibrary.h"
#include "System/Resource/Library/SceneTreeLibrary.h"

namespace DentyEngine
{
    Ref<Texture2D> Mesh::_meshAssetTexture = nullptr;

    Mesh::Mesh() :
        Object(), _subMesh(), _createOnlyMeshes(), _parentNode(), _useDefaultMaterial()
    {
    }

    Mesh::Mesh(std::string_view name) :
        Object(), _root(), _materialFilePaths(), _subMeshes(),
        _materials(), _allVerticesBoundingBox(), _boundingBox(), _systemUnit(),
        _id(), _subMesh(), _createOnlyMeshes(true), _parentNode(), _useDefaultMaterial()
    {
        _name = name;
    }

    Mesh::Mesh(const Mesh& source)
    {
        CopyOf(&source);
    }

    void Mesh::OnGui()
    {
        ImGui::Text("MeshName: %s", _name.CStr());
    }

    void Mesh::OnEvent([[maybe_unused]] Event* e)
    {
    }

    void Mesh::OnSerialized() const
    {
    }

    void Mesh::OnDeserialized()
    {
        // Restore scene tree if parent node.
        if (_parentNode)
        {
            const SceneTreeLibrary* sceneTreeLibrary = LibraryManager::GetInstance().GetSceneTreeLibrary();
            _sceneTree = sceneTreeLibrary->Find(_sceneTreeFilePath).value();

            return;
        }

        // Restore materials.
        {
            const MaterialLibrary* materialLibrary = LibraryManager::GetInstance().GetMaterialLibrary();

            for (const auto& filePath : _materialFilePaths)
            {
                const auto material = materialLibrary->Find(filePath);
                _materials.emplace_back(material.value());
            }
        }
    }

    void Mesh::Save([[maybe_unused]] bool isUpdate) const
    {
        DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)
    }

    void Mesh::Load([[maybe_unused]] const std::filesystem::path& filePath)
    {
    }

    void Mesh::Rename(std::string_view newName, bool deleteOldFile)
    {
        _name = newName;

        if (deleteOldFile)
        {
            _filePathToSerialized.RemoveFile();
        }

        /*DENTY_REPLACE_FILENAME(_filePath, newName);
        DENTY_REPLACE_FILENAME(_filePathToSerialized, newName);*/
    }

    void Mesh::OnReload()
    {
        auto path = _filePathToSerialized;
        path = path.ReplaceExtension(".png");

        if (path.IsValid())
            return;

        // If not take preview texture.
        //AssetPhotoStadium::GetInstance().TakePhoto(this);
    }

    void Mesh::Clone(const Ref<Asset>& asset)
    {
        DENTY_ASSERT(
            asset->GetAssetType() == AssetType::Mesh ||
            asset->GetAssetType() == AssetType::CollisionMesh ||
            asset->GetAssetType() == AssetType::ApproximationCollisionMesh,
            "Asset type must be Mesh, Collision Mesh or Approximation Collision Mesh! (in Mesh::Clone)"
        );

        auto&& target = std::static_pointer_cast<Mesh>(asset);

        target->CopyOf(this);
    }

    void Mesh::AddMaterial(const Ref<Material>& material)
    {
        _materials.emplace_back(material);
        _materialFilePaths.emplace_back(material->GetFilePathToSerialized());
    }

    void Mesh::AddSubMesh(const Ref<Mesh>& mesh)
    {
        mesh->_subMesh = true;
        _subMeshes.emplace_back(mesh);
    }

    void Mesh::RemoveMaterial(std::string_view name)
    {
        const auto index = ObtainMaterialIndex(name);
        if (!index.has_value())
        {
            DENTY_SET_ERR_COLOR
             DENTY_ERR_LOG << "Material " << name << " not found!";

            return;
        }

        _materials.erase(std::ranges::find(_materials, _materials.at(index.value())));

        DENTY_SET_INFO_COLOR
        DENTY_INFO_LOG << "Succeed to remove " << name << " material from mesh!";
    }

    void Mesh::ChangeMaterial(std::string_view targetName, const Ref<Material>& newMaterial)
    {
        // Replace material.
        {
            const auto material = FindMaterialByName(targetName);

            if (not material)
            {
                DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Material " << targetName << " not found!";

                return;
            }

            RemoveMaterial(material->GetName());
            AddMaterial(newMaterial);

            DENTY_SET_INFO_COLOR
            DENTY_INFO_LOG << "Material change from " << targetName << " to " << newMaterial->GetName() << "!";
        }
    }

    template <>
    Ref<Mesh> Mesh::FindSubMeshAt(int index) const
    {
        const int count = static_cast<int>(GetSubMeshCount());

        // Stack overflow.
        if (index < 0 || index > count)
        {
            DENTY_SET_ERR_COLOR

                DENTY_ERR_LOG << "Index overflow. (in Mesh::FindSubMeshAt)";

            return nullptr;
        }

        return std::static_pointer_cast<Mesh>(_subMeshes.at(index));
    }

    template <>
    Ref<StaticMesh> Mesh::FindSubMeshAt(int index) const
    {
        const int count = static_cast<int>(GetSubMeshCount());

        // Index out of range.
        if (index < 0 || index > count)
        {
            DENTY_SET_ERR_COLOR

                DENTY_ERR_LOG << "Index out of range! (in Mesh::FindSubMeshAt)";

            return nullptr;
        }

        return std::static_pointer_cast<StaticMesh>(_subMeshes.at(index));
    }

    template <>
    Ref<SkinnedMesh> Mesh::FindSubMeshAt(int index) const
    {
        const int count = static_cast<int>(GetSubMeshCount());

        // Stack overflow.
        if (index < 0 || index > count)
        {
            DENTY_SET_ERR_COLOR

                DENTY_ERR_LOG << "Index overflow. (in Mesh::FindSubMeshAt)";

            return nullptr;
        }

        return std::static_pointer_cast<SkinnedMesh>(_subMeshes.at(index));
    }

    template <>
    Ref<CollisionMesh> Mesh::FindSubMeshAt(int index) const
    {
        const int count = static_cast<int>(GetSubMeshCount());

        // Stack overflow.
        if (index < 0 || index > count)
        {
            DENTY_SET_ERR_COLOR

                DENTY_ERR_LOG << "Index overflow. (in Mesh::FindSubMeshAt)";

            return nullptr;
        }

        return std::static_pointer_cast<CollisionMesh>(_subMeshes.at(index));
    }

    template <>
    Ref<Mesh> Mesh::FindSubMeshByName(std::string_view name) const
    {
        for (const auto& subMesh : _subMeshes)
        {
            if (subMesh->_name.Get() != name)
                continue;

            return subMesh;
        }

        return nullptr;
    }

    template <>
    Ref<StaticMesh> Mesh::FindSubMeshByName(std::string_view name) const
    {
        for (const auto& subMesh : _subMeshes)
        {
            if (subMesh->_name.Get() != name)
                continue;

            return std::static_pointer_cast<StaticMesh>(subMesh);
        }

        return nullptr;
    }

    template <>
    Ref<SkinnedMesh> Mesh::FindSubMeshByName(std::string_view name) const
    {
        for (const auto& subMesh : _subMeshes)
        {
            if (subMesh->_name.Get() != name)
                continue;

            return std::static_pointer_cast<SkinnedMesh>(subMesh);
        }

        return nullptr;
    }

    std::optional<int32_t> Mesh::FindIndexByName(std::string_view name) const
    {
        for (int subMeshIndex = 0; subMeshIndex < GetSubMeshCount(); ++subMeshIndex)
        {
            const Ref<Mesh> subMesh = GetSubMeshes().at(subMeshIndex);

            if (subMesh->GetName() != name)
                continue;

            return subMeshIndex;
        }

        return std::nullopt;
    }

    std::optional<uint32_t> Mesh::ObtainMaterialIndex(std::string_view name) const
    {
        uint32_t index = 0u;
        for (const auto& material : _materials)
        {
            if (material->GetName() != name)
            {
                ++index;
                continue;
            }

            return index;
        }

        return std::nullopt;
    }

    Ref<Material> Mesh::FindMaterialAt(int index) const
    {
        const int count = static_cast<int>(GetMaterialCount());

        if (index < 0 || index > count)
        {
            DENTY_SET_ERR_COLOR

                DENTY_ERR_LOG << "Index overflow. (in Mesh::FindMaterialAt)";

            return nullptr;
        }

        return _materials.at(index);
    }

    Ref<Material> Mesh::FindMaterialByName(std::string_view name) const
    {
        for (const auto& material : _materials)
        {
            if (material->GetName() == name)
            {
                return material;
            }
        }

        return nullptr;
    }

    bool Mesh::HasMaterial(std::string_view name) const
    {
        for (const auto& material : _materials)
        {
            if (material->GetName() == name)
                return true;
        }

        return false;
    }

    void Mesh::LoadAssetTexture()
    {
        DENTY_ASSERT(not _meshAssetTexture, "Mesh asset texture already loaded!");

        const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
        _meshAssetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::MESH_DEFAULT_THUMBNAIL_FILE_PATH).value();

        DENTY_SET_INFO_COLOR
        DENTY_INFO_LOG << "Mesh asset texture successfully loaded!";
    }

    void Mesh::CopyOf(const Mesh* source)
    {
        // Copy member values.
        {
            // Object class
            {
                _name = source->_name;
                _id = source->_id;
            }

            // Assets class
            {
                _filePath = source->_filePath;
                _filePathToSerialized = source->_filePathToSerialized;
            }

            // This class
            {
                _root = source->_root;
                _nodeName = source->_nodeName;
                _allVerticesBoundingBox = source->_allVerticesBoundingBox;

                _boundingBox = source->_boundingBox;
                _systemUnit = source->_systemUnit;

                // Clone materials
                {
                    for (const auto& material : source->_materials)
                    {
                        _materials.emplace_back(material);
                        _materialFilePaths.emplace_back(material->GetFilePathToSerialized());
                    }
                }

                _sceneTreeFilePath = source->_sceneTreeFilePath;
                _sceneTree = source->_sceneTree;

                _subMesh = source->_subMesh;
                _parentNode = source->_parentNode;
                _createOnlyMeshes = source->_createOnlyMeshes;
                _useDefaultMaterial = source->_useDefaultMaterial;
            }
        }
    }
};