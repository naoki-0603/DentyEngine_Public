#pragma once

#include "Mesh.h"

#include "Skeleton.h"

#include "Graphics/Vertex.h"

namespace DentyEngine
{
	namespace MeshResource
	{
	    class AnimationClip;
	}

	// Delete when debugging finished.
	struct DebugAnimation
	{
		DebugAnimation() :
		    clip(), timer(), ticksPerFrame(), duration(), ticksPerSecond(), deltaTime(), index(), play() {}

		void OnGui();

		Ref<MeshResource::AnimationClip> clip;

		float timer;
		float ticksPerFrame;
		float duration;
		float ticksPerSecond;
		float deltaTime;

		int32_t index;

		bool play;
	};

	struct SkinnedMeshConstants
	{
	public:
		SkinnedMeshConstants() noexcept;
		~SkinnedMeshConstants() noexcept = default;

	public:
		// Vertex shader.
		Matrix world;
		std::array<Matrix, 256> boneTransforms;

		uint32_t entityID;
		Vector3 padding;

		MaterialConstants materialConstants;
	};

	struct SkinnedMeshShadowConstants
	{
		SkinnedMeshShadowConstants() : world(), boneTransforms(), cascadeAreaNo(), padding() {}

		Matrix world;
		std::array<Matrix, 256> boneTransforms;

		uint32_t cascadeAreaNo;
		Vector3 padding;
	};

	class SkinnedMesh final : public Mesh, public std::enable_shared_from_this<SkinnedMesh>
	{
		friend class MeshEditor;
		friend class MeshImporter;
	public:
		// For serialize.
		SkinnedMesh();

		SkinnedMesh(std::string_view name);
		SkinnedMesh(const SkinnedMesh&);
		~SkinnedMesh() override = default;

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

		//
		// Getter
		//
		[[nodiscard]]
		const std::vector<MeshResource::SkinnedMeshVertex>& GetVertices() const { return _vertices; }

		[[nodiscard]]
		const std::vector<uint32_t>& GetIndices() const { return _indices; }

		[[nodiscard]]
		const char* GetMeshType() const override { return "SkinnedMesh"; }
	public:
		static const String ADD_FILENAME_AT_END;

	private:
		void CopyOf(const Mesh* source) override;

	private:
		std::vector<MeshResource::SkinnedMeshVertex> _vertices;

		FilePath _avatarFilePath;

		std::vector<uint32_t> _indices;

		// Only has root mesh.
		// Doesn't serialize.
		Ref<MeshResource::Skeleton> _avatar;
		Ref<DXIndexBuffer> _indexBuffer;

		SkinnedMeshConstants _constants;
		SkinnedMeshShadowConstants _shadowConstants;

		// Delete when debugging finished.
		DebugAnimation _debugAnimation;
		bool _playAnimation = false;
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
					CEREAL_NVP(_indices),
					CEREAL_NVP(_avatarFilePath)
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
					CEREAL_NVP(_indices),
					CEREAL_NVP(_avatarFilePath)
				);    
			}

			OnDeserialized();
		}
	};
}

CEREAL_CLASS_VERSION(DentyEngine::SkinnedMesh, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::SkinnedMesh)