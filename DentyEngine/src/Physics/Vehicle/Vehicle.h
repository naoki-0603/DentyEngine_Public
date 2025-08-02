#pragma once

#include "System/Core/Object.h"
#include "System/Physics/CollisionInfo.h"

namespace DentyEngine
{
	class Chassis;

	struct VehicleComponents
	{
	public:
		VehicleComponents();
		~VehicleComponents() = default;

		void Reset();
	public:
		// Raycast vehicles.
		Ref<btRaycastVehicle> raycastVehicle = nullptr;
		//Ref<btVehicleConvexRaycaster> convexRaycaster = nullptr;
		Ref<btDefaultVehicleRaycaster> defaultVehicleRaycaster = nullptr;
		Ref<btRaycastVehicle::btVehicleTuning> vehicleTuning = nullptr;

		// Chassis rigid body.
		Ref<btRigidBody> chassisRigidBody = nullptr;
		Ref<btCompoundShape> chassisCompoundShape = nullptr;
		Ref<btBoxShape> chassisBoxShape = nullptr;
		Ref<btDefaultMotionState> chassisDefaultMotionState = nullptr;
	};

	struct VehicleStatus
	{
	public:
		VehicleStatus();
		~VehicleStatus() = default;

	public:

	};

	struct VehicleConstants
	{
	public:
		VehicleConstants();
		~VehicleConstants() = default;

	public:
		// Chassis
		static constexpr Vector3 CHASSIS_HALF_EXTENTS = Vector3(0.95f, 0.5f, 2.0f);
		static constexpr float CHASSIS_MASS = 1.0f;

		// Wheel
		static constexpr float WHEEL_RADIUS = 0.5f;
		static constexpr float WHEEL_WIDTH = 0.4f;
		static constexpr float WHEEL_CONNECTION_HEIGHT = 0.8f;
		static constexpr float WHEEL_SUSPENSION_REST_LENGTH = 0.7f;
	};

	enum class VehicleWheel : int32_t
	{
		None = -1,
		FrontLeft = 0,
		FrontRight,
		RearLeft,
		RearRight,

		Max
	};

	class Vehicle : public Object
	{
		friend class VehicleComponent;
	public:
		Vehicle();
		Vehicle(const Vehicle& source);
		~Vehicle() override = default;

		virtual void Create(EntityID owner, uint32_t tagNumber, const Matrix& world);
		virtual void Recreate(EntityID owner, uint32_t tagNumber, const Matrix& world);

		void Update();

		void ShapeGui();

		void OnSerialized() const override;
		void OnDeserialized() override;

		//
		// Getter
		//
		[[nodiscard]]
		const VehicleComponents& GetComponents() const { return _components; }

		[[nodiscard]]
		const VehicleStatus& GetStatus() const { return _status; }
	protected:
		VehicleComponents _components;
		VehicleStatus _status;
		CollisionInfo _collisionInfo;

	private:
		void CreateChassis(EntityID owner, uint32_t tagNumber, const Matrix& world);
		void CreateRaycastVehicle();
		void CreateWheels();

		void Reset();
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<Object>(this)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<Object>(this)
			);

			OnDeserialized();
		}
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Vehicle, 1u)