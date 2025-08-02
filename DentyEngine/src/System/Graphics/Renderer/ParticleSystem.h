#pragma once

namespace DentyEngine
{
    class EffectComponent;

    enum class ParticleSystemHardwareType : int32_t
    {
        None = -1,
        CPU = 0,
        GPU,

        Max
    };

    struct ParticleSystemConstants
    {
    public:
        ParticleSystemConstants();

    public:
        static constexpr uint16_t MAX_EMIT_NUM_CPU = 10000u;
        static constexpr uint32_t MAX_EMIT_NUM_GPU = 1000000u;
    };

    struct ParticleGPUParameters
    {

    };

    struct ParticleSystemParameters
    {
    public:
        ParticleSystemParameters();

    public:
        std::vector<std::vector<Ref<EffectComponent>>> effects;

        ParticleSystemHardwareType hardwareType;
    };

    class ParticleSystem final
    {
    public:
        ParticleSystem();
        ~ParticleSystem();

        static void Initialize();
        static void Update(float deltaTime);
        static void Render();
    	static void Finalize();

        static void OnGui();

        static void Add(Ref<EffectComponent> effect);
        static void Remove(Ref<EffectComponent> effect);

        //
        // Getter
        //
        [[nodiscard]]
        static ParticleSystemHardwareType GetHardwareType() ;
    private:
        // CPU
        static void InitializeCPU();
        static void UpdateCPU(float deltaTime);
        static void RenderCPU();
        static void FinalizeCPU();

        // GPU

    private:
        static ParticleSystemParameters _parameters;
    };
}