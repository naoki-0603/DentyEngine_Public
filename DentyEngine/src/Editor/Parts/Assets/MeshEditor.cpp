#include "stdafx.h"

#include "MeshEditor.h"

#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshImporter.h"

#include "System/Core/DFilePath.h"
#include "System/Core/DString.h"

#include "System/Events/EventEditor.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"

#include "System/Serializer/SerializeExtensions.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	MeshEditor::MeshEditor() :
	    IEditor(), AssetEditor(), _mesh(), _previousScaleFactor(), _previousCoordinateSystem()
	{
	}

	void MeshEditor::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnMeshEditorOpened>(BIND_EVENT_FUNCTION(OnOpened));
		dispatcher.Dispatch<EventOnMeshEditorClosed>(BIND_EVENT_FUNCTION(OnClosed));
	}

	void MeshEditor::OnGui(Ref<Scene> activeScene)
	{
		if (not _open) return;

		if (ImGui::Begin("MeshEditor##AssetEditor", &_open, IMGUI_DEFAULT_WINDOW_FLAGS))
		{
			if (ImGui::CollapsingHeader("Info##MeshEditor", ImGuiTreeNodeFlags_DefaultOpen))
			{
				_mesh->OnGui();
			}

			if (_mesh->GetMeshType() == std::string("StaticMesh"))
			{
				if (ImGui::CollapsingHeader("CollisionMesh##MeshEditor", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (GuiUtils::ButtonCentered("Generate", Vector2(240.0f, 32.0f)))
					{
						using ImportType = MeshImporter::ImportType;

						MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

						FilePath filePath = _mesh->GetFilePathToSerialized().GetAsString();
						filePath = filePath.ObtainDirectory();

						String filePathToCollisionMesh = filePath.GetAsString();
						filePathToCollisionMesh.Append(FilePath(_mesh->GetNodeName().Get()).RemoveExtension().GetAsString());
						filePathToCollisionMesh.Append(COLLISION_MESH_FILE_EXTENSION);

						if (meshLibrary->Exists(filePathToCollisionMesh.Get()))
						{
							DENTY_WARN_CONSOLE_LOG("Already generated %s collision mesh!", _mesh->GetName().c_str());
						}
						else
						{
							MeshImporter importer = {};
							const auto mesh = importer.Import(_mesh->GetFilePath().GetAsString().c_str(), ImportType::ImportAsCollisionMesh);

							mesh->Save();

							meshLibrary->Add(mesh);
						}
					}
				}
			}

		}
		ImGui::End();


		// If closed.
		if (not _open)
		{
			EventOnMeshEditorClosed eventOnMeshEditorClosed(_mesh->GetMeshID());
			NOTIFY_EVENT(eventOnMeshEditorClosed);
		}
	}

    void MeshEditor::OnOpened(EventOnMeshEditorOpened* e)
    {
		_open = true;
		_mesh = e->GetMesh();

		ImGui::SetNextWindowSize(ImVec2(IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y));
    }

    void MeshEditor::OnClosed([[maybe_unused]] EventOnMeshEditorClosed* e)
    {
		_open = false;
		_mesh = nullptr;

		_previousScaleFactor = "";
		_previousCoordinateSystem = "";
    }
}
