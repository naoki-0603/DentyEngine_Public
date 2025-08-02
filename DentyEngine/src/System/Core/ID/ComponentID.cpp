#include "stdafx.h"

#include "ComponentID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
    ComponentID::ComponentID()
        : _componentID(Random::GetRandomValuesUint64T())
    {
    }

    ComponentID::ComponentID(const uint64_t id)
        : _componentID(id)
    {
    }

    void ComponentID::ReCalculate()
    {
        _componentID = Random::GetRandomValuesUint64T();
    }
};