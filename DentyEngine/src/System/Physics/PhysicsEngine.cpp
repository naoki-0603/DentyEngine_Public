#include "stdafx.h"

#include "PhysicsEngine.h"

#include "Component/Physics/DynamicCharacterController.h"
#include "Component/Physics/RigidBody.h"
#include "Component/Physics/Colliders/Collider.h"
#include "Component/Physics/Colliders/ApproximationMeshCollider.h"
#include "Component/Physics/Vehicle/VehicleComponent.h"

#include "System/Physics/DynamicCharacterControllerAction.h"

#include "Editor/Graphics/Rendering/EditorCamera.h"
#include "Editor/Parts/EditorSceneView.h"
#include "Physics/Vehicle/SimplifiedCarAction.h"

#include "System/Core/Setting/GlobalSettingsManager.h"

#include "System/Events/EventRender.h"
#include "System/Events/EventScene.h"
#include "System/Events/EventEditor.h"
#include "System/Events/EventSystem.h"

#include "System/Layer/ApplicationLayer.h"

namespace DentyEngine
{
	Scope<PhysicsEngine> PhysicsEngine::_instance = nullptr;

	PhysicsEngine::PhysicsEngine() :
		_collisionConfiguration(), _dispatcher(), _overlappingPairCache(),
		_debugDrawer(), _solver(), _world(), _collisionAction(), _colliders(), _rigidBodies(), _dynamicControllers(),
		_vehicles(), _reservedDynamicControllers(), _reservedVehicles(),
		_visibleDebugLines(true)
	{
	}

	void PhysicsEngine::Initialize()
	{
		// Create world use default settings.
		{
			_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
			_dispatcher = std::make_unique<btCollisionDispatcher>(_collisionConfiguration.get());
			_overlappingPairCache = std::make_unique<btDbvtBroadphase>();
			_ghostPairCallback = std::make_unique<GhostPairCallback>();
			_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
			_world = std::make_unique<btDiscreteDynamicsWorld>(_dispatcher.get(), _overlappingPairCache.get(), _solver.get(), _collisionConfiguration.get());
		}
		
		DENTY_ASSERT(_world, "Failed create physics world. (in PhysicsEngine::Initialize)");

		// Create debug drawer.
		{
			_debugDrawer = std::make_unique<BulletDebugDrawer>();
			_debugDrawer->Initialize();
			_debugDrawer->setDebugMode(BulletDebugDrawer::DBG_DrawWireframe);
		}

		// Set debug drawer.
		_world->setDebugDrawer(_debugDrawer.get());

		// Set action.
		_collisionAction = std::make_unique<CollisionAction>();
		_world->addAction(_collisionAction.get());

		_world->getPairCache()->setInternalGhostPairCallback(_ghostPairCallback.get());

		_world->setGravity({ 0, -9.8f, 0 });
	}

	void PhysicsEngine::OnUpdate()
	{
		if (!_world)
			return;

		for (auto&& reservedCollisionObject : _reservedColliders)
		{
			AddCollisionObject(reservedCollisionObject);
		}
		_reservedColliders.clear();

		for (auto&& reservedRigidBody : _reservedRigidBodies)
		{
			AddRigidBody(reservedRigidBody);
		}
		_reservedRigidBodies.clear();

		for (auto&& reservedController : _reservedDynamicControllers)
		{
			AddDynamicController(reservedController);
		}
		_reservedDynamicControllers.clear();

		for (auto&& reservedVehicle : _reservedVehicles)
		{
			AddVehicle(reservedVehicle);
		}
		_reservedVehicles.clear();
	}

	void PhysicsEngine::Finalize()
	{
		_debugDrawer->Finalize();

		_world->setDebugDrawer(nullptr);

		ClearAll();

		_world.reset();
		_solver.reset();
		_overlappingPairCache.reset();
		_dispatcher.reset();
		_collisionConfiguration.reset();
	}

	void PhysicsEngine::StepSimulate(float deltaTime)
	{
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::StepSimulate)");

