#pragma once

#include "../IEditor.h"

#include "EditorWindow.h"
#include "Graphics/Color.h"

namespace DentyEngine
{
	class ApproximationCollisionMesh;
	class CollisionPreviewObject;
	class CollisionMesh;
	class StaticMesh;

	class EditorCamera;

	class SceneBuffer;

	class EditorCollisionMeshWindow final : public IEditor, public EditorWindow
	{
	public:
		enum class FilterType
		{
			None = 0,
			ColliderExists = 1,
			ColliderDoesNotExists,

			Max
		};

		struct Log
		{
			Log() : color(Color::WHITE), text() {}

			Color color;
			std::string text;
		};
	public:
		EditorCollisionMeshWindow();
		~EditorCollisionMeshWindow() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(EditorCollisionMeshWindow)

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;

		//
		// Event functions.
		//
		void OnOpened(Event* e) override;
		void OnClosed(Event* e) override;
	private:
		void ViewWindow();
		void HierarchyWindow();
		void InspectorWindow();
		void ConsoleWindow();

		void Copy();

		void Reset();

		[[nodiscard]]
		bool IsExportable(std::string& reason) const;
	private:
		Ref<ApproximationCollisionMesh> _approximationCollisionMesh;

		Ref<CollisionMesh> _collisionMesh;

		std::vector<Ref<CollisionPreviewObject>> _collisionPreviewObjects;

		// For visualize.
		Ref<StaticMesh> _staticMesh;

		// Log
		std::deque<Log> _logs;

		Scope<EditorCamera> _editorCamera;
		Scope<SceneBuffer> _sceneBuffer;

		// Size
		Vector2 _previousWindowSize;
		Vector2 _currentWindowSize;

		// Selected index.
		uint32_t _selectedIndex;

		// filter type.
		FilterType _prevFilterType;
		FilterType _filterType;

		// Hierarchy, inspector
		bool _hierarchyOpen;
		bool _inspectorOpen;

		bool _isViewHovered;
	private:
		static constexpr Vector2 COLLISION_EDITOR_DEFAULT_SIZE = { 1280.0f, 720.0f };
		static constexpr Vector2 COLLISION_EDITOR_VIEW_AREA_DEFAULT_SIZE = { 720.0f, 720.0f };
		static constexpr Vector2 COLLISION_EDITOR_INSPECTOR_AREA_DEFAULT_SIZE = { 1280.0f - 720.0f, 720.0f };

		static constexpr uint32_t MESH_COLLIDER_MAX_VERTICES_COUNT = 4096u;
		static constexpr uint32_t MAX_LOG_COUNT = 10u;

		static const Color OBJECT_NAME_COLOR_COLLIDER_EXISTS;
		static const Color OBJECT_NAME_COLOR_COLLIDER_DOES_NOT_EXISTS;
	};
}
