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

		// �����͓��{���
		for (int32_t index = 0; index < _components.raycastVehicle->getNumWheels(); ++index)
		{
			btWheelInfo& wheel = _components.raycastVehicle->getWheelInfo(index);

			// �T�X�y���V�����̍��� (�O������Ȃ���˂���̗͂�������ꂽ���ɐ��@�ω����N����ɂ��������̂���)
			// ��ݒ肷�� (���쐫�̌��オ�}���) (0 ~ 50 �I�t���[�h, 50 ~ 100 ���[�V���O�J�[, 200�O�� F1�J�[)
			wheel.m_suspensionStiffness = 400.0f;
			wheel.m_maxSuspensionTravelCm = 40.0f;

			// �T�X�y���V�����Ɏg���Ă���B
			// �X�v�����O�����k����Ă���Ƃ��ɓK�p�����_���s���O (���s���̐U�����������邱��)
			// 0.0f �ɐݒ肷��Ƃ͂ˑ����� (�G�l���M�[��ێ����邽��)
			// ��ʓI�Ȏ� (0.3f) ���[�V���O�J�[ (0.5f) ���x
			wheel.m_wheelsDampingCompression = 3.2f;

			wheel.m_frictionSlip = 3.0f;
			//if (index < 1)
			//{
			//	// �O���b�v�͂�ݒ� (0.0f �O���b�v�Ȃ� 1.0f �ʏ�̃O���b�v)
			//	// �h���t�g�J�[�̃Z�b�g�A�b�v�ł́A��ւ̃O���b�v��O�ւ��킸���ɒႭ�ݒ肷�邩�A�^�C���̖��Ղ��V�~�����[�g���邽�߂ɒႢ�l���g�p���Ă݂Ă��������B
			//	wheel.m_frictionSlip = btScalar(0.9f);
			//}
			//else
			//{
			//	// �O���b�v�͂�ݒ� (0.0f �O���b�v�Ȃ� 1.0f �ʏ�̃O���b�v)
			//	// �h���t�g�J�[�̃Z�b�g�A�b�v�ł́A��ւ̃O���b�v��O�ւ��킸���ɒႭ�ݒ肷�邩�A�^�C���̖��Ղ��V�~�����[�g���邽�߂ɒႢ�l���g�p���Ă݂Ă��������B
			//	wheel.m_frictionSlip = btScalar(1.0f);
			//}
			// �o�l���ɂ߂�Ƃ��ɂ����錸�������w�� 
			// DampingCompression �����������߂ɐݒ肷��Ɨǂ�
			wheel.m_wheelsDampingRelaxation = 3.6f;
			//Larger friction slips will result in better handling

			// �Ԃ̃��[���ɉe������l�ł��B�S�ւ�1.0�ɂ���Ɖ��]���₷���Ȃ�A0.0�ɂ���Ɖ��]���ɂ����Ȃ�܂�
			wheel.m_rollInfluence = 0.0f;
		}
	}

    void Vehicle::Reset()
    {

    }
}
