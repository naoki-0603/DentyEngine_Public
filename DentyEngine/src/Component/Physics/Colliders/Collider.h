#pragma once

#include "Component/PhysicsBehavior.h"

#include "Physics/Shapes/IShape.h"

#include "System/Physics/CollisionInfo.h"
#include "System/Physics/CollisionFilter.h"

namespace DentyEngine
{
	class EventRuntimeSceneChanged;
	class EventGameObjectTagChanged;
    class EventOnSettingsWindowClosed;

	class Collider : public PhysicsBehavior, public std::enable_shared_from_this<Collider>
	{
	public:
		enum class Type : int32_t
		{
			Box,
			Sphere,
			Capsule,
			Mesh,
			ConvexMesh,
			ApproximationMesh,

			Max
		};
	public:
		Collider();
		Collider(const Collider& source);
		~Collider() override = default;

		virtual void Awake() override;
		virtual void Start() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnFixedUpdate() override;

		virtual void OnGui() override;
		virtual void OnPrefabGui() override;
		virtual void OnEvent(Event* e) override;

		virtual void OnSerialized() const override;
		virtual void OnDeserialized() override;
		virtual void OnDestroy() override;
		virtual void OnGameObjectDeserialized() override;

		virtual void UpdateState(const Vector3& position, const Quaternion& orientation);

		virtual void ApplyScaling() = 0;

		//
		// Events
		//
		virtual void OnApplicationStateChangedToEdit() {}
		virtual void OnApplicationStateChangedToPlay() {}

		virtual void OnSceneActivated();
		virtual void OnSceneDeActivated();

		virtual void OnRuntimeSceneChanged();

		virtual void OnGameObjectTagChanged();
		
		//
		// Setter
		//
		void SetGhostObjectCollisionShape(Ref<btCollisionShape> shape) { _ghostObject->setCollisionShape(shape.get());  }


		void SetCollisionFilter(int collisionFilter);
		void SetIsTrigger(bool isTrigger) { _isTrigger = isTrigger; }

		//
		// Getter
		//
		[[nodiscard]]
		virtual Ref<IShape> GetCollisionShape() const = 0;

		[[nodiscard]]
		const Ref<btGhostObject>& GetGhostObject() const { return _ghostObject; }

		[[nodiscard]]
		Type GetCollisionShapeType() const { return _type; }

		[[nodiscard]]
		const CollisionFilter& GetCollisionFilter() const { return _collisionFilter; }
	protected:
		void AddWorld();
		void AddWorldReserved();
	protected:
		Ref<btGhostObject> _ghostObject;
		Ref<CollisionInfo> _collisionInfo;

		Vector3 _center;
		Quaternion _rotation;

		Type _type;

		int32_t _collisionFlag;

		// Use for collision filtering.
		CollisionFilter _collisionFilter;

		bool _isTrigger;
	private:
		void UpdateTransform();

		// serialize/deserialize functions.
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version == 1u)
			{
			    archive(
					cereal::base_class<PhysicsBehavior>(this),
					CEREAL_NVP(_center),
					CEREAL_NVP(_type),
					CEREAL_NVP(_collisionFlag),
					CEREAL_NVP(_collisionFilter),
					CEREAL_NVP(_isTrigger)
			    );
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version == 1u)
			{
			    archive(
					cereal::base_class<PhysicsBehavior>(this),
					CEREAL_NVP(_center),
					CEREAL_NVP(_type),
					CEREAL_NVP(_collisionFlag),
					CEREAL_NVP(_collisionFilter),
					CEREAL_NVP(_isTrigger)
			    );
			}
		}
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Collider, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Collider)