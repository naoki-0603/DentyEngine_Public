#include "stdafx.h"

#include "EditorHierarchy.h"

#include "System/Core/Tag.h"

#include "Scene/GameObject.h"
#include "System/Editor/EditorSceneManager.h"
#include "System/Editor/DragAndDrop/DragAndDropSource.h"

#include "System/Events/EventEditor.h"
#include "System/Events/EventScene.h"
#include "System/Layer/ApplicationLayer.h"

#include "System/Scene/Scene.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	namespace GuiUtils
	{
		enum class ConfirmGuiState;
	}

	EditorHierarchy::EditorHierarchy() :
		IEditor(), _deleteObject(), _duplicateObject(), _open(true)
	{
	}

	void EditorHierarchy::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void EditorHierarchy::OnGui(Ref<Scene> activeScene)
	{
		ImGuiWindowFlags windowFlags = {};
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBackground;

		/*{
			static bool open = true;

			if (ImGui::Begin("EditorHierarchyDebug", &open, ImGuiWindowFlags_NoDocking))
			{
				ImGui::Text("Scene");

				ImGui::Text("EntityName: %s", _duplicateObject.entityName.c_str());
				ImGui::Text("EntityID: %u", _duplicateObject.id);
			}
			ImGui::End();
		}*/
		

		if (ImGui::Begin("Hierarchy##Editor", &_open, windowFlags))
		{
			if (ApplicationLayer::IsPlay())
			{
			    ImGui::End();

			    return;
			}

			if (activeScene)
			{
				ShowSceneInformationGui(activeScene);

				// Reset flags
				{
					_duplicateObject.isPressedKeyCurrentFrame = false;
				}
			}

			// Display game objects list.
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::BeginMenu("Create##EditorHierarchy"))
				{
					//
					// Game Objects
					// 
					if (ImGui::BeginMenu("3D##EditorHierarchy"))
					{
						if (ImGui::MenuItem("Empty##EditorHierarchy"))
						{
							const auto empty = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("Empty");

							empty->CreateEmpty(name, Tag::TAGS.at(0), activeScene.get());
						}

						if (ImGui::MenuItem("Sphere##EditorHierarchy"))
						{
							const auto sphere = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("Sphere");

							sphere->CreateSphere(name, Tag::TAGS.at(0), activeScene.get());
						}

						if (ImGui::MenuItem("Box##EditorHierarchy"))
						{
							const auto box = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("Box");

							box->CreateBox(name, Tag::TAGS.at(0), activeScene.get());
						}

						// Create capsule.
						if (ImGui::MenuItem("Capsule##EditorHierarchy"))
						{
							const auto capsule = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("Capsule");

							capsule->CreateCapsule(name, Tag::TAGS.at(0), activeScene.get());
						}

						if (ImGui::MenuItem("Plane##EditorHierarchy"))
						{
							const auto plane = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("Plane");

							plane->CreatePlane(name, Tag::TAGS.at(0), activeScene.get());
						}

						ImGui::EndMenu();
					}

					//
					// 3D Lights
					//

					if (ImGui::BeginMenu("Lights##EditorHierarchy"))
					{
						if (ImGui::MenuItem("PointLight##EditorHierarchy"))
						{
							const auto pointLight = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("PointLight");

							pointLight->CreatePointLight(name, Tag::TAGS.at(0), activeScene.get());
						}

						if (ImGui::MenuItem("SpotLight##EditorHierarchy"))
						{
							const auto spotLight = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("SpotLight");

							spotLight->CreateSpotLight(name, Tag::TAGS.at(0), activeScene.get());
						}

						ImGui::EndMenu();
					}

					//
					// 2D UI
					//
					if (ImGui::BeginMenu("UI##EditorHierarchy"))
					{
						if (ImGui::MenuItem("Text##UI"))
						{
							const auto textUI = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("Text");

							textUI->CreateTextUI(name, Tag::TAGS.at(8), activeScene.get());
						}

						if (ImGui::MenuItem("Image##UI"))
						{
							const auto imageUI = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("Image");

							imageUI->CreateImageUI(name, Tag::TAGS.at(8), activeScene.get());
						}

						if (ImGui::MenuItem("Group##UI"))
						{
							const auto group = std::make_shared<GameObject>();
							const std::string name = activeScene->ObtainProcessedName("Group");

							group->CreateGroupUI(name, Tag::TAGS.at(8), activeScene.get());
						}

						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();

	}

	void EditorHierarchy::ShowSceneInformationGui(Ref<Scene> activeScene)
	{
		if (ImGui::TreeNodeEx(activeScene->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (const auto& gameObject : activeScene->GetGameObjects())
			{
				if (gameObject->HasParent()) continue;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 5));

				// Change color if flag isn't normal.
				{
					const bool enabled = gameObject->Enabled();
					const bool isStatic = gameObject->Static();
					const bool prefab = gameObject->Prefab();

					ImGuiCol imGuiCol = ImGuiCol_Text;

					auto color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

					// Set color to orange.
					if (isStatic)
					{
						constexpr auto orange = ImVec4(253.0f / 256.0f, 127.0f / 256.0f, 0.0f, 1.0f);

						color = orange;
					}

					// Set color to aqua.
					if (prefab)
					{
						constexpr auto aqua = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);

						color = aqua;
					}

					// Make translucent.
					if (not enabled)
					{
						constexpr auto grayScale = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

						color.x *= grayScale.x;
						color.y *= grayScale.y;
						color.z *= grayScale.z;
					}

					ImGui::PushStyleColor(imGuiCol, color);
				}

				// Tree node flags.
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen |
						(gameObject->HasChild() ? 0 : ImGuiTreeNodeFlags_Leaf) | ImGuiTreeNodeFlags_OpenOnArrow;

				const bool isOpen = ImGui::TreeNodeEx(
					std::to_string(gameObject->GetEntityID()).c_str(),
					treeNodeFlags,
					"%s", gameObject->GetName().c_str()
				);

				// If set style color.
				{
					ImGui::PopStyleColor();
				}

				ImGui::PopStyleVar();

				if (ImGui::IsItemClicked())
				{
					if (not ImGui::IsMouseDragging(ImGuiMouseButton_Left))
					{
					    EventSelectedGameObject selectedGameObjectEvent(gameObject.get());
					    NOTIFY_EVENT(selectedGameObjectEvent);
					}
				}

				if (ImGui::IsItemHovered())
				{
				    if (ImGui::IsKeyDown(ImGuiKey_Delete))
				    {
				        _deleteObject.id = gameObject->GetEntityID();
						_deleteObject.entityName = gameObject->GetName();
				    }
				}

				// Drag and drop area
				{
					EntityID id = gameObject->GetEntityID();
					const DragAndDropSpecification specification =
					{
						"GameObjectList", sizeof(EntityID),
						& id, DragAndDropDataType::GameObject,
						DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(GameObjectDragAndDropCallback),
						DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
					};
					DragAndDropSource dragAndDropSource(specification);
				}

				// Make parent-child relation ships.
				int32_t callCounter = 0;
				ParentChildRelationShip(activeScene, gameObject.get(), callCounter, isOpen);
			}

			ImGui::TreePop();
		}

		// Deleting
		{
		    DeleteGameObject(activeScene);
		}

		{
			DuplicateGameObject(activeScene);
		}
	}

	void EditorHierarchy::DuplicateGameObject(Ref<Scene> activeScene)
	{
		if (_duplicateObject.isPressedKeyCurrentFrame)
		{
			// Find source object.
			const std::optional<Ref<GameObject>> gameObject = activeScene->FindByEntityID(_duplicateObject.id);
			if (not gameObject.has_value())
			{
				DENTY_ERR_CONSOLE_LOG("Invalid game object id! (id: %u)", _duplicateObject.id);

				_duplicateObject.Reset();
				return;
			}

			// Create target object.
			auto target = std::make_shared<GameObject>();
			gameObject.value()->Duplicate(target);

			// Add to scene.
			activeScene->ReserveAddGameObject(target);

			_duplicateObject.Reset();
		}
	}

	void EditorHierarchy::DeleteGameObject(Ref<Scene> activeScene)
	{
		if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Delete, false) && _deleteObject.id != 0u)
		{
			const auto gameObject = activeScene->FindByEntityID(_deleteObject.id);
			if (!gameObject.has_value())
			{
				DENTY_SET_ERR_COLOR
					DENTY_ERR_LOG << "GameObject " << _deleteObject.entityName << " not found! (id: " << _deleteObject.id << " )";

				return;
			}

			// First delete children.
			{
				for (auto&& child : gameObject.value()->GetChildren())
				{
					const auto c = activeScene->FindByEntityID(child->GetEntityID());

					if (c.has_value())
					{
						activeScene->ReserveRemoveGameObject(c.value());
					}
				}
			}

			// Delete parent.
			activeScene->ReserveRemoveGameObject(gameObject.value());
		}

		_deleteObject.Reset();
	}

	void EditorHierarchy::ParentChildRelationShip(Ref<Scene> activeScene, GameObject* gameObject, int callCounter, bool isOpen)
	{
		// Create string id.
		const std::string strId = "ParentID_" + std::to_string(gameObject->GetEntityID()) + std::to_string(callCounter);

		ImGui::PushID(strId.c_str());

		// Begin drag drop source
		if (ImGui::BeginDragDropSource())
		{
			const EntityID entityID = gameObject->GetEntityID();
			ImGui::SetDragDropPayload("GameObjectList", &entityID, sizeof(EntityID));
			ImGui::Text("%s", gameObject->GetName().c_str());
			ImGui::EndDragDropSource();
		}

		// Begin drag drop target
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjectList"))
			{
				IM_ASSERT(payload->DataSize == sizeof(EntityID));

				const EntityID sourceEntityId = *static_cast<const EntityID*>(payload->Data);

				const auto sourceObject = activeScene->FindByEntityID(sourceEntityId);
				if (!sourceObject.has_value())
				{
					DENTY_SET_ERR_COLOR
						DENTY_ERR_LOG << "GameObject " << sourceEntityId << " not found!";

					return;
				}

				// If already set as the parent, cancel the parent-child relationship
				if (sourceObject.value()->GetParent())
				{
					// If same id.
					if (sourceObject.value()->GetParent()->GetEntityID() == gameObject->GetEntityID())
					{
						GameObject* childParent = gameObject->GetParent();

						if (childParent)
						{
							// Reserve set child parent.
							sourceObject.value()->ReserveSetParent(childParent);

							// Reserve remove child.
							gameObject->ReserveRemoveChild(sourceObject.value().get());

							// Reserve add child to child parent.
							childParent->ReserveAddChild(sourceObject.value().get());
						}
						else
						{
							sourceObject.value()->SetParent(nullptr);
							gameObject->ReserveRemoveChild(sourceObject.value().get());
						}
					}

				}
				// If Doesn't have parent object.
				else
				{
					// Set
					if (not sourceObject.value()->FindChildByEntityID(gameObject->GetEntityID()))
					{
						sourceObject.value()->ReserveSetParent(gameObject);
						gameObject->ReserveAddChild(sourceObject.value().get());
					}
					else
					{
						DENTY_ERR_CONSOLE_LOG("You cannot be your own child.");
					}
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();

		if (isOpen)
		{
			for (auto&& child : gameObject->GetChildren())
			{
				const std::string childStrId = "ChildObject_Id_" + std::to_string(child->GetEntityID()) + std::to_string(callCounter);
				ImGui::PushID(childStrId.c_str());

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 5));

				// Change color if flag isn't normal.
				{
					const bool topParentEnabled = gameObject->IsTopParentEnabled();
					const bool enabled = child->Enabled();
					const bool isStatic = child->Static();
					const bool prefab = child->Prefab();

					ImGuiCol imGuiCol = ImGuiCol_Text;

					auto color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

					// Set color to orange.
					if (isStatic)
					{
						constexpr auto orange = ImVec4(253.0f / 256.0f, 127.0f / 256.0f, 0.0f, 1.0f);

						color = orange;
					}

					// Set color to aqua.
					if (prefab)
					{
						constexpr auto aqua = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);

						color = aqua;
					}

					// Make translucent.
					if (not enabled || not topParentEnabled)
					{
						constexpr auto grayScale = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

						color.x *= grayScale.x;
						color.y *= grayScale.y;
						color.z *= grayScale.z;
					}

					ImGui::PushStyleColor(imGuiCol, color);
				}

				const ImGuiTreeNodeFlags childTreeNodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen |
					(child->HasChild() ? 0 : ImGuiTreeNodeFlags_Leaf) | ImGuiTreeNodeFlags_OpenOnArrow;

				const bool isOpenInChild = ImGui::TreeNodeEx(
					std::to_string(child->GetEntityID()).c_str(),
					childTreeNodeFlags,
					"%s", child->GetName().c_str()
				);

				ImGui::PopStyleColor();

				ImGui::PopStyleVar();

				// Throw event
				if (ImGui::IsItemClicked())
				{
					if (not ImGui::IsMouseDragging(ImGuiMouseButton_Left))
					{
					    EventSelectedGameObject selectedGameObjectEvent(child);
					    NOTIFY_EVENT(selectedGameObjectEvent);
					}
				}

				if (ImGui::IsItemHovered())
				{
					// Delete
					if (ImGui::IsKeyDown(ImGuiKey_Delete))
					{
						if (not _deleteObject.deleteKeyPressed)
						{
							_deleteObject.id = child->GetEntityID();
							_deleteObject.entityName = child->GetName();
							_deleteObject.deleteKeyPressed = true;
						}
					}

					// Duplicate
					if (Input::IsKeyPressed(KeyCode::LCtrl) && Input::IsKeyTriggered(KeyCode::D))
					{
						if (not _duplicateObject.isPressedKeyCurrentFrame)
						{
							_duplicateObject.id = child->GetEntityID();
							_duplicateObject.entityName = child->GetName();
							_duplicateObject.isPressedKeyCurrentFrame = true;
						}
					}
				}

				ImGui::PopID();

				if (isOpenInChild)
				{
					++callCounter;

					ParentChildRelationShip(activeScene, child, callCounter, isOpenInChild);
				}
			}
			ImGui::TreePop();
		}
	}

	void EditorHierarchy::SwapGameObject([[maybe_unused]] GameObject* gameObject)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)
	}

	void EditorHierarchy::GameObjectDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_CHECK_CONDITION(dataType == DragAndDropDataType::GameObject)

		const auto id = (*static_cast<EntityID*>(data));
		const std::optional<Ref<GameObject>> gameObject = EditorSceneManager::GetActiveScene()->FindByEntityID(id);

		if (gameObject.has_value())
		{
			ImGui::SetDragDropPayload("GameObjectList", &id, sizeof(EntityID));
			ImGui::Text("%s", gameObject.value()->GetName().c_str());
		}
	}
}
