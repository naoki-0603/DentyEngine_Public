#pragma once

#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>

#include "System/Core/ID/EntityID.h"

namespace DentyEngine
{
	class SimplifiedCar;

	enum class DriftDirection : int8_t
	{
		None = -1,
		Left = 0,
		Right,

		Max
	};

	struct SimplifiedCarActionConstants
	{
	public:
		SimplifiedCarActionConstants();

	public:
		static constexpr float HIT_GROUND_RAY_LENGTH = 1.05f;
	};

	struct FindGroundHit
	{
	public:
		FindGroundHit();
		~FindGroundHit() = default;

		void Reset();

		void DebugInfo();
	public:
		const btCollisionObject* object;

		// World
		Vector3 position;

		// World
		Vector3 fixedPosition;

		// World
		Vector3 normal;

		bool onGround;
	};

	struct FindGrassHit
	{
	public:
		FindGrassHit();
		~FindGrassHit() = default;

		void Reset();

		void DebugInfo();
	public:
		const btCollisionObject* object;

		// World space
		Vector3 fixedPosition;

		// World space
		Vector3 normal;

		bool onGrass;
	};

	struct FindWallHit
	{
	public:
		FindWallHit();
		~FindWallHit() = default;

		void Reset();
	public:
		const btCollisionObject* object;

		// World
		Vector3 position;

		// World
		Vector3 fixedPosition;

		// World
		Vector3 normal;

		bool hitWall;
	};

	class SimplifiedCarFindGround final : public btCollisionWorld::AllHitsRayResultCallback
	{
	public:
		SimplifiedCarFindGround(const Vector3& from, const Vector3& to);
		~SimplifiedCarFindGround() override = default;

		btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override;
	};

	class SimplifiedCarFindGrass final : public btCollisionWorld::AllHitsRayResultCallback
	{
	public:
		SimplifiedCarFindGrass(const Vector3& from, const Vector3& to);
		~SimplifiedCarFindGrass() override = default;

		btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override;
	};

	class SimplifiedCarFindWall final : public btCollisionWorld::AllHitsRayResultCallback
	{
	public:
		SimplifiedCarFindWall(const btVector3& from, const btVector3& to);
		~SimplifiedCarFindWall() override = default;

		btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override;
	};

	class SimplifiedCarAction : public btActionInterface
	{
	public:
		SimplifiedCarAction();
		~SimplifiedCarAction() override = default;

		void Setup(btRigidBody* body, btRigidBody* collisionBody, SimplifiedCar* car, EntityID owner);

		void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep) override;
		void debugDraw(btIDebugDraw* debugDrawer) override;

		//
		// Callable from C# script.
		//
		void Jump(const Vector3& velocity);

		void ResetDrift();

		//
		// Setter
		//
		void SetVelocity(const Vector3& velocity);
		void SetPosition(const Vector3& position);
		void SetJump(bool jump);
		void SetDrifting(bool drifting);

		//
		// Getter
		//
		[[nodiscard]]
		const FindGroundHit& GetFindGroundHit() const;

		[[nodiscard]]
		const FindGrassHit& GetFindGrassHit() const;

		[[nodiscard]]
		const FindWallHit& GetFindWallHit() const;

		[[nodiscard]]
		const Vector3& GetVelocity() const;

		[[nodiscard]]
		const Vector3& GetPosition() const;

		[[nodiscard]]
		DriftDirection GetDriftDirection() const;

		[[nodiscard]]
		float GetOnDriftInputLX() const;

		[[nodiscard]]
		bool IsJump() const;

		[[nodiscard]]
		bool IsDrifting() const;
	private:
		void Input(btScalar deltaTimeStep);
		void InputDrift(btScalar deltaTimeStep);

		void ApplyGravity();

		void Drift();


		[[nodiscard]]
		bool CheckOnGround(btCollisionWorld* world);

		[[nodiscard]]
		bool CheckOnGrass(btCollisionWorld* world);

		[[nodiscard]]
		bool CheckHitWall(btCollisionWorld* world);

	private:
		// Store closest hit object.
		FindGroundHit _groundHit;
		FindGrassHit _grassHit;
		FindWallHit _wallHit;

		Vector3 _velocity;
		Vector3 _position;

		Ref<btSphereShape> _detectWallCastShape;

		SimplifiedCar* _car;
		btRigidBody* _body;
		btRigidBody* _collisionBody;

		DriftDirection _driftDirection;
		float _onDriftInputLX;

		bool _jump;
		bool _isDrifting;
	private:
		static constexpr float MIN_DRIFT_STEERING_ANGLE = 0.16f;
		static constexpr float MAX_DRIFT_STEERING_ANGLE = 1.0f;
	};
}
