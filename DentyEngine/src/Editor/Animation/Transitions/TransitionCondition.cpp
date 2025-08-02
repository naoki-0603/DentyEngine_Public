#include "stdafx.h"

#include "TransitionCondition.h"
#include "Transition.h"

#include "Editor/Animation/States/BaseState.h"

#include "System/Resource/Animations/AnimatorController.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
    const std::array<std::string, 6> TransitionCondition::INT_OPERATORS_STRINGS =
    {
	    "Equals", // ==
        "NotEquals", // !=
        "Greater", // >
        "Less", // <
        "GreaterThan", // >=
        "LessThan" // <=
    };

    const std::array<std::string, 4> TransitionCondition::FLOAT_OPERATORS_STRINGS =
    {
        "Greater", // >
        "Less", // <
        "GreaterThan", // >=
        "LessThan" // <=
    };

    const std::array<std::string, 2> TransitionCondition::BOOL_OPERATORS_STRINGS =
    {
        "Equals", // ==
        "NotEquals" // !=
    };

    TransitionCondition::TransitionCondition() :
		_parent(), _operators(), _value(), _name()
    {
    }

    void TransitionCondition::Create(Operators operators, const AnimatorValue& value, std::string_view parameterName, Ref<Transition> parent)
    {
        _operators = operators;
        _value = value;
        _name = parameterName;
        _parent = parent;
    }

    void TransitionCondition::OnGui(uint32_t index)
    {
        const AnimatorController* controller = GetParent()->GetParent()->GetParent();

        // Parameter name.
        {
            const auto parameterNames = controller->ObtainParameterNames();
            const auto unusedParameterNames = GetParent()->ObtainUnusedParameterNames(parameterNames);
            const std::string label = "##TransitionCondition" + std::to_string(index);

            if (GuiUtils::ComboBoxGui(label, unusedParameterNames.data(), static_cast<int32_t>(unusedParameterNames.size()), _name))
            {

            }
        }

        // Value type.
        {
            const auto valueType = controller->FindParameterType(_name);
            if (valueType.has_value())
            {
                using Type = AnimatorValue::Type;

                switch (valueType.value())
                {
                case Type::Int:
                    IntTypeGui(index);
                    break;
                case Type::Float:
                    FloatTypeGui(index);
                    break;
                case Type::Bool:
                    BoolTypeGui(index);
                    break;
                case Type::Trigger:
                    TriggerTypeGui(index);
                    break;
                }
            }
        }
    }

    void TransitionCondition::OnSerialized() const
    {
    }

    void TransitionCondition::OnDeserialized()
    {
    }

    bool TransitionCondition::Compare(int32_t value) const
    {
        switch (_operators)
        {
        case Operators::Equals:
            return (value == _value.valueInt);
        case Operators::NotEquals:
            return (value != _value.valueInt);
        case Operators::Greater:
            return (value > _value.valueInt);
        case Operators::Less:
            return (value < _value.valueInt);
        case Operators::GreaterThan:
            return (value >= _value.valueInt);
        case Operators::LessThan:
            return (value <= _value.valueInt);
        }

        DENTY_WARN_CONSOLE_LOG("Unknown operators type. (in TransitionCondition::Compare)");

        return false;
    }

    bool TransitionCondition::Compare(float value) const
    {
        switch (_operators)
        {
        case Operators::Equals:
	        {
				DENTY_WARN_CONSOLE_LOG("Not supported operators type! (in TransitionCondition::Compare)");

                return false;
	        }
        case Operators::NotEquals:
	        {
				DENTY_WARN_CONSOLE_LOG("Not supported operators type! (in TransitionCondition::Compare)");

                return false;
	        }
        case Operators::Greater:
            return (value > _value.valueFloat);
        case Operators::Less:
            return (value < _value.valueFloat);
        case Operators::GreaterThan:
            return (value >= _value.valueFloat);
        case Operators::LessThan:
            return (value <= _value.valueFloat);
        }

        DENTY_WARN_CONSOLE_LOG("Unknown operators type. (in TransitionCondition::Compare)");

        return false;
    }

    bool TransitionCondition::Compare(bool value) const
    {
        return (value == _value.valueBool);
    }

    Transition* TransitionCondition::GetParent() const
    {
        if (!_parent.expired())
        {
            return _parent.lock().get();
        }

        return nullptr;
    }

    void TransitionCondition::IntTypeGui(uint32_t index)
    {
        // Operators
        {
            const std::string label = "##IntTypeGui" + std::to_string(index);
            if (GuiUtils::ComboBoxGui(label, INT_OPERATORS_STRINGS.data(), static_cast<int32_t>(INT_OPERATORS_STRINGS.size()), _operatorType))
            {
            }
        }

        // Input
        {
            const std::string label = "##IntTypeGui_InputInt" + std::to_string(index);
            ImGui::InputInt(label.c_str(), &_value.valueInt);
        }
    }

    void TransitionCondition::FloatTypeGui(uint32_t index)
    {
        {
            const std::string label = "Operators##FloatTypeGui" + std::to_string(index);
            if (GuiUtils::ComboBoxGui(label, FLOAT_OPERATORS_STRINGS.data(), static_cast<int32_t>(FLOAT_OPERATORS_STRINGS.size()), _operatorType))
            {

            }
        }

        // Input
        {
            const std::string label = "##FloatTypeGui_InputFloat" + std::to_string(index);
            ImGui::InputFloat(label.c_str(), &_value.valueFloat);
        }
    }

    void TransitionCondition::BoolTypeGui(uint32_t index)
    {
        {
            const std::string label = "Operators##BoolTypeGui" + std::to_string(index);
            if (GuiUtils::ComboBoxGui(label, BOOL_OPERATORS_STRINGS.data(), static_cast<int32_t>(BOOL_OPERATORS_STRINGS.size()), _operatorType))
            {
            }
        }

        // Input
        {
            const std::string label = "##BoolTypeGui_InputBool" + std::to_string(index);
            ImGui::Checkbox(label.c_str(), &_value.valueBool);
            ImGui::SameLine();
            ImGui::Text("%s", _value.valueBool ? "true" : "false");
        }
    }

    void TransitionCondition::TriggerTypeGui([[maybe_unused]] uint32_t index)
    {
        // Nothing to do...
    }
}