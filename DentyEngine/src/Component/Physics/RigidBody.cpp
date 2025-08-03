#include "stdafx.h"

#include "RigidBody.h"

#include "Scene/GameObject.h"
#include "System/Core/Tag.h"

#include "Utilities/MathUtilities.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Events/EventPhysics.h"

#include "System/Layer/ApplicationLayer.h"

#include "System/Physics/PhysicsEngine.h"

namespace DentyEngine
{
	RigidBody::RigidBody() :
		PhysicsBehavior(), _rigidBody(), _motionState(), _collisionInfo(),
		_currentShape(), _boxShape(), _sphereShape(), _capsuleShape(), _convexMeshShape(),
		_shapeType(ShapeType::Box),
		_prevShapeType(ShapeType::Box), _collisionFilter(), _mass(1.0f), _previousMass(1.0f),
		_useGravity(true), _isKinematic()
	{
		_name = "RigidBody";
	}

	RigidBody::RigidBody(const RigidBody& source) :
		PhysicsBehavior(source), _shapeType(source._shapeType), _prevShapeType(source._prevShapeType),
		_collisionFilter(source._collisionFilter), _mass(source._mass), _previousMass(source._previousMass),
		_useGravity(source._useGravity), _isKinematic(source._isKinematic)
	{
		if (source._boxShape)
		{
			_boxShape = std::make_shared<BoxShape>(*source._boxShape);
		}

		if (source._sphereShape)
		{
			_sphereShape = std::make_shared<SphereShape>(*source._sphereShape);
		}

		if (source._capsuleShape)
		{
			_capsuleShape = std::make_shared<CapsuleShape>(*source._capsuleShape);
		}

		if (source._convexMeshShape)
		{
			_convexMeshShape = std::make_shared<ConvexMeshShape>(*source._convexMeshShape);
		}

		_collisionInfo.Create(source._collisionInfo.GetOwner());
	}

	//
	// Called from GameObject::AddComponent after set parent.
	//
	void RigidBody::Awake()
	{
		PhysicsBehavior::Awake();

		// Create all support shape type
		{
			_boxShape = std::make_shared<BoxShape>();
			_sphereShape = std::make_unique<SphereShape>();
			_capsuleShape = std::make_shared<CapsuleShape>();
			_convexMeshShape = std::make_shared<ConvexMeshShape>();

			// Call create functions.
			_boxShape->Create();
			_sphereShape->Create();
			_capsuleShape->Create();
		}

		_collisionInfo.Create(GetParent()->GetEntityID());
	}

	void RigidBody::Start()
	{
		PhysicsBehavior::Start();

		// First set box shape.
		{
			_currentShape = _boxShape;

			Generate();

			AddWorld();
		}
	}

	void RigidBody::OnUpdate(float deltaTime)
	{
		if (not ApplicationLayer::IsEdit())
			return;

		PhysicsBehavior::OnUpdate(deltaTime);

		// Detect change mass to zero.
		{
			if (MathUtils::ApproxEqual(_mass, 0.0f))
			{
				if (_previousMass != _mass)
				{
					OnMassChangeToZero();
				}
			}
		}

		// Detect change mass to more than zero.
		{
			if (MathUtils::ApproxEqual(_previousMass, 0.0f))
			{
				if (_previousMass != _mass)
				{
					OnMassChangeToMoreThanZero();
				}
			}
		}

		if (HasRigidBody())
		{
			const Transform& transform = GetParent()->transform;

			ApplyScaling();

			// Set transform.
			if (GetParent()->HasParent())
			{
				const btTransform btTransform = transform.world.ToBtTransformWithoutScaling();

				_rigidBody->setWorldTransform(btTransform);
				_rigidBody->getMotionState()->setWorldTransform(btTransform);
			}
			else
			{
				const btTransform btTransform = transform.local.ToBtTransformWithoutScaling();

				_rigidBody->setWorldTransform(btTransform);
				_rigidBody->getMotionState()->setWorldTransform(btTransform);
			}
		}
	}

