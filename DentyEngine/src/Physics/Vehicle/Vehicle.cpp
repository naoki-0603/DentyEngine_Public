#include "stdafx.h"

#include "Vehicle.h"

#include "Scene/GameObject.h"

#include "System/Physics/PhysicsEngine.h"

namespace DentyEngine
{
	VehicleComponents::VehicleComponents()
	{
	}

    void VehicleComponents::Reset()
    {
		raycastVehicle.reset();
		//convexRaycaster.reset();
		defaultVehicleRaycaster.reset();
		vehicleTuning.reset();
		chassisRigidBody.reset();
		chassisCompoundShape.reset();
		chassisBoxShape.reset();
		chassisDefaultMotionState.reset();
    }

    VehicleStatus::VehicleStatus()
	{
	}

	VehicleConstants::VehicleConstants()
	{
	}

	Vehicle::Vehicle() :
		Object(), _components(), _status(), _collisionInfo()
	{
		_name = "Vehicle";
	}

	Vehicle::Vehicle(const Vehicle& source) :
		Object(source)
	{
	}

	void Vehicle::Create(EntityID owner, uint32_t tagNumber, const Matrix& world)
	{
		// Create vehicle framework.
		CreateChassis(owner, tagNumber, world);
		CreateRaycastVehicle();
		CreateWheels();
	}

    void Vehicle::Recreate(EntityID owner, uint32_t tagNumber, const Matrix& world)
    {
		Reset();

		Create(owner, tagNumber, world);
    }

    void Vehicle::Update()
	{
		_collisionInfo.Update();
	}

	void Vehicle::ShapeGui()
	{
	}

	void Vehicle::OnSerialized() const
	{
		Object::OnSerialized();
	}

	void Vehicle::OnDeserialized()
	{
		Object::OnDeserialized();
	}

	void Vehicle::CreateChassis(EntityID owner, uint32_t tagNumber, const Matrix& world)
	{
		{
			_components.chassisBoxShape = std::make_shared<btBoxShape>(VehicleConstants::CHASSIS_HALF_EXTENTS.ToBtVector3());
			_components.chassisCompoundShape = std::make_shared<btCompoundShape>();
		}

		{
			btTransform localTransform = { };
			localTransform.setIdentity();
			localTransform.setOrigin(btVector3(0, 0.15f, 0));

			_components.chassisCompoundShape->addChildShape(localTransform, _components.chassisBoxShape.get());
		}

		{
			_collisionInfo.Create(owner);
		}

		{
			auto inertia = btVector3(0, 0, 0);
			{
				_components.chassisCompoundShape->calculateLocalInertia(VehicleConstants::CHASSIS_MASS, inertia);
			}
			
			btTransform btTransform = world.ToBtTransformWithoutScaling();

			_components.chassisDefaultMotionState = std::make_shared<btDefaultMotionState>(btTransform);
			btRigidBody::btRigidBodyConstructionInfo info(
				VehicleConstants::CHASSIS_MASS,
				_components.chassisDefaultMotionState.get(),
				_components.chassisCompoundShape.get(), 
				inertia
			);
			_components.chassisRigidBody = std::make_shared<btRigidBody>(info);
			_components.chassisRigidBody->setActivationState(DISABLE_DEACTIVATION);
			_components.chassisRigidBody->setUserIndex(static_cast<int32_t>(tagNumber));
			_components.chassisRigidBody->setUserPointer(&_collisionInfo);
		}
	}

	void Vehicle::CreateRaycastVehicle()
	{
		btDiscreteDynamicsWorld* world = PhysicsEngine::GetInstance()->GetWorld();

		_components.vehicleTuning = std::make_shared<btRaycastVehicle::btVehicleTuning>();
		//_components.convexRaycaster = std::make_shared<btVehicleConvexRaycaster>(world);
		_components.defaultVehicleRaycaster = std::make_shared<btDefaultVehicleRaycaster>(world);
		_components.raycastVehicle = std::make_shared<btRaycastVehicle>(
			*_components.vehicleTuning, _components.chassisRigidBody.get(), _components.defaultVehicleRaycaster.get()
		);
		_components.raycastVehicle->setCoordinateSystem(0, 1, 2);
	}

