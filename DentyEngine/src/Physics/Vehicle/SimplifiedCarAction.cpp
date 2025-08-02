#include "stdafx.h"

#include "SimplifiedCarAction.h"

#include "SimplifiedCar.h"

#include "System/Core/Tag.h"

#include "System/Editor/EditorSceneManager.h"
#include "System/Scene/Scene.h"

namespace DentyEngine
{
	SimplifiedCarActionConstants::SimplifiedCarActionConstants()
	{
	}

	FindGroundHit::FindGroundHit() :
		object(), position(), fixedPosition(), normal(), onGround()
	{
	}

	void FindGroundHit::Reset()
	{
		object = nullptr;
		position = {};
		fixedPosition = {};
		normal = {};
		onGround = false;
	}

	void FindGroundHit::DebugInfo()
	{
		DENTY_DEBUG_CONSOLE_LOG("HitPosition: %f, %f, %f", position.x, position.y, position.z);
		DENTY_DEBUG_CONSOLE_LOG("HitNormal: %f, %f, %f", normal.x, normal.y, normal.z);
		DENTY_DEBUG_CONSOLE_LOG("HasHitObject: %s", object ? "true" : "false");
	}

	FindGrassHit::FindGrassHit() :
		object(), fixedPosition(), normal(), onGrass()
	{
	}

	void FindGrassHit::Reset()
	{
		object = nullptr;
		fixedPosition = Vector3::Zero();
		normal = Vector3::Zero();

		onGrass = false;
	}

	void FindGrassHit::DebugInfo()
	{
	}

	FindWallHit::FindWallHit() :
		object(), position(), fixedPosition(), normal(), hitWall()
	{
	}

	void FindWallHit::Reset()
	{
		object = nullptr;
		position = {};
		fixedPosition = {};
		normal = {};
		hitWall = false;
	}

	SimplifiedCarFindGround::SimplifiedCarFindGround(const Vector3& from, const Vector3& to) :
		AllHitsRayResultCallback(from.ToBtVector3(), to.ToBtVector3())
	{
	}

    btScalar SimplifiedCarFindGround::addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
    {
        return AllHitsRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }

    SimplifiedCarFindGrass::SimplifiedCarFindGrass(const Vector3& from, const Vector3& to) :
		AllHitsRayResultCallback(from.ToBtVector3(), to.ToBtVector3())
    {
    }

    btScalar SimplifiedCarFindGrass::addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
    {
	    return AllHitsRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }

    SimplifiedCarFindWall::SimplifiedCarFindWall(const btVector3& from, const btVector3& to) :
		AllHitsRayResultCallback(from, to)
	{

	}

	btScalar SimplifiedCarFindWall::addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
	{
		const uint32_t tagNumber = Tag::GetTagNumberFrom("InvisibleWall").value();
		if (rayResult.m_collisionObject->getUserIndex() != tagNumber)
		{
			// ignore this result.
			return 1.0f;
		}

		return AllHitsRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
	}

	SimplifiedCarAction::SimplifiedCarAction() :
		btActionInterface(), _groundHit(), _grassHit(),
		_wallHit(), _velocity(), _position(), _detectWallCastShape(), _car(), _body(),
		_collisionBody(), _driftDirection(DriftDirection::None), _onDriftInputLX(), _jump(), _isDrifting()
	{
	}

	void SimplifiedCarAction::Setup(btRigidBody* body, btRigidBody* collisionBody, SimplifiedCar* car, EntityID owner)
	{
		DENTY_CHECK_NULL(body);
		DENTY_CHECK_NULL(car);

		_body = body;
		_collisionBody = collisionBody;
		_car = car;
		_car->SetOwnerID(owner);

		const std::optional<Ref<GameObject>> spawnPoint = EditorSceneManager::GetActiveScene()->FindByName("SpawnPoint");
		DENTY_CHECK_CONDITION(spawnPoint.has_value());

		_position = spawnPoint.value()->transform.localPosition;

		_detectWallCastShape = std::make_shared<btSphereShape>(0.5f);
	}

