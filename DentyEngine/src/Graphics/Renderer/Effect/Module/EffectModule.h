#pragma once

namespace DentyEngine
{
    enum class EffectModuleType : int32_t
    {
        None = -1,
        Renderer = 0,
        Physics,
        Setting,
        Behaviour,

        Max
    };

    class EffectModule
    {
    public:
        EffectModule();
        EffectModule(const EffectModule& source);
        virtual ~EffectModule() = default;

        virtual void Initialize() = 0;

        virtual void OnGui() = 0;

        [[nodiscard]]
        const std::string& GetName() const;

        [[nodiscard]]
        virtual EffectModuleType GetType() const = 0;

        [[nodiscard]]
        virtual bool GetEnabled() const;
    protected:
        std::string _name;

        bool _enabled;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
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
                    CEREAL_NVP(_name),
                    CEREAL_NVP(_enabled)
                );
            }
        }
    };
}

CEREAL_CLASS_VERSION(DentyEngine::EffectModule, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::EffectModule)