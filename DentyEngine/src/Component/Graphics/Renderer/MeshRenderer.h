#pragma once

#include "Renderer.h"

#include "Component/ComponentGenerator.h"

namespace DentyEngine
{
	class EventOnMaterialAssetUpdated;

	class StaticMesh;

	class MeshRenderer final : public Renderer
	{
		friend class RenderManager;
	public:
		MeshRenderer();
		MeshRenderer(const MeshRenderer& source);
		~MeshRenderer() override = default;

		void Awake() override;
		void Start() override;

		void Update(const float deltaTime) override;

		void Render(ID3D11DeviceContext* context) override;
		void RenderShadow(ID3D11DeviceContext* context, uint32_t cascadeShadowAreaNo) override;

		void OnGui() override;
		void OnPrefabGui() override;

		void OnEvent(Event* e) override;
		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnDestroy() override;
		void OnGameObjectDeserialized() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		//
		// Setter
		//
		void SetMesh(Ref<StaticMesh> mesh);

		//
		// Getter
		//
		[[nodiscard]]
		std::filesystem::path GetFilePathToSerialized() const { return _meshFilePathToSerialized; }

		[[nodiscard]]
		const Ref<Mesh>& GetMesh() const { return _mesh; }

		[[nodiscard]]
		bool HasMesh() const { return (_mesh != nullptr); }

	    DENTY_REGISTER_BUILD_IN_COMPONENT(MeshRenderer)
	private:
		void ObtainMaterials();

		//
		// Event functions.
		//
		void OnMeshChanged(EventOnMeshChanged* e) override;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Renderer>(this),
					CEREAL_NVP(_meshFilePathToSerialized)
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
					cereal::base_class<Renderer>(this),
					CEREAL_NVP(_meshFilePathToSerialized)
				);    
			}
			
			OnDeserialized();
		}

	private:
		Ref<Mesh> _mesh;

		std::string _meshFilePathToSerialized;
	};
};

CEREAL_CLASS_VERSION(DentyEngine::MeshRenderer, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::MeshRenderer)