#include "stdafx.h"

#include "Effect.h"

#include "./Module/EffectModuleRenderer.h"
#include "./Module/EffectModulePhysics.h"
#include "./Module/EffectModuleBehaviour.h"
#include "./Module/EffectModuleSetting.h"

#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"
#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
	EffectCPUParameters::EffectCPUParameters() :
        vertexBuffer(), vertices()
	{
	}

    EffectParameters::EffectParameters() :
        modules(), expectedEmitCount(), currentEmitCount()
    {
    }

    Effect::Effect() :
		Object(), _parameters()
    {
        _name = "Effect"; 
    }

    Effect::Effect(const Effect& source) :
        Object(source), _parameters(source._parameters)
    {
    }

    void Effect::Create()
    {
        CreateModules();
        CreateBuffers();
    }

    void Effect::OnUpdateCPU(const Matrix& world, float deltaTime)
    {
        {
            auto&& moduleBehaviourOptional = FindModuleByType(EffectModuleType::Behaviour);
            DENTY_CHECK_CONDITION(moduleBehaviourOptional.has_value());

            auto&& moduleBehaviour = std::static_pointer_cast<EffectModuleBehaviour>(moduleBehaviourOptional.value());
            moduleBehaviour->OnUpdateCPU(deltaTime);
        }

        {
            auto&& moduleRendererOptional = FindModuleByType(EffectModuleType::Renderer);
            DENTY_CHECK_CONDITION(moduleRendererOptional.has_value());

            auto&& moduleRenderer = std::static_pointer_cast<EffectModuleRenderer>(moduleRendererOptional.value());
            moduleRenderer->OnUpdateCPU(world);
        }
    }

    void Effect::OnUpdateGPU([[maybe_unused]] const Matrix& world, [[maybe_unused]] float deltaTime)
    {
        
    }

    void Effect::OnRenderCPU()
    {
        auto&& moduleRendererOptional = FindModuleByType(EffectModuleType::Renderer);
        DENTY_CHECK_CONDITION(moduleRendererOptional.has_value());

        {
            EffectModuleRendererCPUContext context = { };
            context.vertexBuffer = _parameters.cpuParameters.vertexBuffer.get();
            context.dxContext = GraphicsManager::GetInstance().GetContext();

            auto&& moduleRenderer = std::static_pointer_cast<EffectModuleRenderer>(moduleRendererOptional.value());
            moduleRenderer->OnRenderCPU(context);
        }
    }

    void Effect::OnRenderGPU()
    {

    }

    void Effect::OnSerialized() const
    {
        Object::OnSerialized();
    }

    void Effect::OnDeserialized()
    {
        Object::OnDeserialized();
    }

    std::optional<Ref<EffectModule>> Effect::FindModuleByType(EffectModuleType type) const
    {
        auto&& it = _parameters.modules.find(type);
        if (it != _parameters.modules.end())
        {
            return it->second;
        }

        return std::nullopt;
    }

    uint32_t Effect::GetExceptedEmitCount() const
    {
        return _parameters.expectedEmitCount;
    }

    uint32_t Effect::GetCurrentEmitCount() const
    {
        return _parameters.currentEmitCount;
    }

    void Effect::CreateModules()
    {
        // Create modules
        {
            const auto moduleRenderer = std::make_shared<EffectModuleRenderer>();
            const auto modulePhysics = std::make_shared<EffectModulePhysics>();
            const auto moduleBehaviour = std::make_shared<EffectModuleBehaviour>();
        	const auto moduleSetting = std::make_shared<EffectModuleSetting>();

            _parameters.modules.try_emplace(moduleRenderer->GetType(), moduleRenderer);
            _parameters.modules.try_emplace(modulePhysics->GetType(), modulePhysics);
            _parameters.modules.try_emplace(moduleBehaviour->GetType(), moduleBehaviour);
            _parameters.modules.try_emplace(moduleSetting->GetType(), moduleSetting);
        }
    }

    void Effect::CreateBuffers()
    {
        CreateCPUBuffers();
    }

    void Effect::CreateCPUBuffers()
    {
        // Get behaviour module.
        auto&& behaviourModuleOptional = FindModuleByType(EffectModuleType::Behaviour);
        DENTY_CHECK_CONDITION(behaviourModuleOptional.has_value());

        auto&& behaviourModule = std::static_pointer_cast<EffectModuleBehaviour>(behaviourModuleOptional.value());

        {
            using VertexType = DXVertexBuffer::VertexBufferInfo::VertexType;
            using AccessFlag = IDirectXBuffer::CpuAccessFlag;
            using Usage = IDirectXBuffer::Usage;
            using MapType = IDirectXBuffer::MapType;

            DXVertexBuffer::VertexBufferInfo vertexBufferInfo = {};
            vertexBufferInfo.usage = Usage::Dynamic;
            vertexBufferInfo.accessFlag = AccessFlag::AccessWrite;
            vertexBufferInfo.mapType = MapType::WriteDiscard;
            vertexBufferInfo.vertexType = VertexType::ParticleVertex;
            vertexBufferInfo.stride = sizeof(ParticleVertex);
            vertexBufferInfo.size = sizeof(ParticleVertex) * behaviourModule->GetExpectedParticleCountPerFrame();
            vertexBufferInfo.count = behaviourModule->GetExpectedParticleCountPerFrame();
            vertexBufferInfo.data = std::make_shared<ParticleVertex[]>(behaviourModule->GetExpectedParticleCountPerFrame());

            _parameters.cpuParameters.vertexBuffer = std::make_shared<DXVertexBuffer>();
            _parameters.cpuParameters.vertexBuffer->Create(vertexBufferInfo);
        }
    }
}
