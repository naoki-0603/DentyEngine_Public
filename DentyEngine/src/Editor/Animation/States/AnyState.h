#pragma once

#include "BaseState.h"

namespace DentyEngine
{
    class AnyState final : public BaseState
    {
    public:
        // For serialize
        AnyState();

        AnyState(Ref<AnimatorController> parent);
        ~AnyState() override = default;

        void OnGui() override;

        [[nodiscard]]
        const char* GetStateType() override { return "AnyState"; }

        [[nodiscard]]
        bool IsBuildInState() const override { return true; }

        [[nodiscard]]
        bool IsAllowedSetAsTransitionDestination() const override { return false; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                cereal::base_class<BaseState>(this)
            );

            OnSerialized();
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                cereal::base_class<BaseState>(this)
            );

            OnDeserialized();
        }
    };
}

CEREAL_CLASS_VERSION(DentyEngine::AnyState, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::AnyState)