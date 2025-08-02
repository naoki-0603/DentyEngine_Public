#pragma once

#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>

#include "CollisionInfo.h"

#include "Component/Physics/DynamicCharacterStatus.h"

// Based on https://www.pierov.org/2020/05/23/dynamic-character-controller-bullet/

namespace DentyEngine
{
	namespace PhysicsAction
	{
		namespace Utils
		{
		    class CollisionHelper final
		    {
		    public:
				CollisionHelper() = default;
				~CollisionHelper() = default;

				static void SetCollisionInfo(const btCollisionObject* object, _Out_ CollisionObjectInfo& info);
		    };

			class BulletHelper final
			{
			public:
				BulletHelper() = default;
				~BulletHelper() = default;

				[[nodiscard]]
				static Matrix ConvertToMatrix(btRigidBody* body);

				[[nodiscard]]
				static Vector3 ExtractFront(btRigidBody* body);
			};
		}

		class DynamicCharacterController final : public btActionInterface
		{
		public:
			DynamicCharacterController();
			~DynamicCharacterController() override = default;

			void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep) override;
			void debugDraw(btIDebugDraw* debugDrawer) override;

			void Setup(btRigidBody* body, btConvexShape* convexShape, const Vector3& paramGravity);

			//
			// Setter
			//
			void SetGravity(const Vector3& gravity, GravityDirection direction);
			void SetMaxSteeringAngle(float maxSteeringAngle);
			void SetMaxSpeed(float maxSpeed);
			void SetMaxGrip(float maxGrip);
			void SetMaxBrake(float maxBrake);
			void SetMaxThrust(float maxThrust);
			void SetMass(float mass);

			//
			// Getter
			//
			[[nodiscard]]
			btRigidBody* GetBody() const { return _body; }

			[[nodiscard]]
			btConvexShape* GetConvexShape() const { return _shape; }

			[[nodiscard]]
			const Vector3& GetEulerAngles() const { return _status.eulerAngles; }

			[[nodiscard]]
			const std::string& GetKmString() const { return _status.km; }

			[[nodiscard]]
			const Vector3& GetGravity() const { return _gravity; }

			[[nodiscard]]
			float GetReverseFrameCounter() const { return _status.reverseFrameCounter; }

			[[nodiscard]]
			SteerDirection GetSteerDirection() const { return _status.steerDirection; }

			[[nodiscard]]
			bool IsReverse() const { return _status.reverse; }

			[[nodiscard]]
			bool OnGround() const { return _status.onGround; }
		private:
			void SetupBody();

			void UpdateVelocity(float deltaTime);

			// For vehicle.
			[[nodiscard]]
			float CalculateAngle() const;

			void Input();

		    void Handle();
			void Drift();
			void Accel(float factor);
			void Reverse();
			void Brake();

			void Steer();

			void LimitGravity();

			void LimitSpeed();

			void MakeToKm();

			void ClampTorqueVelocity();
		private:
			btRigidBody* _body;
			btConvexShape* _shape;

			Vector3 _gravity;

			DynamicCharacterStatus _status;

			float _maxSteeringAngle;
			float _maxThrust;
			float _maxBrake;
			float _maxSpeed;
			float _maxGrip;
			float _mass;
		};

		class FindGround final : public btCollisionWorld::ContactResultCallback
		{
		public:
			FindGround(const DynamicCharacterController* controller, const btCollisionWorld* world);
			~FindGround() override = default;

			btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,
				int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override;

		public:
			bool onGround;

		private:
			const DynamicCharacterController* _controller;
			const btCollisionWorld* _world;
		};
	}
}