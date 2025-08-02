#include "stdafx.h"

#include "BaseState.h"

#include "Editor/Animation/Transitions/Transition.h"

#include "System/Editor/DragAndDrop/DragAndDropSource.h"

#include "System/Resource/Animations/AnimatorController.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/AnimatorControllerLibrary.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	BaseState::BaseState() :
        _parent(), _transitions(), _animatorControllerFilePath(), _name(), _reservedRemoveTransitions()
	{
	}

    BaseState::BaseState(std::string_view name, Ref<AnimatorController> parent) :
        _parent(parent), _transitions(), _animatorControllerFilePath(), _name(name), _reservedRemoveTransitions()
    {
        if (parent)
        {
            _animatorControllerFilePath = parent->GetFilePathToSerialized();
        }
    }

    void BaseState::OnUpdate()
    {
        // Remove reserved conditions.
        {
            for (auto&& reservedTransition : _reservedRemoveTransitions)
            {
                uint32_t targetIndex = static_cast<uint32_t>(-1);
                for (uint32_t index = 0; index < _transitions.size(); ++index)
                {
                    const auto& transition = _transitions.at(index);

                    if (transition->GetName() != reservedTransition->GetName())
                        continue;

                    targetIndex = index;
                    break;
                }

                if (targetIndex != UINT32_MAX)
                {
                    _transitions.erase(_transitions.cbegin() + targetIndex);
                }

                DENTY_INFO_CONSOLE_LOG("Succeed to remove %s transition!", reservedTransition->GetName().c_str());
            }
            _reservedRemoveTransitions.clear();
        }

        // Update transitions.
        {
            for (auto&& transition : _transitions)
            {
                transition->OnUpdate();
            }
        }
    }

    void BaseState::OnGui()
    {
        ImGui::Text("Name: %s", _name.c_str());
        ImGui::Separator();

        const auto headerLabel = std::string("Transitions") + "##" + "BaseState_" + _name;
        if (ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto&& transition : _transitions)
            {
                std::string label = transition->GetName() + "##Transition";
                if (ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    // Remove button
                    {
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 48.0f);

                        std::string removeButtonLabel = "Remove##" + transition->GetName();
                        if (ImGui::Button(removeButtonLabel.c_str()))
                        {
                            if (!IsAlreadyAddedToReserve(transition->GetName()))
                            {
                                _reservedRemoveTransitions.emplace_back(transition);
                            }
                        }
                    }

                    transition->OnGui();

                    ImGui::TreePop();
                }
            }

            if (!_transitions.empty())
            {
                ImGui::Separator();
            }

            if (GuiUtils::ButtonCentered("Add Transition##Transitions", Vector2(240.0f, 24.0f)))
            {
                const auto transition = std::make_shared<Transition>();
                const auto transitionName = _name + "To_";
                transition->Create(ObtainProceedName(transitionName), shared_from_this());

                AddTransition(transition);
            }
        }
    }

    void BaseState::OnSerialized() const
    {
    }

    void BaseState::OnDeserialized()
    {
        
    }

    void BaseState::OnAnimatorControllerLoaded()
    {
        const AnimatorControllerLibrary* animatorControllerLibrary = LibraryManager::GetInstance().GetAnimatorControllerLibrary();

		const auto controller = animatorControllerLibrary->Find(_animatorControllerFilePath);
        if (controller.has_value())
        {
            _parent = controller.value();
        }
    }

    void BaseState::AddTransition(Ref<Transition> transition)
    {
        _transitions.emplace_back(transition);
    }

    std::string BaseState::ObtainProceedName(std::string_view name, const uint32_t maxTrialCount) const
    {
        auto proceedName = String(name);

        for (uint32_t trialCount = 1; trialCount <= maxTrialCount; ++trialCount)
        {
            const bool duplicated = IsTransitionNameDuplicated(proceedName.Get());

            if (!duplicated)
                break;

            {
                const uint32_t index = proceedName.FindLastOf("_");
                if (index != UINT32_MAX)
                {
                    proceedName = proceedName.ClipUntil(index).value();
                }

                proceedName.Append("_" + std::to_string(trialCount));
            }
        }

        return proceedName.Get();
    }

    std::optional<Ref<Transition>> BaseState::FindTransitionByIndex(uint32_t index) const
    {
        if (index >= _transitions.size())
        {
            DENTY_SET_ERR_COLOR
        	DENTY_ERR_LOG << "Index out of range!";

            return std::nullopt;
        }

        return _transitions.at(index);
    }

    std::optional<Ref<Transition>> BaseState::FindTransitionByName(std::string_view name) const
    {
        for (const auto& transition : _transitions)
        {
            if (transition->GetName() != name)
                continue;

            return transition;
        }

        return std::nullopt;
    }

    AnimatorController* BaseState::GetParent() const
    {
        if (!_parent.expired())
        {
            return _parent.lock().get();
        }

        return nullptr;
    }

    void BaseState::AnimatorStateAssetDragAndDropTargetCallback([[maybe_unused]] void* data, [[maybe_unused]] DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
    {

    }

    bool BaseState::IsTransitionNameDuplicated(std::string_view transitionName) const
    {
        for (const auto& transition : _transitions)
        {
            if (transition->GetName() != transitionName)
                continue;

            return true;
        }

        return false;
    }

    bool BaseState::IsAlreadyAddedToReserve(std::string_view transitionName) const
    {
        for (const auto& reservedTransition : _reservedRemoveTransitions)
        {
            if (reservedTransition->GetName() != transitionName)
                continue;

            return true;
        }

        return false;
    }
}
