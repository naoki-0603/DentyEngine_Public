#include "stdafx.h"

#include "SkinnedMesh.h"

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Buffer/FrameBuffer.h"

#include "Graphics/Buffer/DirectX/DXIndexBuffer.h"
#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"

#include "System/Resource/Animations/AnimationClip.h"
#include "System/Resource/Library/AnimationClipLibrary.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/SkeletonLibrary.h"

#include "System/Serializer/MeshSerializer.h"

namespace DentyEngine
{
    const String SkinnedMesh::ADD_FILENAME_AT_END = "_Skinned";

    void DebugAnimation::OnGui()
    {
        ImGui::Text("ClipName: %s", clip->GetName().c_str());
        ImGui::Text("Duration: %f", duration);
        ImGui::Text("TicksPerSecond: %f", ticksPerSecond);
        ImGui::Text("TicksPerFrame: %f", ticksPerFrame);

        ImGui::Text("CurrentTimer: %f", timer);
        ImGui::Text("CurrentIndex: %d", index);
        ImGui::Separator();

        if (ImGui::Button("Play##DebugAnimation"))
        {
            play = true;
        }

        if (play)
        {
	        if (ImGui::Button("Stop##DebugAnimation"))
	        {
                play = false;

                timer = 0;
                index = 0;
	        }
        }

        if (play)
        {
            timer += deltaTime;

            if (timer >= ticksPerFrame * static_cast<float>(index))
            {
                if (index < clip->GetKeyframes().size() - 1)
                {
                    index++;
                }
            }

            if (timer >= duration)
            {
                index = 0;
                timer = 0.0f;
            }
        }

        if (ImGui::Button("MoveNextFrame##DebugAnimation"))
        {
            timer += ticksPerFrame;

            index++;

            if (timer >= duration)
            {
                index = 0;

                timer = 0.0f;
            }
        }

        if (ImGui::Button("MovePrevFrame##DebugAnimation"))
        {
            timer -= ticksPerFrame;

            index--;

            if (timer < 0.0f)
            {
                index = 0;

                timer = 0.0f;
            }
        }

        ImGui::Separator();
    }

    SkinnedMeshConstants::SkinnedMeshConstants() noexcept :
		world(), boneTransforms(), entityID(), padding(), materialConstants()
    {
    }

    SkinnedMesh::SkinnedMesh() : Mesh()
    {
    }

    SkinnedMesh::SkinnedMesh(std::string_view name) :
        Mesh(name), _vertices(), _avatarFilePath(), _indices(), _avatar(), _indexBuffer(), _constants(), _shadowConstants(), _debugAnimation()
    {
    }

    SkinnedMesh::SkinnedMesh(const SkinnedMesh& source)
    {
        this->CopyOf(&source);
    }

