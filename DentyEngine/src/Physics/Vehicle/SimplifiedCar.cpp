#include "stdafx.h"

#include "SimplifiedCar.h"

#include "SimplifiedCarAction.h"

namespace DentyEngine
{
	SimplifiedCarComponents::SimplifiedCarComponents() :
		simplifiedCarAction(), rigidBody(), collisionRigidBody(), compoundShape(),
		sphereShape(), collisionRigidBodyShape(), defaultMotionState(), collisionRigidBodyDefaultMotionState()
	{
	}

	void SimplifiedCarComponents::Reset()
	{
		simplifiedCarAction.reset();
		rigidBody.reset();
		collisionRigidBody.reset();
		collisionRigidBodyShape.reset();
		compoundShape.reset();
		sphereShape.reset();
		defaultMotionState.reset();
		collisionRigidBodyDefaultMotionState.reset();
	}

	SimplifiedCarConstants::SimplifiedCarConstants()
	{

	}

	SimplifiedCar::SimplifiedCar() :
		Object(), _components(), _collisionInfo(), _front(), _ownerID()
	{
		_ownerID = 0u;

		_name = "SimplifiedCar";
	}

	SimplifiedCar::SimplifiedCar(const SimplifiedCar& source) :
		Object(source), _front(source._front), _ownerID(source._ownerID)
	{
	}

	void SimplifiedCar::Create(EntityID owner, uint32_t tagNumber, const Matrix& world)
	{
		InternalCreate(owner, tagNumber, world);
	}

	void SimplifiedCar::Recreate(EntityID owner, uint32_t tagNumber, const Matrix& world)
	{
		InternalCreate(owner, tagNumber, world);
	}

	void SimplifiedCar::Update()
	{
		_collisionInfo.Update();
	}

	void SimplifiedCar::ResetCollisionInfo()
	{
		_collisionInfo.Reset();
	}

	void SimplifiedCar::SetFront(const Vector3& front)
	{
		_front = front;
	}

	void SimplifiedCar::SetOwnerID(EntityID owner)
	{
		_ownerID = owner;
	}

	const SimplifiedCarComponents& SimplifiedCar::GetComponents() const
	{
		return _components;
	}

	const Vector3& SimplifiedCar::GetFront() const
	{
		return _front;
	}

	EntityID SimplifiedCar::GetOwnerID() const
	{
		return _ownerID;
	}

	void SimplifiedCar::InternalCreate(EntityID owner, uint32_t tagNumber, const Matrix& world)
	{
		// Create action
		{
			_components.simplifiedCarAction = std::make_shared<SimplifiedCarAction>();
		}

		// Create collision info.
		{
			_collisionInfo.Create(owner);
		}

		// Create rigid body
		{
			_components.compoundShape = std::make_shared<btCompoundShape>();
			_components.sphereShape = std::make_shared<btSphereShape>(SimplifiedCarConstants::RADIUS);

			btTransform trans;
			trans.setIdentity();
			trans.setOrigin(SimplifiedCarConstants::RIGID_BODY_POSITION.ToBtVector3());
			_components.compoundShape->addChildShape(trans, _components.sphereShape.get());

			_components.defaultMotionState = std::make_shared<btDefaultMotionState>();

			btVector3 inertia;
			if constexpr (SimplifiedCarConstants::MASS != 0.0f)
			{
				_components.compoundShape->calculateLocalInertia(SimplifiedCarConstants::MASS, inertia);
			}

			btRigidBody::btRigidBodyConstructionInfo info =
			{
				SimplifiedCarConstants::MASS, _components.defaultMotionState.get(),
				_components.compoundShape.get(), inertia
			};
			_components.rigidBody = std::make_shared<btRigidBody>(info);
			_components.rigidBody->setAngularFactor(0.0f);
			_components.rigidBody->setActivationState(DISABLE_DEACTIVATION);
			_components.rigidBody->setUserIndex(tagNumber);
			_components.rigidBody->setCollisionFlags(_components.rigidBody->getCollisionFlags() | BT_DISABLE_WORLD_GRAVITY);
			//_components.rigidBody->setCollisionFlags(_components.rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		}

		// Create collision rigid body.
		{
			_components.collisionRigidBodyShape = std::make_shared<btSphereShape>(SimplifiedCarConstants::RADIUS);
			_components.collisionRigidBodyDefaultMotionState = std::make_shared<btDefaultMotionState>();

			btVector3 inertia;
			if constexpr (SimplifiedCarConstants::MASS != 0.0f)
			{
				_components.collisionRigidBodyShape->calculateLocalInertia(SimplifiedCarConstants::MASS, inertia);
			}

			btRigidBody::btRigidBodyConstructionInfo info =
			{
				SimplifiedCarConstants::MASS, _components.collisionRigidBodyDefaultMotionState.get(),
				_components.collisionRigidBodyShape.get(), inertia
			};
			_components.collisionRigidBody = std::make_shared<btRigidBody>(info);
			_components.collisionRigidBody->setAngularFactor(0.0f);
			_components.collisionRigidBody->setActivationState(DISABLE_DEACTIVATION);
			_components.collisionRigidBody->setUserIndex(tagNumber);
			_components.collisionRigidBody->setUserPointer(&_collisionInfo);
		}

		// Setup action
		{
			_components.simplifiedCarAction->Setup(_components.rigidBody.get(), _components.collisionRigidBody.get(), this, owner);
		}
	}
}
