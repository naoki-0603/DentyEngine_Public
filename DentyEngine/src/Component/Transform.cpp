#include "stdafx.h"

#include "Transform.h"

#include "Physics/RigidBody.h"

#include "Scene/GameObject.h"

#include "System/Core/UndoRedo/ChangePositionCommand.h"
#include "System/Core/UndoRedo/ChangeRotationCommand.h"
#include "System/Core/UndoRedo/ChangeScaleCommand.h"
#include "System/Core/UndoRedo/CommandRecorder.h"

namespace DentyEngine
{
	Transform::Transform() :
		Component(), position(), localPosition(), scale(1, 1, 1), localScale(1, 1, 1),
		eulerAngles(), localEulerAngles(), rotation(), localRotation(), world(Matrix::Identity()), local(Matrix::Identity()),
		_oldPositionForCommand(), _oldScaleForCommand(), _oldEulerAnglesForCommand(), _oldEulerAngles(), _inputRotation(), _inputPosition(), _inputScale(), _disableParentYPosition(),
		_update(true)
	{
		_name = "Transform";
	}

	Transform::Transform(const Transform& source) :
		Component(source), position(source.position), localPosition(source.localPosition),
		scale(source.scale), localScale(source.localScale), eulerAngles(source.eulerAngles),
		localEulerAngles(source.localEulerAngles),
		rotation(source.rotation), localRotation(source.localRotation),
		world(source.world), local(source.local),
		_oldEulerAngles(source._oldEulerAngles),
		_inputRotation(source._inputRotation), _inputPosition(source._inputPosition), _inputScale(source._inputScale),
		_update(source._update)
	{
	}

	void Transform::Awake()
	{
	}

	void Transform::Start()
	{
	}

	void Transform::OnUpdate([[maybe_unused]] const float deltaTime)
	{
		if (IsParentExpired())
		{
			return;
		}

		if (not _update)
		{
			return;
		}

		// Undo/Redo
		{
			if (Input::IsMouseReleased(MouseButton::LeftButton) ||
				Input::IsKeyTriggered(KeyCode::Return))
			{
				if (_inputPosition)
				{
					const auto changePositionCommand = std::make_shared<ChangePositionCommand>(GetParent(), _oldPositionForCommand, localPosition);

					CommandRecorder::GetInstance().Add(changePositionCommand);

					_inputPosition = false;
				}
				else if (_inputRotation)
				{
					const auto changeRotationCommand = std::make_shared<ChangeRotationCommand>(GetParent(), _oldEulerAnglesForCommand, localEulerAngles);

					CommandRecorder::GetInstance().Add(changeRotationCommand);

					_inputRotation = false;
				}
				else if (_inputScale)
				{
					const auto changeScaleCommand = std::make_shared<ChangeScaleCommand>(GetParent(), _oldScaleForCommand, localScale);

					CommandRecorder::GetInstance().Add(changeScaleCommand);

					_inputScale = false;
				}
			}
		}

		{
			// If it has not been updated with gizmos and there have been changes
			if (IsRotationChanged())
			{
				const Vector3 deltaAngles = CalculateDeltaAngles();

				// Set rotation
				SetRotation(deltaAngles);
			}

			// Calculate orientation.
			{
				Matrix P = Matrix::Identity();

				// If has parent, calculate parent-child relation.
				if (GetParent()->HasParent())
				{
					P = CalculateParentRelation(GetParent()->GetParent());
				}

				const Matrix S = DirectX::XMMatrixScaling(localScale.x, localScale.y, localScale.z);
				const Matrix R = DirectX::XMMatrixRotationQuaternion(localRotation.GetXM());
				const Matrix T = DirectX::XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z);

				// Set world transform.
				local = (S * R * T);
				world = local * P;

				position = world.GetOffsetVec3();
				scale = world.ObtainScale();
				eulerAngles = rotation.ConvertToMatrix().ObtainEulerAngles();
			}
		}

		// Clamp angles.
		eulerAngles.ClampAngles();
		localEulerAngles.ClampAngles();

