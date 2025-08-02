#include "stdafx.h"

#include "PrefabPreviewObject.h"

#include "Component/MonoBehaviour.h"
#include "Component/Graphics/Renderer/Renderer.h"

#include "Component/ComponentGenerator.h"
#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	PrefabPreviewObject::PrefabPreviewObject() :
		Object(), _components(), _reservedRemoveComponent(), _transform(), _tag(), _enabled(), _static(),
        _pressedAddComponentButton()
	{
	}

	void PrefabPreviewObject::Create(std::string_view name, const Tag& tag, const std::vector<Ref<Component>>& components, bool enabled, bool isStatic)
	{
		_name = name;
		_tag = tag;
        _enabled = enabled;
        _static = isStatic;

		// Make copy
        for (const auto& component : components)
        {
            const auto copyComponent = component;

            copyComponent->SetParent(nullptr);

            _components.emplace_back(copyComponent);
        }
	}

	void PrefabPreviewObject::ShowGui()
	{
        // remove component
        if (_reservedRemoveComponent)
        {
            _components.erase(std::ranges::find(_components, _reservedRemoveComponent));
        }
        _reservedRemoveComponent = nullptr;

        // GUI
        {
            ImGui::Checkbox("##GameObjectEnabled", &_enabled);
            ImGui::SameLine();

            String::InputTextGui("##GameObjectName", &_name);

            ImGui::SameLine();
            ImGui::Checkbox("Static##GameObject", &_static);

            ImGui::Text("Tag");
            ImGui::SameLine();

            GuiUtils::ComboBoxGui("##GameObjectTag", Tag::TAGS.data(), static_cast<int>(Tag::TAGS.size()), _tag.tag);

            ImGui::SameLine();

            ImGui::Separator();

            for (auto&& component : _components)
            {
                bool enabled = component->Enabled();

                ImGui::PushID(static_cast<int32_t>(component->GetComponentID()));

                ImGui::Checkbox("##ComponentEnabled", &enabled);

                ImGui::PopID();

                component->SetEnabled(enabled);

                ImGui::SameLine();
                if (ImGui::TreeNodeEx(component->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
                {
                    // Remove component button.
                    {
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 48.0f);
                        if (ImGui::Button("Remove"))
                        {
                            _reservedRemoveComponent = component;
                        }
                    }

                    component->OnPrefabGui();

                    ImGui::TreePop();
                }
            }

            ImGui::Separator();

            // Add component button.
            if (GuiUtils::ButtonCentered("AddComponent", Vector2(300.0f, 0.0f)))
            {
                _pressedAddComponentButton = true;
            }

            if (_pressedAddComponentButton)
            {
                bool isBuildIn = false;

                std::string componentName;

                ComponentGenerator::ShowComponentList(componentName, _pressedAddComponentButton, isBuildIn);

                if (not _pressedAddComponentButton)
                {
                    if (isBuildIn)
                    {
                        AddBuildInComponent(componentName);
                    }
                    else
                    {
                        AddMonoComponent(componentName);
                    }
                }
            }
        }
	}

	void PrefabPreviewObject::Copy(PrefabGameObject& gameObject) const
	{
        gameObject.name = _name.Get();
        gameObject.tag = _tag;
        gameObject.enabled = _enabled;
        gameObject.isStatic = _static;

        // Remove old components.
        gameObject.components.clear();

        // Set new components.
        for (const auto& component : _components)
        {
            const auto& copy = component;

            gameObject.components.emplace_back(copy);
        }
	}

	bool PrefabPreviewObject::HasComponent(std::string_view componentName) const
	{
        for (auto&& component : _components)
        {
            if (component->GetName() == componentName)
                return true;
        }

        return false;
	}

	void PrefabPreviewObject::AddBuildInComponent(std::string_view componentName)
	{
        const auto component = ComponentGenerator::FindBuildInComponent(componentName);

        if (not component)
            return;

		if (not component->MultipleAllowed())
        {
            if (HasComponent(componentName))
            {
                DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Already has " << component->GetName().c_str() << " component!";
                return;
            }
        }

        // Check is required components exists.
        for (const auto& requiredComponentName : component->GetRequiredComponentNames())
        {
            if (HasComponent(requiredComponentName)) continue;

            DENTY_SET_ERR_COLOR
            DENTY_ERR_LOG << requiredComponentName.c_str() << " component is required!";

            return;
        }

        _components.emplace_back(component);
	}

	void PrefabPreviewObject::AddMonoComponent(std::string_view componentName)
	{
        const auto component = ComponentGenerator::FindMonoComponent(componentName);

        if (not component) 
            return;

		if (not component->MultipleAllowed())
        {
            if (HasComponent(componentName))
            {
                DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Already has " << component->GetName().c_str() << " component!";
                return;
            }
        }

        _components.emplace_back(component);
	}
}
