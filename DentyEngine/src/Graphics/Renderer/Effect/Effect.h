#pragma once

#include "System/Core/Object.h"

#include "Module/EffectModule.h"

#include "Graphics/Vertex.h"

namespace DentyEngine
{
    class DXVertexBuffer;

	struct EffectCPUParameters
    {
    public:
        EffectCPUParameters();

    public:
        Ref<DXVertexBuffer> vertexBuffer;

		std::vector<ParticleVertex> vertices;
    };

    struct EffectParameters
    {
    public:
        EffectParameters();

    public:
    	std::unordered_map<EffectModuleType, Ref<EffectModule>> modules;

        uint32_t expectedEmitCount;
        uint32_t currentEmitCount;

        EffectCPUParameters cpuParameters;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(modules)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(modules)
                );
            }
        }
    };

    class Effect final : public Object
    {
    public:
        Effect();
        Effect(const Effect& source);
        ~Effect() override = default;

        void Create();

        void OnUpdateCPU(const Matrix& world, float deltaTime);
        void OnUpdateGPU(const Matrix& world, float deltaTime);

        void OnRenderCPU();
        void OnRenderGPU();

        void OnSerialized() const override;
        void OnDeserialized() override;

        //
        // Getter
        //
        [[nodiscard]]
        std::optional<Ref<EffectModule>> FindModuleByType(EffectModuleType type) const;

        [[nodiscard]]
        uint32_t GetExceptedEmitCount() const;

        [[nodiscard]]
        uint32_t GetCurrentEmitCount() const;
    private:
        void CreateModules();
        void CreateBuffers();
        void CreateCPUBuffers();

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Object>(this),
                    CEREAL_NVP(_parameters)
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
                    cereal::base_class<Object>(this),
                    CEREAL_NVP(_parameters)
                );
            }

            OnDeserialized();
        }
    private:
        EffectParameters _parameters;
    };
}