	// Called from GameObject::OnFixedUpdate
	void RigidBody::OnFixedUpdate()
	{
		PhysicsBehavior::OnFixedUpdate();

		if (HasRigidBody())
		{
			if (_isKinematic)
			{
				// Set transform.
				if (GetParent()->HasParent())
				{
					btTransform btTransform = GetParent()->transform.world.ToBtTransformWithoutScaling();

					_rigidBody->setWorldTransform(btTransform);
					_rigidBody->getMotionState()->setWorldTransform(btTransform);
				}
				else
				{
					btTransform btTransform = GetParent()->transform.local.ToBtTransformWithoutScaling();

					_rigidBody->setWorldTransform(btTransform);
					_rigidBody->getMotionState()->setWorldTransform(btTransform);
				}
			}
			else
			{
				btTransform btTransform = {};
				_rigidBody->getMotionState()->getWorldTransform(btTransform);

				UpdateState(
					btTransform.getOrigin(), btTransform.getBasis()
				);
			}
		}

		_collisionInfo.Update();
	}

	void RigidBody::OnGui()
	{
		PhysicsBehavior::OnGui();

		if (ImGui::CollapsingHeader("States##RigidBody"))
		{
			_previousMass = _mass;
			ImGui::DragFloat("Mass##RigidBody", &_mass, 0.01f, 0.0f, 10000.0f);

			if (HasRigidBody())
			{
				if (ImGui::Checkbox("UseGravity##RigidBody", &_useGravity))
				{
					if (_useGravity)
					{
						_rigidBody->setFlags(_rigidBody->getFlags() & ~BT_DISABLE_WORLD_GRAVITY);
					}
					else
					{
						_rigidBody->setFlags(_rigidBody->getFlags() | BT_DISABLE_WORLD_GRAVITY);
					}
				}

				if (ImGui::Checkbox("IsKinematic##RigidBody", &_isKinematic))
				{
					if (_isKinematic)
					{
						_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
					}
					else
					{
						_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
					}
				}
			}
		}

		// Shapes.
		{
			_prevShapeType = _shapeType;

			ImGui::Text("CurrentShape: %s", std::string(NAMEOF_ENUM(_shapeType)).c_str());

			if (ImGui::TreeNodeEx("Shapes##RigidBody", ImGuiTreeNodeFlags_Framed))
			{
				if (ImGui::Selectable("Box##Shapes"))
				{
					_shapeType = ShapeType::Box;
				}

				if (ImGui::Selectable("Sphere##Shapes"))
				{
					_shapeType = ShapeType::Sphere;
				}

				if (ImGui::Selectable("Capsule##Shapes"))
				{
					_shapeType = ShapeType::Capsule;
				}

				if (ImGui::Selectable("ConvexMesh##Shapes"))
				{
					_shapeType = ShapeType::ConvexMesh;
				}

				ImGui::TreePop();
			}

			if (_currentShape)
			{
				_currentShape->OnGui();
			}

			if (_prevShapeType != _shapeType)
			{
				OnShapeTypeChanged();
			}
		}

		//// Collision filtering.
		//{
		//	ImGui::InputInt("CollisionFilter##Collider", &_collisionFilter);

		//	// Clamp
		//	{
		//		_collisionFilter = std::clamp(_collisionFilter, 0, PhysicsSettings::COLLISION_GROUP_AND_MASK_MAX - 1);
		//	}

		//	const auto physicsSettings = SettingsManager::GetInstance().GetPhysicsSettings();

		//	// Get collision filter.
		//	const auto& collisionFilter = physicsSettings->GetCollisionFilterAt(_collisionFilter);
		//	ImGui::Text(collisionFilter.name.c_str());
		//}

		// Only convex mesh.
		{
			if (_shapeType == ShapeType::ConvexMesh)
			{
				if (ImGui::CollapsingHeader("CollisionMesh##RigidBody", ImGuiTreeNodeFlags_DefaultOpen))
				{
					const auto regionSize = ImGui::GetContentRegionAvail();

					ImGui::Text("DragAndDropHere!");
					ImGui::InvisibleButton("DragAndDropHere##MeshCollider", ImVec2(regionSize.x, 32.0f));

					// Target
					{
						const DragAndDropSpecification specification =
						{
							"CollisionMeshItem", sizeof(Utils::DragAndDropCollisionMeshData),
							nullptr, DragAndDropDataType::CollisionMesh,
							DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(CollisionMeshAssetDragAndDropTargetCallback),
							DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
						};
						DragAndDropTarget dragAndDropTarget(specification);
					}
				}
			}
		}

		ImGui::Separator();

		_collisionInfo.OnGui();
	}

	void RigidBody::OnPrefabGui()
	{
		PhysicsBehavior::OnPrefabGui();

		ImGui::DragFloat("Mass##RigidBody", &_mass, 0.01f, 0.0f, 10000.0f);
	}

