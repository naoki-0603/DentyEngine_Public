#pragma once

#include "Event.h"

#include "../Physics/PhysicsData.h"

namespace DentyEngine
{
	class Collider;
	class RigidBody;

	/*class EventOnColliderCreated final : public Event
	{
	public:
		EventOnColliderCreated(const Ref<Component>& collider, uint32_t collisionGroup,
			uint32_t collisionMask) : Event(false),
			_collider(collider), _collisionGroup(collisionGroup), _collisionMask(collisionMask) {}
		~EventOnColliderCreated() override = default;

		[[nodiscard]]
		const Ref<Component>& GetCollider() const { return _collider; }

		[[nodiscard]]
		uint32_t GetCollisionGroup() const { return _collisionGroup; }

		[[nodiscard]]
		uint32_t GetCollisionMask() const { return _collisionMask; }

		EVENT_CLASS_TYPE(OnColliderCreated)
	private:
	    Ref<Component> _collider;

		uint32_t _collisionGroup;
		uint32_t _collisionMask;
	};

	class EventOnRigidBodyCreated final : public Event
	{
	public:
		EventOnRigidBodyCreated(const Ref<Component>& rigidBody, uint32_t collisionGroup,
			uint32_t collisionMask) : Event(false),
			_rigidBody(rigidBody), _collisionGroup(collisionGroup), _collisionMask(collisionMask) {}
		~EventOnRigidBodyCreated() override = default;

		[[nodiscard]]
		const Ref<Component>& GetRigidBody() const { return _rigidBody; }

		[[nodiscard]]
		uint32_t GetCollisionGroup() const { return _collisionGroup; }

		[[nodiscard]]
		uint32_t GetCollisionMask() const { return _collisionMask; }

		EVENT_CLASS_TYPE(OnRigidBodyCreated)
	private:
		Ref<Component> _rigidBody;

		uint32_t _collisionGroup;
		uint32_t _collisionMask;
	};

	class EventOnColliderDestroyed final : public Event
	{
	public:
		EventOnColliderDestroyed(const Ref<Component>& collider) : Event(false), _collider(collider) {}
		~EventOnColliderDestroyed() override = default;

		const Ref<Component>& GetCollider() const { return _collider; }

		EVENT_CLASS_TYPE(OnColliderDestroyed)
	private:
		Ref<Component> _collider;
	};

	class EventOnRigidBodyDestroyed final : public Event
	{
	public:
		EventOnRigidBodyDestroyed(const Ref<Component>& rigidBody) : Event(false), _rigidBody(rigidBody) {}
		~EventOnRigidBodyDestroyed() override = default;

		const Ref<Component>& GetRigidBody() const { return _rigidBody; }

		EVENT_CLASS_TYPE(OnRigidBodyDestroyed)
	private:
		Ref<Component> _rigidBody;
	};

	class EventOnActionCreated final : public Event
	{
	public:
		EventOnActionCreated(btActionInterface* action) : Event(false), _action(action) {}
		~EventOnActionCreated() override = default;

		btActionInterface* GetAction() const { return _action; }

		EVENT_CLASS_TYPE(OnActionCreated)
	private:
		btActionInterface* _action;
	};

	class EventOnActionDestroyed final : public Event
	{
	public:
		EventOnActionDestroyed(btActionInterface* action) : Event(false), _action(action) {}
		~EventOnActionDestroyed() override = default;

		btActionInterface* GetAction() const { return _action; }

		EVENT_CLASS_TYPE(OnActionDestroyed)
	private:
		btActionInterface* _action;
	};*/

	// Collision and trigger event base class.
	class EventCollisionAndTrigger : public Event
	{
	public:
		EventCollisionAndTrigger(EntityID ownerID, const CollisionData& hitCollisionData, bool listenerMultipleExists) :
			Event(listenerMultipleExists), _ownerID(ownerID), _hitCollisionData(hitCollisionData) {}
		~EventCollisionAndTrigger() override = default;

		EventInheritedType GetInheritedType() const override { return EventInheritedType::EventCollisionAndTrigger;  }

		EntityID GetOwnerID() const { return _ownerID; }
		const CollisionData& GetCollisionData() const { return _hitCollisionData; }
	protected:
		EntityID _ownerID;
		CollisionData _hitCollisionData;
	};

	// Collision events.
	class EventOnCollisionEnter final : public EventCollisionAndTrigger
	{
	public:
		EventOnCollisionEnter(EntityID ownerID, const CollisionData& hitCollisionData) :
			EventCollisionAndTrigger(ownerID, hitCollisionData, true) {}
		~EventOnCollisionEnter() override = default;

		EVENT_CLASS_TYPE(OnCollisionEnter)
	};
	
	class EventOnCollisionStay final : public EventCollisionAndTrigger
	{
	public:
		EventOnCollisionStay(EntityID ownerID, const CollisionData& hitCollisionData) :
			EventCollisionAndTrigger(ownerID, hitCollisionData, true) {}
		~EventOnCollisionStay() override = default;

		EVENT_CLASS_TYPE(OnCollisionStay)
	};

	class EventOnCollisionExit final : public EventCollisionAndTrigger
	{
	public:
		EventOnCollisionExit(EntityID ownerID, const CollisionData& hitCollisionData) :
			EventCollisionAndTrigger(ownerID, hitCollisionData, true) {}
		~EventOnCollisionExit() override = default;

		EVENT_CLASS_TYPE(OnCollisionExit)
	};

	// Trigger events.
	class EventOnTriggerEnter final : public EventCollisionAndTrigger
	{
	public:
		EventOnTriggerEnter(EntityID ownerID, const CollisionData& hitCollisionData) :
			EventCollisionAndTrigger(ownerID, hitCollisionData, true) {}
		~EventOnTriggerEnter() override = default;

		EVENT_CLASS_TYPE(OnTriggerEnter)
	};

	class EventOnTriggerStay final : public EventCollisionAndTrigger
	{
	public:
		EventOnTriggerStay(EntityID ownerID, const CollisionData& hitCollisionData) :
			EventCollisionAndTrigger(ownerID, hitCollisionData, true) {}
		~EventOnTriggerStay() override = default;

		EVENT_CLASS_TYPE(OnTriggerStay)
	};

	class EventOnTriggerExit final : public EventCollisionAndTrigger
	{
	public:
		EventOnTriggerExit(EntityID ownerID, const CollisionData& hitCollisionData) :
			EventCollisionAndTrigger(ownerID, hitCollisionData, true) {}
		~EventOnTriggerExit() override = default;

		EVENT_CLASS_TYPE(OnTriggerExit)
	};
}