#pragma once

#include "Component/Component.h"

#include "Graphics/Mesh/SystemUnit.h"

namespace DentyEngine
{
	class Mesh;
	class Material;

	class EventOnMeshChanged;

	class Renderer : public Component, public std::enable_shared_from_this<Renderer>
	{
	public:
		enum class Type
		{
			MeshRenderer,
			SkinnedMeshRenderer,
			SpriteRenderer
		};

		enum class OrderType
		{
			Opacity,
			Transparent,

			Max
		};
	public:
		// For serialize
		Renderer() = default;
		Renderer(Type rendererType, OrderType orderType = OrderType::Opacity);
		Renderer(const Renderer& source);

		// Operators.
		//Renderer& operator=(const Renderer& source);

		virtual ~Renderer() override = default;

		virtual void Awake() override;
		virtual void Start() override;

		virtual void Update(const float deltaTime);

		virtual void Render(ID3D11DeviceContext* context) = 0;
		virtual void RenderShadow(ID3D11DeviceContext* context, uint32_t cascadeShadowAreaNo) = 0;

		virtual void OnGui() override;
		virtual void OnPrefabGui() override;

		void OnEvent(Event* e) override;
		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnDestroy() override;
		void OnGameObjectDeserialized() override;

		void OnSceneActivated();

		//
		// Setter
		//
		void SetSubMeshIndex(uint32_t subMeshIndex) { _subMeshIndex = subMeshIndex; }

		//
		// Getter
		//
		[[nodiscard]]
		uint32_t GetSubMeshIndex() const { return _subMeshIndex; }

		[[nodiscard]]
		Type GetRendererType() const { return _rendererType; }

		[[nodiscard]]
		OrderType GetOrderType() const { return _orderType; }
	public:
	    bool shadowCastEnabled;
	protected:
		virtual void OnMeshChanged(EventOnMeshChanged* e) = 0;

		void OnUpdate(const Ref<Mesh>& mesh, const Ref<Material>& mainMaterial, const float deltaTime);
		void OnRender(ID3D11DeviceContext* context, const Ref<Material>& mainMaterial, const std::vector<Ref<Material>>& materials, const Ref<Mesh>& mesh);
		void OnRenderShadow(ID3D11DeviceContext* context, const Ref<Mesh>& mesh, uint32_t cascadeShadowAreaNo);

		// This function must be call OnGui or OnPrefabGui function.
		void MaterialGui(const Ref<Mesh>& mesh, std::string_view label);
	protected:
		std::vector<FilePath> _materialFilePaths;

		// If subMesh, contain index.
		uint32_t _subMeshIndex;

		// Doesn't serialize.
		std::unordered_map<FilePath, Ref<Material>, FilePath::Hash> _materials;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
			    archive(
					cereal::base_class<Component>(this),
					CEREAL_NVP(_materialFilePaths),
					CEREAL_NVP(_subMeshIndex),
					CEREAL_NVP(_orderType),
					CEREAL_NVP(shadowCastEnabled)
			    );    
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
					cereal::base_class<Component>(this),
					CEREAL_NVP(_materialFilePaths),
					CEREAL_NVP(_subMeshIndex),
					CEREAL_NVP(_orderType),
					CEREAL_NVP(shadowCastEnabled)
			    );    
			}
		}
	private:
		std::string _castShadow;
		std::string _orderTypeStr;

		Type _rendererType;
		OrderType _orderType;

		// static variables.
	private:
		static const std::vector<std::string> CAST_SHADOW_OPTIONS;
		static const std::vector<std::string> ORDER_TYPES;
	};
};

CEREAL_CLASS_VERSION(DentyEngine::Renderer, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Renderer)