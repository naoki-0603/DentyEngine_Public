#pragma once

#include <ImGuizmo.h>

#include "IGizmo.h"

namespace DentyEngine
{
	//
	// Forward declarations
	//

	class EventOnSceneActivated;

	class EventSelectedGameObject;
	class EventOnGameObjectDestroyed;

	class GameObject;

	class SkinnedMeshRenderer;

	class ManipulateGizmo final : public IGizmo
	{
	public:
		ManipulateGizmo();
		~ManipulateGizmo() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ManipulateGizmo)

		void Initialize() override;
		void Finalize() override;
		void Update(const Matrix& view, const Matrix& projection);
		void Render(const Vector2& viewportPosition, const Vector2& viewportSize) ;
		void OnEvent(Event* e) override;
	private:
		void ShortCuts();
		void ManipulatePosition(const Vector3& translation);
		void ManipulateRotation(const Vector3& rotation);
		void ManipulateScale(const Vector3& scale);

		//
		// Event functions.
		// 
		void OnSceneActivated(EventOnSceneActivated* e);
		void OnGameObjectDestroyed(EventOnGameObjectDestroyed* e);
		void OnSelectedGameObject(EventSelectedGameObject* e);
	private:
		GameObject* _selectedObject;

		Matrix _view, _projection;

		ImGuizmo::OPERATION _operation;

		SkinnedMeshRenderer* _skinnedMeshRenderer;

		bool _subMesh;
	};

}
