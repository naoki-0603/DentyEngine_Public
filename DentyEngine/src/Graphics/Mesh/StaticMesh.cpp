#include "stdafx.h"

#include "StaticMesh.h"

#include "SceneTree.h"

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Buffer/FrameBuffer.h"
#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"

#include "System/Serializer/MeshSerializer.h"

namespace DentyEngine
{
	const String StaticMesh::ADD_FILENAME_AT_END = "_Static";

	StaticMeshConstants::StaticMeshConstants() noexcept :
		world(), entityID(), padding(), materialConstants()
	{
	}

	StaticMesh::StaticMesh() :
		Mesh()
	{
	}

	StaticMesh::StaticMesh(std::string_view name) :
		Mesh(name), _vertices(), _constants(), _shadowConstants()
	{
	}

	StaticMesh::StaticMesh(const StaticMesh& source)
	{
		this->CopyOf(&source);
	}

	void StaticMesh::OnUpdate(const Matrix& world, const EntityID entityID,
		const Ref<Material>& rendererMaterial, [[maybe_unused]] const float deltaTime)
	{
		// Default
		{
			// Vertex shader.
			_constants.world = world;

			// Entity id (use for mouse pick)
			_constants.entityID = entityID;

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
			_shadowConstants.world = world;
		}
	}

	void StaticMesh::OnGui()
	{
		Mesh::OnGui();
	}

	void StaticMesh::OnRender(ID3D11DeviceContext* context, const Ref<Material>& rendererMaterial,
		[[maybe_unused]] const std::vector<Ref<Material>>& materials)
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind buffer.
		{
			_vertexBuffer->Bind();
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
		context->Draw(static_cast<UINT>(_vertices.size()), 0);
	}

	void StaticMesh::OnRenderShadow(ID3D11DeviceContext* context, [[maybe_unused]] uint32_t cascadeShadowAreaNo)
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Update shadow constants.
		{
			_shadowConstants.cascadeAreaNo = cascadeShadowAreaNo;
		}

		// Bind buffers.
	    {
		    _vertexBuffer->Bind();
		}

		// Bind shaders.
		const Ref<Material>& rendererMaterial = Material::GetStaticShadowDefaultMaterial();
		{
			rendererMaterial->GetShader()->BindVertexShader(context);
			rendererMaterial->GetShader()->BindPixelShader(context);
		}

		// Bind constant buffer.
		{
			rendererMaterial->GetShader()->constantBuffer->UpdateSubresource(context, &_shadowConstants);
			rendererMaterial->GetShader()->constantBuffer->BindAll(context, 0);
		}

		context->Draw(static_cast<UINT>(_vertices.size()), 0);
	}

	void StaticMesh::OnEvent(Event* e)
	{
		Mesh::OnEvent(e);
	}

	void StaticMesh::OnSerialized() const
	{
		Mesh::OnSerialized();
	}

	void StaticMesh::OnDeserialized()
	{
		Mesh::OnDeserialized();

		// If parent node doesn't deserialize buffers.
		if (_parentNode)
		{
			return;
		}

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
			vertexBufferInfo.vertexType = VertexType::StaticMeshVertex;
			vertexBufferInfo.stride = sizeof(MeshResource::StaticMeshVertex);
			vertexBufferInfo.size = sizeof(MeshResource::StaticMeshVertex) * static_cast<uint32_t>(_vertices.size());
			vertexBufferInfo.count = static_cast<uint32_t>(_vertices.size());
			vertexBufferInfo.data = std::make_shared<MeshResource::StaticMeshVertex[]>(_vertices.size());
			vertexBufferInfo.AssignData(_vertices.data());

			_vertexBuffer = std::make_shared<DXVertexBuffer>();
			_vertexBuffer->Create(vertexBufferInfo);
		}
	}

	void StaticMesh::Save(bool isUpdate) const
	{
		const auto staticMesh = shared_from_this();

		// If failed
		if (!MeshSerializer::SerializeAsBinary(_filePathToSerialized.Get(), staticMesh))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " static mesh!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " static mesh!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " static mesh!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " static mesh!";
		}
	}

	void StaticMesh::Load(const std::filesystem::path& filePath)
	{
		auto staticMesh = std::make_shared<StaticMesh>();

		// If failed
		if (!MeshSerializer::DeserializeFromBinary(filePath, staticMesh))
		{
			DENTY_SET_ERR_COLOR

			DENTY_ERR_LOG << "Failed to load " << filePath << " static mesh!";

			return;
		}

		staticMesh->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " static mesh!";
	}

	void StaticMesh::Destroy()
	{
		Mesh::Destroy();
	}

	void StaticMesh::OnDestroy()
	{
		Mesh::OnDestroy();
	}

	void StaticMesh::Clone(const Ref<Asset>& asset)
	{
		auto&& target = std::static_pointer_cast<StaticMesh>(asset);

		DENTY_CHECK_CONDITION(target->GetMeshType() == std::string("StaticMesh"), "Mesh type must be static!");

		target->CopyOf(this);
	}

	void StaticMesh::ReplaceMainMaterialShaders(Ref<Shader> shader)
	{
		for (uint32_t index = 0u; index < GetSubMeshCount(); ++index)
		{
			auto&& subMesh = GetSubMeshes().at(index);
			subMesh->GetMainMaterial()->ReplaceShaderSoft(shader);
		}
	}

	void StaticMesh::CopyOf(const Mesh* source)
	{
		Mesh::CopyOf(source);

		const auto sourceStaticMesh = static_cast<const StaticMesh*>(source);

		// Clone vertex buffer
		{
			if (sourceStaticMesh->_vertexBuffer)
			{
				_vertexBuffer = std::make_shared<DXVertexBuffer>();
				_vertices = sourceStaticMesh->_vertices;

				sourceStaticMesh->_vertexBuffer->Clone(_vertexBuffer);
			}
		}

		for (int subMeshIndex = 0; subMeshIndex < sourceStaticMesh->_subMeshes.size(); ++subMeshIndex)
		{
			const Ref<StaticMesh> subMesh = std::static_pointer_cast<StaticMesh>(sourceStaticMesh->_subMeshes.at(subMeshIndex));
			const auto staticMesh = std::make_shared<StaticMesh>(subMesh->GetName());

			subMesh->Clone(staticMesh);

			_subMeshes.emplace_back(staticMesh);
		}
	}
}
