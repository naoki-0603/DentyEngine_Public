#include "stdafx.h"

#include "MeshImporter.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"
#include "System/Resource/Library/TextureLibrary.h"
#include "System/Resource/Library/MaterialLibrary.h"

#include "System/Serializer/SerializeExtensions.h"

#include "Graphics/Mesh/StaticMesh.h"
#include "Graphics/Mesh/SkinnedMesh.h"

#include "Graphics/Renderer/Material.h"
#include "Graphics/Renderer/Texture2D.h"

#include "Graphics/Buffer/DirectX/DXIndexBuffer.h"
#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"

#include "Utilities/Utilities.h"

#include "System/Core/DString.h"

#include <assimp/postprocess.h>
#include <assimp/material.h>

#include "Editor/Parts/EditorContentBrowser.h"
#include "Physics/CollisionMesh.h"

#include "System/Resource/Library/AnimationClipLibrary.h"
#include "System/Resource/Library/SceneTreeLibrary.h"
#include "System/Resource/Library/SkeletonLibrary.h"

namespace DentyEngine
{
    namespace Utils
    {
        std::string AiStringToString(const aiString& aiString)
        {
            std::string str = aiString.C_Str();

            return str;
        }
    }
	
    void AssimpScene::Skeleton::Bone::NecessityNode::Fetch(const AssimpScene& scene, const Bone& bone, const std::vector<Node>& nodes)
    {
        const auto numNodes = static_cast<int32_t>(nodes.size());
        for (int localNodeIndex = 0; localNodeIndex < numNodes; ++localNodeIndex)
        {
            const auto& node = nodes.at(localNodeIndex);

            if (bone.name != node.name)
                continue;

            // If root node, add then break, go next bone.
            if (node.rootNode)
            {
                map.emplace_back(std::make_pair(node.name, node));
                break;
            }

            RecursivelyRise(scene, node);

            break;
        }
    }

    void AssimpScene::Skeleton::Bone::NecessityNode::Process([[maybe_unused]] const AssimpScene& scene)
    {
        // Sort
        {
            std::ranges::sort(map,
                [](const std::pair<std::string, AssimpScene::Node>& a, const std::pair<std::string, AssimpScene::Node>& b) -> bool
                {
                    return (a.second.nodeIndex < b.second.nodeIndex);
                }
            );
        }
    }

    void AssimpScene::Skeleton::Bone::NecessityNode::RecursivelyRise(const AssimpScene& scene,
        const AssimpScene::Node& node)
    {
        if (not node.parent)
            return;

        // Find parent node.
        const auto& parentNode = scene.FindNodeByName(node.parent->mName.C_Str());
        {
            map.emplace_back(std::make_pair(parentNode->name, *parentNode));
        }

        RecursivelyRise(scene, *parentNode);
    }

    AssimpScene::Material::Material() noexcept :
        name(), filePath(), convertedFilePath(), diffuse(), emissive(), textures()
    {
        textures.resize(static_cast<size_t>(MaterialTextureIndex::Max));
    }

    std::optional<AssimpScene::Animation::Node::VectorKey> AssimpScene::Animation::Node::FindPositionKeyByTime(float time) const
    {
        for (const auto& positionKey : positionKeys)
        {
            if (!Utils::ApproxEqual(static_cast<float>(positionKey.time), time, MeshImporter::COMPARE_ANIMATION_TIME_EPSILON))
                continue;

            return positionKey;
        }

        return std::nullopt;
    }

    std::optional<AssimpScene::Animation::Node::QuaternionKey> AssimpScene::Animation::Node::FindRotationKeyByTime(float time) const
    {
        for (const auto& rotationKey : rotationKeys)
        {
            if (!Utils::ApproxEqual(static_cast<float>(rotationKey.time), time, MeshImporter::COMPARE_ANIMATION_TIME_EPSILON))
                continue;

            return rotationKey;
        }

        return std::nullopt;
    }

    std::optional<AssimpScene::Animation::Node::VectorKey> AssimpScene::Animation::Node::FindScalingKeyByTime(float time) const
    {
        for (const auto& scalingKey : scalingKeys)
        {
            if (!Utils::ApproxEqual(static_cast<float>(scalingKey.time), time, MeshImporter::COMPARE_ANIMATION_TIME_EPSILON))
                continue;

            return scalingKey;
        }

        return std::nullopt;
    }

    std::optional<AssimpScene::Animation::Node::VectorKey> AssimpScene::Animation::Node::FindPositionKeyBetweenBeginTimeAndEndTime(float beginTime, float endTime) const
    {
        for (const auto& positionKey : positionKeys)
        {
            if (beginTime < positionKey.time && endTime > positionKey.time)
            {
                return positionKey;
            }
        }

        return std::nullopt;
    }

    std::optional<AssimpScene::Animation::Node::QuaternionKey> AssimpScene::Animation::Node::FindRotationKeyBetweenBeginTimeAndEndTime(float beginTime, float endTime) const
    {
        for (const auto& rotationKey : rotationKeys)
        {
            if (beginTime < rotationKey.time && endTime > rotationKey.time)
            {
                return rotationKey;
            }
        }

        return std::nullopt;
    }

    std::optional<AssimpScene::Animation::Node::VectorKey> AssimpScene::Animation::Node::FindScalingKeyBetweenBeginTimeAndEndTime(float beginTime, float endTime) const
    {
        for (const auto& scalingKey : scalingKeys)
        {
            if (beginTime < scalingKey.time && endTime > scalingKey.time)
            {
                return scalingKey;
            }
        }

        return std::nullopt;
    }

