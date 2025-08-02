#pragma once

#include "../AnimatorData.h"

namespace DentyEngine
{
    class Transition;

    class TransitionCondition final
    {
    public:
        enum class Operators
        {
            None = -1,
	        Equals = 0, // ==
            NotEquals, // !=
            Greater, // >
            Less, // <
            GreaterThan, // >=
            LessThan, // <=

            Max
        };
    public:
        TransitionCondition();
        ~TransitionCondition() = default;

        void Create(Operators operators, const AnimatorValue& value, std::string_view parameterName, Ref<Transition> parent);

        void OnGui(uint32_t index);
        void OnSerialized() const;
        void OnDeserialized();

        //
        // Setter
        //
        void SetParent(Ref<Transition> parent) { _parent = parent; }

        //
        // Getter
        //
        [[nodiscard]]
        bool Compare(int32_t value) const;

        [[nodiscard]]
        bool Compare(float value) const;

        [[nodiscard]]
        bool Compare(bool value) const;

        [[nodiscard]]
        Transition* GetParent() const;

        [[nodiscard]]
        Operators GetOperators() const { return _operators; }

        [[nodiscard]]
        AnimatorValue::Type GetType() const { return _value.valueType; }

        [[nodiscard]]
        int32_t GetInt() const { return _value.valueInt; }

        [[nodiscard]]
        float GetFloat() const { return _value.valueFloat; }

        [[nodiscard]]
        bool GetBool() const { return _value.valueBool; }

        [[nodiscard]]
        const std::string& GetName() const { return _name; }
    private:
        void IntTypeGui(uint32_t index);
        void FloatTypeGui(uint32_t index);
        void BoolTypeGui(uint32_t index);
        void TriggerTypeGui(uint32_t index);
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(_operators),
                    CEREAL_NVP(_value),
                    CEREAL_NVP(_name)
                );    
            }

            OnSerialized();
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(_operators),
                    CEREAL_NVP(_value),
                    CEREAL_NVP(_name)
                );    
            }

            OnDeserialized();
        }
    private:
        std::weak_ptr<Transition> _parent;

        Operators _operators;

        AnimatorValue _value;

        // Should be same as parameter name.
        std::string _name;

        // Value
        std::string _operatorType;

    private:
        //
        // Static variables.
        //
        static const std::array<std::string, 6> INT_OPERATORS_STRINGS;
        static const std::array<std::string, 4> FLOAT_OPERATORS_STRINGS;
        static const std::array<std::string, 2> BOOL_OPERATORS_STRINGS;
    };
}

CEREAL_CLASS_VERSION(DentyEngine::TransitionCondition, 1u)