	void Vehicle::CreateWheels()
	{
		btVector3 raycastDirection = { 0, -1 ,0 };

		btVector3 rotationAxis = { -1, 0, 0 };

		// https://www.mitsui-direct.co.jp/car/guide/mycar_guide/new/suspension/
		const btVector3 wheelConnectionPoint = { VehicleConstants::CHASSIS_HALF_EXTENTS.x + VehicleConstants::WHEEL_RADIUS, VehicleConstants::WHEEL_CONNECTION_HEIGHT, VehicleConstants::CHASSIS_HALF_EXTENTS.z - VehicleConstants::WHEEL_WIDTH };

		// Front
		//_components.raycastVehicle->addWheel(btVector3(wheelConnectionPoint.getX() - 0.2f, wheelConnectionPoint.getY(), wheelConnectionPoint.getZ()), raycastDirection, rotationAxis, VehicleConstants::WHEEL_SUSPENSION_REST_LENGTH, VehicleConstants::WHEEL_RADIUS, *_components.vehicleTuning, true);
		_components.raycastVehicle->addWheel(wheelConnectionPoint, raycastDirection, rotationAxis, VehicleConstants::WHEEL_SUSPENSION_REST_LENGTH, VehicleConstants::WHEEL_RADIUS, *_components.vehicleTuning, true);
		_components.raycastVehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, 1), raycastDirection, rotationAxis, VehicleConstants::WHEEL_SUSPENSION_REST_LENGTH, VehicleConstants::WHEEL_RADIUS, *_components.vehicleTuning, true);

		// Rear
		//_components.raycastVehicle->addWheel(btVector3(wheelConnectionPoint.getX() - 0.2f, wheelConnectionPoint.getY(), wheelConnectionPoint.getZ()) * btVector3(1, 1, -1), raycastDirection, rotationAxis, VehicleConstants::WHEEL_SUSPENSION_REST_LENGTH, VehicleConstants::WHEEL_RADIUS, *_components.vehicleTuning, false);
		_components.raycastVehicle->addWheel(wheelConnectionPoint * btVector3(1, 1, -1), raycastDirection, rotationAxis, VehicleConstants::WHEEL_SUSPENSION_REST_LENGTH, VehicleConstants::WHEEL_RADIUS, *_components.vehicleTuning, false);
		_components.raycastVehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, -1), raycastDirection, rotationAxis, VehicleConstants::WHEEL_SUSPENSION_REST_LENGTH, VehicleConstants::WHEEL_RADIUS, *_components.vehicleTuning, false);

		// ここは日本語で
		for (int32_t index = 0; index < _components.raycastVehicle->getNumWheels(); ++index)
		{
			btWheelInfo& wheel = _components.raycastVehicle->getWheelInfo(index);

			// サスペンションの剛性 (外部から曲げやねじりの力が加えられた時に寸法変化が起こりにくい性質のこと)
			// を設定する (操作性の向上が図れる) (0 ~ 50 オフロード, 50 ~ 100 レーシングカー, 200前後 F1カー)
			wheel.m_suspensionStiffness = 400.0f;
			wheel.m_maxSuspensionTravelCm = 40.0f;

			// サスペンションに使われている。
			// スプリングが圧縮されているときに適用されるダンピング (走行時の振動を減衰すること)
			// 0.0f に設定するとはね続ける (エネルギーを保持するため)
			// 一般的な車 (0.3f) レーシングカー (0.5f) 程度
			wheel.m_wheelsDampingCompression = 3.2f;

			wheel.m_frictionSlip = 3.0f;
			//if (index < 1)
			//{
			//	// グリップ力を設定 (0.0f グリップなし 1.0f 通常のグリップ)
			//	// ドリフトカーのセットアップでは、後輪のグリップを前輪よりわずかに低く設定するか、タイヤの摩耗をシミュレートするために低い値を使用してみてください。
			//	wheel.m_frictionSlip = btScalar(0.9f);
			//}
			//else
			//{
			//	// グリップ力を設定 (0.0f グリップなし 1.0f 通常のグリップ)
			//	// ドリフトカーのセットアップでは、後輪のグリップを前輪よりわずかに低く設定するか、タイヤの摩耗をシミュレートするために低い値を使用してみてください。
			//	wheel.m_frictionSlip = btScalar(1.0f);
			//}
			// バネを緩めるときにかかる減衰率を指定 
			// DampingCompression よりも少し高めに設定すると良い
			wheel.m_wheelsDampingRelaxation = 3.6f;
			//Larger friction slips will result in better handling

			// 車のロールに影響する値です。全輪を1.0にすると横転しやすくなり、0.0にすると横転しにくくなります
			wheel.m_rollInfluence = 0.0f;
		}
	}

    void Vehicle::Reset()
    {

    }
}
