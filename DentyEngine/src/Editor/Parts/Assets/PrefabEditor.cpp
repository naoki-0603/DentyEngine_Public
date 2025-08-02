#include "stdafx.h"

#include "PrefabEditor.h"

#include "System/Events/EventEditor.h"

#include "System/Core/Constants.h"

#include "System/Resource/Prefab.h"

#include "System/Object/PrefabPreviewObject.h"

namespace DentyEngine
{
	PrefabEditor::PrefabEditor() :
		IEditor(), AssetEditor(), _prefab(), _parent(), _children()
	{
	}

	void PrefabEditor::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnPrefabEditorOpened>(BIND_EVENT_FUNCTION(OnOpened));
		dispatcher.Dispatch<EventOnPrefabEditorClosed>(BIND_EVENT_FUNCTION(OnClosed));
	}

	void PrefabEditor::OnGui(Ref<Scene> activeScene)
	{
		// If the window is open
		if (not _open) return;

		if (ImGui::Begin("PrefabEditor##AssetEditor", &_open, IMGUI_DEFAULT_WINDOW_FLAGS))
		{
			// Parent
			{
				if (_parent)
				{
					_parent->ShowGui();
				}
			}

			// TODO Children
			{
				
			}

			
		}

		ImGui::End();

		// If closed.
		if (not _open)
		{
			EventOnPrefabEditorClosed eventOnPrefabEditorClosed(_prefab->GetFilePathToSerialized());
			NOTIFY_EVENT(eventOnPrefabEditorClosed);
		}
	}

	void PrefabEditor::OnOpened(EventOnPrefabEditorOpened* e)
	{
		_open = true;
		_prefab = e->GetPrefab();

		// Create temp game object.
		// Doesn't add scene.
		{
			// Parent
			{
				if (_parent)
				{
					_parent = nullptr;
				}
			}

			// TODO Children
			{
				
			}

			// Create game object use prefab data.
			{
				// Parent
				{
					const PrefabGameObject& prefabGameObject = _prefab->_specification.parent;

					_parent = std::make_shared<PrefabPreviewObject>();
					_parent->Create(
						prefabGameObject.name, 
						prefabGameObject.tag, 
						prefabGameObject.components, 
						prefabGameObject.enabled, 
						prefabGameObject.isStatic
					);
				}

				// TODO Children.
				{
				}
			}
		}

		ImGui::SetNextWindowSize(ImVec2(IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y));
	}

	void PrefabEditor::OnClosed([[maybe_unused]] EventOnPrefabEditorClosed* e)
	{
		// Copy data.
		{
			// Parent
			{
				PrefabGameObject& parentInfo = _prefab->_specification.parent;
				_parent->Copy(parentInfo);
			}

			// TODO Children
			{
			}
		}

		// Save.
		{
			_prefab->Save(true);
		}

		// Reset
		{
			_open = false;
			_prefab = nullptr;
			_parent = nullptr;

			_children.clear();
		}
	}
}
