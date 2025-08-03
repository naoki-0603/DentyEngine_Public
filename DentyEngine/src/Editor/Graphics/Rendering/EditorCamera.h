#pragma once

#include "Graphics/Renderer/Camera.h"

namespace DentyEngine
{
	class EventEditorBeginRenderingProcess;
	class EventEditorEndRenderingProcess;

	class EventOnSceneViewSizeChanged;

	class EventRequiredEditorCameraData;

	class EventOnFocusKeyPressed;

	class EventSelectedGameObject;

	class EditorCamera final : public Camera
	{
	public:
		// For serialize.
		EditorCamera() : Camera() {}

		EditorCamera(ID3D11Device* device, const Perspective& perspective);
		~EditorCamera() override = default;

		void OnUpdate();
		void UpdateInput(const float deltaTime);

		void OnEvent(Event* e) override;
		void OnGui() override;

		[[nodiscard]]
		Matrix ComputeBillBoardMatrix() const;

		//
		// Event functions.
		//
		void OnBeginRendering(EventEditorBeginRenderingProcess* e);
		void OnEndRendering(EventEditorEndRenderingProcess* e);

		//
		// Setter
		//
		void SetFocusPosition(const Vector3& focusPosition) { _focusPosition = focusPosition; }

		//
		// Getter
		//
		[[nodiscard]]
		bool IsMoving() const { return _isMoving; }
	private:
		//
		// Event functions.
		//
		void OnViewSizeChanged(EventOnSceneViewSizeChanged* e);
		void OnRequiredEditorCameraData(EventRequiredEditorCameraData* e);
		void OnSelectedGameObject(EventSelectedGameObject* e);

		void MoveToFocusPosition();

		void UpdateFreeCamera(float deltaTime);
		void UpdateMouseLooping();
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<Camera>(this)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<Camera>(this)
			);
		}
	private:
		GameObject* _selectedObject;

		Matrix _orientation;

		// Unit is radian.
		Vector3 _eulerAngles;
		Vector3 _velocity;
		Vector3 _position;

		// Use for when focusing.
		Vector3 _distance;

		// Use for when focusing.
		Vector3 _targetPosition;

		// Use for when focusing.
		Vector3 _focusStartPosition;

		float _rotationSpeed;
		float _moveSpeed;
		float _boostSpeed;
		float _zoomSpeed;

		float _focusingTimer;
		bool _isFocusing;
		bool _isMoving;
	};
};

CEREAL_CLASS_VERSION(DentyEngine::EditorCamera, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::EditorCamera)