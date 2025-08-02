#pragma once

#include "Light.h"

#include "Component/ComponentGenerator.h"

namespace DentyEngine
{
	struct HemisphereLightConstants final
	{
	public:
		HemisphereLightConstants();
		~HemisphereLightConstants() noexcept = default;

	public:
		Vector3 skyDirection;
		float padding;

		Color skyColor;
		Color groundColor;
	};

	class HemisphereLight final : public Light
	{
	public:
		HemisphereLight();
		HemisphereLight(const HemisphereLight& source);
		~HemisphereLight() noexcept override = default;

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

		DENTY_REGISTER_BUILD_IN_COMPONENT(HemisphereLight)
	public:
		HemisphereLightConstants constants;

	private:
		void CreateGizmoSprite() override;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<Light>(this),
				CEREAL_NVP(_skyDirection),
				CEREAL_NVP(_skyColor),
				CEREAL_NVP(_groundColor)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version == 1u)
			{
				archive(
					cereal::base_class<Light>(this),
					CEREAL_NVP(_skyDirection),
					CEREAL_NVP(_skyColor),
					CEREAL_NVP(_groundColor)
				);
			}

			OnDeserialized();
		}
	private:
		Vector3 _skyDirection;
		Color _skyColor;
		Color _groundColor;
	};
}

CEREAL_REGISTER_TYPE(DentyEngine::HemisphereLight)

CEREAL_CLASS_VERSION(DentyEngine::HemisphereLight, 1u)