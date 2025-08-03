#include "stdafx.h"

#include "ComponentGenerator.h"

#include "Utilities/StringUtilities.h"

namespace DentyEngine
{
	std::vector<std::string> ComponentGenerator::_buildInComponentNames = {};
	std::vector<std::string> ComponentGenerator::_monoComponentNames = {};

	std::unordered_map<std::string, std::function<Ref<Component>()>> ComponentGenerator::_buildInComponentGenerators = {};

	ComponentGenerator::ComponentGenerator()
	{
	}

	void ComponentGenerator::AddBuildIn(const Ref<Component>& component)
	{
		_buildInComponentNames.emplace_back(component->GetName());
	}

	void ComponentGenerator::ShowComponentList(std::string& component, bool& isPressAddComponentButton, bool& isAddBuildInComponent)
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

		bool open = true;
		if (ImGui::Begin("Components##ComponentList", &open, windowFlags))
		{
			constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

			//
			// Graphics
			//
			if (ImGui::TreeNodeEx("Graphics##ComponentList", nodeFlags))
			{
				ImGui::Indent();

				for (const auto& componentName : GetBuildInComponentNames())
				{
					if (StringUtils::StringContains(componentName, "Collider")) continue;
					if (StringUtils::StringContains(componentName, "RigidBody")) continue;
					if (StringUtils::StringContains(componentName, "Animator")) continue;
					if (StringUtils::StringContains(componentName, "Script")) continue;
					if (StringUtils::StringContains(componentName, "KinematicCharacterController")) continue;
					if (StringUtils::StringContains(componentName, "DynamicCharacterController")) continue;
					if (StringUtils::StringContains(componentName, "VehicleComponent")) continue;
					if (StringUtils::StringContains(componentName, "WheelComponent")) continue;
					if (StringUtils::StringContains(componentName, "DefaultCameraController")) continue;
					if (StringUtils::StringContains(componentName, "CheckPoint")) continue;

					std::string selectableNodeName = componentName + "##ComponentList";

					if (ImGui::Selectable(selectableNodeName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
					{
						// Set component name.
						component = componentName;

						isAddBuildInComponent = true;

						isPressAddComponentButton = false;
					}
				}

				ImGui::Unindent();


				ImGui::TreePop();
			}

			//
			// Physics
			//
			if (ImGui::TreeNodeEx("Physics##ComponentList", nodeFlags))
			{
				ImGui::Indent();

				for (const auto& componentName : GetBuildInComponentNames())
				{
					if (StringUtils::StringContains(componentName, "Camera")) continue;
					if (StringUtils::StringContains(componentName, "Light")) continue;
					if (StringUtils::StringContains(componentName, "Renderer")) continue;
					if (StringUtils::StringContains(componentName, "Animator")) continue;
					if (StringUtils::StringContains(componentName, "Script")) continue;
					if (StringUtils::StringContains(componentName, "DefaultCameraController")) continue;
					if (StringUtils::StringContains(componentName, "CheckPoint")) continue;

					if (StringUtils::StringContains(componentName, "EffectComponent")) continue;

					std::string selectableNodeName = componentName + "##ComponentList";

					if (ImGui::Selectable(selectableNodeName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
					{
						component = componentName;

						isAddBuildInComponent = true;

						isPressAddComponentButton = false;
					}
				}

				ImGui::Unindent();

				ImGui::TreePop();
			}

			//
			// Controller
			//
			if (ImGui::TreeNodeEx("Controller##ComponentList", nodeFlags))
			{
				ImGui::Indent();

				for (const auto& componentName : GetBuildInComponentNames())
				{
					if (StringUtils::StringContains(componentName, "Collider")) continue;
					if (StringUtils::StringContains(componentName, "RigidBody")) continue;
					if (StringUtils::StringContains(componentName, "Game")) continue;
					if (StringUtils::StringContains(componentName, "Light")) continue;
					if (StringUtils::StringContains(componentName, "Renderer")) continue;
					if (StringUtils::StringContains(componentName, "Script")) continue;
					if (StringUtils::StringContains(componentName, "KinematicCharacter")) continue;
					if (StringUtils::StringContains(componentName, "DynamicCharacter")) continue;
					if (StringUtils::StringContains(componentName, "VehicleComponent")) continue;
					if (StringUtils::StringContains(componentName, "WheelComponent")) continue;
					if (StringUtils::StringContains(componentName, "Animator")) continue;
					if (StringUtils::StringContains(componentName, "CheckPoint")) continue;
					if (StringUtils::StringContains(componentName, "EffectComponent")) continue;


					std::string selectableNodeName = componentName + "##ComponentList";

					if (ImGui::Selectable(selectableNodeName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
					{
						component = componentName;

						isAddBuildInComponent = true;

						isPressAddComponentButton = false;
					}
				}

				ImGui::Unindent();

				ImGui::TreePop();
			}

			//
			// Animations
			//
			if (ImGui::TreeNodeEx("Animations##ComponentList", nodeFlags))
			{
				ImGui::Indent();

				for (const auto& componentName : GetBuildInComponentNames())
				{
					if (StringUtils::StringContains(componentName, "Collider")) continue;
					if (StringUtils::StringContains(componentName, "RigidBody")) continue;
					if (StringUtils::StringContains(componentName, "Camera")) continue;
					if (StringUtils::StringContains(componentName, "Light")) continue;
					if (StringUtils::StringContains(componentName, "Renderer")) continue;
					if (StringUtils::StringContains(componentName, "Script")) continue;
					if (StringUtils::StringContains(componentName, "KinematicCharacterController")) continue;
					if (StringUtils::StringContains(componentName, "DynamicCharacterController")) continue;
					if (StringUtils::StringContains(componentName, "VehicleComponent")) continue;
					if (StringUtils::StringContains(componentName, "WheelComponent")) continue;
					if (StringUtils::StringContains(componentName, "DefaultCameraController")) continue;
					if (StringUtils::StringContains(componentName, "CheckPoint")) continue;
					if (StringUtils::StringContains(componentName, "EffectComponent")) continue;


					std::string selectableNodeName = componentName + "##ComponentList";

					if (ImGui::Selectable(selectableNodeName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
					{
						component = componentName;

						isAddBuildInComponent = true;

						isPressAddComponentButton = false;
					}
				}

				ImGui::Unindent();

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Script##ComponentList", nodeFlags))
			{
				ImGui::Indent();

				for (const auto& componentName : GetBuildInComponentNames())
				{
					if (StringUtils::StringContains(componentName, "Collider")) continue;
					if (StringUtils::StringContains(componentName, "Animator")) continue;
					if (StringUtils::StringContains(componentName, "RigidBody")) continue;
					if (StringUtils::StringContains(componentName, "Camera")) continue;
					if (StringUtils::StringContains(componentName, "Light")) continue;
					if (StringUtils::StringContains(componentName, "Renderer")) continue;
					if (StringUtils::StringContains(componentName, "KinematicCharacterController")) continue;
					if (StringUtils::StringContains(componentName, "DynamicCharacterController")) continue;
					if (StringUtils::StringContains(componentName, "VehicleComponent")) continue;
					if (StringUtils::StringContains(componentName, "WheelComponent")) continue;
					if (StringUtils::StringContains(componentName, "DefaultCameraController")) continue;
					if (StringUtils::StringContains(componentName, "CheckPoint")) continue;
					if (StringUtils::StringContains(componentName, "EffectComponent")) continue;


					std::string selectableNodeName = componentName + "##ComponentList";

					if (ImGui::Selectable(selectableNodeName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
					{
						component = componentName;

						isAddBuildInComponent = true;

						isPressAddComponentButton = false;
					}
				}

				ImGui::Unindent();

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Game##ComponentList", nodeFlags))
			{
				// Race game
				if (ImGui::TreeNodeEx("RaceGame##Game", nodeFlags))
				{
					for (const auto& componentName : GetBuildInComponentNames())
					{
						if (StringUtils::StringContains(componentName, "Collider")) continue;
						if (StringUtils::StringContains(componentName, "Animator")) continue;
						if (StringUtils::StringContains(componentName, "RigidBody")) continue;
						if (StringUtils::StringContains(componentName, "Camera")) continue;
						if (StringUtils::StringContains(componentName, "Light")) continue;
						if (StringUtils::StringContains(componentName, "Renderer")) continue;
						if (StringUtils::StringContains(componentName, "KinematicCharacterController")) continue;
						if (StringUtils::StringContains(componentName, "DynamicCharacterController")) continue;
						if (StringUtils::StringContains(componentName, "VehicleComponent")) continue;
						if (StringUtils::StringContains(componentName, "WheelComponent")) continue;
						if (StringUtils::StringContains(componentName, "DefaultCameraController")) continue;
						if (StringUtils::StringContains(componentName, "ScriptComponent")) continue;
						if (StringUtils::StringContains(componentName, "EffectComponent")) continue;

						std::string selectableNodeName = componentName + "##ComponentList";

						ImGui::Indent();

						if (ImGui::Selectable(selectableNodeName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
						{
							component = componentName;

							isAddBuildInComponent = true;

							isPressAddComponentButton = false;
						}

						ImGui::Unindent();
					}
					
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				isPressAddComponentButton = false;
			}
		}

		ImGui::End();
	}

	Ref<Component> ComponentGenerator::FindBuildInComponent(std::string_view componentName)
	{
		const auto& it = _buildInComponentGenerators.find(std::string(componentName));

		if (it != _buildInComponentGenerators.end())
		{
			return it->second();
		}

		DENTY_SET_ERR_COLOR
		DENTY_ERR_LOG << componentName << " is not found!";

		return nullptr;
	}
}