    void SkinnedMesh::OnUpdate(const Matrix& world, const EntityID entityID,
        const Ref<Material>& rendererMaterial, const float deltaTime)
    {
        if (_parentNode)
        {
             // Debug.
            if (!_debugAnimation.clip)
            {
                const AnimationClipLibrary* clipLibrary = LibraryManager::GetInstance().GetAnimationClipLibrary();
                const auto idle = clipLibrary->Find("Assets\\Meshes\\1_fbx\\jammo\\a_Running.Animation");

                _debugAnimation.clip = idle.value();
                _debugAnimation.duration = idle.value()->GetTicksPerDuration();
                _debugAnimation.ticksPerSecond = idle.value()->GetTicksPerSecond();
                _debugAnimation.ticksPerFrame = idle.value()->GetTicksPerFrame();
                _debugAnimation.timer = 0.0f;
            }

            _debugAnimation.deltaTime = deltaTime;

            const auto keyframe = _debugAnimation.clip->FindKeyframeAt(_debugAnimation.index);
            {
                for (auto&& bone : _avatar->GetResource().bones)
                {
                    const auto node = keyframe.FindNodeByBoneName(bone.GetResource().name);

                    if (node.has_value())
                    {
                        _avatar->UpdateBone(bone.GetResource().name, Matrix::Identity(), node.value().globalTransform);
                    }
                }
            }
        }
        else
        {
            [[maybe_unused]] const auto& unitResource = _root.lock()->GetSystemUnit().GetResource();

            // Default
            {
                // Vertex shaders.
                _constants.world = world;

                // Bone transforms.
                {
                    const auto& rootSkinnedMesh = std::static_pointer_cast<SkinnedMesh>(_root.lock());
                    for (int boneIndex = 0; boneIndex < rootSkinnedMesh->_avatar->GetResource().bones.size(); ++boneIndex)
                    {
                        const auto bone = rootSkinnedMesh->_avatar->FindBoneByIndex(boneIndex);
                        const auto meshIndex = rootSkinnedMesh->FindIndexByName(_nodeName.Get());

                        _constants.boneTransforms.at(boneIndex) = bone->GetResource().offsetTransforms.at(meshIndex.value()) * bone->GetResource().localTransform;
                    }
                }

                // Entity id (use for mouse pick)
                _constants.entityID = entityID;

                // Pixel shader.
                _constants.materialConstants.baseColor = rendererMaterial->parameters.baseColor;
                _constants.materialConstants.emissiveColor = rendererMaterial->parameters.emissiveColor;
                _constants.materialConstants.baseRate = rendererMaterial->parameters.baseRate;
                _constants.materialConstants.roughness = rendererMaterial->parameters.roughness;
                _constants.materialConstants.metallic = rendererMaterial->parameters.metallic;
                _constants.materialConstants.specular = rendererMaterial->parameters.specular;
                _constants.materialConstants.emissiveIntensity = rendererMaterial->parameters.emissiveIntensity;
                _constants.materialConstants.renderingMode = rendererMaterial->parameters.renderingMode;

                _constants.materialConstants.texturesUsed.albedo = (rendererMaterial->HasTextureAt(MaterialTextureIndex::Albedo) ? 1 : 0);
                _constants.materialConstants.texturesUsed.metalness = (rendererMaterial->HasTextureAt(MaterialTextureIndex::Metalness) ? 1 : 0);
                _constants.materialConstants.texturesUsed.normal = (rendererMaterial->HasTextureAt(MaterialTextureIndex::Normal) ? 1 : 0);
                _constants.materialConstants.texturesUsed.roughness = (rendererMaterial->HasTextureAt(MaterialTextureIndex::Roughness) ? 1 : 0);
                _constants.materialConstants.texturesUsed.emissive = (rendererMaterial->HasTextureAt(MaterialTextureIndex::Emissive) ? 1 : 0);
                _constants.materialConstants.texturesUsed.ambientOcclusion = (rendererMaterial->HasTextureAt(MaterialTextureIndex::AmbientOcclusion) ? 1 : 0);
                _constants.materialConstants.texturesUsed.optionalSlot1 = (rendererMaterial->HasTextureAt(MaterialTextureIndex::OptionalSlot1) ? 1 : 0);
                _constants.materialConstants.texturesUsed.optionalSlot2 = (rendererMaterial->HasTextureAt(MaterialTextureIndex::OptionalSlot2) ? 1 : 0);
            }

            // Shadow
            {
                // Vertex shaders.
                _shadowConstants.world = world;

                // Bone transforms.
                {
                    const auto& rootSkinnedMesh = std::static_pointer_cast<SkinnedMesh>(_root.lock());
                    for (int boneIndex = 0; boneIndex < rootSkinnedMesh->_avatar->GetResource().bones.size(); ++boneIndex)
                    {
                        const auto bone = rootSkinnedMesh->_avatar->FindBoneByIndex(boneIndex);
                        const auto meshIndex = rootSkinnedMesh->FindIndexByName(_nodeName.Get());

                        _shadowConstants.boneTransforms.at(boneIndex) = bone->GetResource().offsetTransforms.at(meshIndex.value()) * bone->GetResource().localTransform;
                    }
                }
            }
        }
    }

    void SkinnedMesh::OnGui()
    {
        Mesh::OnGui();
#if 1
        if (_parentNode && _debugAnimation.clip)
        {
            // Debug skeletal animation
            if (ImGui::CollapsingHeader("Skeletal Animation##SkinnedMesh"))
            {
                _debugAnimation.OnGui();
            }
        }
#endif
    }