		_world->stepSimulation(deltaTime);
	}

	void PhysicsEngine::RenderDebugDrawer()
	{
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::OnColliderCreated)");

		if (_visibleDebugLines)
		{
			_world->debugDrawWorld();
		}
	}

	void PhysicsEngine::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventLayerInitialized>(BIND_EVENT_FUNCTION(OnLayerInitialized));
		dispatcher.Dispatch<EventEditorBeginRenderingProcess>(BIND_EVENT_FUNCTION(OnEditorBeginRenderingProcess));
		dispatcher.Dispatch<EventOnSceneActivated>(BIND_EVENT_FUNCTION(OnSceneActivated));
		dispatcher.Dispatch<EventOnSceneDeActivated>(BIND_EVENT_FUNCTION(OnSceneDeActivated));
		dispatcher.Dispatch<EventOnScenePlayButtonPressed>(BIND_EVENT_FUNCTION(OnScenePlayButtonPressed));
		dispatcher.Dispatch<EventRuntimeSceneChanged>(BIND_EVENT_FUNCTION(OnRuntimeSceneChanged));
	}

	void PhysicsEngine::OnGui()
	{
		if (ImGui::CollapsingHeader("PhysicsEngine##CoreLayer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("CollisionObjectCount : %d", _world->getNumCollisionObjects());
			ImGui::Text("ColliderCount: %u", _colliders.size());
			ImGui::Text("RigidBodyCount: %u", _rigidBodies.size());
			ImGui::Text("DynamicCharacterCount: %u", _dynamicControllers.size());
			ImGui::Text("VehicleCount: %u", _vehicles.size());
			//ImGui::Text("ActionCount: %d", _world->getNumActions());
		}
	}

	void PhysicsEngine::ReBuild()
	{
		// Get collision filter setting.
		const CollisionFilterSetting& filterSetting = GlobalSettingsManager::GetCollisionFilterSetting();

		// Remove all collision object and action.
		{
			for (int index = _world->getNumCollisionObjects() - 1; index >= 0; --index)
			{
				btCollisionObject* obj = _world->getCollisionObjectArray()[index];

				_world->removeCollisionObject(obj);
			}
		}

		// Then add all collision object.
		{
			// Colliders
			for (const auto& collider : _colliders | std::views::values)
			{
				// Collision filter.
				//filterSetting.GetCollisionFilterAt(collider->GetCollisionFilter());

				if (collider->GetCollisionShapeType() == Collider::Type::ApproximationMesh)
				{
					/*std::static_pointer_cast<ApproximationMeshCollider>(collider)->AddCollisionObjects(
						collisionFilter, _world.get()
					);*/

				    std::static_pointer_cast<ApproximationMeshCollider>(collider)->AddCollisionObjects(
						_world.get()
					);
				}
				else
				{
					/*_world->addCollisionObject(collider->GetGhostObject().get(),
						collisionFilter.collisionGroup, collisionFilter.collisionMask
					);*/
				    _world->addCollisionObject(collider->GetGhostObject().get());
				}
			}

			// RigidBodies
			for (const auto& rigidBody : _rigidBodies | std::views::values)
			{
				// Collision filter.
				//const auto& collisionFilter = physicsSettings->GetCollisionFilterAt(rigidBody->GetCollisionFilter());

				/*_world->addRigidBody(rigidBody->GetBtRigidBody().get(),
					collisionFilter.collisionGroup, collisionFilter.collisionMask
				);*/
			    _world->addRigidBody(rigidBody->GetBtRigidBody().get());
			}

			/* _world->addAction(_collisionAction.get()); */
		}

		{
			for (const auto& controller : _dynamicControllers | std::views::values)
			{
				_world->addRigidBody(controller->GetBody().get());
				_world->addAction(controller->GetAction().get());
			}
		}

		{
			for (const auto& vehicle : _vehicles | std::views::values)
			{
				const SimplifiedCarComponents& components = vehicle->GetCar().GetComponents();

				_world->addRigidBody(components.rigidBody.get());
				_world->addRigidBody(components.collisionRigidBody.get());
				_world->addAction(components.simplifiedCarAction.get());
			}
		}

		_world->setGravity({ 0, -9.8f, 0 });
	}

	void PhysicsEngine::VisibleDebugLines(bool visibleDebugLines)
	{
		_visibleDebugLines = visibleDebugLines;
	}

	void PhysicsEngine::ClearAll()
	{
		// Remove 
		{
			for (int index = _world->getNumCollisionObjects() - 1; index >= 0; --index)
			{
				btCollisionObject* obj = _world->getCollisionObjectArray()[index];

				_world->removeCollisionObject(obj);
			}
		}

		// Remove dynamic controller actions
		{
			for (const auto& controller : _dynamicControllers | std::views::values)
			{
				_world->removeAction(controller->GetAction().get());
			}
		}

		// Remove vehicles
		{
			for (const auto& vehicle : _vehicles | std::views::values)
			{
				_world->removeAction(vehicle->GetCar().GetComponents().simplifiedCarAction.get());
			}
		}

		// Remove collision action.
		{
			_world->removeAction(_collisionAction.get());
		}

		_world->addAction(_collisionAction.get());

		_debugDrawer->ClearLines();

		_rigidBodies.clear();
		_colliders.clear();
		_vehicles.clear();
		_dynamicControllers.clear();
		_reservedColliders.clear();
		_reservedRigidBodies.clear();
		_reservedVehicles.clear();
	}

	void PhysicsEngine::OnLayerInitialized([[maybe_unused]] EventLayerInitialized* e)
	{
		ClearAll();
	}

	void PhysicsEngine::OnEditorBeginRenderingProcess(EventEditorBeginRenderingProcess* e)
	{
		const Scope<EditorCamera>& editorCamera = EditorSceneView::GetEditorCamera();

		_debugDrawer->Update(e->GetContext(), editorCamera->GetViewProjection());
	}

    void PhysicsEngine::OnSceneActivated([[maybe_unused]] EventOnSceneActivated* e)
	{
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::OnSceneActivated)");
	}

	void PhysicsEngine::OnSceneDeActivated([[maybe_unused]] EventOnSceneDeActivated* e)
	{
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::OnSceneDeActivated)");

		ClearAll();
	}

	void PhysicsEngine::OnScenePlayButtonPressed([[maybe_unused]] EventOnScenePlayButtonPressed* e)
	{
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::OnScenePlayButtonPressed)");

		// Check scene state.
		if (ApplicationLayer::IsEdit())
		{
			return;
		}

		BeforeReBuild();

		// Rebuilding.
		ReBuild();
	}

    void PhysicsEngine::OnRuntimeSceneChanged([[maybe_unused]] EventRuntimeSceneChanged* e)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::OnRuntimeSceneChanged)");

		BeforeReBuild();

		ReBuild();
    }

    void PhysicsEngine::BeforeReBuild()
	{
		// Remove dynamic controller from physics world.
		for (auto&& controller : _dynamicControllers | std::views::values)
		{
			_world->removeAction(controller->GetAction().get());
		}

		for (auto&& vehicle : _vehicles | std::views::values)
		{
		    _world->removeAction(vehicle->GetCar().GetComponents().simplifiedCarAction.get());
		}
	}

	void PhysicsEngine::UpdateAABB(btCollisionObject* collisionObject)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::UpdateAABB)");

		_world->updateSingleAabb(collisionObject);
    }

	//
	// Functions that add or remove from physics world.
	//

    void PhysicsEngine::AddRigidBody(Ref<RigidBody> rigidBody)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::AddRigidBody)");

		// Get collision filter from physics settings.
		//const PhysicsSettings* physicsSettings = SettingsManager::GetInstance().GetPhysicsSettings();
		//const CollisionFilter collisionFilter = physicsSettings->GetCollisionFilterAt(rigidBody->GetCollisionFilter());

		//// Add rigid body.
		//_world->addRigidBody(rigidBody->GetBtRigidBody().get(), collisionFilter.collisionGroup, collisionFilter.collisionMask);

		_world->addRigidBody(rigidBody->GetBtRigidBody().get());

		// Add caches.
		_rigidBodies.emplace(rigidBody->GetComponentID(), rigidBody);
    }

    void PhysicsEngine::RemoveRigidBody(const Ref<RigidBody>& rigidBody)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::RemoveRigidBody)");

		// Remove rigid body.
		_world->removeRigidBody(rigidBody->GetBtRigidBody().get());

		// Remove from caches.
		_rigidBodies.erase(rigidBody->GetComponentID());
    }

    void PhysicsEngine::AddCollisionObject(Ref<Collider> collider)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::AddCollisionObject)");

		//// Get collision filter from physics settings.
		//const PhysicsSettings* physicsSettings = SettingsManager::GetInstance().GetPhysicsSettings();
		//const CollisionFilter& collisionFilter = physicsSettings->GetCollisionFilterAt(collider->GetCollisionFilter());

		if (collider->GetCollisionShapeType() == Collider::Type::ApproximationMesh)
		{
			const auto approximationMeshCollider = std::static_pointer_cast<ApproximationMeshCollider>(collider);
			//approximationMeshCollider->AddCollisionObjects(collisionFilter, _world.get());
			approximationMeshCollider->AddCollisionObjects(_world.get());
		}
		else
		{
			// Add to world.
			//_world->addCollisionObject(collider->GetGhostObject().get(), collisionFilter.collisionGroup, collisionFilter.collisionMask);
			_world->addCollisionObject(collider->GetGhostObject().get());
		}

		// Add to caches.
		_colliders.emplace(collider->GetComponentID(), collider);
    }

    void PhysicsEngine::RemoveCollisionObject(const Ref<Collider>& collider)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::RemoveCollisionObject)");

		if (collider->GetCollisionShapeType() == Collider::Type::ApproximationMesh)
		{
			const auto approximationMeshCollider = std::static_pointer_cast<ApproximationMeshCollider>(collider);
			approximationMeshCollider->RemoveCollisionObjects(_world.get());
		}
		else
		{
			// Remove from world.
			_world->removeCollisionObject(collider->GetGhostObject().get());
		}

		// Remove from caches.
		_colliders.erase(collider->GetComponentID());
    }

    void PhysicsEngine::AddDynamicController(Ref<DynamicCharacterController> controller)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::AddDynamicController)");

		_world->addRigidBody(controller->GetBody().get());
		_world->addAction(controller->GetAction().get());

		_dynamicControllers.try_emplace(controller->GetComponentID(), controller);
    }

    void PhysicsEngine::RemoveDynamicController(const Ref<DynamicCharacterController>& controller)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::RemoveDynamicController)");

		_world->removeRigidBody(controller->GetBody().get());
		_world->removeAction(controller->GetAction().get());

		_dynamicControllers.erase(controller->GetComponentID());
    }

    void PhysicsEngine::AddVehicle(Ref<VehicleComponent> vehicle)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::AddVehicle)");

		const SimplifiedCarComponents& components = vehicle->GetCar().GetComponents();

		_world->addRigidBody(components.rigidBody.get());
		_world->addRigidBody(components.collisionRigidBody.get());
		_world->addAction(components.simplifiedCarAction.get());

		_vehicles.try_emplace(vehicle->GetComponentID(), vehicle);
    }

    void PhysicsEngine::RemoveVehicle(const Ref<VehicleComponent>& vehicle)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::RemoveVehicle)");

		const SimplifiedCarComponents& components = vehicle->GetCar().GetComponents();

		_world->removeRigidBody(components.rigidBody.get());
		_world->removeRigidBody(components.collisionRigidBody.get());
		_world->removeAction(components.simplifiedCarAction.get());

		_vehicles.erase(vehicle->GetComponentID());
    }

    void PhysicsEngine::AddReservedCollisionObject(Ref<Collider> collisionObject)
    {
		_reservedColliders.emplace_back(collisionObject);
    }

    void PhysicsEngine::AddReservedRigidBody(Ref<RigidBody> rigidBody)
    {
		_reservedRigidBodies.emplace_back(rigidBody);
    }

    void PhysicsEngine::AddReservedDynamicController(Ref<DynamicCharacterController> controller)
    {
		_reservedDynamicControllers.emplace_back(controller);
    }

    bool PhysicsEngine::Raycast(const Ray& ray, [[maybe_unused]] const Color& color, _Out_ RaycastHit& raycastHit)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::Raycast)");

		const btVector3 rayBegin = ray.beginPoint.ToBtVector3();
		const btVector3 rayEnd = ray.endPoint.ToBtVector3();

		// Ray test
		{
			btCollisionWorld::ClosestRayResultCallback callback(rayBegin, rayEnd);
			callback.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

			_world->rayTest(rayBegin, rayEnd, callback);

			if (callback.hasHit())
			{
				// Check for self-collision.
				{
					const auto collisionObject = static_cast<const CollisionData*>(callback.m_collisionObject->getUserPointer());
					if (!collisionObject)
					{
						return false;
					}

					// If same return false.
					if (ray.ownerID == collisionObject->ownerID)
					{
						return false;
					}
				}

				btVector3 hitPosition = rayBegin.lerp(rayEnd, callback.m_closestHitFraction);
				raycastHit.hitPosition = Vector3::ToVector3(hitPosition);

				return true;
			}
		}

		return false;
    }

    bool PhysicsEngine::RaycastAll(const Ray& ray, [[maybe_unused]] const Color& color, RaycastHit raycastHits[RAYCAST_RESULT_MAX], [[maybe_unused]] size_t& hitCount)
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::RaycastAll)");

		const btVector3 rayBegin = ray.beginPoint.ToBtVector3();
		const btVector3 rayEnd = ray.endPoint.ToBtVector3();

		// Ray test
		{
			btCollisionWorld::AllHitsRayResultCallback callback(rayBegin, rayEnd);
			callback.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
			callback.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;

			_world->rayTest(rayBegin, rayEnd, callback);

			if (callback.hasHit())
			{
				for (int index = 0; index < callback.m_hitFractions.size(); ++index)
				{
					btVector3 hitPosition = rayBegin.lerp(rayEnd, callback.m_hitFractions[index]);
					raycastHits[index].hitPosition = Vector3::ToVector3(hitPosition);
				}

				return true;
			}
		}

		return false;
    }

    btDiscreteDynamicsWorld* PhysicsEngine::GetWorld() const
    {
		DENTY_ASSERT(_world, "Does not create physics world yet. (in PhysicsEngine::GetWorld)");

		return _world.get();
    }
}
