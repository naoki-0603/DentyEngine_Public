#include "stdafx.h"

#include "MaterialID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
    MaterialID::MaterialID()
        : _materialID(Random::GetRandomValuesUint32T())
    {
    }

    MaterialID::MaterialID(const uint32_t id)
        : _materialID(id)
    {
    }

    void MaterialID::ReCalculate()
    {
        _materialID = Random::GetRandomValuesUint32T();
    }
};