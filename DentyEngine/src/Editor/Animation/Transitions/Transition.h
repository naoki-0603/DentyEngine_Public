#pragma once

#include "TransitionCondition.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
    class Transition final : public std::enable_shared_from_this<Transition>
    {
    public:
        Transition();
        ~Transition() = default;

        void Create(std::string_view name, Ref<BaseState> parent);
        void Create(std::string_view name, const std::vector<TransitionCondition>& conditions, Ref<BaseState> parent);

        void AddCondition(const TransitionCondition& condition);

        void OnUpdate();
        void OnGui();
        void OnSerialized() const;
        void OnDeserialized();

        //
        // Setter
        //
        void SetName(std::string_view name) { _name = name; }
        void SetOriginalName(std::string_view name) { _originalName = name; }
        void SetState(Ref<BaseState> state);

        //
        // Getter
        //
        [[nodiscard]]
        BaseState* GetParent() const;

        // Call on TransitionCondition::OnGui function.
        [[nodiscard]]
        std::vector<std::string> ObtainUnusedParameterNames(const std::vector<std::string>& parameterNames) const;

        [[nodiscard]]
        const std::string& GetName() const { return _name; }

        [[nodiscard]]
        const std::string& GetOriginalName() const { return _name; }
    private:
        void OnStateChanged();

        [[nodiscard]]
        bool IsAlreadyAddedToReserve(std::string_view conditionName) const;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(_name),
                    CEREAL_NVP(_originalName),
                    CEREAL_NVP(_parent),
                    CEREAL_NVP(_transitionToState),
                    CEREAL_NVP(_conditions)
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
                    CEREAL_NVP(_name),
                    CEREAL_NVP(_originalName),
                    CEREAL_NVP(_parent),
                    CEREAL_NVP(_transitionToState),
                    CEREAL_NVP(_conditions)
                );    
            }

            OnDeserialized();
        }
    private:
        std::string _name;
        std::string _originalName;

        std::weak_ptr<BaseState> _parent;

        Ref<BaseState> _transitionToState;

    	std::vector<TransitionCondition> _conditions;

        // Doesn't serialize, use for remove.
        std::vector<TransitionCondition> _reservedRemoveConditions;

        bool _setStateButtonPressed;
    };
}

CEREAL_CLASS_VERSION(DentyEngine::Transition, 1u)