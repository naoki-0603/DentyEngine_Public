#include "stdafx.h"

#include "ParticleSystem.h"

#include "Component/Graphics/Renderer/EffectComponent.h"

#include "Graphics/Renderer/Effect/Module/EffectModuleRenderer.h"

namespace DentyEngine
{
	ParticleSystemConstants::ParticleSystemConstants()
	{
	}

    ParticleSystemParameters::ParticleSystemParameters() :
        effects(), hardwareType(ParticleSystemHardwareType::CPU)
    {

    }

    ParticleSystemParameters ParticleSystem::_parameters = { };

    ParticleSystem::ParticleSystem()
    {
    }

    ParticleSystem::~ParticleSystem()
    {
    }

    void ParticleSystem::Initialize()
    {
        switch (_parameters.hardwareType)
        {
        case ParticleSystemHardwareType::CPU:
            InitializeCPU();
            break;
        case ParticleSystemHardwareType::GPU:
            break;
        default: [[unlikely]]
                   break;
        }
    }

    void ParticleSystem::Update(float deltaTime)
    {
        switch (_parameters.hardwareType)
        {
        case ParticleSystemHardwareType::CPU:
            UpdateCPU(deltaTime);
            break;
        case ParticleSystemHardwareType::GPU:
            break;
        default: [[unlikely]]
            break;
        }
    }

    void ParticleSystem::Render()
    {
        switch (_parameters.hardwareType)
        {
        case ParticleSystemHardwareType::CPU:
            RenderCPU();
            break;
        case ParticleSystemHardwareType::GPU:
            break;
        default: [[unlikely]]
                   break;
        }
    }

    void ParticleSystem::Finalize()
    {
        switch (_parameters.hardwareType)
        {
        case ParticleSystemHardwareType::CPU:
            FinalizeCPU();
            break;
        case ParticleSystemHardwareType::GPU:
            break;
        default: [[unlikely]]
             break;
        }
    }

    void ParticleSystem::OnGui()
    {

    }

    void ParticleSystem::Add(Ref<EffectComponent> effect)
    {
        auto&& moduleRendererOptional = effect->GetEffect()->FindModuleByType(EffectModuleType::Renderer);
        DENTY_CHECK_CONDITION(moduleRendererOptional.has_value());

        auto&& moduleRenderer = std::static_pointer_cast<EffectModuleRenderer>(moduleRendererOptional.value());

        _parameters.effects.at(
            static_cast<uint32_t>(moduleRenderer->GetRenderingOrder())
        ).emplace_back(effect);
    }

    void ParticleSystem::Remove(Ref<EffectComponent> effect)
    {
        auto&& moduleRendererOptional = effect->GetEffect()->FindModuleByType(EffectModuleType::Renderer);
        DENTY_CHECK_CONDITION(moduleRendererOptional.has_value());

        auto&& moduleRenderer = std::static_pointer_cast<EffectModuleRenderer>(moduleRendererOptional.value());

        std::ranges::remove(
            _parameters.effects.at(static_cast<uint32_t>(moduleRenderer->GetRenderingOrder())).begin(),
            _parameters.effects.at(static_cast<uint32_t>(moduleRenderer->GetRenderingOrder())).end(),
            effect
        );
    }

    ParticleSystemHardwareType ParticleSystem::GetHardwareType()
    {
        return _parameters.hardwareType;
    }

    void ParticleSystem::InitializeCPU()
    {
    }

    void ParticleSystem::UpdateCPU(float deltaTime)
    {
        uint32_t currentFrameEmitTotal = 0u;
        for (uint32_t orderIndex = 0u; orderIndex < static_cast<uint32_t>(EffectRenderingOrder::Max); ++orderIndex)
        {
	        for (uint32_t effectIndex = 0u; effectIndex < _parameters.effects.at(orderIndex).size(); ++effectIndex)
	        {
                auto&& effectComponent = _parameters.effects.at(orderIndex).at(effectIndex);
                if (not effectComponent->Enabled())
                    continue;

	        	auto&& effect = effectComponent->GetEffect();

                // Check is over.
	        	if (currentFrameEmitTotal + effect->GetExceptedEmitCount() > ParticleSystemConstants::MAX_EMIT_NUM_CPU)
                    return;

                currentFrameEmitTotal += effect->GetExceptedEmitCount();

                effectComponent->OnUpdateCPU(deltaTime);
	        }
        }
    }

    void ParticleSystem::RenderCPU()
    {
	    for (uint32_t orderIndex = 0u; orderIndex < static_cast<uint32_t>(EffectRenderingOrder::Max); ++orderIndex)
	    {
		    for (uint32_t effectIndex = 0u; effectIndex < _parameters.effects.at(orderIndex).size(); ++effectIndex)
		    {
                auto&& effectComponent = _parameters.effects.at(orderIndex).at(effectIndex);
                if (not effectComponent->Enabled())
                    continue;

                effectComponent->OnRenderCPU();
		    }
	    }
    }

    void ParticleSystem::FinalizeCPU()
    {
    }
}