	void RigidBody::OnEvent(Event* e)
	{
		PhysicsBehavior::OnEvent(e);
	}

	void RigidBody::OnSerialized() const
	{
		PhysicsBehavior::OnSerialized();
	}

	void RigidBody::OnDeserialized()
	{
		PhysicsBehavior::OnDeserialized();
	}

	void RigidBody::OnDestroy()
	{
		PhysicsBehavior::OnDestroy();

		if (HasRigidBody())
		{
			// Remove rigid body from world.
			PhysicsEngine::GetInstance()->RemoveRigidBody(shared_from_this());
		}
	}

	void RigidBody::OnGameObjectDeserialized()
	{
		PhysicsBehavior::OnGameObjectDeserialized();

		_collisionInfo.Create(GetParent()->GetEntityID());
	}

	Ref<Object> RigidBody::MakeCopy() const
	{
		return std::make_shared<RigidBody>(*shared_from_this());
	}

	void RigidBody::OnSceneActivated()
	{
		if (GetShapeType() != ShapeType::None)
		{
			Restore();

			bool hasCollisionMesh = true;

			// Check is convex mesh shape.
			{
				if (GetShapeType() == ShapeType::ConvexMesh)
				{
					const bool hasCreated = std::static_pointer_cast<ConvexMeshShape>(GetCollisionShape())->HasCreated();

					if (not hasCreated)
						hasCollisionMesh = false;
				}
			}

			if (hasCollisionMesh)
			{
				ApplyScaling();

				PhysicsEngine::GetInstance()->AddRigidBody(shared_from_this());
			}
		}
	}

	void RigidBody::OnSceneDeActivated()
	{
		//PhysicsEngine::GetInstance()->RemoveRigidBody(shared_from_this());
	}

	void RigidBody::OnRuntimeSceneChanged()
	{
		Transform& transform = GetParent()->transform;
		transform.SetUpdateDisabled();

		if (GetParent()->HasParent())
		{
			const btTransform btTransform = transform.world.ToBtTransformWithoutScaling();

			_rigidBody->setWorldTransform(btTransform);
			_rigidBody->getMotionState()->setWorldTransform(btTransform);
		}
		else
		{
			const btTransform btTransform = transform.local.ToBtTransformWithoutScaling();

			_rigidBody->setWorldTransform(btTransform);
			_rigidBody->getMotionState()->setWorldTransform(btTransform);
		}

		_collisionInfo.Reset();
	}

	void RigidBody::OnGameObjectTagChanged()
	{
		if (not _rigidBody)
			return;

		// Set user values.
		{
			const std::optional<uint32_t> tagNumber = Tag::GetTagNumberFrom(GetParent()->tag());
			if (tagNumber.has_value())
			{
				_rigidBody->setUserIndex(static_cast<int32_t>(tagNumber.value()));
			}
			else
			{
				_rigidBody->setUserIndex(-1);
			}
		}
	}

	void RigidBody::OnScenePlayButtonPressed([[maybe_unused]] EventOnScenePlayButtonPressed* e)
	{
	}

	void RigidBody::OnApplicationStateChangedToPlay()
	{
		if (_rigidBody)
		{
			btTransform trans = _rigidBody->getWorldTransform();

			UpdateState(trans.getOrigin(), trans.getBasis());
		}
	}

	//
	// Update state
	//

	void RigidBody::UpdateState(const btVector3& btPosition, const btMatrix3x3& btMatrix3x3)
	{
		Transform& transform = GetParent()->transform;

		const Matrix basis = Matrix::ToMatrix(btMatrix3x3);
		const Vector3 position = Vector3::ToVector3(btPosition);
		const Matrix world = 
		{
			basis.GetRightVec3(),
			basis.GetUpVec3(),
			basis.GetFrontVec3(),
			position
		};

		if (GetParent()->HasParent())
		{
			const Matrix scaling = DirectX::XMMatrixScaling(
				transform.scale.x, transform.scale.y, transform.scale.z
			);

			transform.world = scaling * world;
		}
		else
		{
			const Matrix scaling = DirectX::XMMatrixScaling(
				transform.localScale.x, transform.localScale.y, transform.localScale.z
			);

			transform.local = scaling * world;
		}
	}

	void RigidBody::EnableAngularVelocity()
	{
		_rigidBody->setAngularFactor(1.0f);
	}

