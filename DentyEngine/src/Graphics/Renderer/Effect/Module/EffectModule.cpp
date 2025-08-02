#include "stdafx.h"

#include "EffectModule.h"

namespace DentyEngine
{
    EffectModule::EffectModule() :
        _name(), _enabled()
    {
    }

    EffectModule::EffectModule(const EffectModule& source) :
        _name(source._name), _enabled(source._enabled)
    {
    }

    const std::string& EffectModule::GetName() const
    {
        return _name;
    }

    bool EffectModule::GetEnabled() const
    {
        return _enabled;
    }
}
