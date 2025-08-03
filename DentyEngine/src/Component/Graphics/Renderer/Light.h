#pragma once

#include "Component/Behavior.h"

#include "Graphics/Color.h"

namespace DentyEngine
{
	class GameCamera;

	class Sprite3D;

	// Want to call update in light manager,
	// so inherited component class.
	class Light : public Component, public std::enable_shared_from_this<Light>
	{
	public:
		enum class LightType
		{
			Directional,
			Point,
			Spot,
			Hemisphere,

			Max
		};

		enum class ShadowType
		{
			Hard,
			Soft,

			Max
		};
	public:
		Light();
		Light(const Light& source);
		~Light() override = default;

		virtual void Awake() override;
		virtual void Start() override;
		virtual void OnUpdate(float deltaTime);
		virtual void OnEvent(Event* e) override;
		virtual void OnGui() override;
		virtual void OnPrefabGui() override;

		virtual void OnSerialized() const override;
		virtual void OnDeserialized() override;

		virtual void OnDestroy() override;

		virtual void OnGameObjectDeserialized() override;

		void OnGizmoRender() override;

		[[nodiscard]]
		virtual LightType GetType() const noexcept = 0;
	protected:
		virtual void CreateGizmoSprite() = 0;

	protected:
		Color _lightColor;

		[[maybe_unused]] LightType _lightType;
		ShadowType _shadowType;

		Ref<Sprite3D> _gizmoSprite;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			{
				archive(
					cereal::base_class<Component>(this),
					CEREAL_NVP(_lightColor),
					CEREAL_NVP(_shadowType),
					CEREAL_NVP(_shadowTypeString)
				);    
			}
		}

		template <class Archive>
		void load(Archive& archive, uint32_t version)
		{
			if (version == 1u)
			{
				archive(
					cereal::base_class<Component>(this),
					CEREAL_NVP(_lightColor),
					CEREAL_NVP(_lightType),
					CEREAL_NVP(_shadowType),
					CEREAL_NVP(_shadowTypeString)
				);    
			}
			else if (version == 2u)
			{
				archive(
					cereal::base_class<Component>(this),
					CEREAL_NVP(_lightColor),
					CEREAL_NVP(_shadowType),
					CEREAL_NVP(_shadowTypeString)
				);
			}

		}

	private:
		std::string _shadowTypeString;

	private:
		const static std::vector<std::string> SHADOW_TYPES;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Light, 2u)

CEREAL_REGISTER_TYPE(DentyEngine::Light)