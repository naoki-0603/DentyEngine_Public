#include "stdafx.h"

#include "DynamicCharacterControllerAction.h"

#include "Utilities/MathUtilities.h"
#include "System/Core/Timer.h"

#include <bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>

namespace DentyEngine
{
    namespace PhysicsAction
    {
        namespace Utils
        {
            Matrix BulletHelper::ConvertToMatrix(btRigidBody* body)
            {
				DENTY_CHECK_NULL(body)

				const btTransform world = body->getWorldTransform();

				return Matrix::ToMatrix(world);
            }

            Vector3 BulletHelper::ExtractFront(btRigidBody* body)
            {
				DENTY_CHECK_NULL(body)

				const Matrix world = ConvertToMatrix(body);

				return world.GetFrontVec3().Normalize();
            }
        }
        

		DynamicCharacterController::DynamicCharacterController() :
			btActionInterface(), _body(), _shape(), _gravity(0.0f, -9.8f, 0.0f), 
	        _status(), _maxSteeringAngle(120.0f), _maxThrust(15.0f), _maxBrake(25.0f), _maxSpeed(15.0f), _maxGrip(20.0f), _mass(1.0f)
		{
		}

		void DynamicCharacterController::updateAction(btCollisionWorld* collisionWorld, [[maybe_unused]] btScalar deltaTimeStep)
		{
			//FindGroundAndSteps groundSteps(this, collisionWorld);
			FindGround ground(this, collisionWorld);
			collisionWorld->contactTest(_body, ground);

			// Store is on ground.
			_status.onGround = ground.onGround;

			Input();
		
			//UpdateVelocity(deltaTimeStep);

			LimitSpeed();

			MakeToKm();

			if (_status.onGround)
			{
				// Avoid going down on ramps.
				_body->setGravity({ 0, 0, 0 });
			}
			else
			{
				_body->setGravity(_gravity.ToBtVector3());
			}
		}

		void DynamicCharacterController::debugDraw([[maybe_unused]] btIDebugDraw* debugDrawer)
		{
		}

		void DynamicCharacterController::Setup(btRigidBody* body, btConvexShape* convexShape, const Vector3& paramGravity)
		{
			_body = body;
			_shape = convexShape;

			SetGravity(paramGravity, GravityDirection::Down);
			
			SetupBody();
		}

		void DynamicCharacterController::SetGravity(const Vector3& gravity, GravityDirection direction)
		{
			_gravity = gravity;
			_status.gravityDirection = direction;
			_body->setGravity(_gravity.ToBtVector3());
		}

		void DynamicCharacterController::SetMaxSteeringAngle(float maxSteeringAngle)
        {
			_maxSteeringAngle = maxSteeringAngle;
        }

        void DynamicCharacterController::SetMaxSpeed(float maxSpeed)
        {
			_maxSpeed = maxSpeed;
        }

        void DynamicCharacterController::SetMaxGrip(float maxGrip)
        {
			_maxGrip = maxGrip;
        }

        void DynamicCharacterController::SetMaxBrake(float maxBrake)
        {
			_maxBrake = maxBrake;
        }

        void DynamicCharacterController::SetMaxThrust(float maxThrust)
        {
			_maxThrust = maxThrust;
        }

        void DynamicCharacterController::SetMass(float mass)
        {
			_mass = mass;
        }

        void DynamicCharacterController::SetupBody()
		{
			DENTY_ASSERT(_body, "Body is null! (in DynamicCharacterController::SetupBody)");

			_body->setSleepingThresholds(0.0f, 0.0f);
			_body->setAngularFactor(0.0f);
			_body->getCollisionShape()->setMargin(0.5f);
			_body->setActivationState(DISABLE_DEACTIVATION);
		}

		void DynamicCharacterController::UpdateVelocity([[maybe_unused]] float deltaTime)
		{
		}

