#pragma once

#include "Component/ComponentGenerator.h"
#include "Component/PhysicsBehaviour.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

#include "System/Physics/CollisionInfo.h"
#include "System/Physics/PhysicsData.h"

#include "DynamicCharacterStatus.h"

// https://www.pierov.org/2020/05/23/dynamic-character-controller-bullet/

namespace DentyEngine
{
    class EventGameObjectTagChanged;
    class ConvexMeshShape;
	class CapsuleShape;
	class SphereShape;
	class BoxShape;
	class IShape;

	namespace PhysicsAction
	{
		class DynamicCharacterController;
	}

	//
	// Unused class. (2024/01/09)
	//
	class DynamicCharacterController final : public PhysicsBehaviour, public std::enable_shared_from_this<DynamicCharacterController>
	{
	public:
		DynamicCharacterController();
		DynamicCharacterController(const DynamicCharacterController& source);
		~DynamicCharacterController() override = default;

		void Awake() override;
		void Start() override;
		void OnUpdate(float deltaTime) override;
		void OnFixedUpdate() override;
		void OnGui() override;
		void OnPrefabGui() override;
		void OnEvent(Event* e) override;
		void OnDestroy() override;
		void Destroy() override;
		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnGameObjectDeserialized() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		void Restore();

		void OnGravityDirectionChanged(GravityDirection direction);

		//
		// Setter
		//
		void SetGravity(const Vector3& gravity, GravityDirection direction);
		void SetMaxSteeringAngle(float maxSteeringAngle);
		void SetMaxSpeed(float maxSpeed);
		void SetMaxGrip(float maxGrip);
		void SetMaxBrake(float maxBrake);
		void SetMaxThrust(float maxThrust);

		//
		// Getter 
		//
		[[nodiscard]]
		Vector3 GetLinearVelocity() const;

		[[nodiscard]]
		Vector3 GetTorqueVelocity() const;

		[[nodiscard]]
		const Vector3& GetGravity() const;

		[[nodiscard]]
		bool OnGround() const;

		[[nodiscard]]
		Ref<btRigidBody> GetBody() const { return _rigidBody; }

		[[nodiscard]]
		Ref<PhysicsAction::DynamicCharacterController> GetAction() const { return _action; }

		[[nodiscard]]
		bool HasRigidBodyAndAction() const { return (_rigidBody && _action); }

		DENTY_REGISTER_BUILD_IN_COMPONENT(DynamicCharacterController)
	private:
		void Create();
		void CreateShapes();

		void ShapeTypeChanged();
		void Reset();

		void OnGameObjectTagChanged(EventGameObjectTagChanged* e);

		//
		// Drag and drop target callback functions.
		//
		void CollisionMeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive) const
		{
			archive(
				cereal::base_class<PhysicsBehaviour>(this),
				CEREAL_NVP(_currentShapeType),
				CEREAL_NVP(_mass),
				CEREAL_NVP(_maxSteeringAngle),
				CEREAL_NVP(_maxThrust),
				CEREAL_NVP(_maxBrake),
				CEREAL_NVP(_maxSpeed),
				CEREAL_NVP(_maxGrip)
			);
		}

		template <class Archive>
		void load(Archive& archive)
		{
			archive(
				cereal::base_class<PhysicsBehaviour>(this),
				CEREAL_NVP(_currentShapeType),
				CEREAL_NVP(_mass),
				CEREAL_NVP(_maxSteeringAngle),
				CEREAL_NVP(_maxThrust),
				CEREAL_NVP(_maxBrake),
				CEREAL_NVP(_maxSpeed),
				CEREAL_NVP(_maxGrip)
			);
		}
	private:
		Ref<btRigidBody> _rigidBody;
		Ref<btDefaultMotionState> _motionState;
		Ref<PhysicsAction::DynamicCharacterController> _action;

		// Shapes
		Ref<IShape> _currentShape;
		Ref<BoxShape> _boxShape;
		Ref<SphereShape> _sphereShape;
		Ref<CapsuleShape> _capsuleShape;

		CollisionInfo _collisionInfo;

		ShapeType _currentShapeType;
		ShapeType _prevShapeType;

		Vector3 _gravity;

		// Doesn't serialize.
		GravityDirection _gravityDirection;

		float _mass;
		float _maxSteeringAngle;
		float _maxThrust;
		float _maxBrake;
		float _maxSpeed;
		float _maxGrip;

	};
}

CEREAL_REGISTER_TYPE(DentyEngine::DynamicCharacterController)