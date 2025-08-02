#include "stdafx.h"

#include "EditorInspector.h"

#include "System/Events/EventEditor.h"
#include "System/Events/EventScene.h"

#include "Scene/GameObject.h"

#include "Component/ComponentGenerator.h"
#include "Component/Scripts/ScriptComponent.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"
#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Layer/ApplicationLayer.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	EditorInspector::EditorInspector() :
		IEditor(), _selectedGameObject(), _pressedAddComponentButton(), _locked(), _open(true)
	{
	}

	void EditorInspector::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventSelectedGameObject>(BIND_EVENT_FUNCTION(OnSelectedGameObject));
		dispatcher.Dispatch<EventOnGameObjectDestroyed>(BIND_EVENT_FUNCTION(OnGameObjectDestroyed));
		dispatcher.Dispatch<EventOnSceneDeActivated>(BIND_EVENT_FUNCTION(OnSceneDeActivated));
	}

	void EditorInspector::OnGui(Ref<Scene> activeScene)
	{
		ImGuiWindowFlags windowFlags = {};
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBackground;

		if (ImGui::Begin("Inspector##Editor", &_open, windowFlags))
		{
			if (ApplicationLayer::IsPlay())
			{
			    ImGui::End();

				return;
			}

			if (_selectedGameObject)
			{
				// Lock
				ImGui::Checkbox("Lock##Editor", &_locked);

				_selectedGameObject->OnGui();

				if (GuiUtils::ButtonCentered("AddComponent", Vector2(300.0f, 0.0f)))
				{
					_pressedAddComponentButton = true;
				}
			}

			// Drag and drop
		    {
				ImVec2 size = ImGui::GetContentRegionAvail();
				if (size.x == 0.0f)
				{
					size.x = 16.0f;
				}

				if (size.y == 0.0f)
				{
					size.y = 16.0f;
				}

				ImGui::InvisibleButton("##ScriptItemDragAndDrop", size);

				const DragAndDropSpecification specification =
				{
				    "ScriptItem", sizeof(Utils::DragAndDropScriptData),
					nullptr, DragAndDropDataType::Script,
					DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(ScriptAssetDragAndDropTargetCallback),
					DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
				};
				DragAndDropTarget dragAndDropTarget(specification);
		    }
		    
		}

		ImGui::End();

		// Show component list if pressed.
		if (_pressedAddComponentButton)
		{
			AddComponentOnGui();
		}
	}

	void EditorInspector::OnSelectedGameObject(EventSelectedGameObject* e)
	{
		if (not _locked)
		{
			_selectedGameObject = e->GetGameObject();
		}
	}

	void EditorInspector::OnGameObjectDestroyed(EventOnGameObjectDestroyed* e)
	{
		// Set nullptr if the object was deleted.
		if (_selectedGameObject && _selectedGameObject->GetEntityID() == e->GetID())
		{
			_selectedGameObject = nullptr;
		}
	}

	void EditorInspector::OnSceneDeActivated([[maybe_unused]] EventOnSceneDeActivated* e)
	{
		_selectedGameObject = nullptr;
		_locked = false;
	}

	void EditorInspector::AddComponentOnGui()
	{
		// Stores selected component name.
		auto componentName = std::string();

		bool isBuildInComponent = false;

		// Display the component list
		ComponentGenerator::ShowComponentList(componentName, _pressedAddComponentButton, isBuildInComponent);

		// If component name is empty, return.
		if (componentName.empty() || not _selectedGameObject)
		{
			return;
		}

		// The process changes depending on the type of component.
		if (isBuildInComponent)
		{
			_selectedGameObject->AddBuildInComponent(componentName);
		}
		// Mono component.
		else
		{
			_selectedGameObject->AddMonoComponent(componentName);
		}
	}

    void EditorInspector::ScriptAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
    {
		DENTY_ASSERT(dataType == DragAndDropDataType::Script, "Data type must be script!");

        const Utils::DragAndDropScriptData scriptData = *(static_cast<Utils::DragAndDropScriptData*>(data));

        const Ref<Script> script = scriptData.Get();

		if (_selectedGameObject)
		{
		    const auto scriptComponent = _selectedGameObject->AddComponent<ScriptComponent>();
			scriptComponent->SetScript(script);
			scriptComponent->Create();
		}
    }
};