    void AssimpScene::Animation::Convert()
    {
        const auto keyCount = static_cast<uint32_t>(ticksPerDuration / ticksPerFrame) + 1u;

    	for (uint32_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
        {
            auto&& node = nodes.at(nodeIndex);

            // Position keys
            {
                std::vector<Node::VectorKey> convertedPositionKeys = { };
                std::vector<uint32_t> needToConvertIndexes = {};

                for (uint32_t positionKeyIndex = 0; positionKeyIndex < node.positionKeys.size(); ++positionKeyIndex)
                {
                    bool isNeedToConvert = false;
                    for (float time = 0.0f; time <= ticksPerDuration; time += ticksPerFrame)
                    {
                        const auto nodeTime = static_cast<float>(node.positionKeys.at(positionKeyIndex).time);
                        if (Utils::ApproxEqual(nodeTime, time, MeshImporter::COMPARE_ANIMATION_TIME_EPSILON))
                        {
                            convertedPositionKeys.emplace_back(node.positionKeys.at(positionKeyIndex));
                            break;
                        }

                        if (time >= ticksPerDuration - ticksPerFrame)
                        {
                            isNeedToConvert = true;
                        }
                    }

                    if (isNeedToConvert)
                    {
                        needToConvertIndexes.emplace_back(positionKeyIndex);
                    }
                }

                if (node.positionKeys.size() < keyCount)
                {
                    [[maybe_unused]] const uint32_t keySize = keyCount - static_cast<uint32_t>(node.positionKeys.size());
                    const auto startIndex = static_cast<uint32_t>(node.positionKeys.size());

                    const auto endElement = convertedPositionKeys.at(startIndex - 1);
                    for (uint32_t keyIndex = startIndex; keyIndex < keyCount; ++keyIndex)
                    {
                        Node::VectorKey positionKey = { };
                        positionKey.value = endElement.value;
                        positionKey.time = ticksPerFrame * static_cast<double>(keyIndex);

                        convertedPositionKeys.emplace_back(positionKey);
                    }
                }

                // Convert
                if (!needToConvertIndexes.empty())
                {
                }

                node.positionKeys = convertedPositionKeys;
            }

            // Rotation keys
            {
                std::vector<Node::QuaternionKey> convertedRotationKeys = { };
                std::vector<uint32_t> needToConvertIndexes = {};
                for (uint32_t rotationKeyIndex = 0; rotationKeyIndex < node.rotationKeys.size(); ++rotationKeyIndex)
                {
                    bool isNeedToConvert = false;
                    for (float time = 0.0f; time <= ticksPerDuration; time += ticksPerFrame)
                    {
                        const auto nodeTime = static_cast<float>(node.rotationKeys.at(rotationKeyIndex).time);
                        if (Utils::ApproxEqual(nodeTime, time, MeshImporter::COMPARE_ANIMATION_TIME_EPSILON))
                        {
                            convertedRotationKeys.emplace_back(node.rotationKeys.at(rotationKeyIndex));
                            break;
                        }

                        if (time >= ticksPerDuration - ticksPerFrame)
                        {
                            isNeedToConvert = true;
                        }
                    }

                    if (isNeedToConvert)
                    {
                        needToConvertIndexes.emplace_back(rotationKeyIndex);
                    }
                }

                if (node.rotationKeys.size() < keyCount)
                {
                    [[maybe_unused]] const uint32_t keySize = keyCount - static_cast<uint32_t>(node.rotationKeys.size());
                    const auto startIndex = static_cast<uint32_t>(node.rotationKeys.size());

                    const auto endElement = convertedRotationKeys.at(startIndex - 1);
                    for (uint32_t keyIndex = startIndex; keyIndex < keyCount; ++keyIndex)
                    {
                        Node::QuaternionKey rotationKey = {};
                        rotationKey.value = endElement.value;
                        rotationKey.time = ticksPerFrame * static_cast<double>(keyIndex);

                        convertedRotationKeys.emplace_back(rotationKey);
                    }
                }

                // Convert
                if (!needToConvertIndexes.empty())
                {
                }

                node.rotationKeys = convertedRotationKeys;
            }

            // Scaling keys
            {
                std::vector<Node::VectorKey> convertedScalingKeys = { };
                std::vector<uint32_t> needToConvertIndexes = {};
                for (uint32_t scalingKeyIndex = 0; scalingKeyIndex < node.scalingKeys.size(); ++scalingKeyIndex)
                {
                    bool isNeedToConvert = false;
                    for (float time = 0.0f; time <= ticksPerDuration; time += ticksPerFrame)
                    {
                        const auto nodeTime = static_cast<float>(node.scalingKeys.at(scalingKeyIndex).time);
                        if (Utils::ApproxEqual(nodeTime, time, MeshImporter::COMPARE_ANIMATION_TIME_EPSILON))
                        {
                            convertedScalingKeys.emplace_back(node.scalingKeys.at(scalingKeyIndex));
                            break;
                        }

                        if (time >= ticksPerDuration - ticksPerFrame)
                        {
                            isNeedToConvert = true;
                        }
                    }

                    if (isNeedToConvert)
                    {
                        needToConvertIndexes.emplace_back(scalingKeyIndex);
                    }
                }

                if (node.scalingKeys.size() < keyCount)
                {
                    [[maybe_unused]] const uint32_t keySize = keyCount - static_cast<uint32_t>(node.scalingKeys.size());
                    const auto startIndex = static_cast<uint32_t>(node.scalingKeys.size());

                    const auto endElement = convertedScalingKeys.at(startIndex - 1);
                    for (uint32_t keyIndex = startIndex; keyIndex < keyCount; ++keyIndex)
                    {
                        Node::VectorKey scalingKey = {};
                        scalingKey.value = endElement.value;
                        scalingKey.time = ticksPerFrame * static_cast<double>(keyIndex);

                        convertedScalingKeys.emplace_back(scalingKey);
                    }
                }

                // Convert
                if (!needToConvertIndexes.empty())
                {
                }

                node.scalingKeys = convertedScalingKeys;
            }

        }
    }

    const uint32_t MeshImporter::AI_POST_PROCESS_STEPS_STATIC_MESH = 
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenNormals |
        aiProcess_FixInfacingNormals |
        aiProcess_OptimizeMeshes |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_ConvertToLeftHanded;

    const uint32_t MeshImporter::AI_POST_PROCESS_STEPS_SKINNED_MESH =
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_PopulateArmatureData |
        aiProcess_GenNormals |
        aiProcess_FixInfacingNormals |
        aiProcess_ConvertToLeftHanded;

    const uint32_t MeshImporter::AI_POST_PROCESS_STEPS_COLLISION_MESH =
        AI_POST_PROCESS_STEPS_STATIC_MESH |
        aiProcess_SplitLargeMeshes;

    MeshImporter::MeshImporter() noexcept :
        _scene(), _skeleton(), _sceneTree(), _nodes(), _nodeIndexCounter(), _mesh(), _scaleFactor(), _isSkinned()
    {
        _skeleton = std::make_shared<MeshResource::Skeleton>();
    }

    Ref<Mesh> MeshImporter::Import(const char* filePath, ImportType importType, bool fixName)
    {
        if (importType == ImportType::ImportAsCollisionMesh)
        {
            return ImportAsCollision(filePath, importType, fixName);
        }

        // Import as mesh.
        {
            Assimp::Importer importer = {};
            const aiScene* aiScene = nullptr;

            // Import from filepath.
            {
                aiScene = importer.ReadFile(filePath,
                    AI_POST_PROCESS_STEPS_SKINNED_MESH
                );

                if (aiScene == nullptr)
                {
                    DENTY_SET_ERR_COLOR
                    DENTY_ERR_LOG << "Failed to load " << filePath << "Mesh!";

                    return nullptr;
                }

                _scene.filePath = std::filesystem::path(filePath);
            }

            // Get scale factor.
            {
                float scaleFactor = 0.0f;
                aiScene->mMetaData->Get("UnitScaleFactor", scaleFactor);

                _scaleFactor = static_cast<uint32_t>(scaleFactor);
            }

            {
                {
                    TraverseNode(aiScene, aiScene->mRootNode);

                    FetchMeshes(aiScene);

                    SetNodeBoneInfos(aiScene);
                }

                // If doesn't have bones, reimport as static.
                if (!_scene.skeleton.HasBone())
                {
                    Reset();

                    return ReImportAsStatic(filePath, importType, fixName);
                }

                FetchSkeletonNecessityMap(aiScene);

                // Set nodes has skeleton.
                SetSkeletonInfo();

                FetchMaterials(aiScene);

                if (aiScene->HasAnimations())
                {
                    FetchAnimations(aiScene);
                }
            }

            {
                CreateNodes();
                CreateSceneTree();
                CreateSkeleton();
                CreateMaterials();

                CreateMeshes(_skeleton->HasBone(), fixName);
            }

            {
                SetBoneParentNames();
                ComputeBoneGlobalTransforms();
            }

            if (_scene.HasAnimations())
            {
                for (int animationIndex = 0; animationIndex < _scene.animations.size(); ++animationIndex)
                {
                    const auto& aiAnimation = _scene.animations.at(animationIndex);

                    CreateAnimation(aiAnimation);
                }
            }

            SetMeshInfos();

            return _mesh;

        }
    }

    void MeshImporter::TraverseNode(const aiScene* aiScene, aiNode* aiNode)
    {
        auto&& node = _scene.nodes.emplace_back();
        node.name = aiNode->mName.C_Str();
        node.parent = aiNode->mParent;
        node.transform = aiNode->mTransformation;
        node.nodeIndex = _nodeIndexCounter++;
        node.meshIndexes = aiNode->mMeshes;
        node.numMeshes = aiNode->mNumMeshes;
        node.numChildren = aiNode->mNumChildren;
        node.children = aiNode->mChildren;
        node.rootNode = (aiNode->mParent ? false : true);

        for (int childIndex = 0; childIndex < static_cast<int32_t>(aiNode->mNumChildren); ++childIndex)
        {
            TraverseNode(aiScene, aiNode->mChildren[childIndex]);
        }
    }

    void MeshImporter::FetchMeshes(const aiScene* aiScene)
    {
        for (int nodeIndex = 0; nodeIndex < _scene.nodes.size(); ++nodeIndex)
        {
            const auto& node = _scene.nodes.at(nodeIndex);

            if (node.numMeshes == 0)
                continue;

            // Fetch meshes.
            for (int meshIndex = 0; meshIndex < static_cast<int32_t>(node.numMeshes); ++meshIndex)
            {
                const uint32_t index = node.meshIndexes[meshIndex];
                const auto aiMesh = aiScene->mMeshes[index];

                const bool hasNormals = aiMesh->HasNormals();
                const bool hasTangentsAndBitangents = aiMesh->HasTangentsAndBitangents();
                const bool hasTexcoords = aiMesh->HasTextureCoords(0);
                const bool hasColors = aiMesh->HasVertexColors(0);

                auto&& mesh = _scene.meshes.emplace_back();
                mesh.name = aiMesh->mName.C_Str();
                mesh.nodeIndex = nodeIndex;
                mesh.materialIndex = aiMesh->mMaterialIndex;

                mesh.vertices.resize(aiMesh->mNumVertices);
                mesh.indices.resize(aiMesh->mNumFaces * 3u);

                // Fetch Vertices
                for (int vertexIndex = 0; vertexIndex < mesh.vertices.size(); ++vertexIndex)
                {
                    auto&& vertex = mesh.vertices.at(vertexIndex);
                    vertex.position = aiMesh->mVertices[vertexIndex];

                    if (hasNormals)
                    {
                        vertex.normal = aiMesh->mNormals[vertexIndex];
                    }

                    if (hasTangentsAndBitangents)
                    {
                        vertex.tangent = aiMesh->mTangents[vertexIndex];
                        vertex.binormal = aiMesh->mBitangents[vertexIndex];
                    }

                    if (hasTexcoords)
                    {
                        vertex.texcoord = aiMesh->mTextureCoords[0][vertexIndex];
                    }

                    if (hasColors)
                    {
                        vertex.color = aiMesh->mColors[0][vertexIndex];
                    }
                }

                // Fetch indices
                int indicesCounter = 0;
                for (int faceIndex = 0; faceIndex < static_cast<int32_t>(aiMesh->mNumFaces); ++faceIndex)
                {
                    const auto face = aiMesh->mFaces[faceIndex];

                    for (int indicesIndex = 0; indicesIndex < static_cast<int>(face.mNumIndices); indicesIndex += 3)
                    {
                        auto&& index0 = mesh.indices.at(indicesCounter++);
                        auto&& index1 = mesh.indices.at(indicesCounter++);
                        auto&& index2 = mesh.indices.at(indicesCounter++);

                        index0 = face.mIndices[indicesIndex];
                        index1 = face.mIndices[indicesIndex + 1];
                        index2 = face.mIndices[indicesIndex + 2];
                    }
                }

                // Fetch bones.
                if (aiMesh->HasBones())
                {
                    FetchBones(mesh, aiMesh);
                }
            }
        }
    }

    void MeshImporter::FetchBones(AssimpScene::Mesh& mesh, const aiMesh* aiMesh)
    {
        const uint32_t numBones = aiMesh->mNumBones;

        mesh.boneInfo.resize(aiMesh->mNumBones);
        for (int boneIndex = 0; boneIndex < mesh.boneInfo.size(); ++boneIndex)
        {
            const auto aiBone = aiMesh->mBones[boneIndex];
            const auto aiNode = _scene.FindNodeByName(aiBone->mNode->mName.C_Str());

            mesh.boneInfo.at(boneIndex).nodeIndex = aiNode->nodeIndex;
            mesh.boneInfo.at(boneIndex).offsetMatrix = Matrix::ToMatrix(aiBone->mOffsetMatrix).Transpose();
        }

        for (int boneIndex = 0; boneIndex < static_cast<int>(numBones); ++boneIndex)
        {
            const auto aiBone = aiMesh->mBones[boneIndex];

            auto&& bone = mesh.boneInfo.at(boneIndex);
            bone.name = aiBone->mName.C_Str();

            bone.weights.resize(aiBone->mNumWeights);
            for (int weightIndex = 0; weightIndex < static_cast<int>(aiBone->mNumWeights); ++weightIndex)
            {
                bone.weights.at(weightIndex).vertexID = static_cast<uint32_t>(aiBone->mWeights[weightIndex].mVertexId);
                bone.weights.at(weightIndex).weight = static_cast<float>(aiBone->mWeights[weightIndex].mWeight);
            }
        }
    }

    void MeshImporter::FetchSkeletonNecessityMap([[maybe_unused]] const aiScene* aiScene)
    {
        for (int boneIndex = 0; boneIndex < _scene.skeleton.bones.size(); ++boneIndex)
        {
            auto&& bone = _scene.skeleton.bones.at(boneIndex);

            bone.necessityNode.Fetch(_scene, bone, _scene.nodes);
            bone.necessityNode.Process(_scene);
        }
    }

    void MeshImporter::FetchMaterials(const aiScene* aiScene)
    {
        for (int materialIndex = 0; materialIndex < static_cast<int>(aiScene->mNumMaterials); ++materialIndex)
        {
            const auto aiMaterial = aiScene->mMaterials[materialIndex];

            auto&& material = _scene.materials.emplace_back();
            material.name = aiMaterial->GetName().C_Str();

            // Set make unique name.
            if (material.name.empty())
            {
                const FilePath directory = _scene.filePath.ObtainDirectory();

                const auto uniqueName = EditorContentBrowser::MakeUniqueNameInDirectory(directory, "NoName", Asset::AssetType::Material);
                if (!uniqueName.has_value())
                {
                    DENTY_SET_ERR_COLOR
                    DENTY_ERR_LOG << "Couldn't be make unique name!";

                    DENTY_ASSERT(false, "");
                }

                material.name = uniqueName.value();
            }

            auto filePath = _scene.filePath;
            material.filePath = filePath.ReplaceFilename(material.name).ReplaceExtension(MATERIAL_FILE_EXTENSION).GetAsString();
            material.convertedFilePath = material.filePath;

            // Get material colors.
            {
                aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, material.diffuse);
                aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, material.emissive);
            }

            // Debug
            {
                /*DENTY_INFO_LOG << "AITextureTypeDiffuseCount: " << aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
                DENTY_INFO_LOG << "AITextureTypeSpecularCount: " << aiMaterial->GetTextureCount(aiTextureType_SPECULAR);
                DENTY_INFO_LOG << "AITextureTypeAmbientCount: " << aiMaterial->GetTextureCount(aiTextureType_AMBIENT);
                DENTY_INFO_LOG << "AITextureTypeEmissiveCount: " << aiMaterial->GetTextureCount(aiTextureType_EMISSIVE);
                DENTY_INFO_LOG << "AITextureTypeHeightCount: " << aiMaterial->GetTextureCount(aiTextureType_HEIGHT);
                DENTY_INFO_LOG << "AITextureTypeNormalsCount: " << aiMaterial->GetTextureCount(aiTextureType_NORMALS);
                DENTY_INFO_LOG << "AITextureTypeShiniessCount: " << aiMaterial->GetTextureCount(aiTextureType_SHININESS);
                DENTY_INFO_LOG << "AITextureTypeOpacityCount: " << aiMaterial->GetTextureCount(aiTextureType_OPACITY);
                DENTY_INFO_LOG << "AITextureTypeDisplacementCount: " << aiMaterial->GetTextureCount(aiTextureType_DISPLACEMENT);
                DENTY_INFO_LOG << "AITextureTypeLightCount: " << aiMaterial->GetTextureCount(aiTextureType_LIGHTMAP);
                DENTY_INFO_LOG << "AITextureTypeReflectionCount: " << aiMaterial->GetTextureCount(aiTextureType_REFLECTION);
                DENTY_INFO_LOG << "AITextureTypeBaseCount: " << aiMaterial->GetTextureCount(aiTextureType_BASE_COLOR);
                DENTY_INFO_LOG << "AITextureTypeNormalCameraCount: " << aiMaterial->GetTextureCount(aiTextureType_NORMAL_CAMERA);
                DENTY_INFO_LOG << "AITextureTypeEmissionCount: " << aiMaterial->GetTextureCount(aiTextureType_EMISSION_COLOR);
                DENTY_INFO_LOG << "AITextureTypeNormalCameraCount: " << aiMaterial->GetTextureCount(aiTextureType_METALNESS);
                DENTY_INFO_LOG << "AITextureTypeDiffuseRoughnessCount: " << aiMaterial->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
                DENTY_INFO_LOG << "AITextureTypeAmbientOcculusionCount: " << aiMaterial->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
                DENTY_INFO_LOG << "AITextureTypeSheenCount: " << aiMaterial->GetTextureCount(aiTextureType_SHEEN);
                DENTY_INFO_LOG << "AITextureTypeClearCoatCount: " << aiMaterial->GetTextureCount(aiTextureType_CLEARCOAT);
                DENTY_INFO_LOG << "AITextureTypeTransmissionCount: " << aiMaterial->GetTextureCount(aiTextureType_TRANSMISSION);
                DENTY_INFO_LOG << "AITextureTypeUnknownCount: " << aiMaterial->GetTextureCount(aiTextureType_UNKNOWN);*/
            }

            const FilePath directory = _scene.filePath.ObtainDirectory();

            // Get textures. (one texture each for now.
            {
                // Get base color texture.
                if (aiMaterial->GetTextureCount(aiTextureType_BASE_COLOR) >= 1)
                {
                    {
                        aiString textureFilePath;
                        aiMaterial->GetTexture(aiTextureType_BASE_COLOR, 0, &textureFilePath);

                    	const std::string texturePath = Utils::AiStringToString(textureFilePath);

                        AssimpScene::Material::Texture albedo = { };
                        albedo.filePath = directory.GetAsString() + texturePath;

                        material.textures.at(static_cast<size_t>(MaterialTextureIndex::Albedo)) = albedo;
                    }
                }
                else if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) >= 1)
                {
	                {
                        aiString textureFilePath;
                        aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);

                        const std::string texturePath = Utils::AiStringToString(textureFilePath);

                        AssimpScene::Material::Texture albedo = { };
                        albedo.filePath = directory.GetAsString() + texturePath;

                        material.textures.at(static_cast<size_t>(MaterialTextureIndex::Albedo)) = albedo;
	                }
                }

                // Get metalness texture.
                if (aiMaterial->GetTextureCount(aiTextureType_METALNESS) >= 1)
                {
                    {
                        aiString textureFilePath;
                        aiMaterial->GetTexture(aiTextureType_METALNESS, 0, &textureFilePath);

                        const std::string texturePath = Utils::AiStringToString(textureFilePath);

                        AssimpScene::Material::Texture metalness = { };
                        metalness.filePath = directory.GetAsString() + texturePath;

                        material.textures.at(static_cast<size_t>(MaterialTextureIndex::Metalness)) = metalness;
                    }
                }
                else if (aiMaterial->GetTextureCount(aiTextureType_SPECULAR) >= 1)
                {
	                {
                        aiString textureFilePath;
                        aiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &textureFilePath);

                        const std::string texturePath = Utils::AiStringToString(textureFilePath);

                        AssimpScene::Material::Texture metalness = { };
                        metalness.filePath = directory.GetAsString() + texturePath;

                        material.textures.at(static_cast<size_t>(MaterialTextureIndex::Metalness)) = metalness;
	                }
                }

                // Get normal texture.
                if (aiMaterial->GetTextureCount(aiTextureType_NORMALS) >= 1)
                {
	                {
                        aiString textureFilePath;
                        aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &textureFilePath);

	                	const std::string texturePath = Utils::AiStringToString(textureFilePath);

                        AssimpScene::Material::Texture normal = { };
                        normal.filePath = directory.GetAsString() + texturePath;

                        material.textures.at(static_cast<size_t>(MaterialTextureIndex::Normal)) = normal;
	                }
                }

                // Get roughness texture
                if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) >= 1)
                {
	                {
                        aiString textureFilePath;
                        aiMaterial->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &textureFilePath);

                        const std::string texturePath = Utils::AiStringToString(textureFilePath);

                        AssimpScene::Material::Texture roughness = { };
                        roughness.filePath = directory.GetAsString() + texturePath;

                        material.textures.at(static_cast<size_t>(MaterialTextureIndex::Roughness)) = roughness;
	                }
                }

                // Get emissive texture
                if (aiMaterial->GetTextureCount(aiTextureType_EMISSIVE) >= 1)
                {
                    {
                        aiString textureFilePath;
                        aiMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &textureFilePath);

                        const std::string texturePath = Utils::AiStringToString(textureFilePath);

                        AssimpScene::Material::Texture emissive = { };
                        emissive.filePath = directory.GetAsString() + texturePath;

                        material.textures.at(static_cast<size_t>(MaterialTextureIndex::Emissive)) = emissive;
                    }
                }

                // Get ambientOcclusion texture
                if (aiMaterial->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) >= 1)
                {
                    {
                        aiString textureFilePath;
                        aiMaterial->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &textureFilePath);

                        const std::string texturePath = Utils::AiStringToString(textureFilePath);

                        AssimpScene::Material::Texture ambientOcclusion = { };
                        ambientOcclusion.filePath = directory.GetAsString() + texturePath;

                        material.textures.at(static_cast<size_t>(MaterialTextureIndex::AmbientOcclusion)) = ambientOcclusion;
                    }
                }
            }
        }
    }

    void MeshImporter::FetchAnimations(const aiScene* aiScene)
    {
        for (uint32_t animationIndex = 0u; animationIndex < aiScene->mNumAnimations; ++animationIndex)
        {
            const auto aiAnimation = aiScene->mAnimations[animationIndex];

            auto&& animation = _scene.animations.emplace_back();
            animation.name = aiAnimation->mName.C_Str();
            animation.ticksPerSecond = aiAnimation->mTicksPerSecond;
            animation.duration = aiAnimation->mDuration;
            animation.ticksPerDuration = static_cast<float>(animation.duration / animation.ticksPerSecond);
            animation.ticksPerFrame = 1.0f / static_cast<float>(animation.ticksPerSecond);

            for (uint32_t channelIndex = 0u; channelIndex < aiAnimation->mNumChannels; ++channelIndex)
            {
                const auto& aiNodeAnim = aiAnimation->mChannels[channelIndex];

                auto&& node = animation.nodes.emplace_back();
                node.name = aiNodeAnim->mNodeName.C_Str();

                const uint32_t numPositionKeys = aiNodeAnim->mNumPositionKeys;
                const uint32_t numRotationKeys = aiNodeAnim->mNumRotationKeys;
                const uint32_t numScalingKeys = aiNodeAnim->mNumScalingKeys;

                for (int keyIndex = 0; keyIndex < static_cast<int>(numPositionKeys); ++keyIndex)
                {
                    auto&& positionKey = node.positionKeys.emplace_back();
                    positionKey.time = aiNodeAnim->mPositionKeys[keyIndex].mTime / animation.ticksPerSecond;
                    positionKey.value = aiNodeAnim->mPositionKeys[keyIndex].mValue;
                }

                for (int keyIndex = 0; keyIndex < static_cast<int>(numRotationKeys); ++keyIndex)
                {
                    auto&& rotationKey = node.rotationKeys.emplace_back();
                    rotationKey.time = aiNodeAnim->mRotationKeys[keyIndex].mTime / animation.ticksPerSecond;
                    rotationKey.value = aiNodeAnim->mRotationKeys[keyIndex].mValue;
                }

                for (int keyIndex = 0; keyIndex < static_cast<int>(numScalingKeys); ++keyIndex)
                {
                    auto&& scalingKey = node.scalingKeys.emplace_back();
                    scalingKey.time = aiNodeAnim->mScalingKeys[keyIndex].mTime / animation.ticksPerSecond;
                    scalingKey.value = aiNodeAnim->mScalingKeys[keyIndex].mValue;
                }

                const int32_t subPositionKeyAndRotationKey = static_cast<int32_t>(numPositionKeys) - numRotationKeys;
                const int32_t subRotationKeyAndScalingKey = static_cast<int32_t>(numRotationKeys) - numScalingKeys;

                if (subPositionKeyAndRotationKey > 0)
                {
                    // Position keys is bigger.
                    // Add rotation keys current end rotation key.
                    const auto& end = node.rotationKeys.back();
                    for (int32_t index = 0; index < subPositionKeyAndRotationKey; ++index)
                    {
                        node.rotationKeys.emplace_back(end);
                    }
                }
                else if (subPositionKeyAndRotationKey != 0)
                {
                    // Rotation keys is bigger.
                    // Add position keys current end position key.
                    const auto& end = node.positionKeys.back();
                    for (int32_t index = 0; index < abs(subPositionKeyAndRotationKey); ++index)
                    {
                        node.positionKeys.emplace_back(end);
                    }
                }

                //
                // ReComparison
                //
                const auto proceedPositionKeysSize = static_cast<uint32_t>(node.positionKeys.size());
                const auto proceedRotationKeysSize = static_cast<uint32_t>(node.rotationKeys.size());
                const auto proceedScalingKeysSize = static_cast<uint32_t>(node.scalingKeys.size());

                if (proceedPositionKeysSize == proceedRotationKeysSize)
                {
                    if (proceedRotationKeysSize == proceedScalingKeysSize)
                    {
                        continue;
                    }
                }

                if (subRotationKeyAndScalingKey > 0)
                {
                    // rotation is bigger.
                    // Add scaling keys current end scaling key.
                    const auto& end = node.scalingKeys.back();
                    for (int32_t index = 0; index < subRotationKeyAndScalingKey; ++index)
                    {
                        node.scalingKeys.emplace_back(end);
                    }
                }
                else if (subRotationKeyAndScalingKey != 0)
                {
                    // Scaling is bigger
                    // Add rotation keys current end rotation key.
                    const auto& end = node.rotationKeys.back();
                    for (int32_t index = 0; index < abs(subRotationKeyAndScalingKey); ++index)
                    {
                        node.rotationKeys.emplace_back(end);
                    }
                }
            }
        }

        // Convert
        {
            for (auto&& animation : _scene.animations)
            {
                animation.Convert();
            }
        }
    }

    void MeshImporter::FetchMeshesAsStatic(const aiScene* aiScene)
    {
        for (int nodeIndex = 0; nodeIndex < _scene.nodes.size(); ++nodeIndex)
        {
            const auto& node = _scene.nodes.at(nodeIndex);

            if (node.numMeshes == 0)
                continue;

            // Fetch meshes.
            for (int meshIndex = 0; meshIndex < static_cast<int>(node.numMeshes); ++meshIndex)
            {
                const uint32_t index = node.meshIndexes[meshIndex];
                const auto aiMesh = aiScene->mMeshes[index];

                const bool hasNormals = aiMesh->HasNormals();
                const bool hasTangentsAndBitangents = aiMesh->HasTangentsAndBitangents();
                const bool hasTexcoords = aiMesh->HasTextureCoords(0);
                const bool hasColors = aiMesh->HasVertexColors(0);

                auto&& mesh = _scene.meshes.emplace_back();
                mesh.name = aiMesh->mName.C_Str();
                mesh.nodeIndex = nodeIndex;
                mesh.materialIndex = aiMesh->mMaterialIndex;

                mesh.vertices.resize(aiMesh->mNumVertices);

                // Fetch Vertices
                for (int vertexIndex = 0; vertexIndex < mesh.vertices.size(); ++vertexIndex)
                {
                    auto&& vertex = mesh.vertices.at(vertexIndex);
                    vertex.position = aiMesh->mVertices[vertexIndex];

                    if (hasNormals)
                    {
                        vertex.normal = aiMesh->mNormals[vertexIndex];
                    }

                    if (hasTangentsAndBitangents)
                    {
                        vertex.tangent = aiMesh->mTangents[vertexIndex];
                        vertex.binormal = aiMesh->mBitangents[vertexIndex];
                    }

                    if (hasTexcoords)
                    {
                        vertex.texcoord = aiMesh->mTextureCoords[0][vertexIndex];
                    }

                    if (hasColors)
                    {
                        vertex.color = aiMesh->mColors[0][vertexIndex];
                    }
                }

                uint32_t indicesCounter = 0u;
                for (int faceIndex = 0; faceIndex < static_cast<int>(aiMesh->mNumFaces); ++faceIndex)
                {
                    const auto face = aiMesh->mFaces[faceIndex];

                    if (face.mNumIndices % 3 != 0)
                        continue;

                    mesh.indices.resize(mesh.indices.size() + face.mNumIndices);
                    for (int indicesIndex = 0; indicesIndex < static_cast<int>(face.mNumIndices); indicesIndex += 3)
                    {
                        auto&& index0 = mesh.indices.at(indicesCounter++);
                        auto&& index1 = mesh.indices.at(indicesCounter++);
                        auto&& index2 = mesh.indices.at(indicesCounter++);

                        index0 = face.mIndices[indicesIndex];
                        index1 = face.mIndices[indicesIndex + 1];
                        index2 = face.mIndices[indicesIndex + 2];
                    }
                }
            }
        }
    }

    void MeshImporter::GenerateBatchedMeshes(const aiScene* aiScene)
    {
        // Fix names.
        {
            for (auto&& mesh : _scene.meshes)
            {
                const auto& node = _scene.nodes.at(mesh.nodeIndex);

                mesh.name = node.name;
            }
        }

        _scene.batchedMeshes.resize(aiScene->mNumMaterials);

        std::vector<std::string> materialNames = {};
        for (uint32_t materialIndex = 0u; materialIndex < aiScene->mNumMaterials; ++materialIndex)
        {
            const auto& aiMaterial = aiScene->mMaterials[materialIndex];
            materialNames.emplace_back(aiMaterial->GetName().C_Str());
        }

        for (const auto& mesh : _scene.meshes)
        {
            const auto& sceneTreeResource = _sceneTree->Find(mesh.name);

            std::vector<MeshResource::StaticMeshVertex> vertices = { };
            if (sceneTreeResource.has_value())
            {
                // Convert
                for (auto&& vertex : mesh.vertices)
                {
                    Vector3 position = Vector3::ToVector3(vertex.position);
                    position = sceneTreeResource->computedTransform.TransformCoord(position);

                    auto&& staticVertex = vertices.emplace_back();
                    staticVertex.position = Vector4(position, 1.0f);
                    staticVertex.normal = Vector4(Vector3::ToVector3(vertex.normal), 0.0f);
                    staticVertex.tangent = Vector4(Vector3::ToVector3(vertex.tangent), 0.0f);
                    staticVertex.binormal = Vector4(Vector3::ToVector3(vertex.binormal), 0.0f);

                    const Vector2 texcoord = Vector2(Vector3::ToVector3(vertex.texcoord).x, Vector3::ToVector3(vertex.texcoord).y);
                    staticVertex.texcoord = texcoord;
                }
            }
            else
            {
                for (auto&& vertex : mesh.vertices)
                {
                    Vector3 position = Vector3::ToVector3(vertex.position);

                    auto&& staticVertex = vertices.emplace_back();
                    staticVertex.position = Vector4(position, 1.0f);
                    staticVertex.normal = Vector4(Vector3::ToVector3(vertex.normal), 0.0f);
                    staticVertex.tangent = Vector4(Vector3::ToVector3(vertex.tangent), 0.0f);
                    staticVertex.binormal = Vector4(Vector3::ToVector3(vertex.binormal), 0.0f);

                    const Vector2 texcoord = Vector2(Vector3::ToVector3(vertex.texcoord).x, Vector3::ToVector3(vertex.texcoord).y);
                    staticVertex.texcoord = texcoord;
                }
            }

            auto&& batchedMesh = _scene.batchedMeshes.at(mesh.materialIndex);
            for (uint32_t index = 0u; index < mesh.indices.size(); ++index)
            {
                const uint32_t vertexID = mesh.indices.at(index);

                batchedMesh.vertices.emplace_back(vertices.at(vertexID));
            }

            batchedMesh.materialIndex = mesh.materialIndex;
            batchedMesh.materialName = materialNames.at(batchedMesh.materialIndex);
        }
    }

    void MeshImporter::CreateSkeleton()
    {
        // Set name and file paths.
        {
            const auto fileName = _scene.filePath.Filename().Stem();

            auto filePath = _scene.filePath;
            filePath.ReplaceFilename(fileName.GetAsString()).ReplaceExtension(AVATAR_FILE_EXTENSION);

            const auto skeleton = FindNodeByNodeType(MeshResource::NodeType::Skeleton);

            _skeleton->SetName(skeleton.at(0).name);
            _skeleton->_resource.orientation = skeleton.at(0).transform;
            _skeleton->SetFilePath(filePath.GetAsString());
            _skeleton->SetFilePathToSerialized(filePath.GetAsString());
        }

        for (const auto& aiBone : _scene.skeleton.bones)
        {
            // Fetch node local transforms.
            auto node = _scene.FindNodeByName(aiBone.name);
            Matrix localTransform = Matrix::ToMatrix(aiMatrix4x4(node->transform).Transpose());

            MeshResource::BoneResource boneResource = { };
            boneResource.name = aiBone.name;
            boneResource.localTransform = localTransform;

            // Set offset transform each meshes.
            for (const auto mesh : _scene.meshes)
            {
                boneResource.offsetTransforms.emplace_back(mesh.FindBoneInfoByName(boneResource.name)->offsetMatrix);
            }

            MeshResource::Bone bone(boneResource);
            _skeleton->AddBone(bone);
        }

        if (String(_skeleton->GetName()).IsEmpty())
        {
            return;
        }

        // Save
        {
            _skeleton->Save();
        }

        // Add to library.
        SkeletonLibrary* skeletonLibrary = LibraryManager::GetInstance().GetSkeletonLibrary();
        skeletonLibrary->Add(_skeleton);
    }

    void MeshImporter::CreateMaterials()
    {
        const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

        for (int materialIndex = 0; materialIndex < _scene.materials.size(); ++materialIndex)
        {
            const auto aiMaterial = _scene.materials.at(materialIndex);

            // Create material.
            {
                // Replace filename.
                auto filePath = _scene.filePath;
                filePath.ReplaceFilename(aiMaterial.name);

                FilePath path = ConvertMaterialPath(filePath.GetAsString());
                Ref<Shader> shader = nullptr;
                {
                    if (_skeleton->HasBone())
                    {
                        shader = shaderLibrary->Find(ShaderLibrary::SKINNED_MESH_SHADER_PATH).value();
                    }
                    else
                    {
                        shader = shaderLibrary->Find(ShaderLibrary::STATIC_MESH_SHADER_PATH).value();
                    }
                }

                MaterialInitData materialInitData = { };

                // Set attributes.
                materialInitData.name = aiMaterial.name;
                materialInitData.shader = shader;
                materialInitData.filePath = filePath;
                materialInitData.parameters.baseColor = Vector4(Vector3::ToVector3(aiMaterial.diffuse), 1.0f);
                materialInitData.parameters.emissiveColor = Vector4(Vector3::ToVector3(aiMaterial.emissive), 1.0f);

                // Create material instance.
                const auto material = std::make_shared<Material>();
                material->Create(materialInitData);

                // Set textures.
                {
                    const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

                    for (int8_t index = 0; index < static_cast<int8_t>(MaterialTextureIndex::Max); ++index)
                    {
                        const AssimpScene::Material::Texture& texture = aiMaterial.textures.at(index);
                        if (texture.filePath.empty())
                            continue;

                        FilePath textureFilePath = texture.filePath;

                        // Set error texture.
                    	if (!textureFilePath.IsValid())
                        {
                            // TODO Set error texture.

                            continue;
                        }

                        textureFilePath.ReplaceExtension(TEXTURE_FILE_EXTENSION);
                        const std::optional<Ref<Texture2D>> texture2d = textureLibrary->Find<Texture2D>(textureFilePath);
                        if (!texture2d.has_value())
                        {
	                        // TODO Set error texture.

                            continue;
                        }

                        material->SetTextureAt(texture2d.value(), static_cast<MaterialTextureIndex>(index));
                    }
                }

                // Update.
                material->Save(true);
            }
        }
    }

    void MeshImporter::CreateNodes()
    {
        for (const auto& aiNode : _scene.nodes)
        {
            auto transform = aiNode.transform;

            MeshResource::NodeResource nodeResource = { };
            nodeResource.name = aiNode.name;
            nodeResource.nodeIndex = aiNode.nodeIndex;
            nodeResource.transform = Matrix::ToMatrix(transform.Transpose());
            nodeResource.root = aiNode.rootNode;
            nodeResource.hasBone = aiNode.hasBone;
            nodeResource.hasSkeleton = aiNode.skeleton;

            // Get parent node id.
            {
                if (aiNode.parent)
                {
                    const auto& parentNode = _scene.FindNodeByName(aiNode.parent->mName.C_Str());
                    nodeResource.parentNodeIndex = parentNode->nodeIndex;
                }
                else
                {
                    nodeResource.parentNodeIndex = -1;
                }
            }

            for (int childIndex = 0; childIndex < static_cast<int>(aiNode.numChildren); ++childIndex)
            {
                const auto& aiNodeChild = aiNode.children[childIndex];

                const auto childNode = _scene.FindNodeByName(aiNodeChild->mName.C_Str());

                nodeResource.childNodeIndexes.emplace_back(childNode->nodeIndex);
            }

            // Create node.
            MeshResource::Node node(nodeResource);
            _nodes.emplace_back(node);
        }
    }

    void MeshImporter::CreateSceneTree()
    {
        using NodeType = MeshResource::NodeType;

        _sceneTree = std::make_shared<MeshResource::SceneTree>();

        const auto size = static_cast<uint32_t>(_nodes.size());
        _sceneTree->_resources.resize(size);

        auto&& resources = _sceneTree->_resources;

        // Copy resources from nodes.
        {
            for (uint32_t index = 0; index < size; ++index)
            {
                const auto& nodeResource = _nodes.at(index).GetResource();

                resources.at(index).name = nodeResource.name;
                resources.at(index).transform = nodeResource.transform;
                resources.at(index).parentIndex = nodeResource.parentNodeIndex;
                resources.at(index).nodeType = NodeType::Node;

                const auto& node = _scene.FindNodeByIndex(index);

                if (node->numMeshes > 0)
                {
                    resources.at(index).nodeType = NodeType::Mesh;
                }
                else if (node->hasBone)
                {
                    resources.at(index).nodeType = NodeType::Bone;
                }
                else if (node->skeleton)
                {
                    resources.at(index).nodeType = NodeType::Skeleton;
                }

                resources.at(index).root = nodeResource.root;
            }
        }

        // Compute transform.
        {
            for (int32_t index = size - 1; index >= 0; --index)
            {
                auto&& resource = resources.at(index);
                if (resource.parentIndex == -1)
                {
                    resource.computedTransform = resource.transform;

                    continue;
                }
                auto&& parentResource = resources.at(resource.parentIndex);

                resource.computedTransform = _sceneTree->ComputeTransform(resource.transform, parentResource);
            }
        }

        _sceneTree->_name = _scene.filePath.Filename().Stem().GetAsString();

        // Make file path.
        {
            FilePath filePath = _scene.filePath;
            filePath.ReplaceExtension(SCENE_TREE_FILE_EXTENSION);
            filePath.MakePreferred();

            _sceneTree->_filePath = filePath;
            _sceneTree->_filePathToSerialized = filePath;
        }

        // Save
        _sceneTree->Save(false);

        // Add to library.
        {
            SceneTreeLibrary* sceneTreeLibrary = LibraryManager::GetInstance().GetSceneTreeLibrary();

            sceneTreeLibrary->Add(_sceneTree);
        }
    }

    void MeshImporter::CreateMeshes(bool hasSkeleton, bool fixName)
    {
        if (fixName)
        {
            for (auto&& mesh : _scene.meshes)
            {
                const auto& node = _scene.nodes.at(mesh.nodeIndex);

                mesh.name = node.name;
            }
        }

        // Recheck scale unit using vertex position.
        if (_scaleFactor == 0)
        {
            float value = _scene.meshes.at(0).vertices.at(0).position.x / 10.0f;
            if (value > 1.0f)
            {
                _scaleFactor = 1;
            }
        }

        if (hasSkeleton)
        {
            CreateSkinnedMesh();

            _isSkinned = true;
        }
        else
        {
            CreateStaticMesh();
        }

        // Create bounding boxes.
        {
            const auto& scaling = _mesh->GetSystemUnit().GetResource().scaling;

            _mesh->_allVerticesBoundingBox.Generate(_mesh, scaling, true);

            for (const auto& subMesh : _mesh->GetSubMeshes())
            {
                CreateBoundingBox(subMesh, scaling);
            }
        }
    }

    void MeshImporter::CreateCollisionMeshes(bool fixName)
    {
        if (fixName)
        {
            for (auto&& mesh : _scene.meshes)
            {
                const auto& node = _scene.nodes.at(mesh.nodeIndex);

                mesh.name = node.name;
            }
        }

        // Recheck scale unit using vertex position.
        if (_scaleFactor == 0)
        {
            float value = _scene.meshes.at(0).vertices.at(0).position.x / 10.0f;
            if (value > 1.0f)
            {
                _scaleFactor = 1;
            }
        }

        {
            // Parent of meshes and bones. 
            const MeshResource::Node& parentNode = _nodes.at(0);
            const MeshResource::NodeResource& nodeResource = parentNode.GetResource();

            _mesh = std::make_shared<CollisionMesh>(nodeResource.name);
            _mesh->_nodeName = nodeResource.name;
            _mesh->_parentNode = true;

            SetSystemUnit(std::static_pointer_cast<CollisionMesh>(_mesh), true);

            for (int meshIndex = 0; meshIndex < _scene.meshes.size(); ++meshIndex)
            {
                auto& mesh = _scene.meshes.at(meshIndex);

                // Convert.
            	std::vector<AssimpScene::Vertex> vertices = {};
                for (uint32_t indicesIndex = 0u; indicesIndex < mesh.indices.size(); ++indicesIndex)
                {
                    vertices.emplace_back(mesh.vertices.at(mesh.indices.at(indicesIndex)));
                }
                mesh.vertices = vertices;

            	const auto subMesh = std::make_shared<CollisionMesh>(mesh.name);
                subMesh->_nodeName = mesh.name;

                const auto& unitResource = _mesh->GetSystemUnit().GetResource();

                CreateCollisionMesh(subMesh, unitResource.scaling, mesh);

                _mesh->AddSubMesh(subMesh);
            }
        }
        
    }

    void MeshImporter::CreateStaticMesh()
    {
        // Parent of meshes and bones. 
        const MeshResource::Node& parentNode = _nodes.at(0);
        const MeshResource::NodeResource& nodeResource = parentNode.GetResource();

        _mesh = std::make_shared<StaticMesh>(nodeResource.name);
        _mesh->_nodeName = nodeResource.name;
        _mesh->_parentNode = true;

        SetSystemUnit(std::static_pointer_cast<StaticMesh>(_mesh), true);

        {
            for (int meshIndex = 0; meshIndex < _scene.batchedMeshes.size(); ++meshIndex)
            {
                const auto mesh = _scene.batchedMeshes.at(meshIndex);
                if (mesh.materialName.empty())
                    continue;

                const auto subMesh = std::make_shared<StaticMesh>(mesh.materialName);
                subMesh->_nodeName = mesh.materialName;

                CreateStaticMesh(subMesh, _mesh->GetSystemUnit().GetResource().scaling, mesh);

                _mesh->AddSubMesh(subMesh);
            }
        }
    }

    void MeshImporter::CreateSkinnedMesh()
    {
        // Parent of meshes and bones. 
        const MeshResource::Node& parentNode = _nodes.at(1);
        const MeshResource::NodeResource& nodeResource = parentNode.GetResource();

        _mesh = std::make_shared<SkinnedMesh>(nodeResource.name);
        _mesh->_nodeName = nodeResource.name;
        _mesh->_parentNode = true;

        auto&& skinnedMesh = std::static_pointer_cast<SkinnedMesh>(_mesh);
        skinnedMesh->_avatarFilePath = _skeleton->GetFilePathToSerialized();
        skinnedMesh->_avatar = _skeleton;

        SetSystemUnit(skinnedMesh, true);

        {
            for (int meshIndex = 0; meshIndex < _scene.meshes.size(); ++meshIndex)
            {
                const auto mesh = _scene.meshes.at(meshIndex);

                const auto subMesh = std::make_shared<SkinnedMesh>(mesh.name);
                subMesh->_nodeName = mesh.name;
                subMesh->_avatarFilePath = skinnedMesh->_avatarFilePath;

                CreateSkinnedMesh(subMesh, mesh);

                _mesh->AddSubMesh(subMesh);
            }
        }
    }

    void MeshImporter::CreateStaticMesh(const Ref<StaticMesh>& staticMesh, const Matrix& scaling, const AssimpScene::BatchedMesh& mesh)
    {
        for (const auto& vertex : mesh.vertices)
        {
            auto&& staticVertex = staticMesh->_vertices.emplace_back();
            staticVertex.position = Vector4(scaling.TransformCoord(vertex.position.GetAsVector3()), 1.0f);
            staticVertex.normal = vertex.normal;
            staticVertex.binormal = vertex.binormal;
            staticVertex.tangent = vertex.tangent;
            staticVertex.texcoord = vertex.texcoord;
        }

        CreateVertexBuffer(staticMesh);
        CreateMaterial(staticMesh, mesh);

        SetSystemUnit(staticMesh, false);
    }

    void MeshImporter::CreateSkinnedMesh(const Ref<SkinnedMesh>& skinnedMesh, const AssimpScene::Mesh& mesh)
    {
        for (int vertexIndex = 0; vertexIndex < mesh.vertices.size(); ++vertexIndex)
        {
            const auto& vertex = mesh.vertices.at(vertexIndex);

            auto&& skinnedMeshVertex = skinnedMesh->_vertices.emplace_back();

            skinnedMeshVertex.position = Vector4(Vector3::ToVector3(vertex.position), 1.0f);
            skinnedMeshVertex.normal = Vector4(Vector3::ToVector3(vertex.normal), 0.0f);
            skinnedMeshVertex.tangent = Vector4(Vector3::ToVector3(vertex.tangent), 0.0f);
            skinnedMeshVertex.binormal = Vector4(Vector3::ToVector3(vertex.binormal), 0.0f);

            uint32_t boneCounter = 0;
            for (int boneIndex = 0; boneIndex < mesh.boneInfo.size(); ++boneIndex)
            {
                const auto& boneInfo = mesh.boneInfo.at(boneIndex);
                for (int weightIndex = 0; weightIndex < boneInfo.weights.size(); ++weightIndex)
                {
                    const auto& weight = boneInfo.weights.at(weightIndex);

                    if (weight.vertexID != static_cast<uint32_t>(vertexIndex))
                        continue;

                    if (boneCounter >= MAX_BONE_INFLUENCES)
                        break;

                    skinnedMeshVertex.boneIndices.at(boneCounter) = boneIndex;
                    skinnedMeshVertex.boneWeights.at(boneCounter) = weight.weight;

                    ++boneCounter;
                }

                if (boneCounter >= MAX_BONE_INFLUENCES)
                    break;
            }

            const Vector2 texcoord = Vector2(Vector3::ToVector3(vertex.texcoord).x, Vector3::ToVector3(vertex.texcoord).y);
            skinnedMeshVertex.texcoord = texcoord;
        }

        for (const auto& index : mesh.indices)
        {
            skinnedMesh->_indices.emplace_back(index);
        }

        CreateVertexAndIndexBuffer(skinnedMesh);
        CreateMaterial(skinnedMesh, mesh);

        // Set system unit.
        SetSystemUnit(skinnedMesh, false);
    }

    void MeshImporter::CreateCollisionMesh(const Ref<CollisionMesh>& collisionMesh, const Matrix& scaling, const AssimpScene::Mesh& mesh)
    {
        const auto sceneTreeResource = _sceneTree->Find(collisionMesh->GetNodeName().Get());

        for (const auto& vertex : mesh.vertices)
        {
            auto&& staticVertex = collisionMesh->_vertices.emplace_back();

            Vector3 position = Vector3::ToVector3(vertex.position);
            position = sceneTreeResource->computedTransform.TransformCoord(position);
            position = scaling.TransformCoord(position);

        	staticVertex.position = Vector4(position, 1.0f);
        }

        CreateVertexBuffer(collisionMesh);

        SetSystemUnit(collisionMesh, false);
    }

    void MeshImporter::CreateMaterial(const Ref<StaticMesh>& staticMesh, const AssimpScene::BatchedMesh& mesh)
    {
        const MaterialLibrary* materialLibrary = LibraryManager::GetInstance().GetMaterialLibrary();

        const auto& aiMaterial = _scene.materials.at(mesh.materialIndex);
        const auto& material = materialLibrary->Find(aiMaterial.convertedFilePath);

        if (!material.has_value())
        {
            DENTY_SET_ERR_COLOR
            DENTY_ERR_LOG << aiMaterial.name << "Material not found! (in MeshImporter::CreateMaterial)";

            DENTY_ASSERT(false, "");
        }

        staticMesh->AddMaterial(material.value());
    }

    void MeshImporter::CreateMaterial(const Ref<SkinnedMesh>& skinnedMesh, const AssimpScene::Mesh& mesh)
    {
        const MaterialLibrary* materialLibrary = LibraryManager::GetInstance().GetMaterialLibrary();

        const auto& aiMaterial = _scene.materials.at(mesh.materialIndex);
        const auto& material = materialLibrary->Find(aiMaterial.convertedFilePath);

        if (!material.has_value())
        {
            DENTY_SET_ERR_COLOR
            DENTY_ERR_LOG << aiMaterial.name << "Material not found! (in MeshImporter::CreateMaterial)";

            DENTY_ASSERT(false, "");
        }

        skinnedMesh->AddMaterial(material.value());
    }

    void MeshImporter::CreateAnimation(const AssimpScene::Animation& animation)
    {
        const auto animationClip = std::make_shared<MeshResource::AnimationClip>();
        animationClip->SetName(animation.name);

        // Make file path.
        {
            FilePath filePath = _scene.filePath;
            filePath.RemoveExtension();
            filePath.RemoveFileName();
            filePath += animationClip->GetName() + ANIMATION_FILE_EXTENSION;

            animationClip->SetFilePath(filePath.GetAsString());
            animationClip->SetFilePathToSerialized(filePath.GetAsString());
        }

        animationClip->SetDuration(static_cast<float>(animation.duration));
        animationClip->SetTicksPerSecond(static_cast<float>(animation.ticksPerSecond));
        animationClip->SetTicksPerDuration(static_cast<float>(animation.duration) / static_cast<float>(animation.ticksPerSecond));
        animationClip->SetTicksPerFrame(1.0f / static_cast<float>(animation.ticksPerSecond));

        for (float time = 0.0f; time <= animationClip->GetTicksPerDuration(); time += animationClip->GetTicksPerFrame())
        {
            MeshResource::Keyframe keyframe = { };

            // positionKeys == rotationKeys == scalingKeys
            for (int nodeIndex = 0; nodeIndex < animation.nodes.size(); ++nodeIndex)
            {
                const auto& aiNode = animation.nodes.at(nodeIndex);

                auto&& node = keyframe.nodes.emplace_back();
                node.boneName = aiNode.name;

            	// Position key.
                {
                    const auto aiPositionKey = aiNode.FindPositionKeyByTime(time);
                    if (aiPositionKey.has_value())
                    {
                        node.position = Vector3::ToVector3(aiPositionKey.value().value);
                        node.time = static_cast<float>(aiPositionKey.value().time);
                        node.hasPositionValue = true;
                    }
                }

                // Rotation key.
                {
                    const auto aiRotationKey = aiNode.FindRotationKeyByTime(time);
                    if (aiRotationKey.has_value())
                    {
                        node.rotation = Quaternion::ToQuaternion(aiRotationKey.value().value);
                        node.hasRotationValue = true;
                    }
                }

                // Scaling key.
                {
                    const auto aiScalingKey = aiNode.FindScalingKeyByTime(time);
                    if (aiScalingKey.has_value())
                    {
                        node.scaling = Vector3::ToVector3(aiScalingKey.value().value);
                        node.hasScalingValue = true;
                    }
                }

                // Compute local orientation.
                {
                    const Matrix S = DirectX::XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z);
                    const Matrix R = DirectX::XMMatrixRotationQuaternion(node.rotation.GetXM());
                    const Matrix T = DirectX::XMMatrixTranslation(node.position.x, node.position.y, node.position.z);

                    node.orientation = S * R * T;
                }

            }

            // Compute global transforms.
            {
                for (int32_t nodeIndex = 0; nodeIndex < keyframe.nodes.size(); ++nodeIndex)
                {
                    auto&& node = keyframe.nodes.at(nodeIndex);
                    node.globalTransform = node.orientation;

                    const MeshResource::Bone* bone = _skeleton->FindBoneByName(node.boneName.Get());

                    if (bone)
                    {
                        node.globalTransform = ComputeBoneGlobalTransformAtKeyframe(keyframe, node.globalTransform, bone->GetResource().parentBoneName);
                    }
                    else
                    {
                        if (_skeleton->GetName() == node.boneName.Get())
                        {
                            node.globalTransform = ComputeBoneGlobalTransformAtKeyframe(keyframe, node.globalTransform, "NoParentBone");
                        }
                    }
                }
            }

            animationClip->AddKeyframe(keyframe);
        }

        // Save
        {
            animationClip->Save();
        }

        // Add to library.
        {
            AnimationClipLibrary* animationClipLibrary = LibraryManager::GetInstance().GetAnimationClipLibrary();
            animationClipLibrary->Add(animationClip);
        }
    }

	void MeshImporter::CreateBoundingBox(const Ref<Mesh>& mesh, const Matrix& scaling)
    {
        mesh->_boundingBox.Generate(mesh, scaling, false);
    }

    void MeshImporter::CreateVertexBuffer(const Ref<StaticMesh>& staticMesh)
    {
        {
            DXVertexBuffer::VertexBufferInfo vertexBufferInfo = { };
            vertexBufferInfo.stride = static_cast<uint32_t>(sizeof(MeshResource::StaticMeshVertex));
            vertexBufferInfo.offset = 0u;
            vertexBufferInfo.size = static_cast<uint32_t>(sizeof(MeshResource::StaticMeshVertex) * staticMesh->_vertices.size());
            vertexBufferInfo.count = static_cast<uint32_t>(staticMesh->_vertices.size());
            vertexBufferInfo.accessFlag = IDirectXBuffer::CpuAccessFlag::NoAccessWriteAndRead;
            vertexBufferInfo.mapType = IDirectXBuffer::MapType::None;
            vertexBufferInfo.usage = IDirectXBuffer::Usage::Immutable;
            vertexBufferInfo.vertexType = DXVertexBuffer::VertexBufferInfo::VertexType::StaticMeshVertex;
            vertexBufferInfo.data = std::make_shared<MeshResource::StaticMeshVertex[]>(vertexBufferInfo.count);
            vertexBufferInfo.AssignData(staticMesh->_vertices.data());

            staticMesh->_vertexBuffer = std::make_shared<DXVertexBuffer>();
            staticMesh->_vertexBuffer->Create(vertexBufferInfo);
        }
    }

    void MeshImporter::CreateVertexBuffer(const Ref<CollisionMesh>& collisionMesh)
    {
        {
            DXVertexBuffer::VertexBufferInfo vertexBufferInfo = { };
            vertexBufferInfo.stride = static_cast<uint32_t>(sizeof(MeshResource::CollisionMeshVertex));
            vertexBufferInfo.offset = 0u;
            vertexBufferInfo.size = static_cast<uint32_t>(sizeof(MeshResource::CollisionMeshVertex) * collisionMesh->_vertices.size());
            vertexBufferInfo.count = static_cast<uint32_t>(collisionMesh->_vertices.size());
            vertexBufferInfo.accessFlag = IDirectXBuffer::CpuAccessFlag::NoAccessWriteAndRead;
            vertexBufferInfo.mapType = IDirectXBuffer::MapType::None;
            vertexBufferInfo.usage = IDirectXBuffer::Usage::Immutable;
            vertexBufferInfo.vertexType = DXVertexBuffer::VertexBufferInfo::VertexType::CollisionMeshVertex;
            vertexBufferInfo.data = std::make_shared<MeshResource::CollisionMeshVertex[]>(vertexBufferInfo.count);
            vertexBufferInfo.AssignData(collisionMesh->_vertices.data());

            collisionMesh->_vertexBuffer = std::make_shared<DXVertexBuffer>();
            collisionMesh->_vertexBuffer->Create(vertexBufferInfo);
        }
    }

    void MeshImporter::CreateVertexAndIndexBuffer(const Ref<SkinnedMesh>& skinnedMesh)
    {
        {
            DXVertexBuffer::VertexBufferInfo vertexBufferInfo = { };
            vertexBufferInfo.stride = static_cast<uint32_t>(sizeof(MeshResource::SkinnedMeshVertex));
            vertexBufferInfo.offset = 0u;
            vertexBufferInfo.size = static_cast<uint32_t>(sizeof(MeshResource::SkinnedMeshVertex) * skinnedMesh->_vertices.size());
            vertexBufferInfo.count = static_cast<uint32_t>(skinnedMesh->_vertices.size());
            vertexBufferInfo.accessFlag = IDirectXBuffer::CpuAccessFlag::NoAccessWriteAndRead;
            vertexBufferInfo.mapType = IDirectXBuffer::MapType::None;
            vertexBufferInfo.usage = IDirectXBuffer::Usage::Immutable;
            vertexBufferInfo.vertexType = DXVertexBuffer::VertexBufferInfo::VertexType::SkinnedMeshVertex;
            vertexBufferInfo.data = std::make_shared<MeshResource::SkinnedMeshVertex[]>(vertexBufferInfo.count);
            vertexBufferInfo.AssignData(skinnedMesh->_vertices.data());

            skinnedMesh->_vertexBuffer = std::make_shared<DXVertexBuffer>();
            skinnedMesh->_vertexBuffer->Create(vertexBufferInfo);
        }

        {
            DXIndexBuffer::IndexBufferInfo indexBufferInfo = {};
            indexBufferInfo.offset = 0u;
            indexBufferInfo.size = static_cast<uint32_t>(sizeof(uint32_t) * skinnedMesh->_indices.size());
            indexBufferInfo.count = static_cast<uint32_t>(skinnedMesh->_indices.size());
            indexBufferInfo.accessFlag = IDirectXBuffer::CpuAccessFlag::NoAccessWriteAndRead;
            indexBufferInfo.mapType = IDirectXBuffer::MapType::None;
            indexBufferInfo.usage = IDirectXBuffer::Usage::Immutable;
            indexBufferInfo.data = std::make_shared<uint32_t[]>(indexBufferInfo.count);
            indexBufferInfo.AssignData(skinnedMesh->_indices.data());

            skinnedMesh->_indexBuffer = std::make_shared<DXIndexBuffer>();
            skinnedMesh->_indexBuffer->Create(indexBufferInfo);
        }
    }

    void MeshImporter::BindSceneTreeMeshes()
    {
        _sceneTree->BindMeshes(_mesh);
    }

    std::optional<MeshResource::Node> MeshImporter::FindNodeByName(std::string_view name) const
    {
        for (const auto& node : _nodes)
        {
            if (node.GetResource().name != name)
                continue;

            return node;
        }

        return std::nullopt;
    }

    Ref<Mesh> MeshImporter::ReImportAsStatic(const char* filePath, [[maybe_unused]] ImportType importType, bool fixName)
    {
        Assimp::Importer importer = {};
        const aiScene* aiScene = nullptr;

        // Import from filepath.
        {
            aiScene = importer.ReadFile(filePath,
                AI_POST_PROCESS_STEPS_STATIC_MESH
            );

            if (aiScene == nullptr)
            {
                DENTY_SET_ERR_COLOR
            	DENTY_ERR_LOG << "Failed to load " << filePath << "Mesh!";

                return nullptr;
            }

            _scene.filePath = std::filesystem::path(filePath);
        }

        // Get scale factor.
        {
            float scaleFactor = 0.0f;
            aiScene->mMetaData->Get("UnitScaleFactor", scaleFactor);

            _scaleFactor = static_cast<uint32_t>(scaleFactor);
        }

        // Fetch
        {
            TraverseNode(aiScene, aiScene->mRootNode);
            FetchMeshesAsStatic(aiScene);
            FetchMaterials(aiScene);
        }

        // Create or generate
        {
            CreateNodes();
            CreateSceneTree();

            GenerateBatchedMeshes(aiScene);

            CreateMaterials();
            CreateMeshes(false, fixName);
        }

        // Set mesh info.
        {
            SetMeshInfos();
        }

        return _mesh;
    }

    Ref<Mesh> MeshImporter::ImportAsCollision(const char* filePath, [[maybe_unused]] ImportType importType, [[maybe_unused]] bool fixName)
    {
        Assimp::Importer importer = {};
        const aiScene* aiScene = nullptr;

        // Import from filepath.
        {
            aiScene = importer.ReadFile(filePath,
                AI_POST_PROCESS_STEPS_COLLISION_MESH
            );

            if (aiScene == nullptr)
            {
                DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Failed to load " << filePath << "Mesh!";

                return nullptr;
            }

            _scene.filePath = std::filesystem::path(filePath);
        }

        // Get scale factor.
        {
            float scaleFactor = 0.0f;
            aiScene->mMetaData->Get("UnitScaleFactor", scaleFactor);

            _scaleFactor = static_cast<uint32_t>(scaleFactor);
        }

        // Fetch
        {
            TraverseNode(aiScene, aiScene->mRootNode);
            FetchMeshesAsStatic(aiScene);
            FetchMaterials(aiScene);
        }

        // Create or generate
        {
            CreateNodes();
            CreateSceneTree();

            CreateCollisionMeshes();
        }

        // Set mesh info.
        {
            SetMeshInfos(true);
        }

        // Calculate centroids.
        {
	        for (auto& collisionMesh : _mesh->GetSubMeshes())
	        {
		        const auto mesh = std::static_pointer_cast<CollisionMesh>(collisionMesh);
                mesh->CalculateCentroid();
	        }
        }

        // Create bounding boxes.
        {
            for (auto&& collisionMesh : _mesh->GetSubMeshes())
            {
                std::static_pointer_cast<CollisionMesh>(collisionMesh)->GenerateBoundingBox();
            }
        }

        _mesh->Save(true);

        return _mesh;
    }

    void MeshImporter::Reset()
    {
        _scene.Reset();
        _mesh = nullptr;
        _nodeIndexCounter = 0u;
        _nodes.clear();
        _scaleFactor = 0u;
        _sceneTree = nullptr;

        _isSkinned = false;
    }

    void MeshImporter::SetSystemUnit(const Ref<SkinnedMesh>& skinnedMesh, bool mainNode)
    {
        String extension = _scene.filePath.Extension().GetAsString();
        extension.RemoveUntil(extension.FindFirstOf(".") + 1u);

        if (mainNode)
        {
            skinnedMesh->_systemUnit.SetScaleFactor(_scaleFactor);
            skinnedMesh->_systemUnit.CalculateScalingMatrix();
        }

        skinnedMesh->_systemUnit.SetFileType(extension);
    }

    void MeshImporter::SetSystemUnit(const Ref<StaticMesh>& staticMesh, bool mainNode)
    {
        String extension = _scene.filePath.Extension().GetAsString();
        extension.RemoveUntil(extension.FindFirstOf(".") + 1u);

        if (mainNode)
        {
            staticMesh->_systemUnit.SetScaleFactor(_scaleFactor);
            staticMesh->_systemUnit.CalculateScalingMatrix();
        }

        staticMesh->_systemUnit.SetFileType(extension);
    }

    void MeshImporter::SetSystemUnit(const Ref<CollisionMesh>& collisionMesh, bool mainNode)
    {
        String extension = _scene.filePath.Extension().GetAsString();
        extension.RemoveUntil(extension.FindFirstOf(".") + 1u);

        if (mainNode)
        {
            collisionMesh->_systemUnit.SetScaleFactor(_scaleFactor);
            collisionMesh->_systemUnit.CalculateScalingMatrix();
        }

        collisionMesh->_systemUnit.SetFileType(extension);
    }

    void MeshImporter::SetNodeBoneInfos(const aiScene* aiScene)
    {
        const auto& aiMesh = aiScene->mMeshes[0];
        const uint32_t numBones = aiMesh->mNumBones;

        _scene.skeleton.bones.resize(numBones);

        // Fetch bone data.
        for (uint32_t boneIndex = 0; boneIndex < numBones; ++boneIndex)
        {
            const auto& aiBone = aiMesh->mBones[boneIndex];
            auto&& node = _scene.FindNodeByNameRef(aiBone->mName.C_Str());
            node->hasBone = true;

            auto&& bone = _scene.skeleton.bones.at(boneIndex);
            bone.name = aiBone->mName.C_Str();
            bone.nodeIndex = node->nodeIndex;
        }
    }

    void MeshImporter::SetBoneParentNames()
    {
        // Set bone parent name.
        for (int32_t boneIndex = static_cast<int32_t>(_skeleton->GetResource().bones.size()) - 1; boneIndex >= 0; --boneIndex)
        {
            auto& bone = _skeleton->_resource.bones.at(boneIndex);
            const auto& boneNode = _sceneTree->Find(bone.GetResource().name);

            const auto parentBoneName = ObtainBoneParentName(boneNode->parentIndex);
            bone._resource.parentBoneName = parentBoneName;
        }
    }

    std::string MeshImporter::ObtainBoneParentName(int32_t parentIndex) const
    {
        if (parentIndex != -1)
        {
            const auto& parentBoneNode = _sceneTree->Find(parentIndex);
            const auto& parentBone = _skeleton->FindBoneByName(parentBoneNode->name);

            if (parentBone)
            {
                return parentBone->GetResource().name;
            }
            else
            {
                return ObtainBoneParentName(parentBoneNode->parentIndex);
            }
        }

        return "NoParentBone";
    }

    void MeshImporter::SetSkeletonInfo()
    {
        // Set skeleton.
        for (const auto& bone : _scene.skeleton.bones)
        {
            const auto& node = _scene.FindNodeByName(bone.name);

            if (node->parent)
            {
                auto&& parentNode = _scene.FindNodeByNameRef(node->parent->mName.C_Str());

                // Should be scene only has one skeleton node.
                parentNode->skeleton = true;
                break;
            }
        }
    }

    void MeshImporter::SetMeshInfos(bool isCollisionMesh)
    {
        // Set name
        {
            String meshName = _mesh->_name;

            meshName.RemoveExtension();

            if (!isCollisionMesh)
            {
                // If skinned add _skinned name at end.
                if (_isSkinned)
                {
                    meshName.Append("_Skinned");
                }
                else
                {
                    meshName.Append("_Static");
                }
            }

            _mesh->_name = meshName;
        }

        // Set file path.
        {
            String filePath = _scene.filePath.GetAsString();

            _mesh->_filePath = String::ToPath(filePath);

            filePath.RemoveExtension();

            if (isCollisionMesh)
            {
                filePath.Append(COLLISION_MESH_FILE_EXTENSION);
            }
            else
            {
                if (_isSkinned)
                {
                    filePath.Append("_Skinned");
                }
                else
                {
                    filePath.Append("_Static");
                }

                filePath.Append(MESH_FILE_EXTENSION);
            }

            // Set mesh file path to serialized.
            _mesh->_filePathToSerialized = String::ToPath(filePath);

            // Also sub meshes.
            for (auto&& subMesh : _mesh->GetSubMeshes())
            {
                subMesh->_filePath = _mesh->_filePath;
                subMesh->_filePathToSerialized = _mesh->_filePathToSerialized;
            }
        }

        // Set scene tree file path.
        {
            const FilePath& filePath = _sceneTree->GetFilePathToSerialized();
            if (filePath.IsEmpty())
            {
                DENTY_ASSERT(false, "File path is empty! (in MeshImporter::SetMeshInfos)");
            }

            _mesh->_sceneTreeFilePath = _sceneTree->GetFilePathToSerialized();
            for (auto&& mesh : _mesh->GetSubMeshes())
            {
                mesh->_sceneTreeFilePath = _sceneTree->GetFilePathToSerialized();
            }

            _mesh->_sceneTree = _sceneTree;
        }

        // Set root mesh.
        {
            for (auto&& mesh : _mesh->GetSubMeshes())
            {
                mesh->_root = _mesh;
            }
        }
    }

    void MeshImporter::ComputeBoneGlobalTransforms()
    {
        auto&& bones = _skeleton->_resource.bones;
        for (int32_t boneIndex = static_cast<int32_t>(bones.size()) - 1; boneIndex >= 0; --boneIndex)
        {
            const auto& aiBone = _scene.skeleton.bones.at(boneIndex);
            auto&& bone = bones.at(boneIndex);

            bone._resource.localToGlobal = Matrix::Identity();
            for (int32_t necessityNodeIndex = static_cast<int32_t>(aiBone.necessityNode.map.size()) - 1; necessityNodeIndex >= 0; --necessityNodeIndex)
            {
                const auto necessityNode = aiBone.necessityNode.map.at(necessityNodeIndex);
                bone._resource.localToGlobal = bone._resource.localToGlobal * Matrix::ToMatrix(necessityNode.second.transform).Transpose();
            }
        }

        _skeleton->_resource.initialBones = bones;
        _skeleton->Save(true);
    }

    Matrix MeshImporter::ComputeBoneGlobalTransformAtKeyframe(const MeshResource::Keyframe& keyframe, Matrix& orientation, std::string_view parentBoneName)
    {
        if (parentBoneName != "NoParentBone")
        {
            const MeshResource::Bone* parentBone = _skeleton->FindBoneByName(parentBoneName);
            const auto node = keyframe.FindNodeByBoneName(parentBone->GetResource().name);

            if (node.has_value())
            {
                orientation = orientation * node.value().orientation;
                orientation = ComputeBoneGlobalTransformAtKeyframe(keyframe, orientation, parentBone->GetResource().parentBoneName);
            }
        }

        return orientation;
    }

    FilePath MeshImporter::ConvertMaterialPath(const String& materialPath) const
    {
        String path = materialPath;
        path.RemoveExtension();
        path.Append(MATERIAL_FILE_EXTENSION);

        return String::ToPath(path);
    }

    FilePath MeshImporter::ConvertTexturePath(const String& texturePath) const
    {
        String path = texturePath;
        path.RemoveExtension();
        path.Append(TEXTURE_FILE_EXTENSION);

        return String::ToPath(path);
    }

    std::vector<MeshResource::SceneTreeResource> MeshImporter::FindNodeByNodeType(MeshResource::NodeType nodeType) const
    {
        std::vector<MeshResource::SceneTreeResource> nodes = {};
        for (const auto& node : _sceneTree->GetResources())
        {
            if (node.nodeType != nodeType)
                continue;

            nodes.emplace_back(node);
        }

        return nodes;
    }
}
