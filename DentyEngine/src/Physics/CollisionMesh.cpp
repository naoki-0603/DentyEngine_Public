#include "stdafx.h"

#include "CollisionMesh.h"

#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"
#include "Graphics/Buffer/DirectX/IDirectXBuffer.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "System/Serializer/CollisionMeshSerializer.h"

namespace DentyEngine
{
	Ref<Texture2D> CollisionMesh::_collisionMeshAssetTexture = nullptr;

	CollisionMesh::CollisionMesh() :
		Mesh()
	{
	}

	CollisionMesh::CollisionMesh(std::string_view name) :
		Mesh(name), _vertices(), _centroid()
	{
	}

	CollisionMesh::CollisionMesh(const CollisionMesh& source)
	{
		this->CopyOf(&source);
	}

	void CollisionMesh::OnUpdate([[maybe_unused]] const Matrix& world, [[maybe_unused]] const EntityID entityID, [[maybe_unused]] const Ref<Material>& rendererMaterial, [[maybe_unused]] const float deltaTime)
	{
		// Nothing to do...
	}

	void CollisionMesh::OnGui()
	{
		Mesh::OnGui();
	}

	void CollisionMesh::OnRender([[maybe_unused]] ID3D11DeviceContext* context, [[maybe_unused]] const Ref<Material>& rendererMaterial,
		[[maybe_unused]] const std::vector<Ref<Material>>& materials)
	{
		// Nothing to do...
	}

	void CollisionMesh::OnRenderShadow([[maybe_unused]] ID3D11DeviceContext* context, [[maybe_unused]] uint32_t cascadeShadowAreaNo)
	{
		// Nothing to do...
	}

	void CollisionMesh::OnEvent(Event* e)
	{
		Mesh::OnEvent(e);
	}

	void CollisionMesh::OnSerialized() const
	{
	}

	void CollisionMesh::OnDeserialized()
	{
		if (_parentNode)
			return;

		// Restore buffer.
		{
			using VertexType = DXVertexBuffer::VertexBufferInfo::VertexType;
			using AccessFlag = IDirectXBuffer::CpuAccessFlag;
			using Usage = IDirectXBuffer::Usage;
			using MapType = IDirectXBuffer::MapType;

			DXVertexBuffer::VertexBufferInfo vertexBufferInfo = {};
			vertexBufferInfo.usage = Usage::Default;
			vertexBufferInfo.accessFlag = AccessFlag::NoAccessWriteAndRead;
			vertexBufferInfo.mapType = MapType::None;
			vertexBufferInfo.vertexType = VertexType::CollisionMeshVertex;
			vertexBufferInfo.stride = sizeof(MeshResource::CollisionMeshVertex);
			vertexBufferInfo.size = sizeof(MeshResource::CollisionMeshVertex) * static_cast<uint32_t>(_vertices.size());
			vertexBufferInfo.count = static_cast<uint32_t>(_vertices.size());
			vertexBufferInfo.data = std::make_shared<MeshResource::CollisionMeshVertex[]>(_vertices.size());
			vertexBufferInfo.AssignData(_vertices.data());

			_vertexBuffer = std::make_shared<DXVertexBuffer>();
			_vertexBuffer->Create(vertexBufferInfo);
		}
	}

	void CollisionMesh::Save(bool isUpdate) const
	{
		const auto staticMesh = shared_from_this();

		// If failed
		if (!CollisionMeshSerializer::SerializeAsBinary(_filePathToSerialized.Get(), staticMesh))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " collision mesh!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " collision mesh!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " collision mesh!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " collision mesh!";
		}
	}

	void CollisionMesh::Load(const std::filesystem::path& filePath)
	{
		auto collisionMesh = std::make_shared<CollisionMesh>();

		// If failed
		if (!CollisionMeshSerializer::DeserializeFromBinary(filePath, collisionMesh))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " collision mesh!";

			return;
		}

		collisionMesh->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " collision mesh!";
	}

	void CollisionMesh::OnReload()
	{
		
	}

	void CollisionMesh::Clone(const Ref<Asset>& asset)
	{
		DENTY_CHECK_CONDITION(asset->GetAssetType() == AssetType::CollisionMesh);

		auto&& target = std::static_pointer_cast<CollisionMesh>(asset);
		target->CopyOf(this);
	}

	void CollisionMesh::GenerateBoundingBox()
	{
		_boundingBox.Generate(shared_from_this(), Matrix::Identity(), false);
	}

	void CollisionMesh::CalculateCentroid()
	{
		// Calculate centroid.
		{
			for (const auto& vertex : _vertices)
			{
				_centroid += vertex.position.GetAsVector3();
			}

			_centroid /= static_cast<float>(_vertices.size());
		}
	}

	void CollisionMesh::LoadAssetTexture()
	{
		DENTY_ASSERT(not _collisionMeshAssetTexture, "Collision mesh asset texture already loaded!");

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		_collisionMeshAssetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::COLLISION_MESH_THUMBNAIL_FILE_PATH).value();

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Collision mesh asset texture successfully loaded!";
	}

	void CollisionMesh::CopyOf(const Mesh* source)
	{
		Mesh::CopyOf(source);

		const auto sourceCollisionMesh = static_cast<const CollisionMesh*>(source);

		_centroid = sourceCollisionMesh->_centroid;

		// Vertex buffer.
		if (not sourceCollisionMesh->_vertices.empty())
		{
			_vertices = sourceCollisionMesh->_vertices;
			_vertexBuffer = std::make_shared<DXVertexBuffer>();
			sourceCollisionMesh->_vertexBuffer->Clone(_vertexBuffer);
		}

		// Sub meshes.
		{
			for (int subMeshIndex = 0; subMeshIndex < sourceCollisionMesh->_subMeshes.size(); ++subMeshIndex)
			{
				const auto subMesh = std::static_pointer_cast<CollisionMesh>(sourceCollisionMesh->_subMeshes.at(subMeshIndex));
				const auto collisionMesh = std::make_shared<CollisionMesh>(subMesh->GetName());

				subMesh->Clone(collisionMesh);

				_subMeshes.emplace_back(collisionMesh);
			}
		}
	}
}
