#pragma once

#include "IEditor.h"

#include "System/Core/ID/EntityID.h"
#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
	class Scene;
	class GameObject;

	struct EditorHierarchyDeleteObject
	{
	public:
		EditorHierarchyDeleteObject() :
			entityName(), id(), deleteKeyPressed()
		{
			id = 0u;
		}
		~EditorHierarchyDeleteObject() = default;

		void Reset()
		{
			entityName = std::string();
			id = 0u;
			deleteKeyPressed = false;
		}
	public:
		std::string entityName;

		EntityID id;
		bool deleteKeyPressed;
	};

	struct EditorHierarchyDuplicateObject
	{
	public:
		EditorHierarchyDuplicateObject() :
			entityName(), id(), isPressedKeyCurrentFrame()
		{
			id = 0u;
		}
		~EditorHierarchyDuplicateObject() = default;

		void Reset()
		{
			entityName = std::string();
			id = 0u;
			isPressedKeyCurrentFrame = false;
		}
	public:
		std::string entityName;

		EntityID id;

		bool isPressedKeyCurrentFrame;
	};

	class EditorHierarchy final : public IEditor
	{
	public:
		EditorHierarchy();
		~EditorHierarchy() override = default;

		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;
	private:

		//
		// Called from class.
		//

		void ShowSceneInformationGui(Ref<Scene> activeScene);

		// Duplicate game object.
		void DuplicateGameObject(Ref<Scene> activeScene);

		// Delete game object.
		void DeleteGameObject(Ref<Scene> activeScene);

		// Make parent-child relation ship.
		// call counter is used to prevent imgui IDs from overlapping.
		void ParentChildRelationShip(Ref<Scene> activeScene, GameObject* gameObject, int callCounter, bool isOpen);

		// Swap game object.
		void SwapGameObject(GameObject* gameObject);

		//
		// Drag and drop source callback functions.
		//
		void GameObjectDragAndDropCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
	private:
		EditorHierarchyDeleteObject _deleteObject;
		EditorHierarchyDuplicateObject _duplicateObject;


		bool _open;
	};
};