    void SkinnedMesh::OnRender(ID3D11DeviceContext* context, const Ref<Material>& rendererMaterial, [[maybe_unused]] const std::vector<Ref<Material>>& materials)
    {
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Bind buffers.
        {
            _vertexBuffer->Bind();
            _indexBuffer->Bind();
        }

        // Bind shaders.
        {
            rendererMaterial->GetShader()->BindVertexShader(context);
            rendererMaterial->GetShader()->BindPixelShader(context);
        }

        // Update, bind constant buffer.
        {
            rendererMaterial->GetShader()->constantBuffer->UpdateSubresource(context, &_constants);
            rendererMaterial->GetShader()->constantBuffer->BindAll(context, 0u);
        }

        // Bind texture.
        {
            rendererMaterial->BindTextures();
        }

        // Render
        {
            const uint32_t indexCount = static_cast<uint32_t>(_indices.size());

            context->DrawIndexed(indexCount, 0u, 0);
        }
    }

    void SkinnedMesh::OnRenderShadow(ID3D11DeviceContext* context, [[maybe_unused]] uint32_t cascadeShadowAreaNo)
    {
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Update constants.
        {
            _shadowConstants.cascadeAreaNo = cascadeShadowAreaNo;
        }

        // Bind buffers.
        {
            _vertexBuffer->Bind();
            _indexBuffer->Bind();
        }

        // Bind shaders.
        const Ref<Material>& rendererMaterial = Material::GetSkinnedShadowDefaultMaterial();
        {
            rendererMaterial->GetShader()->BindVertexShader(context);
            rendererMaterial->GetShader()->BindPixelShader(context);
        }

        // Bind constant buffer.
        {
            rendererMaterial->GetShader()->constantBuffer->UpdateSubresource(context, &_shadowConstants);
            rendererMaterial->GetShader()->constantBuffer->BindAll(context, 0);
        }

        // Draw
        {
            const uint32_t indexCount = static_cast<uint32_t>(_indices.size());

        	context->DrawIndexed(indexCount, 0u, 0);
        }
    }

    void SkinnedMesh::OnEvent(Event* e)
    {
        Mesh::OnEvent(e);
    }

    void SkinnedMesh::OnSerialized() const
    {
        Mesh::OnSerialized();
    }

    void SkinnedMesh::OnDeserialized()
    {
        // If parent node doesn't deserialize buffers.
        if (_parentNode)
        {
            // Set avatar.
            const SkeletonLibrary* skeletonLibrary = LibraryManager::GetInstance().GetSkeletonLibrary();

            const auto& avatar = skeletonLibrary->Find(_avatarFilePath);
            if (!avatar.has_value())
            {
                DENTY_SET_ERR_COLOR
                    DENTY_ERR_LOG << "Avatar " << _avatarFilePath << " not found!";

                DENTY_ASSERT(false, "");
            }

            _avatar = avatar.value();
        }

        Mesh::OnDeserialized();

        if (_parentNode)
            return;

        // Restore vertex buffer.
        {
            using VertexType = DXVertexBuffer::VertexBufferInfo::VertexType;
            using AccessFlag = IDirectXBuffer::CpuAccessFlag;
            using Usage = IDirectXBuffer::Usage;
            using MapType = IDirectXBuffer::MapType;

            DXVertexBuffer::VertexBufferInfo vertexBufferInfo = {};
            vertexBufferInfo.usage = Usage::Default;
            vertexBufferInfo.accessFlag = AccessFlag::NoAccessWriteAndRead;
            vertexBufferInfo.mapType = MapType::None;
            vertexBufferInfo.vertexType = VertexType::SkinnedMeshVertex;
            vertexBufferInfo.stride = sizeof(MeshResource::SkinnedMeshVertex);
            vertexBufferInfo.size = sizeof(MeshResource::SkinnedMeshVertex) * static_cast<uint32_t>(_vertices.size());
            vertexBufferInfo.count = static_cast<uint32_t>(_vertices.size());
            vertexBufferInfo.data = std::make_shared<MeshResource::SkinnedMeshVertex[]>(_vertices.size());
            vertexBufferInfo.AssignData(_vertices.data());

            _vertexBuffer = std::make_shared<DXVertexBuffer>();
            _vertexBuffer->Create(vertexBufferInfo);
        }

        // Restore index buffer.
        {
            using AccessFlag = IDirectXBuffer::CpuAccessFlag;
            using Usage = IDirectXBuffer::Usage;
            using MapType = IDirectXBuffer::MapType;

            // Create index buffer.
            {
                DXIndexBuffer::IndexBufferInfo indexBufferInfo = { };
                indexBufferInfo.usage = Usage::Default;
                indexBufferInfo.accessFlag = AccessFlag::NoAccessWriteAndRead;
                indexBufferInfo.mapType = MapType::None;
                indexBufferInfo.offset = 0u;
                indexBufferInfo.count = static_cast<uint32_t>(_indices.size());
                indexBufferInfo.size = sizeof(uint32_t) * static_cast<uint32_t>(_indices.size());
                indexBufferInfo.data = std::make_shared<uint32_t[]>(_indices.size());
                indexBufferInfo.AssignData(_indices.data());

                _indexBuffer = std::make_shared<DXIndexBuffer>();
                _indexBuffer->Create(indexBufferInfo);
            }
        }
    }

