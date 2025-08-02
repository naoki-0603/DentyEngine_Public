#pragma once

#include "IDirectXBuffer.h"

#include "Graphics/Vertex.h"

namespace DentyEngine
{
	class DXVertexBuffer final : public IDirectXBuffer
	{
	public:
		struct VertexBufferInfo
		{
			enum class VertexType
			{
				Vertex,
				ParticleVertex,
				StaticMeshVertex,
				SkinnedMeshVertex,
				CollisionMeshVertex,
				MeshVertex,
				LineVertex,
				SpriteVertex,

				Max
			};
		public:
			VertexBufferInfo() : buffer(), stride(0u), offset(0u), size(0u), count(0u), data(nullptr), accessFlag(), usage(), mapType(), vertexType() {}

			VertexBufferInfo& operator=(const VertexBufferInfo& source)
			{
				stride = source.stride;
				offset = source.offset;
				count = source.count;
				size = source.size;

				data = source.data;

				accessFlag = source.accessFlag;
				usage = source.usage;
				mapType = source.mapType;

				vertexType = source.vertexType;

				return *this;
			}

			void AssignData(const void* sourceData)
			{
				switch (vertexType)
				{
				case VertexType::Vertex:
				{
					auto&& vertices = std::static_pointer_cast<Vertex[]>(data);
					const auto& sourceVertices = static_cast<const Vertex*>(sourceData);

					for (uint32_t index = 0u; index < count; ++index)
					{
						vertices[index] = sourceVertices[index];
					}
				}
				break;
				case VertexType::ParticleVertex:
				{
					auto&& vertices = std::static_pointer_cast<ParticleVertex[]>(data);
					const auto& sourceVertices = static_cast<const ParticleVertex*>(sourceData);

					for (uint32_t index = 0u; index < count; ++index)
					{
						vertices[index] = sourceVertices[index];
					}
				}
				break;
				case VertexType::StaticMeshVertex:
				{
					auto&& vertices = std::static_pointer_cast<MeshResource::StaticMeshVertex[]>(data);
					const auto& sourceVertices = static_cast<const MeshResource::StaticMeshVertex*>(sourceData);

					for (uint32_t index = 0u; index < count; ++index)
					{
						vertices[index] = sourceVertices[index];
					}
				}
				break;
				case VertexType::SkinnedMeshVertex:
				{
					auto&& vertices = std::static_pointer_cast<MeshResource::SkinnedMeshVertex[]>(data);
					const auto& sourceVertices = static_cast<const MeshResource::SkinnedMeshVertex*>(sourceData);

					for (uint32_t index = 0u; index < count; ++index)
					{
						vertices[index] = sourceVertices[index];
					}
				}
				break;
				case VertexType::CollisionMeshVertex:
				{
					auto&& vertices = std::static_pointer_cast<MeshResource::CollisionMeshVertex[]>(data);
					const auto& sourceVertices = static_cast<const MeshResource::CollisionMeshVertex*>(sourceData);

					for (uint32_t index = 0u; index < count; ++index)
					{
						vertices[index] = sourceVertices[index];
					}
				}
				break;
				case VertexType::MeshVertex:
				{
					auto&& meshVertices = std::static_pointer_cast<MeshVertex[]>(data);
					const auto& sourceMeshVertices = static_cast<const MeshVertex*>(sourceData);

					for (uint32_t index = 0u; index < count; ++index)
					{
						meshVertices[index] = sourceMeshVertices[index];
					}
				}

				break;
				case VertexType::LineVertex:
				{
					auto&& lineVertices = std::static_pointer_cast<LineVertex[]>(data);
					const auto& sourceLineVertices = static_cast<const LineVertex*>(sourceData);

					for (uint32_t index = 0u; index < count; ++index)
					{
						lineVertices[index] = sourceLineVertices[index];
					}
				}
				break;
				case VertexType::SpriteVertex:
				{
					auto&& spriteVertices = std::static_pointer_cast<SpriteVertex[]>(data);
					const auto& sourceSpriteVertices = static_cast<const SpriteVertex*>(sourceData);

					for (uint32_t index = 0u; index < count; ++index)
					{
						spriteVertices[index] = sourceSpriteVertices[index];
					}
				}
				break;
				default:
					DENTY_ASSERT(false, "Unknown vertex type. (in VertexBufferInfo::AssignData)");
					break;
				}
			}

			ComRef<ID3D11Buffer> buffer;

			uint32_t stride;
			uint32_t offset;
			uint32_t size;
			uint32_t count;

			// Data, should be vertices.
			Ref<void> data;

			CpuAccessFlag accessFlag;
			Usage usage;
			MapType mapType;

			VertexType vertexType;
		};
	public:
		DXVertexBuffer();
		DXVertexBuffer(const DXVertexBuffer& source);
		~DXVertexBuffer() override = default;

		DXVertexBuffer& operator=(const DXVertexBuffer& source);

		// Create functions.
		void Create(
			const VertexBufferInfo& info
		);

		// Map/UnMap
		void* Map() override;
		void UnMap() const override;

		void Bind() const override;
		void Clone(const Ref<IDirectXBuffer>& buffer) const override;
	private:
		VertexBufferInfo _info;
	};
}