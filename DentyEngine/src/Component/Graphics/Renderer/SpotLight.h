#pragma once

#include "Light.h"
#include "Component/ComponentGenerator.h"

namespace DentyEngine
{
	class SpotLight final : public Light
	{
	public:
		struct Constants
		{
			Constants() : position(), direction(), color(), phi(), theta(), intensity(),
				padding(), lightViewProjection() {}

			Vector4 position;
			Vector4 direction;

			Color color;

			float phi;
			float theta;
			float intensity;
			float padding;

			Matrix lightViewProjection;
		};
	public:
		SpotLight();
		SpotLight(const SpotLight& source);
		~SpotLight() override = default;

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
		LightType GetType() const noexcept override;

		DENTY_REGISTER_BUILD_IN_COMPONENT(SpotLight)
	public:
		Constants constants;
	private:
		void CreateGizmoSprite() override;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Light>(this),
					CEREAL_NVP(_direction),
					CEREAL_NVP(_cone),
					CEREAL_NVP(_constantAttenuation),
					CEREAL_NVP(_linearAttenuation),
					CEREAL_NVP(_secondOrderAttenuation),
					CEREAL_NVP(_intensity)
				);    
			}
			
			if (version == 2u)
			{
			    archive(
				    cereal::base_class<Light>(this),
					CEREAL_NVP(_direction),
					CEREAL_NVP(_phi),
					CEREAL_NVP(_theta)
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
					cereal::base_class<Light>(this),
					CEREAL_NVP(_direction),
					CEREAL_NVP(_cone),
					CEREAL_NVP(_constantAttenuation),
					CEREAL_NVP(_linearAttenuation),
					CEREAL_NVP(_secondOrderAttenuation),
					CEREAL_NVP(_intensity)
				);    
			}
			
			if (version == 2u)
			{
			    archive(
				    cereal::base_class<Light>(this),
					CEREAL_NVP(_direction),
					CEREAL_NVP(_phi),
					CEREAL_NVP(_theta)
				);
			}

			OnDeserialized();
		}
	private:
		Vector4 _direction;

		// Version 1
		[[maybe_unused]] float _cone;
		[[maybe_unused]] float _constantAttenuation;
		[[maybe_unused]] float _linearAttenuation;
		[[maybe_unused]] float _secondOrderAttenuation;

		// Version 2
		float _phi;
		float _theta;

		float _intensity;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::SpotLight, 2u)

CEREAL_REGISTER_TYPE(DentyEngine::SpotLight)