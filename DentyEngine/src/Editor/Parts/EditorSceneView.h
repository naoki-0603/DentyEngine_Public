#pragma once

#include "EditorView.h"
#include "Editor/Gizmos/ManipulateGizmo.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

namespace DentyEngine
{
	class EventRequiredSceneViewportData;
	class EventSelectedGameObject;

	class SceneBuffer;

	class EditorCamera;

	class EditorSceneView final : public EditorView
	{
	public:
		enum class DragAndDropReceiveType
		{
			None = 0x00,
			Prefab = BIT(0),
			Mesh = BIT(1),

			Max = 2
		};
	public:
		EditorSceneView(ID3D11Device* device);
		~EditorSceneView() override;

		void OnUpdate(float deltaTime);
		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

		//
		// Event functions.
		//
		void OnSelectedGameObject(EventSelectedGameObject* e);

		//
		// Static functions.
		//
		static void RenderGizmos();

		//
		// Getter
		//
		[[nodiscard]]
		static const Scope<EditorCamera>& GetEditorCamera() { return _editorCamera; }

		[[nodiscard]]
		static const Vector2I& GetViewportSize() { return _viewportSize; }

		[[nodiscard]]
		static const Vector2I& GetViewportPosition() { return _viewportPosition; }
	private:
		void SetFocusPosition();

		// Mouse picking.
		void PickEntity(ID3D11Device* device, ID3D11DeviceContext* context, const SceneBuffer* sceneBuffer, Scene* activeScene);

		// Update preview game object.
		void UpdatePreviewGameObject(const Vector2& windowPosition, const Vector2& windowSize);

		// Receive drag and drop item functions.
		void ReceivePrefabItem(Scene* activeScene, const Vector2& windowPosition, const Vector2& windowSize);
		void ReceiveMeshItem(Scene* activeScene, const Vector2& windowPosition, const Vector2& windowSize);
		void ReceiveSkyboxItem(Scene* activeScene);

		//
		// Drag and drop callback functions.
		//
		void PrefabItemDragAndDropCallbackTargetBeforeDelivery(void* data, DragAndDropDataType dragAndDropDataType, const Ref<void>& userData = nullptr);
		void PrefabItemDragAndDropCallbackTarget(void* data, DragAndDropDataType dragAndDropDataType, const Ref<void>& userData = nullptr);
		void PrefabItemDragAndDropCallbackTargetInDestructor(void* data, DragAndDropDataType dragAndDropDataType, const Ref<void>& userData = nullptr);

		void MeshItemDragAndDropCallbackTargetBeforeDelivery(void* data, DragAndDropDataType dragAndDropDataType, const Ref<void>& userData = nullptr);
		void MeshItemDragAndDropCallbackTarget(void* data, DragAndDropDataType dragAndDropDataType, const Ref<void>& userData = nullptr);
		void MeshItemDragAndDropCallbackTargetInDestructor(void* data, DragAndDropDataType dragAndDropDataType, const Ref<void>& userData = nullptr);

		void SkyboxItemDragAndDropCallbackTarget(void* data, DragAndDropDataType dragAndDropDataType, const Ref<void>& userData = nullptr);

		//
		// Event function.
		//
		void OnSkyboxItemReceived(Ref<Skybox> skybox);
	private:
		// Manipulate gizmo.
		Scope<ManipulateGizmo> _manipulateGizmo;

		// Selected game object.
		GameObject* _selectedGameObject;

		// Preview game object.
		Ref<GameObject> _previewGameObject;
		std::vector<Ref<GameObject>> _previewChildren;

		// Imgui drag and drop type.
		DragAndDropReceiveType _dragAndDropReceiveType;

		// Is Window hovered
		bool _isHoveredWindow;

		// Clear when ongui function.
		bool _isDragAndDrop;
	private:
		// Editor camera.
		static Scope<EditorCamera> _editorCamera;

		static Vector2I _viewportSize;
		static Vector2I _viewportPosition;
	private:
		static constexpr float OFFSET_Y = WINDOW_PADDING_Y + 14.0f;
	};
};

DEFINE_ENUM_CLASS_BIT_OPERATORS(DentyEngine::EditorSceneView::DragAndDropReceiveType)