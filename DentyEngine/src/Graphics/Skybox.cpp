#include "stdafx.h"

#include "Skybox.h"

#include "Renderer/CubeMap.h"
#include "Renderer/Material.h"
#include "Renderer/CubeMaterial.h"

#include "Buffer/ConstantBuffer.h"
#include "Buffer/DirectX/DXIndexBuffer.h"
#include "Buffer/DirectX/DXVertexBuffer.h"

#include "States/RasterizerState.h"
#include "States/SamplerState.h"

#include "System/Graphics/GraphicsManager.h"

#include "Utilities/MeshUtilities.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MaterialLibrary.h"
#include "System/Resource/Library/ShaderLibrary.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "System/Serializer/SerializeExtensions.h"
#include "System/Serializer/SkyboxSerializer.h"

namespace DentyEngine
{
	Ref<Texture2D> Skybox::_skyboxAssetTexture = nullptr;

	Skybox::Skybox() : Object(), Asset(),
		_vertices(), _indices(), _cubeMaterial(), _cubeMaterialFilePath(), _vertexBuffer(), _indexBuffer(), _constants()
	{
	}

	void Skybox::Create(const FilePath& cubeMapFilePath)
	{
		// Create box Mesh.
		{
			MeshUtils::CalculateBoxMesh(_vertices, _indices);
		}

		// Create d3d11 buffers.
		{
			CreateBuffers();
		}

		// Create material.
		{
			FilePath filePath = cubeMapFilePath;

			const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();
			const Ref<Shader> shader = shaderLibrary->Find(ShaderLibrary::SKYBOX_PATH).value();

			CubeMaterialInitData initData = { };
			initData.name = filePath.Filename().RemoveExtension().GetAsString();
			initData.textureFilePath = cubeMapFilePath;
			initData.filePath = filePath.RemoveExtension();
			initData.shader = shader;

			_cubeMaterial = std::make_shared<CubeMaterial>();
			_cubeMaterial->Create(initData);
		}

		// Set cube map.
		{
			const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
			const Ref<CubeMap> cubeMap = textureLibrary->Find<CubeMap>(cubeMapFilePath).value();

			_cubeMaterial->SetTexture(cubeMap);
			_cubeMaterial->Save(true);
			_cubeMaterialFilePath = _cubeMaterial->GetFilePathToSerialized();
		}

		// Calculate transform.
		{
			const Matrix S = DirectX::XMMatrixScaling(500.0f, 500.0f, 500.0f);
			const Matrix R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
			const Matrix T = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			_constants.world = S * R * T;
		}

		// Make path.
		{
			FilePath skyboxPath;
			skyboxPath = cubeMapFilePath;
			skyboxPath.ReplaceExtension(SKYBOX_FILE_EXTENSION);

			_filePath = skyboxPath;
			_filePathToSerialized = _filePath;
		}
	}

	void Skybox::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void Skybox::OnSerialized() const
	{
	}

	void Skybox::OnDeserialized()
	{
		// Restore buffer.
		{
			CreateBuffers();
		}

		// Find material.
		{
			const MaterialLibrary* materialLibrary = LibraryManager::GetInstance().GetMaterialLibrary();

			const std::optional<Ref<CubeMaterial>> material = materialLibrary->FindCube(_cubeMaterialFilePath);
			if (!material.has_value())
			{
				DENTY_ASSERT(false, "CubeMaterial file path is invalid! (Skybox::OnDeserialized)");
			}

			_cubeMaterial = material.value();
		}

		// Calculate transform.
		{
			const Matrix S = DirectX::XMMatrixScaling(500.0f, 500.0f, 500.0f);
			const Matrix R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
			const Matrix T = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			_constants.world = S * R * T;
		}
	}

