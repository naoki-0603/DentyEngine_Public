#include "stdafx.h"

#include "EffectModulePhysics.h"

namespace DentyEngine
{
    EffectModulePhysics::EffectModulePhysics() :
        EffectModule()
    {
        _name = "EffectModulePhysics";
        _enabled = true;
    }

    EffectModulePhysics::EffectModulePhysics(const EffectModulePhysics& source) :
        EffectModule(source)
    {
    }

    void EffectModulePhysics::Initialize()
    {

    }

    void EffectModulePhysics::OnGui()
    {
    }

    EffectModuleType EffectModulePhysics::GetType() const
    {
        return EffectModuleType::Physics;
    }
}
