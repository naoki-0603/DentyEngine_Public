#pragma once

#include "Collider.h"

#include "Component/ComponentGenerator.h"

#include "Physics/Shapes/BoxShape.h"

namespace DentyEngine
{
	class BoxCollider final : public Collider
	{
	public:
		BoxCollider();
		BoxCollider(const BoxCollider& source);
		~BoxCollider() override = default;

		void Awake() override;
		void Start() override;
		void OnUpdate(float deltaTime) override;
		void OnFixedUpdate() override;
		void OnGui() override;
		void OnPrefabGui() override;
		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnDestroy() override;
		void OnGameObjectDeserialized() override;

		void ApplyScaling() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		//
		// Event functions.
		//
		void OnSceneActivated() override;
		void OnSceneDeActivated() override;

		void OnGameObjectTagChanged() override;

		//
		// Getter
		//
		[[nodiscard]]
		Ref<IShape> GetCollisionShape() const override { return _boxShape; }

		[[nodiscard]]
		const Vector3& GetCenter() const { return _center; }

		DENTY_REGISTER_BUILD_IN_COMPONENT(BoxCollider)
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Collider>(this),
					CEREAL_NVP(_center),
					CEREAL_NVP(_boxShape)
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
					CEREAL_NVP(_center),
					CEREAL_NVP(_boxShape)
				);    
			}

			OnDeserialized();
		}
	private:
		Ref<BoxShape> _boxShape;

		Vector3 _center;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::BoxCollider, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::BoxCollider)