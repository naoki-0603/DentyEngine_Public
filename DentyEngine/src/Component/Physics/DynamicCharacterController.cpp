#include "stdafx.h"

#include "DynamicCharacterController.h"

#include "Physics/Shapes/BoxShape.h"
#include "Physics/Shapes/CapsuleShape.h"
#include "Physics/Shapes/ConvexMeshShape.h"
#include "Physics/Shapes/IShape.h"
#include "Physics/Shapes/SphereShape.h"

#include "Scene/GameObject.h"

#include "System/Core/Tag.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Layer/ApplicationLayer.h"

#include "System/Physics/DynamicCharacterControllerAction.h"
#include "System/Physics/PhysicsEngine.h"

namespace DentyEngine
{
	DynamicCharacterController::DynamicCharacterController() :
		PhysicsBehavior(), _rigidBody(), _motionState(), _action(), _currentShape(),
		_boxShape(), _sphereShape(), _capsuleShape(), 
		_collisionInfo(), _currentShapeType(), _prevShapeType(), _gravityDirection(GravityDirection::Down),
        _mass(1.0f), _maxSteeringAngle(120.0f), _maxThrust(15.0f), _maxBrake(25.0f), _maxSpeed(15.0f), _maxGrip(20.0f)
	{ 
		_name = "DynamicCharacterController";
	}

	DynamicCharacterController::DynamicCharacterController(const DynamicCharacterController& source) :
		PhysicsBehavior(source), _collisionInfo(), _currentShapeType(source._currentShapeType),
		_prevShapeType(source._prevShapeType), _gravity(source._gravity),
        _gravityDirection(source._gravityDirection), _mass(source._mass), _maxSteeringAngle(source._maxSteeringAngle),
        _maxThrust(source._maxThrust), _maxBrake(source._maxBrake), _maxSpeed(source._maxSpeed), _maxGrip(source._maxGrip)
	{
		_collisionInfo.Create(source._collisionInfo.GetOwner());
	}

	void DynamicCharacterController::Awake()
	{
		PhysicsBehavior::Awake();

		_collisionInfo.Create(GetParent()->GetEntityID());

		CreateShapes();

		_currentShape = _boxShape;

		Create();
	}

	void DynamicCharacterController::Start()
	{
		PhysicsBehavior::Start();
	}

	void DynamicCharacterController::OnUpdate(float deltaTime)
	{
		PhysicsBehavior::OnUpdate(deltaTime);

		if (HasRigidBodyAndAction())
		{
			if (ApplicationLayer::IsEdit())
			{
				Transform transform = GetParent()->transform;
				btTransform withoutScalingTransform = {};

				if (GetParent()->HasParent())
				{
					withoutScalingTransform = transform.world.ToBtTransformWithoutScaling();
				}
				else
				{
					withoutScalingTransform = transform.local.ToBtTransformWithoutScaling();
				}

				_rigidBody->getMotionState()->setWorldTransform(withoutScalingTransform);
				_rigidBody->setWorldTransform(withoutScalingTransform);
			}
			else
			{
				btTransform btTransform = {};
				_rigidBody->getMotionState()->getWorldTransform(btTransform);

				const Matrix transform = Matrix::ToMatrix(btTransform);

				if (GetParent()->HasParent())
				{
					GetParent()->transform.world = transform;
				}
				else
				{
					GetParent()->transform.local = transform;
				}
			}
		}

		_collisionInfo.Update();
	}

	void DynamicCharacterController::OnFixedUpdate()
	{
		PhysicsBehavior::OnFixedUpdate();
	}

