#pragma once

#include "Light.h"

#include "Component/ComponentGenerator.h"

namespace DentyEngine
{
	// Scene only has one directional light.
	class DirectionalLight final : public Light
	{
		friend class ShadowCamera;
	public:
		struct Constants
		{
			Vector4 lightPosition;
			Vector4 lightDirection;
			Color lightColor;

			std::array<Matrix, 3u> lightViewProjections;
		};
	public:
		DirectionalLight();
		DirectionalLight(const DirectionalLight& source);
		~DirectionalLight() override = default;

		void Awake() override;
		void Start() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(Event* e) override;
		void OnGui() override;
		void OnPrefabGui() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void OnDestroy() override;

		void OnGameObjectDeserialized() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		//
		// Getter
		//
		[[nodiscard]]
		const Vector4& GetDirection() const { return _direction; }

		[[nodiscard]]
		LightType GetType() const noexcept override;

		DENTY_REGISTER_BUILD_IN_COMPONENT(DirectionalLight)
	public:
		Constants constants;

	private:
		void CreateGizmoSprite() override;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Light>(this),
					CEREAL_NVP(_direction)
				);    
			}

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, uint32_t version) 
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Light>(this),
					CEREAL_NVP(_direction)
				);    
			}

			OnDeserialized();
		}
	private:
		Vector4 _direction;

		GameCamera* _lightCameraRef;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::DirectionalLight, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::DirectionalLight)