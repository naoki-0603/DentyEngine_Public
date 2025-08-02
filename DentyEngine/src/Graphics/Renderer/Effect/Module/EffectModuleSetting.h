#pragma once

#include "EffectModule.h"

namespace DentyEngine
{
    class EffectModuleSetting final : public EffectModule
    {
    public:
        EffectModuleSetting();
        EffectModuleSetting(const EffectModuleSetting& source);
        ~EffectModuleSetting() override = default;

        void Initialize() override;

        void OnGui() override;

        //
        // Getter
        //

        [[nodiscard]]
        EffectModuleType GetType() const override;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<EffectModule>(this),
                    CEREAL_NVP(_name),
                    CEREAL_NVP(_enabled)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<EffectModule>(this),
                    CEREAL_NVP(_name),
                    CEREAL_NVP(_enabled)
                );
            }
        }
    };
}

CEREAL_CLASS_VERSION(DentyEngine::EffectModuleSetting, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::EffectModuleSetting)