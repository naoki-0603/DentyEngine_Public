#pragma once

#include "Behaviour.h"

namespace DentyEngine
{
    // This class for physics components.
    class PhysicsBehaviour : public Behaviour
    {
    public:
        PhysicsBehaviour();
        PhysicsBehaviour(const PhysicsBehaviour& source);
        ~PhysicsBehaviour() override = default;

        void Awake() override;
        void Start() override;
        void OnUpdate(float deltaTime) override;
        void OnFixedUpdate() override;

        void OnGui() override;
        void OnPrefabGui() override;
    	void OnEvent(Event* e) override;

        void OnDestroy() override;
        void Destroy() override;

        void OnSerialized() const override;
        void OnDeserialized() override;
        void OnGameObjectDeserialized() override;

        [[nodiscard]]
        Ref<Object> MakeCopy() const override;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Behaviour>(this)
                );    
            }
        }

        template <class Archive>
        void load(Archive& archive, uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Behaviour>(this)
                );    
            }
        }
    };
}

CEREAL_CLASS_VERSION(DentyEngine::PhysicsBehaviour, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::PhysicsBehaviour)