	void Skybox::Render(ID3D11DeviceContext* context)
	{
		ID3D11SamplerState* samplers[] =
		{
			SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::PointWrap),
			SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::LinearWrap),
			SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::AnisotropicWrap)
		};

        // Get cached
        ComRef<ID3D11RasterizerState> cachedRasterizerState;
        context->RSGetState(cachedRasterizerState.ReleaseAndGetAddressOf());

		ComRef<ID3D11BlendState> cachedBlendState;
		FLOAT cachedBlendFactor[4] = { };
		UINT cachedSamplerMask = 0;

		context->OMGetBlendState(cachedBlendState.ReleaseAndGetAddressOf(), cachedBlendFactor, &cachedSamplerMask);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_vertexBuffer->Bind();
		_indexBuffer->Bind();

		_cubeMaterial->GetShader()->constantBuffer->BindAll(context, 0);
		_cubeMaterial->GetShader()->constantBuffer->UpdateSubresource(context, &_constants);

		_cubeMaterial->GetShader()->BindVertexShader(context);
		_cubeMaterial->GetShader()->BindPixelShader(context);

		const Ref<CubeMap>& cubeMap = _cubeMaterial->GetTexture();
		if (cubeMap)
		{
			context->PSSetShaderResources(0, 1, cubeMap->GetResource());
		}

        context->PSSetSamplers(0, _countof(samplers), samplers);

		context->RSSetState(
			RasterizerState::GetBuildInRasterizerStatesAt(RasterizerState::Type::FillNoCull)
		);

        context->DrawIndexed(36, 0, 0);

        context->RSSetState(cachedRasterizerState.Get());
		context->OMSetBlendState(cachedBlendState.Get(), cachedBlendFactor, cachedSamplerMask);
	}

	void Skybox::Save(bool isUpdate) const
	{
		// If failed.
		if (!SkyboxSerializer::SerializeAsBinary(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " skybox!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " skybox!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " skybox!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " skybox!";
		}
	}

	void Skybox::Load(const std::filesystem::path& filePath)
	{
		auto skybox = std::make_shared<Skybox>();

		// If failed
		if (!SkyboxSerializer::DeserializeFromBinary(filePath, skybox))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " skybox!";

			return;
		}

		skybox->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " skybox!";
	}

	void Skybox::Rename([[maybe_unused]] std::string_view newName, [[maybe_unused]] bool deleteOldFile)
	{
	}

	void Skybox::OnReload()
	{
	}

	void Skybox::Clone(const Ref<Asset>& asset)
	{
		DENTY_ASSERT(asset->GetAssetType() == AssetType::Skybox, "Asset type must be skybox! (in Skybox::Clone)");

		auto&& target = std::static_pointer_cast<Skybox>(asset);

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
			target->_referenceObjects = _referenceObjects;
		}

		// This class
		{
			target->_vertices = _vertices;
			target->_indices = _indices;

			target->CreateBuffers();

			target->_cubeMaterial = _cubeMaterial;
			target->_cubeMaterialFilePath = _cubeMaterialFilePath;

			target->_constants = _constants;
		}
	}

	void Skybox::BindCubeMap()
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();
		ID3D11ShaderResourceView* const* srv = _cubeMaterial->GetTexture()->GetResource();

		context->PSSetShaderResources(30u, 1u, srv);
	}

	void Skybox::LoadAssetTexture()
	{
		DENTY_ASSERT(not _skyboxAssetTexture, "Skybox asset texture already loaded!");

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		_skyboxAssetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::SKYBOX_DEFAULT_THUMBNAIL_FILE_PATH).value();

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Skybox asset texture successfully loaded!";
	}

	void Skybox::CreateBuffers()
	{
		// Vertex buffer
		{
			using VertexType = DXVertexBuffer::VertexBufferInfo::VertexType;
			using AccessFlag = IDirectXBuffer::CpuAccessFlag;
			using Usage = IDirectXBuffer::Usage;
			using MapType = IDirectXBuffer::MapType;

			DXVertexBuffer::VertexBufferInfo vertexBufferInfo = {};
			vertexBufferInfo.usage = Usage::Immutable;
			vertexBufferInfo.accessFlag = AccessFlag::NoAccessWriteAndRead;
			vertexBufferInfo.mapType = MapType::None;
			vertexBufferInfo.vertexType = VertexType::Vertex;
			vertexBufferInfo.stride = sizeof(Vertex);
			vertexBufferInfo.size = sizeof(Vertex) * static_cast<uint32_t>(_vertices.size());
			vertexBufferInfo.count = static_cast<uint32_t>(_vertices.size());
			vertexBufferInfo.data = std::make_shared<Vertex[]>(_vertices.size());
			vertexBufferInfo.AssignData(_vertices.data());

			_vertexBuffer = std::make_shared<DXVertexBuffer>();
			_vertexBuffer->Create(vertexBufferInfo);
		}

		// Index buffer.
		{
			using AccessFlag = IDirectXBuffer::CpuAccessFlag;
			using Usage = IDirectXBuffer::Usage;
			using MapType = IDirectXBuffer::MapType;

			DXIndexBuffer::IndexBufferInfo indexBufferInfo = { };
			indexBufferInfo.usage = Usage::Immutable;
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
