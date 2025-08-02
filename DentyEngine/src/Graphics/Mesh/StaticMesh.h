#pragma once

#include "Mesh.h"

#include "Graphics/Vertex.h"

namespace DentyEngine
{
	struct StaticMeshConstants
	{
	public:
		StaticMeshConstants() noexcept;
		~StaticMeshConstants() noexcept = default;

	public:
		// Vertex shader.
		Matrix world;

		uint32_t entityID;
		Vector3 padding;

		MaterialConstants materialConstants;
	};

	struct StaticMeshShadowConstants
	{
		StaticMeshShadowConstants() : world(), cascadeAreaNo(), padding() {}

		Matrix world;

		uint32_t cascadeAreaNo;
		Vector3 padding;
	};

	// Batch.
	class StaticMesh final : public Mesh, public std::enable_shared_from_this<StaticMesh>
	{
		friend class MeshEditor;
		friend class MeshImporter;
		friend class BatchGenerator;
	public:
		// For serialize.
		StaticMesh();

		StaticMesh(std::string_view name);
		StaticMesh(const StaticMesh&);
		~StaticMesh() override = default;

		//
		// Member functions.
		//
		void OnUpdate(const Matrix& world, const EntityID entityID, const Ref<Material>& rendererMaterial, const float deltaTime) override;
		void OnGui() override;
		void OnRender(ID3D11DeviceContext* context, const Ref<Material>& rendererMaterial, const std::vector<Ref<Material>>& materials) override;
		void OnRenderShadow(ID3D11DeviceContext* context, uint32_t cascadeShadowAreaNo) override;

		void OnEvent(Event* e) override;
		void OnSerialized() const override;
		void OnDeserialized() override;

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Destroy() override;
		void OnDestroy() override;

		void Clone(const Ref<Asset>& asset) override;

		void ReplaceMainMaterialShaders(Ref<Shader> shader);

		//
		// Getter
		//
		[[nodiscard]]
		const std::vector<MeshResource::StaticMeshVertex>& GetVertices() const { return _vertices; }

		[[nodiscard]]
		const char* GetMeshType() const override { return "StaticMesh"; }
	public:
		static const String ADD_FILENAME_AT_END;

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
					CEREAL_NVP(_vertices)
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
					CEREAL_NVP(_vertices)
				);    
			};

			OnDeserialized();
		}
	private:
		std::vector<MeshResource::StaticMeshVertex> _vertices;
		StaticMeshConstants _constants;
		StaticMeshShadowConstants _shadowConstants;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::StaticMesh, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::StaticMesh)