	    float DynamicCharacterController::CalculateAngle() const
        {
			float yaw = 0.0f;

            Vector3 f0 = Vector3::Right();
            Vector3 u = Vector3::Up();

			btTransform world;
			_body->getMotionState()->getWorldTransform(world);

			const Vector3 front = Matrix::ToMatrix(world).GetFrontVec3().Normalize();

			Vector3 r = f0.Cross(u);
			r = r.Normalize();
			
			yaw = DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(f0.GetXM(), front.GetXM()));

            const float sign = front.Dot(r);
            if (sign < 0.0f)
            {
                yaw *= -1.0f;
            }

            return yaw;
        }

        void DynamicCharacterController::Input()
        {
			{
			    Handle();
				Drift();
			}

			if (not _status.onGround)
				return;

			{
			    if (Input::IsPadPressed(PadKeyCode::X))
			    {
					if (_status.reverse)
						_status.ResetReverseState();

			        Accel(1.0f);
			    }

			    {
					Reverse();
			    }
			}

			{
				if (not _status.reverse)
				{
					if (Input::IsPadPressed(PadKeyCode::A))
					{
						Brake();
					}
				}
			}

			LimitGravity();
        }

        void DynamicCharacterController::Handle()
        {
			_status.steerDirection = SteerDirection::None;

			_status.handling = false;
			if (Input::GetNormalizedThumbLX() < 0.0f)
			{
				_status.steerDirection = SteerDirection::Left;
				_status.handling = true;
			}

			if (Input::GetNormalizedThumbLX() > 0.0f)
			{
				_status.steerDirection = SteerDirection::Right;
				_status.handling = true;
			}

			if (_status.handling)
			{
				Steer();

				const btVector3 angularVelocity = _body->getAngularVelocity();
				_body->setAngularVelocity(btVector3(0.0f, angularVelocity.getY(), 0.0f));
			}
			else
			{
				btVector3 zero;
				zero.setZero();

				_body->setAngularVelocity(zero);
			}
        }

        void DynamicCharacterController::Drift()
        {

        }

        void DynamicCharacterController::Accel(float factor)
        {
			const Vector3 front = Utils::BulletHelper::ExtractFront(_body);

			const auto force = Vector3(
				front.x * _maxThrust * factor * Timer::GetDeltaTime(),
				front.y * _maxThrust * factor * Timer::GetDeltaTime(),
				front.z * _maxThrust * factor * Timer::GetDeltaTime()
			);

			const Matrix rotation = DirectX::XMMatrixRotationRollPitchYaw(0.0f, MathUtils::ToRadian(90.0f), 0.0f);
			_body->applyCentralImpulse(rotation.TransformCoord(force).ToBtVector3());
        }

        void DynamicCharacterController::Reverse()
        {
			const Vector3 linearVelocity = Vector3::ToVector3(_body->getLinearVelocity());
			if (linearVelocity.IsAlmostZero() && (not _status.reverse))
			{
				if (Input::IsPadPressed(PadKeyCode::A))
				{
					_status.reverse = true;
				}
			}

			if (_status.reverse)
			{
				if (Input::IsPadPressed(PadKeyCode::A))
				{
					if (_status.reverseFrameCounter >= DynamicCharacterStatus::TO_CHANGE_REVERSE_FRAME_TIME)
					{
						Accel(-1.0f);
					}

					++_status.reverseFrameCounter;
				}

				if (Input::IsPadReleased(PadKeyCode::A))
				{
					_status.ResetReverseState();
				}
			}
        }

        void DynamicCharacterController::Brake()
        {
			const Vector3 linearVelocity = Vector3::ToVector3(_body->getLinearVelocity());

			const Vector3 B = (Vector3::Zero() - linearVelocity) / Timer::GetDeltaTime() * _body->getMass();

            const float brake = std::min(B.Length(), _maxBrake);

            if (not MathUtils::ApproxEqual(brake, 0.0f))
			{
				const Vector3 brakeDirection = B.Normalize();

				_body->applyCentralImpulse(Vector3(brakeDirection * brake * Timer::GetDeltaTime()).ToBtVector3());
            }
        }

