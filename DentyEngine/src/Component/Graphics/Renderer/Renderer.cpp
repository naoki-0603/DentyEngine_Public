#include "stdafx.h"

#include "Renderer.h"

#include "Scene/GameObject.h"

#include "Graphics/Mesh/Mesh.h"

#include "System/Events/EventEditor.h"
#include "System/Events/EventRender.h"

#include "System/Graphics/Renderer/RenderManager.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	const std::vector<std::string> Renderer::CAST_SHADOW_OPTIONS =
	{
		"On", "Off"
	};

	const std::vector<std::string> Renderer::ORDER_TYPES =
	{
		"Opacity", "Transparent"
	};

	Renderer::Renderer(Type rendererType, OrderType orderType) :
		Component(), shadowCastEnabled(true),
		_materialFilePaths(), _subMeshIndex(UINT32_MAX), _materials(),
		_castShadow(CAST_SHADOW_OPTIONS.at(0)), _orderTypeStr("Opacity"),
		_rendererType(rendererType), _orderType(orderType)
	{
	}

	Renderer::Renderer(const Renderer& source) :
		Component(source), shadowCastEnabled(source.shadowCastEnabled),
		_materialFilePaths(source._materialFilePaths), _subMeshIndex(source._subMeshIndex),
		_materials(source._materials),
		_castShadow(source._castShadow),
		_orderTypeStr(source._orderTypeStr), _rendererType(source._rendererType), _orderType(source._orderType)
	{
	}

	void Renderer::Awake()
	{
		
	}

	void Renderer::Start()
	{
		RenderManager::GetInstance().AddRenderer(shared_from_this());
	}

	void Renderer::Update([[maybe_unused]] const float deltaTime)
	{
	}

	void Renderer::OnGui()
	{
		// Tree node flags
		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;

		// Order
		if (ImGui::CollapsingHeader("Order##Renderer", treeNodeFlags))
		{
			GuiUtils::ComboBoxGui("##RendererOrderType", ORDER_TYPES.data(), static_cast<int>(ORDER_TYPES.size()), _orderTypeStr);

			// Opacity
			if (_orderTypeStr == ORDER_TYPES.at(static_cast<uint32_t>(OrderType::Opacity)))
			{
				if (_orderType != OrderType::Opacity)
				{
					_orderType = OrderType::Opacity;

					RenderManager::GetInstance().OnRendererOrderTypeChanged(shared_from_this());
				}
				else
				{
					_orderType = OrderType::Opacity;
				}
			}
			// Transparent
			else if (_orderTypeStr == ORDER_TYPES.at(static_cast<uint32_t>(OrderType::Transparent)))
			{
				if (_orderType != OrderType::Transparent)
				{
					_orderType = OrderType::Transparent;

					RenderManager::GetInstance().OnRendererOrderTypeChanged(shared_from_this());
				}
				else
				{
					_orderType = OrderType::Transparent;
				}
			}
		}

	    // Lighting 
		if (ImGui::CollapsingHeader("Lighting##Renderer", treeNodeFlags))
		{
			ImGui::Text("Cast Shadows");
			ImGui::SameLine();
			GuiUtils::ComboBoxGui("##RendererCastShadows", CAST_SHADOW_OPTIONS.data(), static_cast<int>(CAST_SHADOW_OPTIONS.size()), _castShadow);

			// If cast shadow is on.
			if (_castShadow == CAST_SHADOW_OPTIONS.at(0))
			{
				shadowCastEnabled = true;
			}
			// Otherwise.
			else
			{
				shadowCastEnabled = false;
			}
		}

	}

	void Renderer::OnPrefabGui()
	{
		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;

		// Lighting 
		if (ImGui::TreeNodeEx("Lighting##Renderer", treeNodeFlags))
		{
			ImGui::Text("Cast Shadows");
			ImGui::SameLine();
			GuiUtils::ComboBoxGui("##RendererCastShadows", CAST_SHADOW_OPTIONS.data(), static_cast<int>(CAST_SHADOW_OPTIONS.size()), _castShadow);

			// If cast shadow is on.
			if (_castShadow == CAST_SHADOW_OPTIONS.at(0))
			{
				shadowCastEnabled = true;
			}
			// Otherwise.
			else
			{
				shadowCastEnabled = false;
			}

			ImGui::TreePop();
		}
	}

	void Renderer::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnMeshChanged>(BIND_EVENT_FUNCTION(OnMeshChanged));
	}

	void Renderer::OnSerialized() const
	{

	}

	void Renderer::OnDeserialized()
	{
		_orderTypeStr = std::string(NAMEOF_ENUM(_orderType)).c_str();
	}

	void Renderer::OnDestroy()
	{
		Component::OnDestroy();

		// Remove renderer.
		{
			RenderManager::GetInstance().RemoveRenderer(shared_from_this());
		}
	}

	void Renderer::OnGameObjectDeserialized()
	{
	}

    void Renderer::OnSceneActivated()
    {
		_orderTypeStr = std::string(NAMEOF_ENUM(_orderType)).c_str();

		RenderManager::GetInstance().AddRenderer(shared_from_this());
    }

    void Renderer::OnUpdate(const Ref<Mesh>& mesh, const Ref<Material>& mainMaterial, const float deltaTime)
    {
		// Get parent.
		const auto& parent = GetParent();

		if (parent->HasParent())
		{
			mesh->OnUpdate(parent->transform.world, parent->GetEntityID(), mainMaterial, deltaTime);
		}
		else
		{
			mesh->OnUpdate(parent->transform.local, parent->GetEntityID(), mainMaterial, deltaTime);
		}
    }

    void Renderer::OnRender(ID3D11DeviceContext* context, const Ref<Material>& mainMaterial, const std::vector<Ref<Material>>& materials, const Ref<Mesh>& mesh)
    {
		if (not mesh) return;
		if (mesh->GetMaterials().empty()) return;
		if (not mesh->GetMainMaterial()) return;

		mesh->OnRender(context, mainMaterial, materials);
    }

    void Renderer::OnRenderShadow(ID3D11DeviceContext* context, const Ref<Mesh>& mesh, uint32_t cascadeShadowAreaNo)
    {
		if (not mesh) return;

		mesh->OnRenderShadow(context, cascadeShadowAreaNo);
    }

	void Renderer::MaterialGui(const Ref<Mesh>& mesh, std::string_view label)
    {
		if (not mesh) return;

        constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

        // Materials
        if (ImGui::TreeNodeEx("Materials##Renderer", treeNodeFlags))
        {
            ImGui::Text("Size");
            ImGui::SameLine();

            // Element num zero is main material.
            const size_t materialCount = mesh->GetMaterialCount();
            ImGui::Text("%u", materialCount);

            for (int materialIndex = 0; materialIndex < materialCount; ++materialIndex)
            {
                const auto material = mesh->FindMaterialAt(materialIndex);
                const auto materialName = material->GetName() + std::string(label);

                if (material->IsDefault())
                {
                    ImGui::Text("DefaultMaterial");
                }
                else
                {
					// Create open material editor event.
                    if (ImGui::Button(materialName.c_str()))
                    {
						EventOnMaterialEditorOpened eventOnMaterialEditorOpened(material);
						NOTIFY_EVENT(eventOnMaterialEditorOpened);
                    }
                }
            }

            ImGui::TreePop();
        }
    }
};