	void DynamicCharacterController::OnGui()
	{
		PhysicsBehavior::OnGui();

		// Status
		{
			if (ImGui::CollapsingHeader("Status##DynamicCharacterController", ImGuiTreeNodeFlags_DefaultOpen))
			{
				const btRigidBody* body = _action->GetBody();
				const Vector3 linearVelocity = Vector3::ToVector3(body->getLinearVelocity());
				const Vector3 angularVelocity = Vector3::ToVector3(body->getAngularVelocity());
				const Vector3 eulerAngles = _action->GetEulerAngles();

				ImGui::Text("Gravity: %f, %f, %f", _gravity.x, _gravity.y, _gravity.z);
				ImGui::Text("GravityDirection: %s", std::string(NAMEOF_ENUM(_gravityDirection)).c_str());

				ImGui::Text("LinearVelocity: %f, %f, %f", linearVelocity.x, linearVelocity.y, linearVelocity.z);
				ImGui::Text("AngularVelocity: %f, %f, %f", angularVelocity.x, angularVelocity.y, angularVelocity.z);

				ImGui::Text("EulerAngles: %f, %f, %f", eulerAngles.x, eulerAngles.y, eulerAngles.z);

				ImGui::Text("SteerDirection: %s", std::string(NAMEOF_ENUM(_action->GetSteerDirection())).c_str());
				ImGui::Text("Km: %s", _action->GetKmString().c_str());

				ImGui::Text("ReverseFrameCounter: %f", _action->GetReverseFrameCounter());

				ImGui::Text("Reverse: %s", _action->IsReverse() ? "true" : "false");
				ImGui::Text("OnGround: %s", _action->OnGround() ? "true" : "false");
			}
		}

		// Shapes.
		{
			_prevShapeType = _currentShapeType;

			ImGui::Text("CurrentShape: %s", std::string(NAMEOF_ENUM(_currentShapeType)).c_str());

			if (ImGui::TreeNodeEx("Shapes##KinematicCharacterController", ImGuiTreeNodeFlags_Framed))
			{
				if (ImGui::Selectable("Box##Shapes"))
				{
					_currentShapeType = ShapeType::Box;
				}

				if (ImGui::Selectable("Sphere##Shapes"))
				{
					_currentShapeType = ShapeType::Sphere;
				}

				if (ImGui::Selectable("Capsule##Shapes"))
				{
					_currentShapeType = ShapeType::Capsule;
				}

				if (ImGui::Selectable("ConvexMesh##Shapes"))
				{
					_currentShapeType = ShapeType::ConvexMesh;
				}

				ImGui::TreePop();
			}

			if (_currentShape)
			{
				_currentShape->OnGui();
			}

			if (_prevShapeType != _currentShapeType)
			{
				ShapeTypeChanged();
			}
		}

		// Only convex mesh.
		{
			if (_currentShapeType == ShapeType::ConvexMesh)
			{
				if (ImGui::CollapsingHeader("CollisionMesh##DynamicCharacterController", ImGuiTreeNodeFlags_DefaultOpen))
				{
					const auto regionSize = ImGui::GetContentRegionAvail();

					ImGui::Text("DragAndDropHere!");
					ImGui::InvisibleButton("DragAndDropHere##DynamicCharacterController", ImVec2(regionSize.x, 32.0f));

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

	void DynamicCharacterController::OnPrefabGui()
	{
		PhysicsBehavior::OnPrefabGui();
	}

	void DynamicCharacterController::OnEvent(Event* e)
	{
		PhysicsBehavior::OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventGameObjectTagChanged>(BIND_EVENT_FUNCTION(OnGameObjectTagChanged));
	}

	void DynamicCharacterController::OnDestroy()
	{
		PhysicsBehavior::OnDestroy();

		if (HasRigidBodyAndAction())
		{
			PhysicsEngine::GetInstance()->RemoveDynamicController(shared_from_this());
		}
	}

	void DynamicCharacterController::Destroy()
	{
		PhysicsBehavior::Destroy();
	}

	void DynamicCharacterController::OnSerialized() const
	{
		PhysicsBehavior::OnSerialized();
	}

	void DynamicCharacterController::OnDeserialized()
	{
		PhysicsBehavior::OnDeserialized();
	}

	void DynamicCharacterController::OnGameObjectDeserialized()
	{
		PhysicsBehavior::OnGameObjectDeserialized();

		_collisionInfo.Create(GetParent()->GetEntityID());

		// Set user values.
		if (_rigidBody)
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

	Ref<Object> DynamicCharacterController::MakeCopy() const
	{
		return std::make_shared<DynamicCharacterController>(*shared_from_this());
	}

	void DynamicCharacterController::Restore()
	{
		CreateShapes();

		switch (_currentShapeType)
		{
		case ShapeType::Box:
			_currentShape = _boxShape;
			break;
		case ShapeType::Sphere:
			_currentShape = _sphereShape;
			break;
		case ShapeType::Capsule:
			_currentShape = _capsuleShape;
			break;
		}

		Create();

	}

	void DynamicCharacterController::OnGravityDirectionChanged([[maybe_unused]] GravityDirection direction)
	{

	}

	void DynamicCharacterController::SetGravity(const Vector3& gravity, GravityDirection direction)
	{
		_action->SetGravity(gravity, direction);
	}

    void DynamicCharacterController::SetMaxSteeringAngle(float maxSteeringAngle)
    {
		_maxSteeringAngle = maxSteeringAngle;

	    _action->SetMaxSteeringAngle(maxSteeringAngle);
    }

    void DynamicCharacterController::SetMaxSpeed(float maxSpeed)
    {
		_maxSpeed = maxSpeed;

		_action->SetMaxSpeed(maxSpeed);
    }

    void DynamicCharacterController::SetMaxGrip(float maxGrip)
    {
		_maxGrip = maxGrip;

		_action->SetMaxGrip(maxGrip);
    }

    void DynamicCharacterController::SetMaxBrake(float maxBrake)
    {
		_maxBrake = maxBrake;

		_action->SetMaxBrake(maxBrake);
    }

    void DynamicCharacterController::SetMaxThrust(float maxThrust)
    {
		_maxThrust = maxThrust;

		_action->SetMaxThrust(maxThrust);
    }

    Vector3 DynamicCharacterController::GetLinearVelocity() const
	{
		return Vector3::ToVector3(_rigidBody->getLinearVelocity());
	}

	Vector3 DynamicCharacterController::GetTorqueVelocity() const
	{
		return Vector3::ToVector3(_rigidBody->getAngularVelocity());
	}

	const Vector3& DynamicCharacterController::GetGravity() const
	{
		return _action->GetGravity();
	}

	bool DynamicCharacterController::OnGround() const
	{
		return _action->OnGround();
	}

	void DynamicCharacterController::Create()
	{
		// Create rigid body.
		{
			_motionState = std::make_shared<btDefaultMotionState>();

			btVector3 inertia = { 0, 0, 0 };
			{
				const bool isDynamic = (_mass != 0.0f);
				if (isDynamic)
				{
					_currentShape->GetBtCollisionShape()->calculateLocalInertia(_mass, inertia);
				}
			}

			btRigidBody::btRigidBodyConstructionInfo info =
			{
				_mass, _motionState.get(),
				_currentShape->GetBtCollisionShape().get(), inertia
			};

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

			// Set flags
			{
			    _rigidBody->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
			}
		}

		// Create action.
		{
			_action = std::make_shared<PhysicsAction::DynamicCharacterController>();
			_action->Setup(
				_rigidBody.get(),
				std::static_pointer_cast<btConvexShape>(_currentShape->GetBtCollisionShape()).get(),
				{ 0, -9.8f, 0 }
			);
		}

		{
			PhysicsEngine::GetInstance()->AddDynamicController(shared_from_this());
		}

		SetGravity({ 0, -9.8f, 0 }, GravityDirection::Down);
	}

	void DynamicCharacterController::CreateShapes()
	{
		// Create all support shape type
		{
			_boxShape = std::make_shared<BoxShape>();
			_sphereShape = std::make_unique<SphereShape>();
			_capsuleShape = std::make_shared<CapsuleShape>();

			// Call create functions.
			_boxShape->Create();
			_sphereShape->Create();
			_capsuleShape->Create();
		}
	}

	void DynamicCharacterController::ShapeTypeChanged()
	{
		// Remove action from physics engine.
		{
			PhysicsEngine::GetInstance()->RemoveDynamicController(shared_from_this());
		}

		// Reset
		{
			Reset();
		}

		// Shape
		{

			switch (_currentShapeType)
			{
			case ShapeType::Box:
				_currentShape = _boxShape;
				break;
			case ShapeType::Sphere:
				_currentShape = _sphereShape;
				break;
			case ShapeType::Capsule:
				_currentShape = _capsuleShape;
				break;
			}
		}

		// Create
		{
			Create();
		}
	}

	void DynamicCharacterController::Reset()
	{
		_currentShape.reset();
		_action.reset();
		_rigidBody.reset();
	}

    void DynamicCharacterController::OnGameObjectTagChanged(EventGameObjectTagChanged* e)
    {
		if (not _rigidBody)
			return;

		const std::string& tag = e->GetTag();

		const std::optional<uint32_t> number = Tag::GetTagNumberFrom(tag);
		if (number.has_value())
		{
		    _rigidBody->setUserIndex(static_cast<int32_t>(number.value()));
		}
		else
		{
		    _rigidBody->setUserIndex(-1);
		}
    }

    void DynamicCharacterController::CollisionMeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::CollisionMesh, "Data type must be collision mesh! (in DynamicCharacterController::CollisionMeshAssetDragAndDropTargetCallback)");

		const auto convexMeshShape = std::static_pointer_cast<ConvexMeshShape>(_currentShape);
		if (convexMeshShape->HasCreated())
		{
			PhysicsEngine::GetInstance()->RemoveDynamicController(shared_from_this());
		}

		convexMeshShape->Reset();
		convexMeshShape->OnMeshAssetReceived(data, dataType, userData);

		Create();

	}
}
