#include "stdafx.h"

#include "EditorCamera.h"

#include "Scene/GameObject.h"

#include "Graphics/Buffer/ConstantBuffer.h"

#include "System/Core/Timer.h"
#include "System/Core/Window.h"

#include "System/Events/EventRender.h"
#include "System/Events/EventEditor.h"

#include "Utilities/Easing.h"

namespace DentyEngine
{
	EditorCamera::EditorCamera(ID3D11Device* device, const Perspective& perspective) :
		Camera(device), _selectedObject(), _orientation(), _eulerAngles(), _velocity(), _position(), _distance(),
		_targetPosition(), _focusStartPosition(), _rotationSpeed(0.3f), _moveSpeed(5.0f), _boostSpeed(10.0f), _zoomSpeed(30.0f), _focusingTimer(),
		_isFocusing(), _isMoving()
	{
		{
			DirectX::XMStoreFloat3(
				&_position, DirectX::XMVectorScale(_orientation.GetFrontXM(), -perspective.distance)
			);
		}

		// Calculate view projection.
		{
			CalculateView(_orientation, _position, _focusPosition, Vector3::Up());
			CalculatePerspectiveProjection(perspective);

			_viewProjection = _view * _projection;

			_perspective = perspective;
		}

		_focusPosition = (_orientation.GetFrontVec3().Normalize() * -1.0f);
	}

	void EditorCamera::OnUpdate()
	{
		// Focusing
		{
			if (_isFocusing)
			{
				MoveToFocusPosition();
			}

			if (_isFocusing)
			{
				if (_focusingTimer >= 1.0f)
				{
					_isFocusing = false;
					_focusingTimer = 0.0f;
				}
				else
				{
					_focusingTimer += Timer::GetDeltaTime();
				}
			}
		}

		// Update position.
		{
			const Vector3 right = _orientation.GetRightVec3().Normalize();
			const Vector3 front = _orientation.GetFrontVec3().Normalize();

			_position += _velocity.x * right * Timer::GetDeltaTime();
			_position += _velocity.z * front * Timer::GetDeltaTime();

			_orientation.SetOffsetVec3(_position);
		}

		// Calculate view projection
		{
			_view = _orientation.Inverse();

			CalculatePerspectiveProjection(_perspective);

			_viewProjection = _view * _projection;
		}

		// Update constants.
		{
			_constants.view = _view;
			_constants.projection = _projection;
			_constants.viewProjection = _viewProjection;
			_constants.inverseProjection = _projection.Inverse();
			_constants.inverseView = _view.Inverse();
			_constants.cameraPosition = Vector4(_position, 1.0f);
		}
	}

	void EditorCamera::UpdateInput(const float deltaTime)
	{
		_isMoving = false;

		// Focusing.
		{
			if (Input::IsKeyTriggered(KeyCode::F) && !_isFocusing)
			{
				_isFocusing = true;

				_focusStartPosition = {};
			}
		}

		// Update free camera.
		if (!_isFocusing)
		{
			UpdateFreeCamera(deltaTime);
			UpdateMouseLooping();
		}
	}

	void EditorCamera::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventEditorBeginRenderingProcess>(BIND_EVENT_FUNCTION(OnBeginRendering));
		dispatcher.Dispatch<EventEditorEndRenderingProcess>(BIND_EVENT_FUNCTION(OnEndRendering));
		dispatcher.Dispatch<EventOnSceneViewSizeChanged>(BIND_EVENT_FUNCTION(OnViewSizeChanged));

		dispatcher.Dispatch<EventRequiredEditorCameraData>(BIND_EVENT_FUNCTION(OnRequiredEditorCameraData));

