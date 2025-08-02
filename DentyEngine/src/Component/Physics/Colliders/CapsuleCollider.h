#pragma once

#include "Collider.h"

#include "Component/ComponentGenerator.h"

#include "Physics/Shapes/CapsuleShape.h"

namespace DentyEngine
{
	class CapsuleCollider final : public Collider
	{
	public:
		CapsuleCollider();
		CapsuleCollider(const CapsuleCollider& source);
		~CapsuleCollider() override = default;

		void Awake() override;
		void Start() override;
		void OnUpdate(float deltaTime) override;
		void OnFixedUpdate() override;

		void OnEvent(Event* e) override;
		void OnGui() override;
		void OnPrefabGui() override;

		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnDestroy() override;
		void OnGameObjectDeserialized() override;

		void OnSceneActivated() override;
		void OnSceneDeActivated() override;

		void OnGameObjectTagChanged() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		[[nodiscard]]
		Ref<IShape> GetCollisionShape() const override { return _shape; }

		DENTY_REGISTER_BUILD_IN_COMPONENT(CapsuleCollider)
	private:
		void ApplyScaling() override;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Collider>(this),
					CEREAL_NVP(_shape)
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
					cereal::base_class<Collider>(this),
					CEREAL_NVP(_shape)
				);    
			}

			OnDeserialized();
		}
	private:
		Ref<CapsuleShape> _shape;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::CapsuleCollider, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::CapsuleCollider)