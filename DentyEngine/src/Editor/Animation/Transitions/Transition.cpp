#include "stdafx.h"

#include "Transition.h"

#include "Editor/Animation/States/BaseState.h"

#include "System/Resource/Animations/AnimatorController.h"

namespace DentyEngine
{
    Transition::Transition() :
        _name(), _originalName(), _parent(),
        _transitionToState(), _conditions(), _reservedRemoveConditions(), _setStateButtonPressed()
    {
    }

    void Transition::Create(std::string_view name, Ref<BaseState> parent)
    {
        _name = name;
        _originalName = name;
        _parent = parent;
    }

    void Transition::Create(std::string_view name, const std::vector<TransitionCondition>& conditions, Ref<BaseState> parent)
    {
        _name = name;
        _originalName = name;
        _conditions = conditions;
        _parent = parent;
    }

    void Transition::AddCondition(const TransitionCondition& condition)
    {
        _conditions.emplace_back(condition);
    }

    void Transition::OnUpdate()
    {
        // Remove reserved conditions.
        {
            for (auto&& reservedCondition : _reservedRemoveConditions)
            {
                uint32_t targetIndex = static_cast<uint32_t>(-1);
                for (uint32_t index = 0; index < _conditions.size(); ++index)
                {
                    const auto& condition = _conditions.at(index);

                    if (condition.GetName() != reservedCondition.GetName())
                        continue;

                    targetIndex = index;
                    break;
                }

                if (targetIndex != UINT32_MAX)
                {
                    _conditions.erase(_conditions.cbegin() + targetIndex);
                }

                DENTY_INFO_CONSOLE_LOG("Succeed to remove %s transition condition!", reservedCondition.GetName().c_str());
            }
            _reservedRemoveConditions.clear();
        }
    }

    void Transition::OnGui()
    {
        const AnimatorController* controller = GetParent()->GetParent();
        if (!controller)
            return;

        if (_transitionToState)
        {
            ImGui::Text("State: %s", _transitionToState->GetName().c_str());
        }
        // Set state to transition.
        else
        {
            if (ImGui::Button("SetState##Transition"))
            {
                _setStateButtonPressed = true;
            }

            if (_setStateButtonPressed)
            {
                if (ImGui::BeginPopupContextItem("##SetState", ImGuiPopupFlags_MouseButtonLeft))
                {
                    const auto stateIndex = controller->OnStatesGui(_setStateButtonPressed);
                    if (stateIndex.has_value())
                    {
                        const auto state = controller->FindStateByName(stateIndex.value());

                        SetState(state);
                    }

                    ImGui::EndPopup();
                }
            }
        }

        ImGui::Separator();

        // Add condition.
        {
            for (uint32_t index = 0; index < _conditions.size(); ++index)
            {
                auto&& condition = _conditions.at(index);
                condition.OnGui(index);

                // Remove button
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 48.0f);

                    const auto label = "Remove##TransitionConditions_" + std::to_string(index);
                    if (ImGui::Button(label.c_str()))
                    {
                        if (!IsAlreadyAddedToReserve(condition.GetName()))
                        {
                            _reservedRemoveConditions.emplace_back(condition);
                        }
                    }
                }

                ImGui::Separator();
            }

            if (ImGui::Button("AddCondition##Transition"))
            {
                {
                    TransitionCondition condition = { };
                    AnimatorValue value = { };
                    value.valueType = AnimatorValue::Type::Int;
                    value.valueInt = 0;
                    condition.Create(TransitionCondition::Operators::Equals, value, "NoName", shared_from_this());

                    AddCondition(condition);
                }
            }
        }
    }

    void Transition::OnSerialized() const
    {
    }

    void Transition::OnDeserialized()
    {
        // Set parent
        for (auto&& condition : _conditions)
        {
            condition.SetParent(shared_from_this());
        }
    }

    void Transition::SetState(Ref<BaseState> state)
    {
        if (_transitionToState)
        {
            DENTY_WARN_CONSOLE_LOG("Already %s state added!", state->GetName().c_str());

            return;
        }

        if (state->GetName() == GetParent()->GetName())
        {
            DENTY_WARN_CONSOLE_LOG("Can not add same state to _transitionToStates");

            return;
        }

        _transitionToState = state;

        OnStateChanged();
    }

    BaseState* Transition::GetParent() const
    {
        if (!_parent.expired())
        {
            return _parent.lock().get();
        }

        return nullptr;
    }

    std::vector<std::string> Transition::ObtainUnusedParameterNames(const std::vector<std::string>& parameterNames) const
    {
        std::vector<std::string> conditionNames = { };
        for (const auto& condition : _conditions)
        {
            const std::string& name = condition.GetName();
            if (!name.empty())
            {
                conditionNames.emplace_back(name);
            }
        }

        std::vector<std::string> unusedParameterNames = { };
        for (const auto& parameterName : parameterNames)
        {
            bool isDuplicated = false;
            for (const auto& conditionName : conditionNames)
            {
                if (conditionName != parameterName)
                    continue;

                isDuplicated = true;
            }

            if (!isDuplicated)
            {
                unusedParameterNames.emplace_back(parameterName);
            }
        }

        return unusedParameterNames;
    }

    void Transition::OnStateChanged()
    {
        _name = _originalName;

        // Rename.
        _name += _transitionToState->GetName();
    }

    bool Transition::IsAlreadyAddedToReserve(std::string_view conditionName) const
    {
        for (const auto& reservedCondition : _reservedRemoveConditions)
        {
            if (reservedCondition.GetName() != conditionName)
                continue;

            return true;
        }

        return false;
    }
}