		dispatcher.Dispatch<EventSelectedGameObject>(BIND_EVENT_FUNCTION(OnSelectedGameObject));
	}

	void EditorCamera::OnGui()
	{
		static bool open = true;

		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		if (ImGui::Begin("EditorCamera", &open, windowFlags))
		{
			if (ImGui::CollapsingHeader("Status##EditorCamera"))
			{
				ImGui::SliderFloat("RotationSpeed##EditorCamera", &_rotationSpeed, 0.1f, 10.0f);
				ImGui::SliderFloat("BoostSpeed##EditorCamera", &_boostSpeed, 0.1f, 30.0f);
				ImGui::SliderFloat("ZoomSpeed##EditorCamera", &_zoomSpeed, 10.0f, 70.0f);
				ImGui::Text("Velocity: %f, %f, %f", _velocity.x, _velocity.y, _velocity.z);
				ImGui::Text("Position: %f, %f, %f", _position.x, _position.y, _position.z);

				ImGui::Text("IsMoving: %s", _isMoving ? "true" : "false");
			}

			if (ImGui::CollapsingHeader("Focusing##EditorCamera"))
			{
				ImGui::Text("TargetPosition: %f, %f, %f", _targetPosition.x, _targetPosition.y, _targetPosition.z);
			}

			ImGui::InputFloat3("FocusPosition", &_focusPosition.x, "%3.f", ImGuiInputTextFlags_ReadOnly);
			ImGui::Text("Distance: %f", _perspective.distance);
			ImGui::Text("FocusingTimer : %f", _focusingTimer);
			ImGui::Text("Focusing: %s", _isFocusing ? "true" : "false");
		}

		ImGui::End();
	}

	Matrix EditorCamera::ComputeBillBoardMatrix() const
	{
		Vector3 toCamera = _position - Vector3(0, 0, 0);
		toCamera = toCamera.Normalize();

		Matrix view = DirectX::XMMatrixLookAtLH(DirectX::FXMVECTOR(), toCamera.GetXM(), Vector3::Up().GetXM());
		Matrix inverse = view.Inverse();
		inverse._41 = 0.0f;
		inverse._42 = 0.0f;
		inverse._43 = 0.0f;

		return inverse;
	}

	//
	// Event functions.
	//

	// Called from Program::Run function.
	void EditorCamera::OnBeginRendering(EventEditorBeginRenderingProcess* e)
	{
		_constantBuffer->BindAll(e->_context, e->_bindSlot);
		_constantBuffer->UpdateSubresource(e->_context, &_constants);
	}

	void EditorCamera::OnEndRendering([[maybe_unused]] EventEditorEndRenderingProcess* e)
	{
	}

	void EditorCamera::OnViewSizeChanged(EventOnSceneViewSizeChanged* e)
	{
		// Call base class function.
		OnResize(e->GetSize());
	}

	void EditorCamera::OnRequiredEditorCameraData(EventRequiredEditorCameraData* e)
	{
		e->SetView(_view);
		e->SetProjection(_projection);
	}

	void EditorCamera::OnSelectedGameObject(EventSelectedGameObject* e)
	{
		_selectedObject = e->GetGameObject();
	}

	void EditorCamera::MoveToFocusPosition()
	{
		// Call first time.
		if (_focusingTimer <= 0.0f)
		{
			const Vector3 front = _orientation.GetFrontVec3().Normalize();

			if (_selectedObject)
			{
				const Vector3 position = _selectedObject->transform.position;

				_targetPosition = position - front;
			}
			else
			{
				_targetPosition = _focusPosition;
			}

			_focusStartPosition = _position;
		}
		else
		{
			const float rate = Easing::EaseOutCirc(_focusingTimer);

			_position = DirectX::XMVectorLerp(_focusStartPosition.GetXM(), _targetPosition.GetXM(), rate);
		}
	}

	void EditorCamera::UpdateFreeCamera(float deltaTime)
	{
		{
			if (Input::IsMousePressed(MouseButton::RightButton))
			{
				_isMoving = true;

				// Move
				{
					const bool isBoost = Input::IsKeyPressed(KeyCode::LShift);

					if (Input::IsKeyPressed(KeyCode::W))
					{
						_velocity.z += (_moveSpeed + (isBoost ? _boostSpeed : 0.0f)) * deltaTime;

					}
					else if (Input::IsKeyPressed(KeyCode::S))
					{
						_velocity.z -= (_moveSpeed + (isBoost ? _boostSpeed : 0.0f)) * deltaTime;
					}
					else
					{
						_velocity.z = 0.0f;
					}

					if (Input::IsKeyPressed(KeyCode::A))
					{
						_velocity.x -= (_moveSpeed + (isBoost ? _boostSpeed : 0.0f)) * deltaTime;
					}
					else if (Input::IsKeyPressed(KeyCode::D))
					{
						_velocity.x += (_moveSpeed + (isBoost ? _boostSpeed : 0.0f)) * deltaTime;
					}
					else
					{
						_velocity.x = 0.0f;
					}
				}

				// Rotation
				{
					// Calculate rotation.
					{
						const float panSpeedX = Input::GetMousePanX();
						const float panSpeedY = Input::GetMousePanY();

						_eulerAngles.x += (panSpeedY * _rotationSpeed) * deltaTime;
						_eulerAngles.y += (panSpeedX * _rotationSpeed) * deltaTime;
					}

					// Calculate orientation.
					{
						if (!Utils::ApproxEqual(_eulerAngles.x, 0.0f) ||
							!Utils::ApproxEqual(_eulerAngles.y, 0.0f))
						{
							const Matrix rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(_eulerAngles.x, _eulerAngles.y, 0.0f);
							_focusPosition = XMVector3TransformCoord(_focusPosition.GetXM(), rotationMatrix.GetXM());

							Vector3 defaultRight = { 1, 0, 0 };
							Vector3 defaultFront = { 0, 0, 1 };

							Vector3 cameraRight = { };
							Vector3 cameraFront = { };
							Vector3 cameraUp = { };

							defaultRight = XMVector3TransformCoord(defaultRight.GetXM(), rotationMatrix.GetXM());
							cameraRight = defaultRight.Normalize();

							defaultFront = XMVector3TransformCoord(defaultFront.GetXM(), rotationMatrix.GetXM());
							cameraFront = defaultFront.Normalize();

							cameraUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(cameraFront.GetXM(), cameraRight.GetXM()));

							_orientation.SetRightVec3(cameraRight);
							_orientation.SetUpVec3(cameraUp);
							_orientation.SetFrontVec3(cameraFront);
						}
					}
				}
			}
			// Reset velocity if not moving.
			else
			{
				_velocity = { };
			}
		}

		// Zoom
		{
			const float wheelSpeed = Input::GetMouseWheelSpeed();
			if (fabsf(wheelSpeed) > 0.0f)
			{
				_isMoving = true;
			}

			const Vector3 front = _orientation.GetFrontVec3().Normalize();

			_position += (front * (_zoomSpeed * wheelSpeed)) * deltaTime;
		}
	}

	void EditorCamera::UpdateMouseLooping()
	{
		const Vector2I mousePosition = Input::GetMousePosition();
		const Vector2I windowSize = Window::GetInfo().GetSize<Vector2I>();
		constexpr int32_t offset = 32;

		// Right
		if (mousePosition.x > windowSize.x - offset)
		{
			Input::SetMousePositionX(offset);
		}
		// Left
		else if (mousePosition.x < offset)
		{
			Input::SetMousePositionX(windowSize.x - offset);
		}

		// Down
		if (mousePosition.y > windowSize.y - offset)
		{
			Input::SetMousePositionY(offset + 100);
		}
		// Up
		else if (mousePosition.y < offset)
		{
			Input::SetMousePositionY(windowSize.y - offset);
		}
	}
};