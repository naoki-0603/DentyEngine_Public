#include "stdafx.h"

#include "ManipulateGizmo.h"

#include "Scene/GameObject.h"

#include "System/Events/EventEditor.h"

namespace DentyEngine
{
	ManipulateGizmo::ManipulateGizmo() : IGizmo(), _selectedObject(),
		_view(), _projection(), _operation(), _skinnedMeshRenderer(), _subMesh()
	{
	}

	void ManipulateGizmo::Initialize()
	{
		_operation = ImGuizmo::OPERATION::TRANSLATE;
	}

	void ManipulateGizmo::Finalize()
	{
	}

	void ManipulateGizmo::Update(const Matrix& view, const Matrix& projection)
	{
		_view = view;
		_projection = projection;

		ShortCuts();
	}

	void ManipulateGizmo::Render(const Vector2& viewportPosition, const Vector2& viewportSize)
	{
		if (_selectedObject && _operation != -1)
		{
			if (not ImGui::IsWindowHovered())
				return;

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(viewportPosition.x, viewportPosition.y, viewportSize.x, viewportSize.y);

			Transform& transform = _selectedObject->transform;

			// Convert to float pointer
			const std::array<const float, 16> projection =
			{
				_projection._11, _projection._12, _projection._13, _projection._14,
				_projection._21, _projection._22, _projection._23, _projection._23,
				_projection._31, _projection._32, _projection._33, _projection._34,
				_projection._41, _projection._42, _projection._43, _projection._44
			};

			const std::array<const float, 16> view =
			{
				_view._11, _view._12, _view._13, _view._14,
				_view._21, _view._22, _view._23, _view._24,
				_view._31, _view._32, _view._33, _view._34,
				_view._41, _view._42, _view._43, _view._44
			};

			Matrix tempTransform = transform.local;

			Vector3 translation = {};
			Vector3 rotation = {};
			Vector3 scale = {};

			if (_selectedObject->HasParent())
			{
				if (_subMesh)
				{
					const auto position = transform.position;

					ImGuizmo::RecomposeMatrixFromComponents(
						&position.x, &rotation.x, &transform.localScale.x, *tempTransform.m
					);
				}
				else
				{
					ImGuizmo::RecomposeMatrixFromComponents(
						&transform.position.x, &rotation.x, &transform.localScale.x, *tempTransform.m
					);
				}
			}
			else
			{
				ImGuizmo::RecomposeMatrixFromComponents(
					&transform.localPosition.x, &rotation.x, &transform.localScale.x, *tempTransform.m
				);
			}

			ImGuizmo::Manipulate(view.data(), projection.data(), _operation,
				ImGuizmo::LOCAL, *tempTransform.m
			);

			if (ImGuizmo::IsUsing())
			{
				ImGuizmo::DecomposeMatrixToComponents(
					*tempTransform.m, &translation.x, &rotation.x, &scale.x
				);

				switch (_operation)
				{
				case ImGuizmo::TRANSLATE:
					{
						if (_selectedObject->HasParent())
						{
							{
								translation = transform.ConvertToLocalRecursive(translation);
								translation = transform.local.TransformCoord(translation);
							}
						}

						ManipulatePosition(translation);
					}
					break;
				case ImGuizmo::ROTATE:
					ManipulateRotation(rotation);
					break;
				case ImGuizmo::SCALE:
					ManipulateScale(scale);
					break;
				default:
					break;
				}
			}
		}
	}

	void ManipulateGizmo::OnEvent(Event * e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<EventOnSceneActivated>(BIND_EVENT_FUNCTION(OnSceneActivated));
		dispatcher.Dispatch<EventOnGameObjectDestroyed>(BIND_EVENT_FUNCTION(OnGameObjectDestroyed));
		dispatcher.Dispatch<EventSelectedGameObject>(BIND_EVENT_FUNCTION(OnSelectedGameObject));
	}

	void ManipulateGizmo::ShortCuts()
	{
		// Translate
		if (Input::IsKeyTriggered(KeyCode::W))
		{
			_operation = ImGuizmo::TRANSLATE;
		}

		// Rotate
		if (Input::IsKeyTriggered(KeyCode::R))
		{
			_operation = ImGuizmo::ROTATE;
		}

		// Scale
		if (Input::IsKeyTriggered(KeyCode::S))
		{
			_operation = ImGuizmo::SCALE;
		}
	}

	void ManipulateGizmo::OnSceneActivated([[maybe_unused]] EventOnSceneActivated * e)
	{
		_selectedObject = nullptr;
	}

	void ManipulateGizmo::OnGameObjectDestroyed([[maybe_unused]] EventOnGameObjectDestroyed * e)
	{
		_selectedObject = nullptr;
	}

	void ManipulateGizmo::OnSelectedGameObject(EventSelectedGameObject * e)
	{
		_selectedObject = e->GetGameObject();
	}


	void ManipulateGizmo::ManipulatePosition(const Vector3& translation)
	{
		Transform& transform = _selectedObject->transform;

		if (not transform._inputPosition)
		{
			transform._oldPositionForCommand = transform.localPosition;

			transform._inputPosition = true;
		}

		transform.localPosition = translation;
	}

	void ManipulateGizmo::ManipulateRotation(const Vector3& rotation)
	{
		Transform& transform = _selectedObject->transform;

		if (not transform._inputRotation)
		{
			transform._oldEulerAnglesForCommand = transform.localEulerAngles;

			transform._inputRotation = true;
		}

		transform.localEulerAngles += rotation.ToRadian();
	}

	void ManipulateGizmo::ManipulateScale(const Vector3& scale)
	{
		Transform& transform = _selectedObject->transform;

		if (not transform._inputScale)
		{
			transform._oldScaleForCommand = transform.localScale;

			transform._inputScale = true;
		}

		transform.localScale = scale;
	}
};