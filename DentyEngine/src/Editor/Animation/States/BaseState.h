#pragma once

#include "System/Editor/DragAndDrop/DragAndDrop.h"

#include "Editor/Animation/Transitions/Transition.h"

namespace DentyEngine
{
    class Transition;

    class AnimatorController;

    class BaseState : public std::enable_shared_from_this<BaseState>
    {
    public:
        // For serialize.
        BaseState();

        BaseState(std::string_view name, Ref<AnimatorController> parent);
        virtual ~BaseState() = default;

        virtual void OnUpdate();

        virtual void OnGui();

        virtual void OnSerialized() const;
        virtual void OnDeserialized();
        virtual void OnAnimatorControllerLoaded();

    	void AddTransition(Ref<Transition> transition);

        [[nodiscard]]
        std::string ObtainProceedName(std::string_view name, const uint32_t maxTrialCount = 100u) const;

        [[nodiscard]]
        std::optional<Ref<Transition>> FindTransitionByIndex(uint32_t index) const;

        [[nodiscard]]
        std::optional<Ref<Transition>> FindTransitionByName(std::string_view name) const;

        [[nodiscard]]
        AnimatorController* GetParent() const;

        [[nodiscard]]
        const std::string& GetName() const { return _name; }

        [[nodiscard]]
        virtual const char* GetStateType() = 0;

        [[nodiscard]]
        virtual bool IsBuildInState() const = 0;

        [[nodiscard]]
        virtual bool IsAllowedSetAsTransitionDestination() const = 0;
    protected:
        std::weak_ptr<AnimatorController> _parent;

    	std::vector<Ref<Transition>> _transitions;

        FilePath _animatorControllerFilePath;

        std::string _name;
    private:
        //
        // Drag and drop receive functions.
        //
        void AnimatorStateAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);

        [[nodiscard]]
        bool IsTransitionNameDuplicated(std::string_view transitionName) const;

        [[nodiscard]]
        bool IsAlreadyAddedToReserve(std::string_view transitionName) const;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(_transitions),
                    CEREAL_NVP(_animatorControllerFilePath),
                    CEREAL_NVP(_name)
                );    
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(_transitions),
                    CEREAL_NVP(_animatorControllerFilePath),
                    CEREAL_NVP(_name)
                );    
            }
        }
    private:
        std::vector<Ref<Transition>> _reservedRemoveTransitions;
    };
}

CEREAL_CLASS_VERSION(DentyEngine::BaseState, 1u)