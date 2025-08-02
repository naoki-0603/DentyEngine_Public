#include "stdafx.h"

#include "KinematicCharacterController.h"

#include "Physics/Shapes/BoxShape.h"
#include "Physics/Shapes/SphereShape.h"
#include "Physics/Shapes/CapsuleShape.h"
#include "Physics/Shapes/ConvexMeshShape.h"

#include "System/Physics/PhysicsEngine.h"

#include "Scene/GameObject.h"

#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>

namespace DentyEngine
{
	KinematicCharacterController::KinematicCharacterController() :
		PhysicsBehaviour(), _controller(), _pairCachingGhostObject(),
		_currentShape(), _boxShape(), _sphereShape(), _capsuleShape(), _convexMeshShape(),
		_currentShapeType(), _prevShapeType()
	{
		_name = "KinematicCharacterController";
	}

	KinematicCharacterController::KinematicCharacterController(const KinematicCharacterController& source) :
		PhysicsBehaviour(source), _currentShapeType(source._currentShapeType), _prevShapeType(source._prevShapeType)
	{
	}

	void KinematicCharacterController::Awake()
	{
		PhysicsBehaviour::Awake();

		CreateShapes();

		// Set default shape as box.
		_currentShape = _boxShape;

		Create();

		SetGravity(Vector3(0.0f, -9.8f, 0.0f));
	}

	void KinematicCharacterController::Start()
	{
		PhysicsBehaviour::Start();
	}

	void KinematicCharacterController::OnUpdate(float deltaTime)
	{
		PhysicsBehaviour::OnUpdate(deltaTime);


	}

	void KinematicCharacterController::OnFixedUpdate()
	{
		PhysicsBehaviour::OnFixedUpdate();
	}

	void KinematicCharacterController::OnGui()
	{
		PhysicsBehaviour::OnGui();

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
				ShapeChanged();
			}
		}
	}

	void KinematicCharacterController::OnPrefabGui()
	{
		PhysicsBehaviour::OnPrefabGui();
	}

	void KinematicCharacterController::OnEvent(Event* e)
	{
		PhysicsBehaviour::OnEvent(e);
	}

	void KinematicCharacterController::OnDestroy()
	{
		PhysicsBehaviour::OnDestroy();

		if (HasControllerAndGhostObject())
		{
		}
	}

	void KinematicCharacterController::Destroy()
	{
		PhysicsBehaviour::Destroy();
	}

	void KinematicCharacterController::OnSerialized() const
	{
		PhysicsBehaviour::OnSerialized();
	}

	void KinematicCharacterController::OnDeserialized()
	{
		PhysicsBehaviour::OnDeserialized();
	}

	void KinematicCharacterController::OnGameObjectDeserialized()
	{
		PhysicsBehaviour::OnGameObjectDeserialized();
	}

	void KinematicCharacterController::OnSceneActivated()
	{
	}

	void KinematicCharacterController::OnSceneDeActivated()
	{
		
	}

	void KinematicCharacterController::Restore()
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
		case ShapeType::ConvexMesh:
			_currentShape = _convexMeshShape;
			break;
		}

		Create();

		SetGravity(Vector3(0.0f, -9.8f, 0.0f));
	}

	Ref<Object> KinematicCharacterController::MakeCopy() const
	{
		return std::make_shared<KinematicCharacterController>(*shared_from_this());
	}

	void KinematicCharacterController::SetGravity(const Vector3& gravity)
	{
		_controller->setGravity(gravity.ToBtVector3());
	}

	void KinematicCharacterController::SetWalkDirection(const Vector3& direction)
	{
		_controller->setWalkDirection(direction.ToBtVector3());
	}

	void KinematicCharacterController::SetFallSpeed(float fallSpeed)
	{
		_controller->setFallSpeed(fallSpeed);
	}

	bool KinematicCharacterController::HasControllerAndGhostObject() const
	{
		return (_pairCachingGhostObject && _controller);
	}

	void KinematicCharacterController::CreateShapes()
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

	void KinematicCharacterController::Create()
	{
		_pairCachingGhostObject = std::make_shared<btPairCachingGhostObject>();
		_pairCachingGhostObject->setCollisionShape(_currentShape->GetBtCollisionShape().get());
		_pairCachingGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		_controller = std::make_shared<btKinematicCharacterController>(
			_pairCachingGhostObject.get(), std::static_pointer_cast<btConvexShape>(_currentShape->GetBtCollisionShape()).get(),
			0.35f, btVector3(0, 1, 0)
		);
	}

	void KinematicCharacterController::ShapeChanged()
	{
		OnShapeChanged();
	}

	void KinematicCharacterController::OnShapeChanged()
	{
		// Remove action from engine.
		{
			
		}

		// Reset values.
		{
			_controller.reset();
			_pairCachingGhostObject.reset();
			_currentShape.reset();
		}

		// Set new shape.
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
			case ShapeType::ConvexMesh:
				_currentShape = _convexMeshShape;
				break;
			}
		}

		// Create
		{
			Create();
		}

		// Set Gravity
		{
			SetGravity(Vector3(0, -9.8f, 0));
		}
	}
}
