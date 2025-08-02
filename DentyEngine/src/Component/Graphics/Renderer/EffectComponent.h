#pragma once

#include "Component/Behaviour.h"
#include "Component/ComponentGenerator.h"

#include "Graphics/Renderer/Effect/Effect.h"

namespace DentyEngine
{
    class EffectComponent final : public Component, public std::enable_shared_from_this<EffectComponent>
    {
    public:
        EffectComponent();
        EffectComponent(const EffectComponent& source);
        ~EffectComponent() override = default;

        void Awake() override;
        void Start() override;

        void OnUpdateCPU(float deltaTime);
        void OnUpdateGPU(float deltaTime);

        void OnRenderCPU();
        void OnRenderGPU();

        void OnGui() override;
        void OnPrefabGui() override;

        void OnEvent(Event* e) override;

        void OnSerialized() const override;
        void OnDeserialized() override;
        void OnSceneDeserialized() override;

        void OnSceneActivated();
        void OnSceneDeActivated();

        //
        // Getter
        //
        [[nodiscard]]
        Ref<Effect> GetEffect() const;

        DENTY_REGISTER_BUILD_IN_COMPONENT(EffectComponent)
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Component>(this),
                    CEREAL_NVP(_effect)
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
                    cereal::base_class<Component>(this),
                    CEREAL_NVP(_effect)
                );    
            }

            OnDeserialized();
        }
    private:
        Ref<Effect> _effect;
    };
}
CEREAL_CLASS_VERSION(DentyEngine::EffectComponent, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::EffectComponent)