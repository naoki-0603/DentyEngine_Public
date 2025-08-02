#pragma once

#include "BulletDebugDrawer.h"

#include "CollisionAction.h"

#include "GhostPairCallback.h"

#include "Raycast.h"

#include "System/Core/ID/ComponentID.h"

namespace DentyEngine
{
    class EventRuntimeSceneChanged;
    // Events
	class EventLayerInitialized;
	class EventEditorBeginRenderingProcess;
	class EventTellRenderBulletDebug;

	class EventOnColliderCreated;
	class EventOnRigidBodyCreated;

	class EventOnColliderDestroyed;
	class EventOnRigidBodyDestroyed;

	class EventOnActionCreated;
	class EventOnActionDestroyed;

	class EventOnSceneActivated;
	class EventOnSceneDeActivated;

	class EventOnScenePlayButtonPressed;

	class EventOnSettingsWindowClosed;

	class Collider;
	class RigidBody;

	class DynamicCharacterController;

	class VehicleComponent;

	class PhysicsEngine final
	{
	public:
		PhysicsEngine();
		~PhysicsEngine() = default;

		void Initialize();
		void OnUpdate();
		void Finalize();
		void StepSimulate(float deltaTime);

		void RenderDebugDrawer();

		void OnEvent(Event* e);
		void OnGui();

		void ReBuild();

		void VisibleDebugLines(bool visibleDebugLines);

		// Clear debug drawer data and holder objects.
		void ClearAll();

		void UpdateAABB(btCollisionObject* collisionObject);

		//
		// Add or remove from world functions.
		//
		void AddRigidBody(Ref<RigidBody> rigidBody);
		void RemoveRigidBody(const Ref<RigidBody>& rigidBody);

		void AddCollisionObject(Ref<Collider> collisionObject);
		void RemoveCollisionObject(const Ref<Collider>& collisionObject);

		void AddDynamicController(Ref<DynamicCharacterController> controller);
		void RemoveDynamicController(const Ref<DynamicCharacterController>& controller);

		void AddVehicle(Ref<VehicleComponent> vehicle);
		void RemoveVehicle(const Ref<VehicleComponent>& vehicle);

		//
		// Add reserved functions.
		//
		void AddReservedCollisionObject(Ref<Collider> collisionObject);
		void AddReservedRigidBody(Ref<RigidBody> rigidBody);
		void AddReservedDynamicController(Ref<DynamicCharacterController> controller);

		// Raycast

		[[nodiscard]]
		bool Raycast(const Ray& ray, const Color& color, _Out_ RaycastHit& raycastHit);

		[[nodiscard]]
	    bool RaycastAll(const Ray& ray, const Color& color, _Out_ RaycastHit raycastHits[RAYCAST_RESULT_MAX], _Out_ size_t& hitCount);

		// Getter
		[[nodiscard]]
		btDiscreteDynamicsWorld* GetWorld() const;

		[[nodiscard]]
		bool HasWorld() const { return (_world != nullptr); }

		//
		// Singleton.
		// 
		static Scope<PhysicsEngine>& GetInstance()
		{
			if (!_instance)
				_instance = std::make_unique<PhysicsEngine>();

			return _instance;
		}

		//
		// Events functions
		// 
	private:
		void OnLayerInitialized(EventLayerInitialized* e);
		void OnEditorBeginRenderingProcess(EventEditorBeginRenderingProcess* e);

		void OnSceneActivated(EventOnSceneActivated* e);
		void OnSceneDeActivated(EventOnSceneDeActivated* e);
		void OnScenePlayButtonPressed(EventOnScenePlayButtonPressed* e);

		void OnRuntimeSceneChanged(EventRuntimeSceneChanged* e);

		void BeforeReBuild();

	private:
		// Bullet physics member values.
		Scope<btDefaultCollisionConfiguration> _collisionConfiguration;
		Scope<btCollisionDispatcher> _dispatcher;
		Scope<btBroadphaseInterface> _overlappingPairCache;
		Scope<BulletDebugDrawer> _debugDrawer;
		Scope<GhostPairCallback> _ghostPairCallback;
		Scope<btSequentialImpulseConstraintSolver> _solver;
		Scope<btDiscreteDynamicsWorld> _world;
		Scope<CollisionAction> _collisionAction;

		// Holder
		std::unordered_map<ComponentID, Ref<Collider>> _colliders;
		std::unordered_map<ComponentID, Ref<RigidBody>> _rigidBodies;
		std::unordered_map<ComponentID, Ref<DynamicCharacterController>> _dynamicControllers;
		std::unordered_map<ComponentID, Ref<VehicleComponent>> _vehicles;

		// Reserved physical objects.
		// Add to world when world created.
		std::vector<Ref<Collider>> _reservedColliders;
		std::vector<Ref<RigidBody>> _reservedRigidBodies;
		std::vector<Ref<DynamicCharacterController>> _reservedDynamicControllers;
		std::vector<Ref<VehicleComponent>> _reservedVehicles;

		bool _visibleDebugLines;
	private:
		// Instance.
		static Scope<PhysicsEngine> _instance;
	};
}