	void RigidBody::DisableAngularVelocity()
	{
		_rigidBody->setAngularFactor(0.0f);
	}

	void RigidBody::AddForce(const Vector3& force)
	{
		_rigidBody->applyCentralForce(force.ToBtVector3());
	}

	void RigidBody::AddImpulse(const Vector3& impulse)
	{
		_rigidBody->applyCentralImpulse(impulse.ToBtVector3());
	}

	void RigidBody::AddTorque(const Vector3& torque)
	{
		_rigidBody->applyTorque(torque.ToBtVector3());
	}

	void RigidBody::SetActivationState(int32_t state)
	{
		_rigidBody->setActivationState(state);
	}

	void RigidBody::SetCollisionFilter(int32_t collisionFilter)
	{
		//_collisionFilter = std::clamp(collisionFilter, 0, PhysicsSettings::COLLISION_GROUP_AND_MASK_MAX - 1);
	}

	void RigidBody::SetGravity(const Vector3& gravity)
	{
		_rigidBody->setGravity(gravity.ToBtVector3());
	}

	void RigidBody::SetLinearVelocity(const Vector3& velocity)
	{
		_rigidBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
	}

	void RigidBody::SetPosition(const Vector3& position)
	{
		btTransform transform = {};
		transform = _rigidBody->getWorldTransform();

		transform.setOrigin(position.ToBtVector3());

		_rigidBody->setWorldTransform(transform);
	}

	const Matrix& RigidBody::GetRigidBodyWorld() const
	{
		btTransform btTransform;
		_rigidBody->getMotionState()->getWorldTransform(btTransform);
		return Matrix::ToMatrix(btTransform);
	}

	//
	// Generate Rigid body
	//
	void RigidBody::Generate()
	{
		// Check is convex mesh.
		{
			if (_currentShape->GetShapeType() == std::string("ConvexMesh"))
			{
				const bool hasCreated = std::static_pointer_cast<ConvexMeshShape>(_currentShape)->HasCreated();

				if (not hasCreated)
					return;
			}
		}

		const Transform& transform = GetParent()->transform;

		btVector3 inertia = { 0, 0, 0 };
		{
			const bool isDynamic = (_mass != 0.0f);

			if (isDynamic)
			{
				_currentShape->GetBtCollisionShape()->calculateLocalInertia(_mass, inertia);
			}
		}

		// Create default motion state.
		{
			btTransform btTransform = transform.local.ToBtTransform();

			_motionState = std::make_shared<btDefaultMotionState>(btTransform);
		}

		// Create rigid body.
		{
			btRigidBody::btRigidBodyConstructionInfo info = { _mass, _motionState.get(), _currentShape->GetBtCollisionShape().get(), inertia };
			_rigidBody = std::make_shared<btRigidBody>(info);

			// Set user values.
		    {
			    const std::optional<uint32_t> tagNumber = Tag::GetTagNumberFrom(GetParent()->tag());
			    if (tagNumber.has_value())
			    {
		            _rigidBody->setUserIndex(static_cast<int32_t>(tagNumber.value()));
			    }
			    else
			    {
			        _rigidBody->setUserIndex(-1);
			    }
				_rigidBody->setUserPointer(&_collisionInfo);
		    }
		}
	}

	void RigidBody::Restore()
	{
		GenerateCollisionShape();

		Generate();

		// Restore flags.
		{
			if (_isKinematic)
			{
				_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			}

			if (not _useGravity)
			{
				_rigidBody->setFlags(_rigidBody->getFlags() | BT_DISABLE_WORLD_GRAVITY);
			}
		}
	}