    void SkinnedMesh::Save(bool isUpdate) const
    {
        const auto skinnedMesh = shared_from_this();

        // If failed
        if (!MeshSerializer::SerializeAsBinary(_filePathToSerialized.Get(), skinnedMesh))
        {
            DENTY_SET_ERR_COLOR

                if (isUpdate)
                {
                    DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " skinned mesh!";
                }
                else
                {
                    DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " skinned mesh!";
                }

            return;
        }

        DENTY_SET_INFO_COLOR

            if (isUpdate)
            {
                DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " skinned mesh!";
            }
            else
            {
                DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " skinned mesh!";
            }
    }

    void SkinnedMesh::Load(const std::filesystem::path& filePath)
    {
        auto skinnedMesh = std::make_shared<SkinnedMesh>();

        // If failed
        if (!MeshSerializer::DeserializeFromBinary(filePath, skinnedMesh))
        {
            DENTY_SET_ERR_COLOR

                DENTY_ERR_LOG << "Failed to load " << filePath << " skinned mesh!";

            return;
        }

        skinnedMesh->Clone(shared_from_this());

        DENTY_SET_INFO_COLOR
            DENTY_INFO_LOG << "Succeed to load " << filePath << " skinned mesh!";
    }

    void SkinnedMesh::Destroy()
    {
        Mesh::Destroy();
    }

    void SkinnedMesh::OnDestroy()
    {
        Mesh::OnDestroy();
    }

    void SkinnedMesh::Clone(const Ref<Asset>& asset)
    {
        auto&& target = std::static_pointer_cast<SkinnedMesh>(asset);

        DENTY_CHECK_CONDITION(target->GetMeshType() == std::string("SkinnedMesh"));

        target->CopyOf(this);
    }

    void SkinnedMesh::CopyOf(const Mesh* source)
    {
        Mesh::CopyOf(source);

        const auto sourceSkinnedMesh = static_cast<const SkinnedMesh*>(source);

        // Clone vertex buffer
        {
            if (sourceSkinnedMesh->_vertexBuffer)
            {
                _vertexBuffer = std::make_shared<DXVertexBuffer>();
                _vertices = sourceSkinnedMesh->_vertices;
                sourceSkinnedMesh->_vertexBuffer->Clone(_vertexBuffer);
            }
        }

        // Index buffer as well.
        {
            if (sourceSkinnedMesh->_indexBuffer)
            {
                _indexBuffer = std::make_shared<DXIndexBuffer>();
                _indices = sourceSkinnedMesh->_indices;

                sourceSkinnedMesh->_indexBuffer->Clone(_indexBuffer);
            }
        }

        for (int subMeshIndex = 0; subMeshIndex < sourceSkinnedMesh->_subMeshes.size(); ++subMeshIndex)
        {
            const Ref<SkinnedMesh> subMesh = std::static_pointer_cast<SkinnedMesh>(sourceSkinnedMesh->_subMeshes.at(subMeshIndex));
            const auto skinnedMesh = std::make_shared<SkinnedMesh>(subMesh->GetName());

            subMesh->Clone(skinnedMesh);

            _subMeshes.emplace_back(skinnedMesh);
        }

        _avatarFilePath = sourceSkinnedMesh->_avatarFilePath;
        _avatar = sourceSkinnedMesh->_avatar;
    }
}