	void SimplifiedCarAction::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
	{
		Input(deltaTimeStep);

		ApplyGravity();

		// Update transform.
		{
			_position += _velocity * deltaTimeStep;

			btTransform trans;
			_body->getMotionState()->getWorldTransform(trans);

			Matrix orientation = Matrix::ComputeOrientationFromFront(_car->GetFront().Normalize());
			orientation.SetOffsetVec3(_position);

			_body->setWorldTransform(orientation.ToBtTransformWithoutScaling());
			_body->getMotionState()->setWorldTransform(orientation.ToBtTransformWithoutScaling());

			_collisionBody->setWorldTransform(orientation.ToBtTransformWithoutScaling());
			_collisionBody->getMotionState()->setWorldTransform(orientation.ToBtTransformWithoutScaling());
		}

		// Check on ground.
		{
			// If hit set position of rigid body.
			if (CheckOnGround(collisionWorld))
			{
				_groundHit.fixedPosition = _groundHit.position;
				_position = _groundHit.fixedPosition;
			}
		}

		// Check on grass.
		{
			if (CheckOnGrass(collisionWorld))
			{
				_position = _grassHit.fixedPosition;
			}
		}

		// Check hit wall
		{
			if (CheckHitWall(collisionWorld))
			{
				_position = _wallHit.fixedPosition;
			}
		}

		{
			if (_jump)
			{
				if (_groundHit.onGround)
				{
					if (Input::IsPadPressed(PadKeyCode::RightShoulder))
					{
						if (_driftDirection == DriftDirection::None)
						{
							const float lx = Input::GetNormalizedThumbLX();
							if (lx < 0.0f)
							{
								_driftDirection = DriftDirection::Left;
								_onDriftInputLX = lx;
								_isDrifting = true;
							}
							else if (lx > 0.0f)
							{
								_driftDirection = DriftDirection::Right;
								_onDriftInputLX = lx;

								_isDrifting = true;
							}
						}
					}
					else
					{
						_jump = false;
					}
				}
			}
		}
	}

	void SimplifiedCarAction::debugDraw(btIDebugDraw* debugDrawer)
	{
		btTransform trans;
		_body->getMotionState()->getWorldTransform(trans);

		// OnGround ray 
		{
		    const Vector3 from = Vector3::ToVector3(trans.getOrigin()) + SimplifiedCarConstants::RIGID_BODY_POSITION;
		    const Vector3 to = from + Vector3::Down() * SimplifiedCarActionConstants::HIT_GROUND_RAY_LENGTH;

			debugDrawer->drawLine(from.ToBtVector3(), to.ToBtVector3(), btVector3(1, 0, 0));
		}

		// Hit wall rays
		{
			/*const Vector3 from = Vector3::ToVector3(trans.getOrigin()) + SimplifiedCarConstants::RIGID_BODY_POSITION;
			const Vector3 to = from + _car->GetFront().Normalize() * 2.0f;

			btTransform detectWallTrans;
			detectWallTrans.setIdentity();
			detectWallTrans.setOrigin(to.ToBtVector3());

			debugDrawer->drawSphere(0.5f, detectWallTrans, btVector3(0, 0, 1));*/
		}
	}

	void SimplifiedCarAction::Jump(const Vector3& velocity)
	{
		_velocity += velocity;

		SetJump(true);
	}

	void SimplifiedCarAction::ResetDrift()
	{
		_driftDirection = DriftDirection::None;
		_onDriftInputLX = 0.0f;
		_isDrifting = false;
	}

	void SimplifiedCarAction::SetVelocity(const Vector3& velocity)
	{
		_velocity = velocity;
	}

	void SimplifiedCarAction::SetPosition(const Vector3& position)
	{
		_position = position;
	}

	void SimplifiedCarAction::SetJump(bool jump)
	{
		_jump = jump;
	}

	void SimplifiedCarAction::SetDrifting(bool drifting)
	{
		_isDrifting = drifting;
	}

	const FindGroundHit& SimplifiedCarAction::GetFindGroundHit() const
	{
		return _groundHit;
	}

	const FindGrassHit& SimplifiedCarAction::GetFindGrassHit() const
	{
		return _grassHit;
	}

	const FindWallHit& SimplifiedCarAction::GetFindWallHit() const
	{
		return _wallHit;
	}

	const Vector3& SimplifiedCarAction::GetVelocity() const
	{
		return _velocity;
	}

	const Vector3& SimplifiedCarAction::GetPosition() const
	{
		return _position;
	}

	DriftDirection SimplifiedCarAction::GetDriftDirection() const
	{
		return _driftDirection;
	}

	float SimplifiedCarAction::GetOnDriftInputLX() const
	{
		return _onDriftInputLX;
	}

	bool SimplifiedCarAction::IsJump() const
	{
		return _jump;
	}

	bool SimplifiedCarAction::IsDrifting() const
	{
		return _isDrifting;
	}

	void SimplifiedCarAction::Input([[maybe_unused]] btScalar deltaTimeStep)
	{
		if (not _jump)
		{
			if (Input::IsPadPressed(PadKeyCode::RightShoulder))
			{
				Jump(Vector3(0.0f, 32.0f, 0.0f));
			}
		}
	}

	void SimplifiedCarAction::InputDrift(btScalar deltaTimeStep)
	{
		Vector3 front = _car->GetFront();

		float sign = (_driftDirection == DriftDirection::Left ? -1.0f : 1.0f);
		float rate = Input::GetNormalizedThumbLX() * sign;
		float driftSteeringAngle = MIN_DRIFT_STEERING_ANGLE;

		if (rate > 0.0f)
		{
			driftSteeringAngle += MAX_DRIFT_STEERING_ANGLE * rate;
		}

		// Rotate 
		{
			Matrix rotation = DirectX::XMMatrixRotationAxis(Vector3::Up().GetXM(), driftSteeringAngle * deltaTimeStep * sign);
			front = DirectX::XMVector3TransformNormal(front.GetXM(), rotation.GetXM());
			front = front.Normalize();

			_velocity += front;

			_car->SetFront(front);
		}
	}

