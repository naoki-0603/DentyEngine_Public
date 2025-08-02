#include "stdafx.h"

#include "EffectComponent.h"

#include "Scene/GameObject.h"
#include "System/Graphics/Renderer/ParticleSystem.h"

namespace DentyEngine
{
    EffectComponent::EffectComponent() :
        Component(), _effect()
    {
        _name = "EffectComponent";
    }

    EffectComponent::EffectComponent(const EffectComponent& source) :
        Component(source), _effect(source._effect)
    {
    }

    void EffectComponent::Awake()
    {
        Component::Awake();
    }

    void EffectComponent::Start()
    {
        Component::Start();
    }

    void EffectComponent::OnUpdateCPU(float deltaTime)
    {
        _effect->OnUpdateCPU(GetParent()->transform.world, deltaTime);
    }

    void EffectComponent::OnUpdateGPU(float deltaTime)
    {
        _effect->OnUpdateGPU(GetParent()->transform.world, deltaTime);
    }

    void EffectComponent::OnRenderCPU()
    {
        _effect->OnRenderCPU();
    }

    void EffectComponent::OnRenderGPU()
    {
        _effect->OnRenderGPU();
    }

    void EffectComponent::OnGui()
    {
        Component::OnGui();
    }

    void EffectComponent::OnPrefabGui()
    {
    }

    void EffectComponent::OnEvent(Event* e)
    {
        Component::OnEvent(e);
    }

    void EffectComponent::OnSerialized() const
    {
        Component::OnSerialized();
    }

    void EffectComponent::OnDeserialized()
    {
        Component::OnDeserialized();
    }

    void EffectComponent::OnSceneDeserialized()
    {
        Component::OnSceneDeserialized();
    }

    void EffectComponent::OnSceneActivated()
    {
        ParticleSystem::Add(shared_from_this());
    }

    void EffectComponent::OnSceneDeActivated()
    {
        ParticleSystem::Remove(shared_from_this());
    }

    Ref<Effect> EffectComponent::GetEffect() const
    {
        return _effect;
    }
}
