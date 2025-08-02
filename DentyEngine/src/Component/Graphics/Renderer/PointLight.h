#pragma once

#include "Light.h"
#include "Component/ComponentGenerator.h"

namespace DentyEngine
{
	class PointLight final : public Light
	{
	public:
		struct Constants
		{
			Constants() : lightPosition(), lightColor(), intensity(), range(), padding(), lightViewProjection() {}

			Vector4 lightPosition;
			Color lightColor;

			float intensity;
			float range;
			Vector2 padding;

			Matrix lightViewProjection;
		};
	public:
		PointLight();
		PointLight(const PointLight& source);
		~PointLight() override = default;

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

		DENTY_REGISTER_BUILD_IN_COMPONENT(PointLight)
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
					CEREAL_NVP(_intensity)
				);
			}

			if (version == 3u)
			{
				archive(
					cereal::base_class<Light>(this),
					CEREAL_NVP(_intensity),
					CEREAL_NVP(_range)
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
					CEREAL_NVP(_intensity)
				);
			}

			if (version == 3u)
			{
				archive(
					cereal::base_class<Light>(this),
					CEREAL_NVP(_intensity),
					CEREAL_NVP(_range)
				);
			}

			OnDeserialized();
		}

	private:
		// Version 1
	    [[maybe_unused]] float _constantAttenuation;
		[[maybe_unused]] float _linearAttenuation;
		[[maybe_unused]] float _secondOrderAttenuation;

		// Version 2
		float _intensity;
		float _range;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::PointLight, 3u)

CEREAL_REGISTER_TYPE(DentyEngine::PointLight)