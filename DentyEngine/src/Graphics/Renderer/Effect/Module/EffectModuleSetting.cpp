#include "stdafx.h"

#include "EffectModuleSetting.h"

namespace DentyEngine
{
    EffectModuleSetting::EffectModuleSetting() :
        EffectModule()
    {
        _name = "EffectModuleSetting";
        _enabled = true;
    }

    EffectModuleSetting::EffectModuleSetting(const EffectModuleSetting& source) :
        EffectModule(source)
    {
    }

    void EffectModuleSetting::Initialize()
    {

    }

    void EffectModuleSetting::OnGui()
    {

    }

    EffectModuleType EffectModuleSetting::GetType() const
    {
        return EffectModuleType::Setting;
    }
}