	void SimplifiedCarAction::ApplyGravity()
	{
		_velocity += EditorSceneManager::GetActiveScene()->GetSceneSetting().physics.gravity;
	}

	void SimplifiedCarAction::Drift()
	{
	}

	bool SimplifiedCarAction::CheckOnGround(btCollisionWorld* world)
	{
		btTransform trans;
		_body->getMotionState()->getWorldTransform(trans);

		const Vector3 from = Vector3::ToVector3(trans.getOrigin()) + SimplifiedCarConstants::RIGID_BODY_POSITION;
		const Vector3 to = from + Vector3::Down() * SimplifiedCarActionConstants::HIT_GROUND_RAY_LENGTH;

		btTransform fromTrans;
		btTransform toTrans;

		fromTrans.setIdentity();
		fromTrans.setOrigin(from.ToBtVector3());

		toTrans.setIdentity();
		toTrans.setOrigin(to.ToBtVector3());

		SimplifiedCarFindGround findGround(from, to);
		world->rayTest(from.ToBtVector3(), to.ToBtVector3(),findGround);

		_groundHit.Reset();
		if (findGround.hasHit())
		{
			uint32_t tagNumber = Tag::GetTagNumberFrom("Stage").value();
			for (uint32_t index = 0u; index < findGround.m_collisionObjects.size(); ++index)
			{
				const btCollisionObject* collisionObject = findGround.m_collisionObjects.at(index);
			    if (collisionObject->getUserIndex() == tagNumber)
			    {
			        _groundHit.onGround = true;
					_groundHit.position = Vector3::ToVector3(findGround.m_hitPointWorld.at(index));
			    }
			}

			_groundHit.object = findGround.m_collisionObject;

			return true;
		}

		return false;
	}

	bool SimplifiedCarAction::CheckOnGrass(btCollisionWorld* world)
	{
		_grassHit.Reset();

		// If already hit ground return false.
		if (_groundHit.onGround)
		{
			return false;
		}

		btTransform trans;
		_body->getMotionState()->getWorldTransform(trans);

		const Vector3 from = Vector3::ToVector3(trans.getOrigin()) + SimplifiedCarConstants::RIGID_BODY_POSITION;
		const Vector3 to = from + Vector3::Down() * SimplifiedCarActionConstants::HIT_GROUND_RAY_LENGTH;

		btTransform fromTrans;
		btTransform toTrans;

		fromTrans.setIdentity();
		fromTrans.setOrigin(from.ToBtVector3());

		toTrans.setIdentity();
		toTrans.setOrigin(to.ToBtVector3());

		SimplifiedCarFindGrass findGrass(from, to);
		world->rayTest(from.ToBtVector3(), to.ToBtVector3(), findGrass);

		if (findGrass.hasHit())
		{
			uint32_t tagNumber = Tag::GetTagNumberFrom("Grass").value();
			for (uint32_t index = 0u; index < findGrass.m_collisionObjects.size(); ++index)
			{
				const btCollisionObject* collisionObject = findGrass.m_collisionObjects.at(index);
				if (collisionObject->getUserIndex() == tagNumber)
				{
					Vector3 hitPointWorld = Vector3::ToVector3(findGrass.m_hitPointWorld.at(index));
					hitPointWorld.y = 0.0f;

					_grassHit.fixedPosition = hitPointWorld;
					_grassHit.object = collisionObject;
					_grassHit.onGrass = true;

					return true;
				}
			}
		}

		return false;
	}

	bool SimplifiedCarAction::CheckHitWall(btCollisionWorld* world)
	{
		_wallHit.Reset();

		btTransform trans;
		_body->getMotionState()->getWorldTransform(trans);

		btVector3 origin = trans.getOrigin() + SimplifiedCarConstants::RIGID_BODY_POSITION.ToBtVector3();
		btVector3 front = _car->GetFront().ToBtVector3();
		front.normalize();

		btVector3 rayBegin = origin;
		btVector3 rayEnd = origin + front * 2.0f;

		SimplifiedCarFindWall findWall(rayBegin, rayEnd);
		world->rayTest(rayBegin, rayEnd, findWall);
		if (findWall.hasHit())
		{
			for (uint32_t index = 0u; index < findWall.m_collisionObjects.size(); ++index)
			{
				const btCollisionObject* collisionObject = findWall.m_collisionObjects.at(index);
				{
					Vector3 hitPosition = Vector3::ToVector3(findWall.m_hitPointWorld.at(index));
					hitPosition.y = 0.0f;

					_wallHit.object = collisionObject;
					_wallHit.fixedPosition = hitPosition;
					_wallHit.normal = Vector3::ToVector3(findWall.m_hitNormalWorld.at(index));
					_wallHit.hitWall = true;

					return true;
				}
			}
		}

		return false;
	}
}