	void RigidBody::ApplyScaling()
	{
		const Transform& transform = GetParent()->transform;

		const bool hasParent = GetParent()->HasParent();

		switch (_shapeType)
		{
		case ShapeType::Box:
			{
				if (_currentShape->GetShapeType() != std::string("Box"))
					DENTY_ASSERT(false, "Shape type must be box! (in RigidBody::ApplyScaling)");

				const Vector3 size = std::static_pointer_cast<BoxShape>(_currentShape)->GetSize();

				if (hasParent)
				{
					_boxShape->Update(transform.scale + size);
				}
				else
				{
					_boxShape->Update(transform.localScale + size);
				}
			}
			break;
		case ShapeType::Sphere:
			{
				if (_currentShape->GetShapeType() != std::string("Sphere"))
					DENTY_ASSERT(false, "Shape tpye must be sphere! (in RigidBody::ApplyScaling)");

				if (hasParent)
				{
					_currentShape->Update(transform.scale);
				}
				else
				{
					_currentShape->Update(transform.localScale);
				}
			}
			break;
		case ShapeType::Capsule:
			{
				if (_currentShape->GetShapeType() != std::string("Capsule"))
					DENTY_ASSERT(false, "Shape type must be capsule! (in RigidBody::ApplyScaling)");

				const float height = std::static_pointer_cast<CapsuleShape>(_currentShape)->GetHeight();
				if (hasParent)
				{
					Vector3 size = transform.scale;
					size.y += height;

					_currentShape->Update(size);
				}
				else
				{
					Vector3 size = transform.localScale;
					size.y += height;

					_currentShape->Update(size);
				}
			}
			break;
		case ShapeType::ConvexMesh:
			{
				if (_currentShape->GetShapeType() != std::string("ConvexMesh"))
					DENTY_ASSERT(false, "Shape type must be convex mesh! (in RigidBody::ApplyScaling)");

				const bool hasCreated = std::static_pointer_cast<ConvexMeshShape>(_currentShape)->HasCreated();

				if (hasCreated)
				{
					if (hasParent)
					{
						_currentShape->Update(transform.scale);
					}
					else
					{
						_currentShape->Update(transform.localScale);
					}
				}
			}
			break;
		}
	}

    void RigidBody::AddWorld()
    {
		// Check convex mesh shape.
		{
			if (_currentShape->GetShapeType() == std::string("ConvexMesh"))
			{
				const bool hasCreated = std::static_pointer_cast<ConvexMeshShape>(_currentShape)->HasCreated();

				if (not hasCreated)
					return;
			}
		}

		PhysicsEngine::GetInstance()->AddRigidBody(shared_from_this());
    }

	void RigidBody::Reset()
	{
		_motionState.reset();
		_rigidBody.reset();
	}

	void RigidBody::GenerateCollisionShape()
	{
		switch (_shapeType)
		{
		case ShapeType::Box:
			{
				if (not _boxShape)
				{
					_boxShape = std::make_shared<BoxShape>();
					_boxShape->Create();
				}

				_currentShape = _boxShape;
			}
			break;
		case ShapeType::Sphere:
			{
				if (not _sphereShape)
				{
					_sphereShape = std::make_shared<SphereShape>();
					_sphereShape->Create();
				}

				_currentShape = _sphereShape;
			}
			break;
		case ShapeType::Capsule:
			{
				if (not _capsuleShape)
				{
					_capsuleShape = std::make_shared<CapsuleShape>();
					_capsuleShape->Create();
				}

				_currentShape = _capsuleShape;
			}
			break;
		case ShapeType::ConvexMesh:
			{
				if (not _convexMeshShape)
					_convexMeshShape = std::make_shared<ConvexMeshShape>();

				_currentShape = _convexMeshShape;
			}
			break;
		default:
			DENTY_ASSERT(false, "Unknown or unsupported shape type! (in RigidBody::GenerateCollisionShape)");
			break;
		}
	}

	void RigidBody::OnShapeTypeChanged()
	{
		PhysicsEngine::GetInstance()->RemoveRigidBody(shared_from_this());

		Reset();

		GenerateCollisionShape();

		Generate();

		AddWorld();
	}

	void RigidBody::OnMassChangeToZero()
	{
		PhysicsEngine::GetInstance()->RemoveRigidBody(shared_from_this());

		Reset();

		GenerateCollisionShape();

		Generate();

		AddWorld();
	}

	void RigidBody::OnMassChangeToMoreThanZero()
	{
		PhysicsEngine::GetInstance()->RemoveRigidBody(shared_from_this());

		Reset();

		GenerateCollisionShape();

		Generate();

		AddWorld();
	}

	

    void RigidBody::CollisionMeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::CollisionMesh, "Data type must be collision mesh!");

		const auto convexMeshShape = std::static_pointer_cast<ConvexMeshShape>(_currentShape);
		if (convexMeshShape->HasCreated())
		{
			PhysicsEngine::GetInstance()->RemoveRigidBody(shared_from_this());
		}

		convexMeshShape->Reset();
		convexMeshShape->OnMeshAssetReceived(data, dataType, userData);

		Generate();

		AddWorld();
	}
}
