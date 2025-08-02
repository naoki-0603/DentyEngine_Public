#pragma once

#include "BaseState.h"

#include "System/Core/DFilePath.h"

namespace DentyEngine
{
    namespace MeshResource
    {
        class AnimationClip;
    }

    class AnimationState final : public BaseState
    {
    public:
        // For serialize.
        AnimationState();

        AnimationState(std::string_view name, Ref<AnimatorController> parent);
        ~AnimationState() override = default;

        void OnGui() override;

        void OnSerialized() const override;
        void OnDeserialized() override;

        //
        // Setter
        //
        void SetAnimationClip(Ref<MeshResource::AnimationClip> clip);

        //
        // Getter
        //

        [[nodiscard]]
        const Ref<MeshResource::AnimationClip>& GetAnimationClip() const { return _clip; }

        [[nodiscard]]
        const char* GetStateType() override { return "AnimationState"; }

        [[nodiscard]]
        bool IsBuildInState() const override { return false; }

        [[nodiscard]]
        bool IsAllowedSetAsTransitionDestination() const override { return true; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<BaseState>(this),
                    CEREAL_NVP(_clipFilePath),
                    CEREAL_NVP(_oneShot)
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
                    cereal::base_class<BaseState>(this),
                    CEREAL_NVP(_clipFilePath),
                    CEREAL_NVP(_oneShot)
                );    
            }

            OnDeserialized();
        }
    private:
        // Doesn't serialize.
        Ref<MeshResource::AnimationClip> _clip;

        // Serialize instead of _clip.
        FilePath _clipFilePath;

        bool _oneShot;
    };
}

CEREAL_CLASS_VERSION(DentyEngine::AnimationState, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::AnimationState)