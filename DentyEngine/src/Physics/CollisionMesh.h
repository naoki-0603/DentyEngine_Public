#pragma once

#include "Graphics/Mesh/Mesh.h"

#include "Graphics/Vertex.h"

namespace DentyEngine
{
	class CollisionMesh final : public Mesh, public std::enable_shared_from_this<CollisionMesh>
	{
		friend class MeshImporter;
	public:
		// For serialize.
		CollisionMesh();
		CollisionMesh(std::string_view name);
		CollisionMesh(const CollisionMesh& source);
		~CollisionMesh() override = default;

		void OnUpdate(const Matrix& world, const EntityID entityID, const Ref<Material>& rendererMaterial, const float deltaTime) override;
		void OnGui() override;
		void OnRender(ID3D11DeviceContext* context, const Ref<Material>& rendererMaterial, const std::vector<Ref<Material>>& materials) override;
		void OnRenderShadow(ID3D11DeviceContext* context, [[maybe_unused]] uint32_t cascadeShadowAreaNo) override;

		void OnEvent(Event* e) override;
		void OnSerialized() const override;
		void OnDeserialized() override;

		void Save(bool isUpdate) const override;
		void Load(const std::filesystem::path& filePath) override;

		void OnReload() override;

		void Clone(const Ref<Asset>& asset) override;

		void GenerateBoundingBox();

		// Call on MeshImporter::ImportAsCollision function.
		void CalculateCentroid();

		//
		// Getter
		//
		[[nodiscard]]
		AssetType GetAssetType() const override { return AssetType::CollisionMesh; }

		[[nodiscard]]
		const std::vector<MeshResource::CollisionMeshVertex>& GetVertices() const { return _vertices; }

		[[nodiscard]]
		const Vector3& GetCentroid() const { return _centroid; }

		[[nodiscard]]
		const char* GetMeshType() const override { return "CollisionMesh"; }
	public:
		static void LoadAssetTexture();

		static Ref<Texture2D> GetAssetTexture() { return _collisionMeshAssetTexture; }

	private:
		void CopyOf(const Mesh* source) override;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
			   archive(
				cereal::base_class<Mesh>(this),
				CEREAL_NVP(_vertices),
				CEREAL_NVP(_centroid)
			); 
			}
			

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
			   archive(
				cereal::base_class<Mesh>(this),
				CEREAL_NVP(_vertices),
				CEREAL_NVP(_centroid)
			); 
			}

			OnDeserialized();
		}
	private:
		std::vector<MeshResource::CollisionMeshVertex> _vertices;

		Vector3 _centroid;
	private:
		static Ref<Texture2D> _collisionMeshAssetTexture;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::CollisionMesh, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::CollisionMesh)