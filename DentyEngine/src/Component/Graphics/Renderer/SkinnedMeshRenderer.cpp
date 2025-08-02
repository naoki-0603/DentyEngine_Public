#include "stdafx.h"

#include "SkinnedMeshRenderer.h"

#include "Graphics/Mesh/SkinnedMesh.h"
#include "System/Core/Assertion.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Events/EventRender.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"

namespace DentyEngine
{
	SkinnedMeshRenderer::SkinnedMeshRenderer() :
		Renderer(Type::SkinnedMeshRenderer), _mesh(), _meshFilePathToSerialized()
	{
		_name = "SkinnedMeshRenderer";

		// Is allowed multiple same component exists.
		_multipleAllowed = false;
	}

	SkinnedMeshRenderer::SkinnedMeshRenderer(const SkinnedMeshRenderer& source) :
		Renderer(source), _mesh(source._mesh), _meshFilePathToSerialized(source._meshFilePathToSerialized)
	{
	}

	void SkinnedMeshRenderer::Awake()
	{
		Renderer::Awake();
	}

	void SkinnedMeshRenderer::Start()
	{
		// Add renderer.
		Renderer::Start();
	}

	void SkinnedMeshRenderer::Update(const float deltaTime)
	{
		Renderer::Update(deltaTime);

		if (not _mesh)
			return;

		if (_mesh->ParentNode())
		{
			OnUpdate(_mesh, nullptr, deltaTime);
		}
		else
		{
			OnUpdate(_mesh, _mesh->GetMainMaterial(), deltaTime);
		}
	}

	void SkinnedMeshRenderer::Render(ID3D11DeviceContext* context)
	{
		if (not _mesh)
			return;

		if (_mesh->ParentNode())
			return;

		OnRender(context, _mesh->GetMainMaterial(), _mesh->GetMaterials(), _mesh);
	}

	void SkinnedMeshRenderer::RenderShadow(ID3D11DeviceContext* context, uint32_t cascadeShadowAreaNo)
	{
		if (not _mesh)
			return;

		if (_mesh->ParentNode())
			return;

		OnRenderShadow(context, _mesh, cascadeShadowAreaNo);
	}

	void SkinnedMeshRenderer::OnGui()
	{
		if (_mesh)
		{
		    MaterialGui(_mesh, "##SkinnedMeshRenderer");
		}

		// Call lighting settings.
		Renderer::OnGui();

		ImGui::Text("Mesh");
		ImGui::SameLine();

		if (_mesh)
		{
			_mesh->OnGui();
		}
		
		// Drag and drop
		{
			char buf[] = "Drag and drop here!";
			ImGui::InputText("##SkinnedMeshRenderer", buf, sizeof(buf), ImGuiInputTextFlags_ReadOnly);

			const DragAndDropSpecification specification =
			{
				"MeshItem", sizeof(FilePath),
				nullptr, DragAndDropDataType::Mesh,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(MeshAssetDragAndDropTargetCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropTarget dragAndDropTarget(specification);
		}
	}

	void SkinnedMeshRenderer::OnPrefabGui()
	{
		if (_mesh)
		{
		    MaterialGui(_mesh, "##SkinnedMeshRenderer");
		}

		Renderer::OnPrefabGui();
	}

	void SkinnedMeshRenderer::OnEvent(Event* e)
	{
		Renderer::OnEvent(e);
	}

	void SkinnedMeshRenderer::OnSerialized() const
	{
		Renderer::OnSerialized();
	}

	void SkinnedMeshRenderer::OnDeserialized()
	{
		Renderer::OnDeserialized();

		if (!_mesh)
		{
			const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

			const std::optional<Ref<SkinnedMesh>> skinnedMesh = meshLibrary->Find<SkinnedMesh>(_meshFilePathToSerialized);
			if (!skinnedMesh.has_value())
			{
				DENTY_ASSERT(false, "Invalid mesh file path! (in SkinnedMeshRenderer::OnDeserialized)");
			}

			// Main mesh.
			if (_subMeshIndex == UINT32_MAX)
			{
				_mesh = skinnedMesh.value();
			}
			// Sub mesh.
			else
			{
				_mesh = skinnedMesh.value()->GetSubMeshes().at(_subMeshIndex);
			}
		}
	}

	void SkinnedMeshRenderer::OnDestroy()
	{
		// Remove renderer.
		Renderer::OnDestroy();
	}

	void SkinnedMeshRenderer::OnGameObjectDeserialized()
	{
		Renderer::OnGameObjectDeserialized();
	}

	Ref<Object> SkinnedMeshRenderer::MakeCopy() const
	{
		return std::make_shared<SkinnedMeshRenderer>(*std::static_pointer_cast<const SkinnedMeshRenderer>(shared_from_this()));
	}

	void SkinnedMeshRenderer::SetMesh(Ref<SkinnedMesh> mesh)
	{
		_mesh = mesh;
		_meshFilePathToSerialized = _mesh->GetFilePathToSerialized();
	}

	void SkinnedMeshRenderer::OnMeshChanged(EventOnMeshChanged* e)
	{
		if (e->GetComponentID() != _componentID)
			return;
	}

	void SkinnedMeshRenderer::MeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::Mesh, "Data type must be mesh!");

		const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

		const auto meshFilePath = *static_cast<const FilePath*>(data);
		const auto mesh = meshLibrary->Find<SkinnedMesh>(meshFilePath);

		if (mesh.has_value())
		{
			SetMesh(mesh.value());
		}
		else
		{
			DENTY_SET_ERR_COLOR

			DENTY_ERR_LOG << "Mesh " << meshFilePath << " not found!";
		}
	}
}
