#pragma once

#include "Renderer.h"

#include "Graphics/Mesh/Mesh.h"

#include "Component/ComponentGenerator.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
	class Mesh;

	class SkinnedMeshRenderer final : public Renderer
	{
		friend class RenderManager;
	public:
		SkinnedMeshRenderer();
		SkinnedMeshRenderer(const SkinnedMeshRenderer& source);
		~SkinnedMeshRenderer() override = default;

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
		void SetMesh(Ref<SkinnedMesh> mesh);

		//
		// Getter
		//
		[[nodiscard]]
		const FilePath& GetFilePathToSerialized() const { return _meshFilePathToSerialized; }

		[[nodiscard]]
		const Ref<Mesh>& GetMesh() const { return _mesh; }

		[[nodiscard]]
		bool HasMesh() const { return (_mesh != nullptr); }

		DENTY_REGISTER_BUILD_IN_COMPONENT(SkinnedMeshRenderer)
	private:
		//
		// Event functions.
		//
		void OnMeshChanged(EventOnMeshChanged* e) override;

		void MeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
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

		FilePath _meshFilePathToSerialized;
	};
};

CEREAL_CLASS_VERSION(DentyEngine::SkinnedMeshRenderer, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::SkinnedMeshRenderer)