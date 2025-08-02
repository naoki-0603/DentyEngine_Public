#pragma once

#include "System/Core/Object.h"
#include "System/Core/ID/EntityID.h"
#include "System/Physics/CollisionInfo.h"

namespace DentyEngine
{
	class SimplifiedCarAction;

	struct SimplifiedCarComponents
	{
	public:
		SimplifiedCarComponents();
		~SimplifiedCarComponents() = default;

		void Reset();
	public:
		Ref<SimplifiedCarAction> simplifiedCarAction;

		Ref<btRigidBody> rigidBody;
		Ref<btRigidBody> collisionRigidBody;
		Ref<btCompoundShape> compoundShape;
		Ref<btSphereShape> sphereShape;
		Ref<btSphereShape> collisionRigidBodyShape;
		Ref<btDefaultMotionState> defaultMotionState;
		Ref<btDefaultMotionState> collisionRigidBodyDefaultMotionState;
	};

	struct SimplifiedCarConstants
	{
	public:
		SimplifiedCarConstants();
		~SimplifiedCarConstants() = default;

	public:
		static constexpr Vector3 HALF_EXTENTS = Vector3(0.95f, 0.5f, 2.0f);
		static constexpr float RADIUS = 1.0f;
		static constexpr Vector3 RIGID_BODY_POSITION = Vector3(0.0f, 1.0f, 0.0f);
		static constexpr float MASS = 1.0f;
	};

	class SimplifiedCar : public Object
	{
	public:
		SimplifiedCar();
		SimplifiedCar(const SimplifiedCar& source);
		~SimplifiedCar() override = default;

		virtual void Create(EntityID owner, uint32_t tagNumber, const Matrix& world);
		virtual void Recreate(EntityID owner, uint32_t tagNumber, const Matrix& world);

		virtual void Update();

		void ResetCollisionInfo();

		//
		// Setter
		//
		void SetFront(const Vector3& front);
		void SetOwnerID(EntityID owner);

		//
		// Getter
		//
		[[nodiscard]]
		const SimplifiedCarComponents& GetComponents() const;

		[[nodiscard]]
		const Vector3& GetFront() const;

		[[nodiscard]]
		EntityID GetOwnerID() const;
	protected:
		virtual void InternalCreate(EntityID owner, uint32_t tagNumber, const Matrix& world);

	private:
		SimplifiedCarComponents _components;
		CollisionInfo _collisionInfo;

		Vector3 _front;

		EntityID _ownerID;
	};
}
