#include "stdafx.h"

#include "DefaultCameraController.h"

#include "Component/Graphics/Renderer/GameCamera.h"
#include "Component/Physics/Vehicle/VehicleComponent.h"
#include "Physics/Vehicle/SimplifiedCarAction.h"
#include "Scene/GameObject.h"
#include "System/Editor/EditorSceneManager.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"
#include "System/Layer/ApplicationLayer.h"

#include "System/Core/Tag.h"

#include "System/Scene/Scene.h"

namespace DentyEngine
{
	DefaultCameraController::DefaultCameraController() :
		Behaviour(), _targetGameObject(), _gameCamera(), _vehicleComponent(),
		_targetID(), _offset(), _height(5.0f), _previousHeight(), _distance(10.0f)
	{
		_name = "DefaultCameraController";

		// Set id zero.
		_targetID = 0u;
	}

	DefaultCameraController::DefaultCameraController(const DefaultCameraController& source) :
		Behaviour(source), _targetGameObject(), _gameCamera(), _vehicleComponent(), _targetID(source._targetID),
		_offset(source._offset), _height(source._height), _distance(source._distance)
	{
	}

	void DefaultCameraController::Awake()
	{
		Behaviour::Awake();
	}

	void DefaultCameraController::Start()
	{
		Behaviour::Start();
	}

	void DefaultCameraController::OnUpdate([[maybe_unused]] float deltaTime)
	{
		if (ApplicationLayer::IsEdit())
		{
			UpdateCameraTransform();
		}
	}

	void DefaultCameraController::OnFixedUpdate()
	{
		Behaviour::OnFixedUpdate();

		UpdateCameraTransform();
	}

	void DefaultCameraController::OnGui()
	{
		Behaviour::OnGui();

		if (ImGui::CollapsingHeader("CameraStatus##DefaultCameraController", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderFloat3("Offset##CameraController", &_offset.x, 0.0f, 5.0f);
			ImGui::SliderFloat("Distance##CameraController", &_distance, 0.1f, 20.0f);
			ImGui::SliderFloat("Height##CameraController", &_height, 0.1f, 10.0f);
		}

		// Receive drag and drop.
		if (ImGui::CollapsingHeader("DragAndDrop##DefaultCameraController", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("DragAndDropHere!");

			ImVec2 size = ImGui::GetContentRegionAvail();
			size.y = 32.0f;
			ImGui::InvisibleButton("##DefaultCameraController", size);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjectList"))
				{
					IM_ASSERT(payload->DataSize == sizeof(EntityID));

					const EntityID sourceEntityID = *static_cast<const EntityID*>(payload->Data);

					std::optional<Ref<GameObject>> targetParent = GetParent()->scene->FindByEntityID(sourceEntityID);
					if (not targetParent.has_value())
					{
						DENTY_ASSERT(false, "Target not found!");
					}

					for (const auto& child : targetParent.value()->GetChildren())
					{
						// If Camera target
						if (child->tag() == Tag::TAGS.at(5))
						{
							_targetID = child->GetEntityID();
						}
					}

					RestoreTargetGameObject();
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void DefaultCameraController::OnPrefabGui()
	{
		Behaviour::OnPrefabGui();
	}

	void DefaultCameraController::OnEvent(Event* e)
	{
		Behaviour::OnEvent(e);
	}

	void DefaultCameraController::OnSerialized() const
	{
		Behaviour::OnSerialized();
	}

	void DefaultCameraController::OnDeserialized()
	{
		Behaviour::OnDeserialized();
	}

	void DefaultCameraController::OnDestroy()
	{
		Behaviour::OnDestroy();
	}

	void DefaultCameraController::OnGameObjectDeserialized()
	{
		Behaviour::OnGameObjectDeserialized();
	}

	void DefaultCameraController::OnSceneActivated()
	{
		RestoreTargetGameObject();
	}

	void DefaultCameraController::OnSceneDeActivated()
	{
		_targetGameObject = nullptr;
	}

	Ref<Object> DefaultCameraController::MakeCopy() const
	{
		return std::make_shared<DefaultCameraController>(*shared_from_this());
	}

	void DefaultCameraController::RestoreTargetGameObject()
	{
		if (_targetID != 0u)
		{
			if (EditorSceneManager::HasActiveScene())
			{
				std::optional<Ref<GameObject>> gameObject = GetParent()->scene->FindByEntityID(_targetID);
				if (gameObject.has_value())
				{
					_targetGameObject = gameObject.value();

					_gameCamera = GetParent()->GetComponentAsSharedPtr<GameCamera>();

					_vehicleComponent = _targetGameObject->GetParent()->GetComponentAsSharedPtr<VehicleComponent>();
				}
			}
		}
	}

	// https://qiita.com/TakayukiKiyohara/items/df6aa0501be03a4caee8
	void DefaultCameraController::UpdateCameraTransform()
	{
		if (not _targetGameObject)
		{
			DENTY_ERR_CONSOLE_LOG("TargetGameObject doesn't exists!");

			return;
		}

		if (not _vehicleComponent)
		{
			DENTY_ERR_CONSOLE_LOG("VehicleComponent doesn't exists!");

			return;
		}

		const Ref<SimplifiedCarAction> action = _vehicleComponent->GetCar().GetComponents().simplifiedCarAction;

		const Vector3 cameraPosition = GetParent()->transform.localPosition;

		Vector3 targetPosition = action->GetPosition();
		targetPosition.y = 0.0f;
		Vector3 toNewCameraPosition = Vector3::Zero();
		Vector3 toTargetPositionXZ = cameraPosition - _gameCamera->GetFocusPosition();

		if (action->IsJump())
		{
			//DENTY_DEBUG_CONSOLE_LOG("[DefaultCameraController::UpdateCameraTransform] CAR WAS JUMPED!");

			toTargetPositionXZ.y = 0.0f;

			const float toTargetPositionXZLength = toTargetPositionXZ.Length();
			toTargetPositionXZ = toTargetPositionXZ.Normalize();

			targetPosition.y += 3.85f;

			toNewCameraPosition = cameraPosition - targetPosition;
			toNewCameraPosition.y = 0.0f;
			toNewCameraPosition = toNewCameraPosition.Normalize();

			constexpr float weight = 0.7f;
			toNewCameraPosition = toNewCameraPosition * weight + toTargetPositionXZ * (1.0f - weight);
			toNewCameraPosition = toNewCameraPosition.Normalize();

			toNewCameraPosition *= toTargetPositionXZLength;
			toNewCameraPosition.y = _height;
		}
		else
		{
			const float height = toTargetPositionXZ.y;

			toTargetPositionXZ.y = 0.0f;

			const float toTargetPositionXZLength = toTargetPositionXZ.Length();
			toTargetPositionXZ = toTargetPositionXZ.Normalize();

			targetPosition.y += 3.85f;

			toNewCameraPosition = cameraPosition - targetPosition;
			toNewCameraPosition.y = 0.0f;
			toNewCameraPosition = toNewCameraPosition.Normalize();

			constexpr float weight = 0.7f;
			toNewCameraPosition = toNewCameraPosition * weight + toTargetPositionXZ * (1.0f - weight);
			toNewCameraPosition = toNewCameraPosition.Normalize();

			toNewCameraPosition *= toTargetPositionXZLength;
			toNewCameraPosition.y = height;

			_height = height;
		}

		Vector3 eye = targetPosition + toNewCameraPosition;

		_gameCamera->SetFocusPosition(targetPosition);

		GetParent()->transform.localPosition = eye;
	}
}