		_oldEulerAngles.ClampAngles();
		_oldEulerAngles = localEulerAngles;
	}

	void Transform::OnGui()
	{
		Component::OnGui();

		// Input position.
		{
			Vector3 tempLocalPosition = localPosition;
			if (ImGui::DragFloat3("Position", &tempLocalPosition.x, 0.01f))
			{
				if (not _inputPosition)
				{
					_oldPositionForCommand = localPosition;
				}

				_inputPosition = true;
			}
			localPosition = tempLocalPosition;
		}

		// Input rotation.
		{
			Vector3 tempLocalEulerAngles = localEulerAngles.ToDegree();

			localEulerAngles = localEulerAngles.ToDegree();
			
			if (ImGui::DragFloat3("Rotation", &tempLocalEulerAngles.x))
			{
				if (not _inputRotation)
				{
					_oldEulerAnglesForCommand = localEulerAngles.ToRadian();
				}

				_inputRotation = true;
			}
			localEulerAngles = tempLocalEulerAngles.ToRadian();
		}

		// Input scale.
		{
			Vector3 tempLocalScale = localScale;
			if (ImGui::DragFloat3("Scale", &tempLocalScale.x, 0.01f))
			{
				if (not _inputScale)
				{
					_oldScaleForCommand = localScale;
				}

				_inputScale = true;
			}
			localScale = tempLocalScale;
		}

		// Disable parent position each components.
		{
			ImGui::Checkbox("DisableParentYPosition##Transform", &_disableParentYPosition);
		}

		// Debug
		// ImGui::Separator();
		ImGui::Text("WorldOffset: %f, %f, %f", world.GetOffsetVec3().x, world.GetOffsetVec3().y, world.GetOffsetVec3().z);
		ImGui::Text("LocalOffset: %f, %f, %f", local.GetOffsetVec3().x, local.GetOffsetVec3().y, local.GetOffsetVec3().z);
		ImGui::Text("DeltaAngles: %f, %f, %f", CalculateDeltaAngles().x, CalculateDeltaAngles().y, CalculateDeltaAngles().z);
	}

	void Transform::OnPrefabGui()
	{
		ImGui::DragFloat3("Position", &localPosition.x, 0.01f);

		localEulerAngles = localEulerAngles.ToDegree();

		ImGui::DragFloat3("Rotation", &localEulerAngles.x);

		localEulerAngles = localEulerAngles.ToRadian();

		ImGui::DragFloat3("Scale", &localScale.x, 0.01f);
	}

	void Transform::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void Transform::OnSerialized() const
	{
	}

	void Transform::OnDeserialized()
	{
		_oldEulerAngles = localEulerAngles;
	}

	void Transform::OnGameObjectDeserialized()
	{
	}

	Ref<Object> Transform::MakeCopy() const
	{
		return std::make_shared<Transform>(*shared_from_this());
	}

	Vector3 Transform::ConvertToLocal(const Vector3& positionInWorld) const
	{
		const Matrix& inverseWorld = world.Inverse();

		return inverseWorld.TransformCoord(positionInWorld);
	}

	Vector3 Transform::ConvertToLocalRecursive(const Vector3& positionInWorld) const
	{
		Vector3 convertedPosition = positionInWorld;
		if (GetParent()->HasParent())
		{
			const Matrix& inverseWorld = GetParent()->transform.world.Inverse();
			convertedPosition = inverseWorld.TransformCoord(convertedPosition);
		}

		return convertedPosition;
	}

	void Transform::SetRotation(const Vector3& paramEulerAngles)
	{
		localRotation = localRotation.Multiply(
			localRotation.RotationRollPitchYaw(paramEulerAngles.x, paramEulerAngles.y, paramEulerAngles.z)
		);
	}

	Vector3 Transform::CalculateDeltaAngles() const
	{
		return localEulerAngles - _oldEulerAngles;
	}

	bool Transform::IsRotationChanged() const
	{
		const Vector3 deltaAngles = CalculateDeltaAngles();

		const float magnitude = (deltaAngles.x * deltaAngles.x + deltaAngles.y * deltaAngles.y + deltaAngles.z * deltaAngles.z);

		return (magnitude > 0.0f);
	}

	Matrix Transform::CalculateParentRelation(GameObject* parent)
	{
		Matrix P = Matrix::Identity();

		if (parent->HasParent())
		{
			P = CalculateParentRelation(parent->GetParent());
		}

		const Matrix S = DirectX::XMMatrixScaling(parent->transform.localScale.x, parent->transform.localScale.y, parent->transform.localScale.z);
		const Matrix R = DirectX::XMMatrixRotationQuaternion(parent->transform.localRotation.GetXM());
		const Matrix T = DirectX::XMMatrixTranslation(parent->transform.localPosition.x, _disableParentYPosition ? 0.0f : parent->transform.localPosition.y, parent->transform.localPosition.z);

		Matrix Result = (S * R * T) * P;

		return Result;
	}
};