        void DynamicCharacterController::Steer()
        {
			const float steeringAngle = MathUtils::ToRadian(_maxSteeringAngle) * Input::GetNormalizedThumbLX();
			const float deltaRadian = MathUtils::ToRadian(steeringAngle) * Timer::GetDeltaTime();

			_status.eulerAngles.y += deltaRadian;

			const Matrix rotation = DirectX::XMMatrixRotationRollPitchYaw(_status.eulerAngles.x, _status.eulerAngles.y, _status.eulerAngles.z);

			const Vector3 linearVelocity = Vector3::ToVector3(_body->getLinearVelocity());
			_body->setLinearVelocity(rotation.TransformCoord(linearVelocity).ToBtVector3());

			_status.eulerAngles.y = MathUtils::ClampAngle(_status.eulerAngles.y);
        }

        void DynamicCharacterController::LimitGravity()
        {
			const Vector3 linearVelocity = Vector3::ToVector3(_body->getLinearVelocity());

			switch (_status.gravityDirection)
			{
			case GravityDirection::Down:
			{
				if (linearVelocity.y < 0.0f)
				{
					Vector3 newLinearVelocity = linearVelocity;
					newLinearVelocity.y = 0.0f;

					_body->setLinearVelocity(newLinearVelocity.ToBtVector3());
				}
			}
			break;
			case GravityDirection::Up:
				break;
			case GravityDirection::Left:
				break;
			case GravityDirection::Right:
				break;
			}
        }

        void DynamicCharacterController::LimitSpeed()
        {
            const float speed = Vector3::ToVector3(_body->getLinearVelocity()).Length();

			if (speed > _maxSpeed)
			{
				const float rate = (_maxSpeed / speed);

				Vector3 currentVelocity = Vector3::ToVector3(_body->getLinearVelocity());
				currentVelocity *= rate;

				_body->setLinearVelocity(currentVelocity.ToBtVector3());
			}
        }

        void DynamicCharacterController::MakeToKm()
        {
			// Make look nice and clean.
			constexpr float constant = 3.6f;

			const Vector3 linearVelocity = Vector3::ToVector3(_body->getLinearVelocity());
			const float km = linearVelocity.Length() * constant;

			_status.km = std::to_string(km);
        }

        void DynamicCharacterController::ClampTorqueVelocity()
        {
        }

        FindGround::FindGround(const DynamicCharacterController* controller, const btCollisionWorld* world) :
            ContactResultCallback(), onGround(), _controller(controller), _world(world)
        {

        }

        btScalar FindGround::addSingleResult([[maybe_unused]] btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,
            [[maybe_unused]] int partId0, [[maybe_unused]] int index0, [[maybe_unused]] const btCollisionObjectWrapper* colObj1Wrap, [[maybe_unused]] int partId1, [[maybe_unused]] int index1)
        {
			onGround = false;

			if (colObj0Wrap->m_collisionObject == _controller->GetBody())
			{
				const btVector3 origin = colObj0Wrap->m_collisionObject->getWorldTransform().getOrigin();
				const btMatrix3x3 basis = colObj0Wrap->m_collisionObject->getWorldTransform().getBasis();

			    const Matrix orientation = Matrix::ToMatrix(basis);
				const Vector3 position = Vector3::ToVector3(origin);

			    Vector3 down = orientation.GetUpVec3() * -1.0f;
				down = down.Normalize();

				Vector3 rayFromWorld = position;
				Vector3 rayToWorld = position + down * 0.5f;

				// RayTest
				btCollisionWorld::ClosestRayResultCallback closestRayResultCallback(rayFromWorld.ToBtVector3(), rayToWorld.ToBtVector3());
				_world->rayTest(rayFromWorld.ToBtVector3(), rayToWorld.ToBtVector3(), closestRayResultCallback);

				// If hit, object should be on ground.
				if (closestRayResultCallback.hasHit())
				{
					//if (colObj0Wrap->m_collisionObject->getUserIndex() == Tag::GetTagNumberFrom("Stage"))
					{
				        onGround = true;
					}
				}
			}

			// Bullet ignore this value.
			return 0.0f;
        }
	}
}
