#include "stdafx.h"

#include "MeshRenderer.h"

#include "Scene/GameObject.h"

#include "Graphics/Mesh/StaticMesh.h"

#include "System/Core/Assertion.h"

#include "System/Events/EventRender.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"

namespace DentyEngine
{
	MeshRenderer::MeshRenderer() :
		Renderer(Type::MeshRenderer), _mesh(), _meshFilePathToSerialized()
	{
		_name = "MeshRenderer";

		// Is allowed multiple same component exists.
		_multipleAllowed = false;
	}

	MeshRenderer::MeshRenderer(const MeshRenderer& source) :
		Renderer(source), _meshFilePathToSerialized(source._meshFilePathToSerialized)
	{
		if (source._mesh)
		{
			_mesh = std::make_shared<StaticMesh>(*std::static_pointer_cast<StaticMesh>(source._mesh));
		}
	}

	void MeshRenderer::Awake()
	{
		Renderer::Awake();
	}

	void MeshRenderer::Start()
	{
		// Add renderer.
		Renderer::Start();
	}

	void MeshRenderer::Update(const float deltaTime)
	{
		Renderer::Update(deltaTime);

		if (not _mesh)
			return;

		if (_mesh->ParentNode())
			return;

	    OnUpdate(_mesh, _mesh->GetMainMaterial(), deltaTime);
	}

	void MeshRenderer::Render(ID3D11DeviceContext* context)
	{
		if (not _mesh)
			return;

		if (_mesh->ParentNode())
			return;

		OnRender(context, _mesh->GetMainMaterial(), _mesh->GetMaterials(), _mesh);
	}

	void MeshRenderer::RenderShadow(ID3D11DeviceContext* context, uint32_t cascadeShadowAreaNo)
	{
		if (not _mesh)
			return;

		if (_mesh->ParentNode())
			return;

	    OnRenderShadow(context, _mesh, cascadeShadowAreaNo);
	}

	void MeshRenderer::OnGui()
	{
		if (_mesh)
		{
		    MaterialGui(_mesh, "##MeshRenderer");
		}

		Renderer::OnGui();
	}

	void MeshRenderer::OnPrefabGui()
	{
        if (_mesh)
		{
		    MaterialGui(_mesh, "##MeshRenderer");
		}

		Renderer::OnPrefabGui();
	}

	void MeshRenderer::OnEvent(Event* e)
	{
		Renderer::OnEvent(e);
	}

	void MeshRenderer::OnSerialized() const
	{
		Renderer::OnSerialized();
	}

	void MeshRenderer::OnDeserialized()
	{
		Renderer::OnDeserialized();

		if (!_mesh)
		{
			const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

			const std::optional<Ref<StaticMesh>> staticMesh = meshLibrary->Find<StaticMesh>(_meshFilePathToSerialized);
			if (!staticMesh.has_value())
			{
				DENTY_ASSERT(false, "Invalid mesh file path! (in MeshRenderer::OnDeserialized)");
			}

			// Main mesh.
			if (_subMeshIndex == UINT32_MAX)
			{
				_mesh = staticMesh.value();
			}
			// Sub mesh.
			else
			{
				_mesh = staticMesh.value()->GetSubMeshes().at(_subMeshIndex);
			}
		}
	}

	void MeshRenderer::OnDestroy()
	{
		// Destroy renderer.
		Renderer::OnDestroy();
	}

	void MeshRenderer::OnGameObjectDeserialized()
	{
		Renderer::OnGameObjectDeserialized();
	}

	Ref<Object> MeshRenderer::MakeCopy() const
	{
		return std::make_shared<MeshRenderer>(*std::static_pointer_cast<const MeshRenderer>(shared_from_this()));
	}

	void MeshRenderer::SetMesh(Ref<StaticMesh> mesh)
	{
		_mesh = mesh;
		_meshFilePathToSerialized = _mesh->GetFilePathToSerialized().GetAsString();
	}

	void MeshRenderer::ObtainMaterials()
	{
		_materials.clear();
		_materialFilePaths.clear();

		if (_mesh)
		{
			for (const auto& material : _mesh->GetMaterials())
			{
				if (material->IsDefault())
					continue;

				_materials.emplace(material->GetFilePathToSerialized(), material);
				_materialFilePaths.emplace_back(material->GetFilePathToSerialized());
			}
		}
	}

    void MeshRenderer::OnMeshChanged(EventOnMeshChanged* e)
    {
		if (e->GetComponentID() != _componentID)
			return;

		ObtainMaterials();
    }
};