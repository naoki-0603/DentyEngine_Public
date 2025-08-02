#pragma once

#include "Component/ComponentGenerator.h"
#include "Component/PhysicsBehaviour.h"
#include "Component/Transform.h"

#include "Physics/Shapes/BoxShape.h"
#include "Physics/Shapes/CapsuleShape.h"
#include "Physics/Shapes/ConvexMeshShape.h"
#include "Physics/Shapes/SphereShape.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

#include "System/Physics/CollisionInfo.h"

#include "System/Physics/PhysicsData.h"

namespace DentyEngine
{
    class EventGameObjectTagChanged;
    class CollisionMesh;
	class Mesh;

	class EventOnScenePlayButtonPressed;

	class RigidBody final : public PhysicsBehaviour, public std::enable_shared_from_this<RigidBody>
	{
	public:
		RigidBody();
		RigidBody(const RigidBody&);
		~RigidBody() override = default;

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

		void Generate();

		// Call when GameObject::OnSceneActivated function.
		void Restore();

		void ApplyScaling();

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		void OnSceneActivated();
		void OnSceneDeActivated();
		void OnRuntimeSceneChanged();

		void OnGameObjectTagChanged();

		// 
		// Event functions.
		//
		void OnScenePlayButtonPressed(EventOnScenePlayButtonPressed* e);

		// Call on Scene::OnApplicationStateChangedToPlay function.
		void OnApplicationStateChangedToPlay();

		void UpdateState(const btVector3& btPosition, const btMatrix3x3& btMatrix3X3);

		void EnableAngularVelocity();
		void DisableAngularVelocity();

		//
		// Maybe called from mono on fixed update.
		//
		void AddForce(const Vector3& force);
		void AddImpulse(const Vector3& impulse);
		void AddTorque(const Vector3& torque);

		//
		// Setter
		//
		void SetDefaultMotionState(Ref<btDefaultMotionState> defaultMotionState) { _motionState = defaultMotionState; }
		void SetActivationState(int32_t state);
		void SetCollisionFilter(int32_t collisionFilter);
		void SetGravity(const Vector3& gravity);
		void SetLinearVelocity(const Vector3& velocity);
		void SetPosition(const Vector3& position);

		//
		// Getter
		//
		[[nodiscard]]
		const Matrix& GetRigidBodyWorld() const;

		[[nodiscard]]
		Ref<IShape> GetCollisionShape() const { return _currentShape; }

		[[nodiscard]]
		const Ref<btDefaultMotionState>& GetDefaultMotionState() const { return _motionState; }

		[[nodiscard]]
		const Ref<btRigidBody>& GetBtRigidBody() const { return _rigidBody; }

		[[nodiscard]]
		ShapeType GetShapeType() const { return _shapeType; }

		[[nodiscard]]
		Vector3 GetLinearVelocity() const
		{
			const btVector3 linearVelocity = _rigidBody->getLinearVelocity();

			return Vector3(linearVelocity.getX(), linearVelocity.getY(), linearVelocity.getZ());
		}

		[[nodiscard]]
		float GetMass() const { return _mass; }

		[[nodiscard]]
		int GetCollisionFilter() const { return _collisionFilter; }

		[[nodiscard]]
		bool IsKinematic() const { return _isKinematic; }

		[[nodiscard]]
		bool HasRigidBody() const { return (_rigidBody != nullptr); }

		[[nodiscard]]
		bool HasCollisionShape() const { return (_currentShape != nullptr); }

		DENTY_REGISTER_BUILD_IN_COMPONENT(RigidBody)
	private:
		void AddWorld();

		//
		// Collision shapes.
		//
		void Reset();

		void GenerateCollisionShape();

		//
		// Event functions.
		//

		// Call when collision shape type changed.
		void OnShapeTypeChanged();
		void OnMassChangeToZero();
		void OnMassChangeToMoreThanZero();

		//
		// Drag and drop target callback functions.
		//
		void CollisionMeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<PhysicsBehaviour>(this),
					CEREAL_NVP(_boxShape),
					CEREAL_NVP(_sphereShape),
					CEREAL_NVP(_capsuleShape),
					CEREAL_NVP(_convexMeshShape),
					CEREAL_NVP(_shapeType),
					CEREAL_NVP(_mass),
					CEREAL_NVP(_collisionFilter),
					CEREAL_NVP(_useGravity),
					CEREAL_NVP(_isKinematic)
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
					cereal::base_class<PhysicsBehaviour>(this),
					CEREAL_NVP(_boxShape),
					CEREAL_NVP(_sphereShape),
					CEREAL_NVP(_capsuleShape),
					CEREAL_NVP(_convexMeshShape),
					CEREAL_NVP(_shapeType),
					CEREAL_NVP(_mass),
					CEREAL_NVP(_collisionFilter),
					CEREAL_NVP(_useGravity),
					CEREAL_NVP(_isKinematic)
				);    
			}

			OnDeserialized();
		}
	private:
		Ref<btRigidBody> _rigidBody;
	    Ref<btDefaultMotionState> _motionState;

		CollisionInfo _collisionInfo;

		//
		// Shapes.
		//
		Ref<IShape> _currentShape;
		Ref<BoxShape> _boxShape;
		Ref<SphereShape> _sphereShape;
		Ref<CapsuleShape> _capsuleShape;
		Ref<ConvexMeshShape> _convexMeshShape;

		ShapeType _shapeType;
		ShapeType _prevShapeType;

		int32_t _collisionFilter;

		float _mass;
		float _previousMass;

		bool _useGravity;
		bool _isKinematic;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::RigidBody, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::RigidBody)